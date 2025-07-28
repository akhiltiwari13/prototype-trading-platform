#include "MarketDataProvider/StreamManager.hpp"
#include "MarketDataProvider/LadderBuilder.hpp"
#include "MarketDataProvider/Structure.hpp"

#include <spdlog/spdlog.h>
#include <cstring>

namespace MarketDataProvider {

StreamManager::StreamManager(int size_) : _manager(size_) {
    // Initialize function pointers for message processing
    _function['N' - 'A'] = [this](const char* buffer) { newOrder(buffer); };
    _function['M' - 'A'] = [this](const char* buffer) { modifyOrder(buffer); };
    _function['X' - 'A'] = [this](const char* buffer) { cancelOrder(buffer); };
    _function['T' - 'A'] = [this](const char* buffer) { tradeOrder(buffer); };
}

void StreamManager::process(const char* buffer_, size_t size_) {
    if (size_ < sizeof(StreamHeader)) {
        spdlog::error("Invalid buffer size: {}", size_);
        return;
    }

    const auto* header = reinterpret_cast<const StreamHeader*>(buffer_);
    
    // Check sequence number
    if (header->_sequence != _sequence + 1) {
        spdlog::warn("Sequence gap detected. Expected: {}, Received: {}", 
                     _sequence + 1, header->_sequence);
        // Trigger recovery mechanism here
    }
    
    _sequence = header->_sequence;
    
    // Process message based on type
    char messageType = buffer_[sizeof(StreamHeader)];
    int functionIndex = messageType - 'A';
    
    if (functionIndex >= 0 && functionIndex < 26 && _function[functionIndex]) {
        _function[functionIndex](buffer_ + sizeof(StreamHeader) + 1);
    } else {
        spdlog::error("Unknown message type: {}", messageType);
    }
}

void StreamManager::init(const TokenListT& tokenList_) {
    _manager.clear();
    _manager.reserve(tokenList_.size());
    
    for (int token : tokenList_) {
        _manager.emplace_back(std::make_unique<LadderBuilder>(token));
    }
    
    spdlog::info("StreamManager initialized with {} tokens", tokenList_.size());
}

void StreamManager::newOrder(const char* buffer_) {
    const auto* order = reinterpret_cast<const OrderMessage*>(buffer_);
    
    // Find the appropriate ladder builder
    for (auto& builder : _manager) {
        if (builder) {
            builder->processNewOrder(*order);
            break;
        }
    }
}

void StreamManager::modifyOrder(const char* buffer_) {
    const auto* order = reinterpret_cast<const OrderMessage*>(buffer_);
    
    for (auto& builder : _manager) {
        if (builder) {
            builder->processModifyOrder(*order);
            break;
        }
    }
}

void StreamManager::cancelOrder(const char* buffer_) {
    const auto* order = reinterpret_cast<const OrderMessage*>(buffer_);
    
    for (auto& builder : _manager) {
        if (builder) {
            builder->processCancelOrder(*order);
            break;
        }
    }
}

void StreamManager::tradeOrder(const char* buffer_) {
    const auto* trade = reinterpret_cast<const TradeMessage*>(buffer_);
    
    for (auto& builder : _manager) {
        if (builder) {
            builder->processTrade(*trade);
            break;
        }
    }
}

} // namespace MarketDataProvider