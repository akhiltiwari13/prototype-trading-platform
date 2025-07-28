#include "SampleData.hpp"
#include <DatabaseLayer/DatabaseLayer.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace Simulation {

DatabaseLayer::TableWithColumnIndexT SampleData::getSampleContracts() {
    DatabaseLayer::TableWithColumnIndexT contracts;
    
    // Add NIFTY contracts
    auto niftyContracts = createNiftyContracts();
    contracts.insert(contracts.end(), niftyContracts.begin(), niftyContracts.end());
    
    // Add BANKNIFTY contracts
    auto bankNiftyContracts = createBankNiftyContracts();
    contracts.insert(contracts.end(), bankNiftyContracts.begin(), bankNiftyContracts.end());
    
    // Add equity contracts
    auto equityContracts = createEquityContracts();
    contracts.insert(contracts.end(), equityContracts.begin(), equityContracts.end());
    
    return contracts;
}

std::vector<uint32_t> SampleData::getSampleTokens() {
    return {
        35019, 35020, 35021, 35022, 35023, // NIFTY options
        36690, 36691, 36692, 36693, 36694, // BANKNIFTY options
        1594, 11536, 5900, 4963, 11915     // Equity tokens
    };
}

void SampleData::createSampleDatabase(const std::string& dbPath) {
    try {
        DatabaseLayer::ContractFetcher fetcher(dbPath);
        auto contracts = getSampleContracts();
        fetcher.Insert(contracts);
        
        spdlog::info("Created sample database with {} contracts", contracts.size());
    } catch (const std::exception& e) {
        spdlog::error("Failed to create sample database: {}", e.what());
        throw;
    }
}

nlohmann::json SampleData::getSampleTradingConfig() {
    nlohmann::json config;
    
    config["server"]["port"] = 8080;
    config["server"]["max_connections"] = 1000;
    
    config["database"]["path"] = "sample_trading.db";
    config["database"]["connection_pool_size"] = 10;
    
    config["risk_management"]["max_order_value"] = 1000000.0;
    config["risk_management"]["max_order_quantity"] = 10000;
    config["risk_management"]["max_daily_loss"] = 50000.0;
    config["risk_management"]["max_position_value"] = 500000.0;
    config["risk_management"]["max_orders_per_second"] = 10;
    
    config["simulation"]["enabled"] = true;
    config["simulation"]["auto_fill_orders"] = true;
    config["simulation"]["fill_delay_ms"] = 100;
    
    return config;
}

nlohmann::json SampleData::getSampleMarketDataConfig() {
    nlohmann::json config;
    
    config["stream_count"] = 4;
    config["host"] = "localhost";
    config["port"] = 9999;
    config["recovery_host"] = "localhost";
    config["recovery_port"] = 9998;
    
    config["tokens"] = getSampleTokens();
    
    config["simulation"]["enabled"] = true;
    config["simulation"]["base_price"] = 18500.0;
    config["simulation"]["volatility"] = 0.02;
    config["simulation"]["ticks_per_second"] = 10;
    config["simulation"]["max_orders_per_tick"] = 5;
    config["simulation"]["enable_trades"] = true;
    
    return config;
}

DatabaseLayer::TableWithColumnIndexT SampleData::createNiftyContracts() {
    return {
        // token, symbol, name, exchange, instrument_type, option_type, strike_price, expiry_date, lot_size
        {"35019", "NIFTY24JAN18500CE", "NIFTY 24JAN2024 18500 CE", "NSE", "OPTION", "CALL", "18500", "1706140800", "50"},
        {"35020", "NIFTY24JAN18500PE", "NIFTY 24JAN2024 18500 PE", "NSE", "OPTION", "PUT", "18500", "1706140800", "50"},
        {"35021", "NIFTY24JAN18600CE", "NIFTY 24JAN2024 18600 CE", "NSE", "OPTION", "CALL", "18600", "1706140800", "50"},
        {"35022", "NIFTY24JAN18600PE", "NIFTY 24JAN2024 18600 PE", "NSE", "OPTION", "PUT", "18600", "1706140800", "50"},
        {"35023", "NIFTY24JAN18700CE", "NIFTY 24JAN2024 18700 CE", "NSE", "OPTION", "CALL", "18700", "1706140800", "50"},
    };
}

DatabaseLayer::TableWithColumnIndexT SampleData::createBankNiftyContracts() {
    return {
        {"36690", "BANKNIFTY24JAN45000CE", "BANKNIFTY 24JAN2024 45000 CE", "NSE", "OPTION", "CALL", "45000", "1706140800", "25"},
        {"36691", "BANKNIFTY24JAN45000PE", "BANKNIFTY 24JAN2024 45000 PE", "NSE", "OPTION", "PUT", "45000", "1706140800", "25"},
        {"36692", "BANKNIFTY24JAN45100CE", "BANKNIFTY 24JAN2024 45100 CE", "NSE", "OPTION", "CALL", "45100", "1706140800", "25"},
        {"36693", "BANKNIFTY24JAN45100PE", "BANKNIFTY 24JAN2024 45100 PE", "NSE", "OPTION", "PUT", "45100", "1706140800", "25"},
        {"36694", "BANKNIFTY24JAN45200CE", "BANKNIFTY 24JAN2024 45200 CE", "NSE", "OPTION", "CALL", "45200", "1706140800", "25"},
    };
}

DatabaseLayer::TableWithColumnIndexT SampleData::createEquityContracts() {
    return {
        {"1594", "RELIANCE", "Reliance Industries Ltd", "NSE", "EQUITY", "NONE", "0", "0", "1"},
        {"11536", "TCS", "Tata Consultancy Services Ltd", "NSE", "EQUITY", "NONE", "0", "0", "1"},
        {"5900", "HDFCBANK", "HDFC Bank Ltd", "NSE", "EQUITY", "NONE", "0", "0", "1"},
        {"4963", "ICICIBANK", "ICICI Bank Ltd", "NSE", "EQUITY", "NONE", "0", "0", "1"},
        {"11915", "INFY", "Infosys Ltd", "NSE", "EQUITY", "NONE", "0", "0", "1"},
    };
}

} // namespace Simulation