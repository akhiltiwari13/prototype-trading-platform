#include <iostream>
#include <OptionsGreeks/OptionsGreeks.hpp>
#include <DatabaseLayer/DatabaseLayer.hpp>
#include <MarketDataProvider/MarketDataProvider.hpp>
#include <TradingEngine/TradingEngine.hpp>

int main() {
    std::cout << "Testing basic functionality..." << std::endl;
    
    try {
        // Test OptionsGreeks
        std::cout << "Testing OptionsGreeks..." << std::endl;
        double price = OptionsGreeks::GetOptionPrice(100.0, 100.0, 0.2, 0.05, 0.25, true);
        std::cout << "Option price: " << price << std::endl;
        
        // Test MarketDataProvider
        std::cout << "Testing MarketDataProvider..." << std::endl;
        MarketDataProvider::LadderBuilder builder(12345);
        std::cout << "LadderBuilder created successfully" << std::endl;
        
        // Test TradingEngine
        std::cout << "Testing TradingEngine..." << std::endl;
        TradingEngine::OrderManager orderManager;
        TradingEngine::RiskManager riskManager;
        std::cout << "OrderManager and RiskManager created successfully" << std::endl;
        
        std::cout << "All basic functionality tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}