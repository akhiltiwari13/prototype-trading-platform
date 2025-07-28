#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <array>
#include <utility>

namespace MarketDataProvider {

using PriceT    = int;
using QuantityT = int;
using OrderIdT  = double;
using TokenT    = int;

constexpr int MaxStream = 16;
constexpr int LADDER_DEPTH = 5;

using ComparatorT = std::less<>;

// Allocator for price-quantity pairs in ladder
using LadderAllocatorT = boost::fast_pool_allocator<
    std::pair<PriceT, QuantityT>, 
    boost::default_user_allocator_malloc_free, 
    boost::details::pool::null_mutex, 
    8, 32
>;

template <typename Comparator>
using ContainerT = boost::container::flat_map<PriceT, QuantityT, Comparator, LadderAllocatorT>;

/**
 * @brief Order structure
 */
struct Order {
    PriceT    _price;
    QuantityT _quantity;
};

// Allocator for order ID to order pairs
using OrderAllocatorT = boost::fast_pool_allocator<
    std::pair<OrderIdT, Order>,
    boost::default_user_allocator_malloc_free, 
    boost::details::pool::null_mutex, 
    8, 32
>;

using OrderContainerT = boost::container::flat_map<OrderIdT, Order, ComparatorT, OrderAllocatorT>;

#pragma pack(push, 1)

/**
 * @brief Ladder entry for order book
 */
struct Ladder {
    PriceT    _price    = 0;
    QuantityT _quantity = 0;
};

/**
 * @brief Market depth structure
 */
struct LadderDepth {
    TokenT  _token = 0;
    Ladder  _bid[LADDER_DEPTH];
    Ladder  _ask[LADDER_DEPTH];
};

/**
 * @brief Stream message header
 */
struct StreamHeader {
    short _len;
    short _streamId;
    int   _sequence;
    char  _type;
};

/**
 * @brief Order message structure
 */
struct OrderMessage {
    double _timestamp;
    double _orderId;
    TokenT _token;
    char   _orderType;
    PriceT _price;
    QuantityT _quantity;
};

/**
 * @brief Trade message structure
 */
struct TradeMessage {
    double _timeStamp;
    double _buyOrderId;
    double _sellOrderId;
    TokenT _token;
    PriceT _price;
    QuantityT _quantity;
};

/**
 * @brief Stream data packet
 */
struct StreamData {
    StreamHeader _header;
    char         _type;
    char         _data[40];
    int          _counter;
};

/**
 * @brief Recovery request structure
 */
struct RecoveryRequest {
    char  _msgType;
    short _streamId;
    int   _startSeqNo;
    int   _endSeqNo;
};

/**
 * @brief Recovery response structure
 */
struct RecoveryResponse {
    short _msgLen;
    short _streamId;
    int   _seqNo;
    char  _msgType;
    char  _requestStatus;
};

#pragma pack(pop)

/**
 * @brief Message types for market data
 */
enum MessageType : char {
    NEW      = 'N',
    REPLACE  = 'M',
    CANCEL   = 'X',
    TRADE    = 'T',
    RECOVERY = 'R'
};

} // namespace MarketDataProvider