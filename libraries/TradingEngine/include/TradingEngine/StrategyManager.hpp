#pragma once

#include <nlohmann/json.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>

namespace TradingEngine {

/**
 * @brief Base class for trading strategies
 */
class Strategy {
public:
    virtual ~Strategy() = default;
    
    /**
     * @brief Initialize strategy with parameters
     */
    virtual bool initialize(const nlohmann::json& params) = 0;
    
    /**
     * @brief Start strategy execution
     */
    virtual void start() = 0;
    
    /**
     * @brief Stop strategy execution
     */
    virtual void stop() = 0;
    
    /**
     * @brief Update strategy parameters
     */
    virtual void updateParameters(const nlohmann::json& params) = 0;
    
    /**
     * @brief Get strategy status
     */
    virtual nlohmann::json getStatus() const = 0;
    
    /**
     * @brief Get strategy name
     */
    virtual std::string getName() const = 0;
};

using StrategyPtr = std::shared_ptr<Strategy>;
using StrategyFactory = std::function<StrategyPtr()>;

/**
 * @brief Manages trading strategies
 */
class StrategyManager {
public:
    StrategyManager();
    ~StrategyManager() = default;

    /**
     * @brief Register a strategy factory
     */
    void registerStrategy(const std::string& name, StrategyFactory factory);
    
    /**
     * @brief Subscribe to a strategy
     */
    bool subscribeStrategy(int portfolioId, const std::string& strategyName, const nlohmann::json& params);
    
    /**
     * @brief Apply strategy parameters
     */
    bool applyStrategy(int portfolioId, const std::string& strategyName, const nlohmann::json& params);
    
    /**
     * @brief Unsubscribe from a strategy
     */
    bool unsubscribeStrategy(int portfolioId, const std::string& strategyName);
    
    /**
     * @brief Get strategy status
     */
    nlohmann::json getStrategyStatus(int portfolioId, const std::string& strategyName);
    
    /**
     * @brief Get all active strategies
     */
    std::vector<std::string> getActiveStrategies(int portfolioId);

private:
    struct StrategyInstance {
        StrategyPtr strategy;
        nlohmann::json parameters;
        bool isActive = false;
    };
    
    std::unordered_map<std::string, StrategyFactory> _strategyFactories;
    std::unordered_map<int, std::unordered_map<std::string, StrategyInstance>> _activeStrategies;
    
    std::string makeStrategyKey(int portfolioId, const std::string& strategyName);
};

} // namespace TradingEngine