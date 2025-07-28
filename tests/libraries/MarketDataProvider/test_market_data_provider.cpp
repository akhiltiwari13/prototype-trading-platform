#include <gtest/gtest.h>
#include <MarketDataProvider/MarketDataProvider.hpp>

class MarketDataProviderTest : public ::testing::Test {
protected:
    void SetUp() override {
        token = 12345;
        builder = std::make_unique<MarketDataProvider::LadderBuilder>(token);
    }

    MarketDataProvider::TokenT token;
    std::unique_ptr<MarketDataProvider::LadderBuilder> builder;
};

TEST_F(MarketDataProviderTest, LadderBuilderCreation) {
    EXPECT_NE(builder, nullptr);
}

TEST_F(MarketDataProviderTest, NewOrderProcessing) {
    MarketDataProvider::OrderMessage order;
    order._orderId = 1.0;
    order._token = token;
    order._orderType = 'B';  // Buy
    order._price = 100;
    order._quantity = 50;
    order._timestamp = 1640995200.0;

    EXPECT_NO_THROW({
        builder->processNewOrder(order);
    });

    auto depth = builder->getLadderDepth();
    EXPECT_EQ(depth._token, token);
    EXPECT_GT(depth._bid[0]._quantity, 0);
}

TEST_F(MarketDataProviderTest, OrderModification) {
    // First, add an order
    MarketDataProvider::OrderMessage order;
    order._orderId = 1.0;
    order._token = token;
    order._orderType = 'B';
    order._price = 100;
    order._quantity = 50;
    
    builder->processNewOrder(order);
    
    // Now modify it
    order._price = 101;
    order._quantity = 75;
    
    EXPECT_NO_THROW({
        builder->processModifyOrder(order);
    });
    
    auto depth = builder->getLadderDepth();
    EXPECT_EQ(depth._bid[0]._price, 101);
}

TEST_F(MarketDataProviderTest, OrderCancellation) {
    // Add an order
    MarketDataProvider::OrderMessage order;
    order._orderId = 1.0;
    order._token = token;
    order._orderType = 'B';
    order._price = 100;
    order._quantity = 50;
    
    builder->processNewOrder(order);
    
    // Cancel it
    EXPECT_NO_THROW({
        builder->processCancelOrder(order);
    });
    
    auto depth = builder->getLadderDepth();
    EXPECT_EQ(depth._bid[0]._quantity, 0);
}

TEST_F(MarketDataProviderTest, TradeProcessing) {
    // Add buy and sell orders
    MarketDataProvider::OrderMessage buyOrder;
    buyOrder._orderId = 1.0;
    buyOrder._token = token;
    buyOrder._orderType = 'B';
    buyOrder._price = 100;
    buyOrder._quantity = 50;
    
    MarketDataProvider::OrderMessage sellOrder;
    sellOrder._orderId = 2.0;
    sellOrder._token = token;
    sellOrder._orderType = 'S';
    sellOrder._price = 100;
    sellOrder._quantity = 30;
    
    builder->processNewOrder(buyOrder);
    builder->processNewOrder(sellOrder);
    
    // Process trade
    MarketDataProvider::TradeMessage trade;
    trade._buyOrderId = 1.0;
    trade._sellOrderId = 2.0;
    trade._token = token;
    trade._price = 100;
    trade._quantity = 25;
    trade._timeStamp = 1640995200.0;
    
    EXPECT_NO_THROW({
        builder->processTrade(trade);
    });
}

TEST_F(MarketDataProviderTest, StreamManagerCreation) {
    MarketDataProvider::StreamManager manager(1000);
    
    MarketDataProvider::TokenListT tokens = {12345, 12346, 12347};
    EXPECT_NO_THROW({
        manager.init(tokens);
    });
}

TEST_F(MarketDataProviderTest, MessageTypes) {
    EXPECT_EQ(MarketDataProvider::MessageType::NEW, 'N');
    EXPECT_EQ(MarketDataProvider::MessageType::REPLACE, 'M');
    EXPECT_EQ(MarketDataProvider::MessageType::CANCEL, 'X');
    EXPECT_EQ(MarketDataProvider::MessageType::TRADE, 'T');
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}