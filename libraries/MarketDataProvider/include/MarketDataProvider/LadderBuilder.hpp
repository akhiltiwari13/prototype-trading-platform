#pragma once

#include "MarketDataProvider/Structure.hpp"
#include <memory>

namespace MarketDataProvider {

/**
 * @brief Builds and maintains order book ladder
 */
class LadderBuilder {
public:
    explicit LadderBuilder(TokenT token_);
    ~LadderBuilder() = default;

    /**
     * @brief Process new order
     */
    void processNewOrder(const OrderMessage& order_);
    
    /**
     * @brief Process order modification
     */
    void processModifyOrder(const OrderMessage& order_);
    
    /**
     * @brief Process order cancellation
     */
    void processCancelOrder(const OrderMessage& order_);
    
    /**
     * @brief Process trade execution
     */
    void processTrade(const TradeMessage& trade_);
    
    /**
     * @brief Get current market depth
     */
    LadderDepth getLadderDepth() const;

private:
    TokenT _token;
    ContainerT<std::less<PriceT>>    _bidLadder;    // Bids (descending order)
    ContainerT<std::greater<PriceT>> _askLadder;    // Asks (ascending order)
    OrderContainerT                  _orderBook;    // Order tracking
    
    void updateLadder();
    void removeBidOrder(PriceT price_, QuantityT quantity_);
    void removeAskOrder(PriceT price_, QuantityT quantity_);
    void addBidOrder(PriceT price_, QuantityT quantity_);
    void addAskOrder(PriceT price_, QuantityT quantity_);
};

} // namespace MarketDataProvider