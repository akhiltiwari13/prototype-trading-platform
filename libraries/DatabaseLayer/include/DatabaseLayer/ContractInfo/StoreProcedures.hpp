#pragma once

#include <string>

namespace DatabaseLayer {

/**
 * @brief Database stored procedures and queries
 */
namespace StoreProcedures {

    // Common queries
    constexpr const char* SELECT_ALL_CONTRACTS = "SELECT * FROM contracts";
    constexpr const char* SELECT_BY_TOKEN = "SELECT * FROM contracts WHERE token = ?";
    constexpr const char* SELECT_BY_SYMBOL = "SELECT * FROM contracts WHERE symbol = ?";
    
    // Contract management
    constexpr const char* INSERT_CONTRACT = 
        "INSERT INTO contracts (token, symbol, name, exchange, instrument_type, option_type, strike_price, expiry_date, lot_size) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    
    constexpr const char* UPDATE_CONTRACT = 
        "UPDATE contracts SET symbol = ?, name = ?, exchange = ?, instrument_type = ?, option_type = ?, "
        "strike_price = ?, expiry_date = ?, lot_size = ? WHERE token = ?";
    
    constexpr const char* DELETE_CONTRACT = "DELETE FROM contracts WHERE token = ?";
    
    // Option-specific queries
    constexpr const char* SELECT_OPTIONS_BY_UNDERLYING = 
        "SELECT * FROM contracts WHERE instrument_type = 'OPTION' AND underlying_token = ?";
    
    constexpr const char* SELECT_CALL_OPTIONS = 
        "SELECT * FROM contracts WHERE instrument_type = 'OPTION' AND option_type = 'CALL'";
    
    constexpr const char* SELECT_PUT_OPTIONS = 
        "SELECT * FROM contracts WHERE instrument_type = 'OPTION' AND option_type = 'PUT'";

} // namespace StoreProcedures

} // namespace DatabaseLayer