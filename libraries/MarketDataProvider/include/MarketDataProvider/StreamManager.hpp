#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace MarketDataProvider {

class LadderBuilder;
using LadderBuilderPtrT = std::unique_ptr<LadderBuilder>;
using LadderContainerT  = std::vector<LadderBuilderPtrT>;
using TokenListT        = std::vector<int>;

using FunctionPointerT = std::function<void(const char*)>;

/**
 * @brief Manages market data streams and processes incoming messages
 */
class StreamManager final {
public:
    explicit StreamManager(int size_);
    
    /**
     * @brief Process incoming market data buffer
     */
    void process(const char* buffer_, size_t size_);
    
    /**
     * @brief Initialize with token list
     */
    void init(const TokenListT& tokenList_);

protected:
    void newOrder(const char* buffer_);
    void modifyOrder(const char* buffer_);
    void cancelOrder(const char* buffer_);
    void tradeOrder(const char* buffer_);

private:
    int              _sequence = 0;
    LadderContainerT _manager;
    FunctionPointerT _function[26];
};

} // namespace MarketDataProvider