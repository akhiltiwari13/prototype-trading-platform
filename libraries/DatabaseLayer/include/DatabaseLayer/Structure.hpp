#pragma once

#include <cstdint>
#include <string>
#include "DatabaseLayer/Enums.hpp"

namespace DatabaseLayer {

/**
 * @brief Structure representing a financial instrument result set
 */
struct ResultSetT {
    uint32_t   _token       = 0;        ///< Unique token identifier
    uint32_t   _futureToken = 0;        ///< Associated future token
    uint32_t   _expiryDate  = 0;        ///< Expiration date
    uint32_t   _lotMultiple = 0;        ///< Lot multiple
    uint32_t   _lotSize     = 0;        ///< Lot size
    uint32_t   _tickSize    = 0;        ///< Minimum price movement
    uint32_t   _divisor     = 0;        ///< Price divisor
    Instrument _instType    = Instrument_OTHER;  ///< Instrument type
    OptionType _option      = OptionType_NONE;   ///< Option type
    Exchange   _exchange    = Exchange_END;      ///< Exchange
    float      _strikePrice = 0;        ///< Strike price for options

    std::string _symbol;                ///< Trading symbol
    std::string _segment;               ///< Market segment
    std::string _name;                  ///< Instrument name
    std::string _description;           ///< Detailed description
};

} // namespace DatabaseLayer