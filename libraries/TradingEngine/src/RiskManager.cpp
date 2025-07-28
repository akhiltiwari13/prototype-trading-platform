#include "TradingEngine/RiskManager.hpp"
#include <DatabaseLayer/Enums.hpp>
#include <spdlog/spdlog.h>
#include <chrono>

namespace TradingEngine {

RiskManager::RiskManager() {
    spdlog::info("RiskManager initialized");
}

bool RiskManager::validateOrder(const nlohmann::json& orderRequest, const std::string& clientId) {
    try {
        // Get or create risk limits for client
        auto it = _clientLimits.find(clientId);
        if (it == _clientLimits.end()) {
            _clientLimits[clientId] = RiskLimits{}; // Use default limits
            it = _clientLimits.find(clientId);
        }
        
        const RiskLimits& limits = it->second;
        
        // Basic validation
        if (!orderRequest.contains(JSON_PRICE) || !orderRequest.contains(JSON_QUANTITY)) {
            spdlog::error("Order missing required fields");
            return false;
        }
        
        // Check order value
        if (!checkOrderValue(orderRequest, limits)) {
            return false;
        }
        
        // Check position limits
        uint32_t token = orderRequest[JSON_TOKEN];
        int quantity = orderRequest[JSON_QUANTITY];
        if (!checkPositionLimits(clientId, token, quantity, limits)) {
            return false;
        }
        
        // Check daily loss
        if (!checkDailyLoss(clientId, limits)) {
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        spdlog::error("Risk validation error: {}", e.what());
        return false;
    }
}

void RiskManager::updatePosition(const std::string& clientId, uint32_t token, int quantity, double price) {
    auto& position = _positions[clientId][token];
    
    if (position.quantity == 0) {
        // New position
        position.token = token;
        position.quantity = quantity;
        position.averagePrice = price;
    } else {
        // Update existing position
        double totalValue = position.quantity * position.averagePrice + quantity * price;
        position.quantity += quantity;
        
        if (position.quantity != 0) {
            position.averagePrice = totalValue / position.quantity;
        } else {
            position.averagePrice = 0.0;
        }
    }
    
    spdlog::debug("Updated position for client {} token {}: qty={}, avg_price={}", 
                  clientId, token, position.quantity, position.averagePrice);
}

void RiskManager::setRiskLimits(const std::string& clientId, const RiskLimits& limits) {
    _clientLimits[clientId] = limits;
    spdlog::info("Updated risk limits for client: {}", clientId);
}

Position RiskManager::getPosition(const std::string& clientId, uint32_t token) const {
    auto clientIt = _positions.find(clientId);
    if (clientIt != _positions.end()) {
        auto tokenIt = clientIt->second.find(token);
        if (tokenIt != clientIt->second.end()) {
            return tokenIt->second;
        }
    }
    
    // Return empty position
    Position emptyPosition;
    emptyPosition.token = token;
    return emptyPosition;
}

std::vector<Position> RiskManager::getAllPositions(const std::string& clientId) const {
    std::vector<Position> positions;
    
    auto clientIt = _positions.find(clientId);
    if (clientIt != _positions.end()) {
        for (const auto& [token, position] : clientIt->second) {
            positions.push_back(position);
        }
    }
    
    return positions;
}

double RiskManager::calculatePortfolioPnL(const std::string& clientId) const {
    double totalPnL = 0.0;
    
    auto clientIt = _positions.find(clientId);
    if (clientIt != _positions.end()) {
        for (const auto& [token, position] : clientIt->second) {
            totalPnL += position.realizedPnL + position.unrealizedPnL;
        }
    }
    
    return totalPnL;
}

bool RiskManager::isWithinRiskLimits(const std::string& clientId) const {
    auto it = _clientLimits.find(clientId);
    if (it == _clientLimits.end()) {
        return true; // No limits set
    }
    
    const RiskLimits& limits = it->second;
    
    // Check daily PnL
    double dailyPnL = calculatePortfolioPnL(clientId);
    if (dailyPnL < -limits.maxDailyLoss) {
        return false;
    }
    
    return true;
}

bool RiskManager::checkOrderValue(const nlohmann::json& orderRequest, const RiskLimits& limits) {
    try {
        double price = std::stod(orderRequest[JSON_PRICE].get<std::string>());
        int quantity = orderRequest[JSON_QUANTITY];
        
        if (price <= 0 || quantity <= 0) {
            spdlog::error("Invalid price or quantity: price={}, quantity={}", price, quantity);
            return false;
        }
        
        double orderValue = price * quantity;
        if (orderValue > limits.maxOrderValue) {
            spdlog::error("Order value {} exceeds limit {}", orderValue, limits.maxOrderValue);
            return false;
        }
        
        if (quantity > limits.maxOrderQuantity) {
            spdlog::error("Order quantity {} exceeds limit {}", quantity, limits.maxOrderQuantity);
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        spdlog::error("Error checking order value: {}", e.what());
        return false;
    }
}

bool RiskManager::checkPositionLimits(const std::string& clientId, uint32_t token, int quantity, const RiskLimits& limits) {
    Position currentPosition = getPosition(clientId, token);
    int newQuantity = currentPosition.quantity + quantity;
    
    double positionValue = std::abs(newQuantity * currentPosition.averagePrice);
    if (positionValue > limits.maxPositionValue) {
        spdlog::error("Position value {} would exceed limit {}", positionValue, limits.maxPositionValue);
        return false;
    }
    
    return true;
}

bool RiskManager::checkDailyLoss(const std::string& clientId, const RiskLimits& limits) {
    double dailyPnL = calculatePortfolioPnL(clientId);
    if (dailyPnL < -limits.maxDailyLoss) {
        spdlog::error("Daily loss {} exceeds limit {}", -dailyPnL, limits.maxDailyLoss);
        return false;
    }
    
    return true;
}

} // namespace TradingEngine