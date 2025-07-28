#pragma once

#include <DatabaseLayer/DatabaseLayer.hpp>
#include <nlohmann/json.hpp>
#include <memory>
#include <unordered_map>
#include <functional>

namespace TradingEngine {

using OrderId = uint64_t;
using ClientId = std::string;

/**
 * @brief Order structure for trading operations
 */
struct Order {
    OrderId orderId;
    uint32_t token;
    DatabaseLayer::Side side;
    double price;
    int quantity;
    int fillQuantity = 0;
    ClientId client;
    DatabaseLayer::RequestType orderType;
    std::string uniqueId;
    uint64_t timestamp;
    DatabaseLayer::ResponseType status;
};

using OrderPtr = std::shared_ptr<Order>;
using OrderCallback = std::function<void(const Order&)>;

/**
 * @brief Manages trading orders and their lifecycle
 */
class OrderManager {
public:
    OrderManager();
    ~OrderManager() = default;

    /**
     * @brief Place a new order
     */
    OrderId placeOrder(const nlohmann::json& orderRequest);
    
    /**
     * @brief Modify an existing order
     */
    bool modifyOrder(const nlohmann::json& modifyRequest);
    
    /**
     * @brief Cancel an order
     */
    bool cancelOrder(const nlohmann::json& cancelRequest);
    
    /**
     * @brief Get order by ID
     */
    OrderPtr getOrder(OrderId orderId) const;
    
    /**
     * @brief Set callback for order updates
     */
    void setOrderCallback(OrderCallback callback);
    
    /**
     * @brief Process order fill
     */
    void processOrderFill(OrderId orderId, double fillPrice, int fillQuantity);

private:
    std::unordered_map<OrderId, OrderPtr> _orders;
    OrderCallback _orderCallback;
    OrderId _nextOrderId = 1;
    
    OrderId generateOrderId();
    void notifyOrderUpdate(const Order& order);
    bool validateOrder(const nlohmann::json& orderRequest);
};

} // namespace TradingEngine