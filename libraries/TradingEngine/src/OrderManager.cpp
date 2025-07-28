#include "TradingEngine/OrderManager.hpp"
#include <DatabaseLayer/Enums.hpp>
#include <spdlog/spdlog.h>
#include <chrono>

namespace TradingEngine {

OrderManager::OrderManager() {
    spdlog::info("OrderManager initialized");
}

OrderId OrderManager::placeOrder(const nlohmann::json& orderRequest) {
    if (!validateOrder(orderRequest)) {
        spdlog::error("Order validation failed");
        return 0;
    }

    auto order = std::make_shared<Order>();
    order->orderId = generateOrderId();
    order->token = orderRequest[JSON_TOKEN];
    order->side = static_cast<DatabaseLayer::Side>(orderRequest[JSON_SIDE]);
    order->price = std::stod(orderRequest[JSON_PRICE].get<std::string>());
    order->quantity = orderRequest[JSON_QUANTITY];
    order->client = orderRequest[JSON_CLIENT];
    order->orderType = static_cast<DatabaseLayer::RequestType>(orderRequest[JSON_ORDER_TYPE]);
    order->timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    order->status = DatabaseLayer::ResponseType_PLACED;

    _orders[order->orderId] = order;
    
    spdlog::info("Order placed: ID={}, Token={}, Side={}, Price={}, Quantity={}", 
                 order->orderId, order->token, static_cast<int>(order->side), 
                 order->price, order->quantity);
    
    notifyOrderUpdate(*order);
    return order->orderId;
}

bool OrderManager::modifyOrder(const nlohmann::json& modifyRequest) {
    if (!modifyRequest.contains(JSON_ORDER_ID)) {
        spdlog::error("Modify request missing order ID");
        return false;
    }
    
    OrderId orderId = modifyRequest[JSON_ORDER_ID];
    auto it = _orders.find(orderId);
    
    if (it == _orders.end()) {
        spdlog::error("Order not found for modification: {}", orderId);
        return false;
    }

    auto& order = it->second;
    
    if (modifyRequest.contains(JSON_PRICE)) {
        order->price = std::stod(modifyRequest[JSON_PRICE].get<std::string>());
    }
    
    if (modifyRequest.contains(JSON_QUANTITY)) {
        order->quantity = modifyRequest[JSON_QUANTITY];
    }
    
    order->status = DatabaseLayer::ResponseType_REPLACED;
    
    spdlog::info("Order modified: ID={}, New Price={}, New Quantity={}", 
                 orderId, order->price, order->quantity);
    
    notifyOrderUpdate(*order);
    return true;
}

bool OrderManager::cancelOrder(const nlohmann::json& cancelRequest) {
    if (!cancelRequest.contains(JSON_ORDER_ID)) {
        spdlog::error("Cancel request missing order ID");
        return false;
    }
    
    OrderId orderId = cancelRequest[JSON_ORDER_ID];
    auto it = _orders.find(orderId);
    
    if (it == _orders.end()) {
        spdlog::error("Order not found for cancellation: {}", orderId);
        return false;
    }

    auto& order = it->second;
    order->status = DatabaseLayer::ResponseType_CANCELLED;
    
    spdlog::info("Order cancelled: ID={}", orderId);
    
    notifyOrderUpdate(*order);
    return true;
}

OrderPtr OrderManager::getOrder(OrderId orderId) const {
    auto it = _orders.find(orderId);
    return (it != _orders.end()) ? it->second : nullptr;
}

void OrderManager::setOrderCallback(OrderCallback callback) {
    _orderCallback = std::move(callback);
}

void OrderManager::processOrderFill(OrderId orderId, double fillPrice, int fillQuantity) {
    auto it = _orders.find(orderId);
    if (it == _orders.end()) {
        spdlog::error("Order not found for fill: {}", orderId);
        return;
    }

    auto& order = it->second;
    order->fillQuantity += fillQuantity;
    
    if (order->fillQuantity >= order->quantity) {
        order->status = DatabaseLayer::ResponseType_FILLED;
    } else {
        order->status = DatabaseLayer::ResponseType_PARTIAL_FILLED;
    }
    
    spdlog::info("Order fill processed: ID={}, Fill Price={}, Fill Quantity={}, Total Filled={}", 
                 orderId, fillPrice, fillQuantity, order->fillQuantity);
    
    notifyOrderUpdate(*order);
}

OrderId OrderManager::generateOrderId() {
    return _nextOrderId++;
}

void OrderManager::notifyOrderUpdate(const Order& order) {
    if (_orderCallback) {
        _orderCallback(order);
    }
}

bool OrderManager::validateOrder(const nlohmann::json& orderRequest) {
    // Basic validation
    if (!orderRequest.contains(JSON_TOKEN) || 
        !orderRequest.contains(JSON_PRICE) || 
        !orderRequest.contains(JSON_QUANTITY) || 
        !orderRequest.contains(JSON_CLIENT) || 
        !orderRequest.contains(JSON_SIDE)) {
        return false;
    }

    try {
        double price = std::stod(orderRequest[JSON_PRICE].get<std::string>());
        int quantity = orderRequest[JSON_QUANTITY];
        
        if (price <= 0 || quantity <= 0) {
            return false;
        }
    } catch (const std::exception& e) {
        spdlog::error("Order validation error: {}", e.what());
        return false;
    }

    return true;
}

} // namespace TradingEngine