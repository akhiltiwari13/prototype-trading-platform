#pragma once

#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>

namespace TradingEngine {

/**
 * @brief Risk limits and parameters
 */
struct RiskLimits {
    double maxOrderValue = 1000000.0;      // Maximum order value
    int maxOrderQuantity = 10000;          // Maximum order quantity
    double maxDailyLoss = 50000.0;         // Maximum daily loss
    double maxPositionValue = 500000.0;    // Maximum position value
    int maxOrdersPerSecond = 10;           // Rate limiting
};

/**
 * @brief Position tracking
 */
struct Position {
    uint32_t token;
    int quantity = 0;
    double averagePrice = 0.0;
    double unrealizedPnL = 0.0;
    double realizedPnL = 0.0;
};

/**
 * @brief Risk management for trading operations
 */
class RiskManager {
public:
    RiskManager();
    ~RiskManager() = default;

    /**
     * @brief Validate order against risk limits
     */
    bool validateOrder(const nlohmann::json& orderRequest, const std::string& clientId);
    
    /**
     * @brief Update position after order fill
     */
    void updatePosition(const std::string& clientId, uint32_t token, int quantity, double price);
    
    /**
     * @brief Set risk limits for a client
     */
    void setRiskLimits(const std::string& clientId, const RiskLimits& limits);
    
    /**
     * @brief Get current position
     */
    Position getPosition(const std::string& clientId, uint32_t token) const;
    
    /**
     * @brief Get all positions for a client
     */
    std::vector<Position> getAllPositions(const std::string& clientId) const;
    
    /**
     * @brief Calculate portfolio PnL
     */
    double calculatePortfolioPnL(const std::string& clientId) const;
    
    /**
     * @brief Check if client is within risk limits
     */
    bool isWithinRiskLimits(const std::string& clientId) const;

private:
    std::unordered_map<std::string, RiskLimits> _clientLimits;
    std::unordered_map<std::string, std::unordered_map<uint32_t, Position>> _positions;
    std::unordered_map<std::string, double> _dailyPnL;
    
    bool checkOrderValue(const nlohmann::json& orderRequest, const RiskLimits& limits);
    bool checkPositionLimits(const std::string& clientId, uint32_t token, int quantity, const RiskLimits& limits);
    bool checkDailyLoss(const std::string& clientId, const RiskLimits& limits);
};

} // namespace TradingEngine