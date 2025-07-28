#include "MarketDataSimulator.hpp"
#include <spdlog/spdlog.h>
#include <chrono>

namespace Simulation {

MarketDataSimulator::MarketDataSimulator(const SimulationConfig& config) 
    : _config(config)
    , _rng(std::chrono::steady_clock::now().time_since_epoch().count())
    , _priceDistribution(-_config.volatility, _config.volatility)
    , _quantityDistribution(1, 100)
    , _orderCountDistribution(1, _config.maxOrdersPerTick)
{
    // Initialize current prices
    for (auto token : _config.tokens) {
        _currentPrices[token] = _config.basePrice;
    }
    
    spdlog::info("MarketDataSimulator initialized with {} tokens", _config.tokens.size());
}

MarketDataSimulator::~MarketDataSimulator() {
    stop();
}

void MarketDataSimulator::start() {
    if (_running.exchange(true)) {
        return; // Already running
    }
    
    spdlog::info("Starting market data simulation");
    _simulationThread = std::thread(&MarketDataSimulator::simulationLoop, this);
}

void MarketDataSimulator::stop() {
    if (!_running.exchange(false)) {
        return; // Already stopped
    }
    
    if (_simulationThread.joinable()) {
        _simulationThread.join();
    }
    
    spdlog::info("Market data simulation stopped");
}

void MarketDataSimulator::setOrderCallback(DataCallback callback) {
    _orderCallback = std::move(callback);
}

void MarketDataSimulator::setTradeCallback(TradeCallback callback) {
    _tradeCallback = std::move(callback);
}

void MarketDataSimulator::simulationLoop() {
    auto tickInterval = std::chrono::milliseconds(1000 / _config.ticksPerSecond);
    
    while (_running) {
        auto startTime = std::chrono::steady_clock::now();
        
        try {
            generateOrders();
            
            if (_config.enableTrades) {
                generateTrades();
            }
        } catch (const std::exception& e) {
            spdlog::error("Simulation error: {}", e.what());
        }
        
        // Sleep until next tick
        auto endTime = std::chrono::steady_clock::now();
        auto elapsed = endTime - startTime;
        
        if (elapsed < tickInterval) {
            std::this_thread::sleep_for(tickInterval - elapsed);
        }
    }
}

void MarketDataSimulator::generateOrders() {
    for (auto token : _config.tokens) {
        int orderCount = _orderCountDistribution(_rng);
        
        for (int i = 0; i < orderCount; ++i) {
            auto order = createRandomOrder(token);
            
            if (_orderCallback) {
                _orderCallback(order);
            }
        }
    }
}

void MarketDataSimulator::generateTrades() {
    for (auto token : _config.tokens) {
        // Generate trades less frequently than orders
        if (_rng() % 5 == 0) { // 20% chance per token per tick
            auto trade = createRandomTrade(token);
            
            if (_tradeCallback) {
                _tradeCallback(trade);
            }
        }
    }
}

MarketDataProvider::OrderMessage MarketDataSimulator::createRandomOrder(MarketDataProvider::TokenT token) {
    MarketDataProvider::OrderMessage order;
    
    order._orderId = _currentOrderId++;
    order._token = token;
    order._orderType = (_rng() % 2 == 0) ? 'B' : 'S'; // Buy or Sell
    order._timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() / 1e9;
    
    // Update price with random walk
    double& currentPrice = _currentPrices[token];
    double priceChange = _priceDistribution(_rng);
    currentPrice *= (1.0 + priceChange);
    
    // Add some spread
    double spread = currentPrice * 0.001; // 0.1% spread
    if (order._orderType == 'B') {
        order._price = static_cast<int>((currentPrice - spread) * 100); // Convert to integer price
    } else {
        order._price = static_cast<int>((currentPrice + spread) * 100);
    }
    
    order._quantity = _quantityDistribution(_rng);
    
    return order;
}

MarketDataProvider::TradeMessage MarketDataSimulator::createRandomTrade(MarketDataProvider::TokenT token) {
    MarketDataProvider::TradeMessage trade;
    
    trade._buyOrderId = _currentOrderId - _rng() % 100; // Random recent order
    trade._sellOrderId = _currentOrderId - _rng() % 100;
    trade._token = token;
    trade._timeStamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() / 1e9;
    
    double currentPrice = _currentPrices[token];
    trade._price = static_cast<int>(currentPrice * 100);
    trade._quantity = _quantityDistribution(_rng) / 2; // Smaller trade quantities
    
    return trade;
}

} // namespace Simulation