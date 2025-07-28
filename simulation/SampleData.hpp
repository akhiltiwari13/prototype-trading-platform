#pragma once

#include <DatabaseLayer/DatabaseLayer.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <cstdint>

namespace Simulation {

/**
 * @brief Provides sample contract data for testing
 */
class SampleData {
public:
    /**
     * @brief Get sample contract data
     */
    static DatabaseLayer::TableWithColumnIndexT getSampleContracts();
    
    /**
     * @brief Get sample tokens for market data simulation
     */
    static std::vector<uint32_t> getSampleTokens();
    
    /**
     * @brief Create sample database with contracts
     */
    static void createSampleDatabase(const std::string& dbPath);
    
    /**
     * @brief Get sample trading configuration
     */
    static nlohmann::json getSampleTradingConfig();
    
    /**
     * @brief Get sample market data configuration
     */
    static nlohmann::json getSampleMarketDataConfig();

private:
    static DatabaseLayer::TableWithColumnIndexT createNiftyContracts();
    static DatabaseLayer::TableWithColumnIndexT createBankNiftyContracts();
    static DatabaseLayer::TableWithColumnIndexT createEquityContracts();
};

} // namespace Simulation