#include "MarketDataProvider/LadderBuilder.hpp"
#include "MarketDataProvider/Structure.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>

namespace MarketDataProvider {

LadderBuilder::LadderBuilder(TokenT token_) : _token(token_) {
    spdlog::debug("LadderBuilder created for token: {}", _token);
}

void LadderBuilder::processNewOrder(const OrderMessage& order_) {
    if (order_._token != _token) {
        return;
    }

    // Store order for tracking
    _orderBook[order_._orderId] = MarketDataProvider::Order{order_._price, order_._quantity};
    
    // Add to appropriate ladder
    if (order_._orderType == 'B') { // Buy order
        addBidOrder(order_._price, order_._quantity);
    } else if (order_._orderType == 'S') { // Sell order
        addAskOrder(order_._price, order_._quantity);
    }
    
    updateLadder();
}

void LadderBuilder::processModifyOrder(const OrderMessage& order_) {
    if (order_._token != _token) {
        return;
    }

    auto it = _orderBook.find(order_._orderId);
    if (it != _orderBook.end()) {
        // Remove old order
        MarketDataProvider::Order& existingOrder = it->second;
        if (order_._orderType == 'B') {
            removeBidOrder(existingOrder._price, existingOrder._quantity);
        } else {
            removeAskOrder(existingOrder._price, existingOrder._quantity);
        }
        
        // Add modified order
        existingOrder._price = order_._price;
        existingOrder._quantity = order_._quantity;
        if (order_._orderType == 'B') {
            addBidOrder(order_._price, order_._quantity);
        } else {
            addAskOrder(order_._price, order_._quantity);
        }
        
        updateLadder();
    }
}

void LadderBuilder::processCancelOrder(const OrderMessage& order_) {
    if (order_._token != _token) {
        return;
    }

    auto it = _orderBook.find(order_._orderId);
    if (it != _orderBook.end()) {
        const MarketDataProvider::Order& orderToCancel = it->second;
        if (order_._orderType == 'B') {
            removeBidOrder(orderToCancel._price, orderToCancel._quantity);
        } else {
            removeAskOrder(orderToCancel._price, orderToCancel._quantity);
        }
        
        _orderBook.erase(it);
        updateLadder();
    }
}

void LadderBuilder::processTrade(const TradeMessage& trade_) {
    if (trade_._token != _token) {
        return;
    }

    // Remove traded quantities from both buy and sell orders
    auto buyIt = _orderBook.find(trade_._buyOrderId);
    auto sellIt = _orderBook.find(trade_._sellOrderId);
    
    if (buyIt != _orderBook.end()) {
        MarketDataProvider::Order& buyOrder = buyIt->second;
        removeBidOrder(buyOrder._price, trade_._quantity);
        buyOrder._quantity -= trade_._quantity;
        if (buyOrder._quantity <= 0) {
            _orderBook.erase(buyIt);
        } else {
            addBidOrder(buyOrder._price, buyOrder._quantity);
        }
    }
    
    if (sellIt != _orderBook.end()) {
        MarketDataProvider::Order& sellOrder = sellIt->second;
        removeAskOrder(sellOrder._price, trade_._quantity);
        sellOrder._quantity -= trade_._quantity;
        if (sellOrder._quantity <= 0) {
            _orderBook.erase(sellIt);
        } else {
            addAskOrder(sellOrder._price, sellOrder._quantity);
        }
    }
    
    updateLadder();
}

LadderDepth LadderBuilder::getLadderDepth() const {
    LadderDepth depth;
    depth._token = _token;
    
    // Fill bid ladder (top 5 bids)
    int bidIndex = 0;
    for (auto it = _bidLadder.begin(); it != _bidLadder.end() && bidIndex < LADDER_DEPTH; ++it, ++bidIndex) {
        depth._bid[bidIndex]._price = it->first;
        depth._bid[bidIndex]._quantity = it->second;
    }
    
    // Fill ask ladder (top 5 asks)
    int askIndex = 0;
    for (auto it = _askLadder.begin(); it != _askLadder.end() && askIndex < LADDER_DEPTH; ++it, ++askIndex) {
        depth._ask[askIndex]._price = it->first;
        depth._ask[askIndex]._quantity = it->second;
    }
    
    return depth;
}

void LadderBuilder::updateLadder() {
    // This method can be used for additional processing after ladder updates
    // For now, it's a placeholder for future enhancements
}

void LadderBuilder::removeBidOrder(PriceT price_, QuantityT quantity_) {
    auto it = _bidLadder.find(price_);
    if (it != _bidLadder.end()) {
        it->second -= quantity_;
        if (it->second <= 0) {
            _bidLadder.erase(it);
        }
    }
}

void LadderBuilder::removeAskOrder(PriceT price_, QuantityT quantity_) {
    auto it = _askLadder.find(price_);
    if (it != _askLadder.end()) {
        it->second -= quantity_;
        if (it->second <= 0) {
            _askLadder.erase(it);
        }
    }
}

void LadderBuilder::addBidOrder(PriceT price_, QuantityT quantity_) {
    _bidLadder[price_] += quantity_;
}

void LadderBuilder::addAskOrder(PriceT price_, QuantityT quantity_) {
    _askLadder[price_] += quantity_;
}

} // namespace MarketDataProvider