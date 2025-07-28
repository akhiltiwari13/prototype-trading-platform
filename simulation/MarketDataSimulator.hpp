#pragma once

#include <MarketDataProvider/MarketDataProvider.hpp>
#include <vector>
#include <random>
#include <thread>
#include <atomic>
#include <functional>

namespace Simulation {

/**
 * @brief Simulates realistic market data for testing
 */
class MarketDataSimulator {
public:
    struct SimulationConfig {
        std::vector<MarketDataProvider::TokenT> tokens;
        double basePrice = 100.0;
        double volatility = 0.02;  // 2% volatility
        int ticksPerSecond = 10;
        int maxOrdersPerTick = 5;
        bool enableTrades = true;
    };

    using DataCallback = std::function<void(const MarketDataProvider::OrderMessage&)>;
    using TradeCallback = std::function<void(const MarketDataProvider::TradeMessage&)>;

    explicit MarketDataSimulator(const SimulationConfig& config);
    ~MarketDataSimulator();

    /**
     * @brief Start simulation
     */
    void start();
    
    /**
     * @brief Stop simulation
     */
    void stop();
    
    /**
     * @brief Set callback for order messages
     */
    void setOrderCallback(DataCallback callback);
    
    /**
     * @brief Set callback for trade messages
     */
    void setTradeCallback(TradeCallback callback);

private:
    SimulationConfig _config;
    std::atomic<bool> _running{false};
    std::thread _simulationThread;
    
    std::mt19937 _rng;
    std::uniform_real_distribution<double> _priceDistribution;
    std::uniform_int_distribution<int> _quantityDistribution;
    std::uniform_int_distribution<int> _orderCountDistribution;
    
    DataCallback _orderCallback;
    TradeCallback _tradeCallback;
    
    double _currentOrderId = 1.0;
    std::unordered_map<MarketDataProvider::TokenT, double> _currentPrices;
    
    void simulationLoop();
    void generateOrders();
    void generateTrades();
    
    MarketDataProvider::OrderMessage createRandomOrder(MarketDataProvider::TokenT token);
    MarketDataProvider::TradeMessage createRandomTrade(MarketDataProvider::TokenT token);
};

} // namespace Simulation