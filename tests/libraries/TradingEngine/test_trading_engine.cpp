#include <gtest/gtest.h>
#include <TradingEngine/TradingEngine.hpp>
#include <DatabaseLayer/Enums.hpp>
#include <nlohmann/json.hpp>

class TradingEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        orderManager = std::make_unique<TradingEngine::OrderManager>();
        riskManager = std::make_unique<TradingEngine::RiskManager>();
        
        // Set up callback to capture order updates
        orderUpdates.clear();
        orderManager->setOrderCallback([this](const TradingEngine::Order& order) {
            orderUpdates.push_back(order);
        });
    }

    std::unique_ptr<TradingEngine::OrderManager> orderManager;
    std::unique_ptr<TradingEngine::RiskManager> riskManager;
    std::vector<TradingEngine::Order> orderUpdates;
};

TEST_F(TradingEngineTest, OrderManagerCreation) {
    EXPECT_NE(orderManager, nullptr);
}

TEST_F(TradingEngineTest, PlaceNewOrder) {
    nlohmann::json orderRequest;
    orderRequest[JSON_TOKEN] = 12345;
    orderRequest[JSON_PRICE] = "100.50";
    orderRequest[JSON_QUANTITY] = 50;
    orderRequest[JSON_CLIENT] = "TestClient";
    orderRequest[JSON_SIDE] = static_cast<int>(DatabaseLayer::Side_BUY);
    orderRequest[JSON_ORDER_TYPE] = static_cast<int>(DatabaseLayer::RequestType_NEW);

    auto orderId = orderManager->placeOrder(orderRequest);
    
    EXPECT_GT(orderId, 0);
    EXPECT_EQ(orderUpdates.size(), 1);
    EXPECT_EQ(orderUpdates[0].orderId, orderId);
    EXPECT_EQ(orderUpdates[0].token, 12345);
    EXPECT_DOUBLE_EQ(orderUpdates[0].price, 100.50);
    EXPECT_EQ(orderUpdates[0].quantity, 50);
}

TEST_F(TradingEngineTest, ModifyOrder) {
    // First place an order
    nlohmann::json orderRequest;
    orderRequest[JSON_TOKEN] = 12345;
    orderRequest[JSON_PRICE] = "100.50";
    orderRequest[JSON_QUANTITY] = 50;
    orderRequest[JSON_CLIENT] = "TestClient";
    orderRequest[JSON_SIDE] = static_cast<int>(DatabaseLayer::Side_BUY);
    orderRequest[JSON_ORDER_TYPE] = static_cast<int>(DatabaseLayer::RequestType_NEW);

    auto orderId = orderManager->placeOrder(orderRequest);
    orderUpdates.clear(); // Clear the placement update
    
    // Now modify it
    nlohmann::json modifyRequest;
    modifyRequest[JSON_ORDER_ID] = orderId;
    modifyRequest[JSON_PRICE] = "101.00";
    modifyRequest[JSON_QUANTITY] = 75;
    modifyRequest[JSON_UNIQUE_ID] = "modify_1";

    bool result = orderManager->modifyOrder(modifyRequest);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(orderUpdates.size(), 1);
    EXPECT_DOUBLE_EQ(orderUpdates[0].price, 101.00);
    EXPECT_EQ(orderUpdates[0].quantity, 75);
}

TEST_F(TradingEngineTest, CancelOrder) {
    // First place an order
    nlohmann::json orderRequest;
    orderRequest[JSON_TOKEN] = 12345;
    orderRequest[JSON_PRICE] = "100.50";
    orderRequest[JSON_QUANTITY] = 50;
    orderRequest[JSON_CLIENT] = "TestClient";
    orderRequest[JSON_SIDE] = static_cast<int>(DatabaseLayer::Side_BUY);
    orderRequest[JSON_ORDER_TYPE] = static_cast<int>(DatabaseLayer::RequestType_NEW);

    auto orderId = orderManager->placeOrder(orderRequest);
    orderUpdates.clear();
    
    // Cancel it
    nlohmann::json cancelRequest;
    cancelRequest[JSON_ORDER_ID] = orderId;
    cancelRequest[JSON_UNIQUE_ID] = "cancel_1";

    bool result = orderManager->cancelOrder(cancelRequest);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(orderUpdates.size(), 1);
    EXPECT_EQ(orderUpdates[0].status, DatabaseLayer::ResponseType_CANCELLED);
}

TEST_F(TradingEngineTest, OrderFill) {
    // Place an order
    nlohmann::json orderRequest;
    orderRequest[JSON_TOKEN] = 12345;
    orderRequest[JSON_PRICE] = "100.50";
    orderRequest[JSON_QUANTITY] = 50;
    orderRequest[JSON_CLIENT] = "TestClient";
    orderRequest[JSON_SIDE] = static_cast<int>(DatabaseLayer::Side_BUY);
    orderRequest[JSON_ORDER_TYPE] = static_cast<int>(DatabaseLayer::RequestType_NEW);

    auto orderId = orderManager->placeOrder(orderRequest);
    orderUpdates.clear();
    
    // Process fill
    orderManager->processOrderFill(orderId, 100.25, 25);
    
    EXPECT_EQ(orderUpdates.size(), 1);
    EXPECT_EQ(orderUpdates[0].fillQuantity, 25);
    EXPECT_EQ(orderUpdates[0].status, DatabaseLayer::ResponseType_PARTIAL_FILLED);
    
    // Complete fill
    orderManager->processOrderFill(orderId, 100.30, 25);
    
    EXPECT_EQ(orderUpdates.size(), 2);
    EXPECT_EQ(orderUpdates[1].fillQuantity, 50);
    EXPECT_EQ(orderUpdates[1].status, DatabaseLayer::ResponseType_FILLED);
}

TEST_F(TradingEngineTest, RiskManagerBasicValidation) {
    nlohmann::json orderRequest;
    orderRequest[JSON_TOKEN] = 12345;
    orderRequest[JSON_PRICE] = "100.50";
    orderRequest[JSON_QUANTITY] = 50;
    orderRequest[JSON_CLIENT] = "TestClient";
    orderRequest[JSON_SIDE] = static_cast<int>(DatabaseLayer::Side_BUY);
    orderRequest[JSON_ORDER_TYPE] = static_cast<int>(DatabaseLayer::RequestType_NEW);

    bool isValid = riskManager->validateOrder(orderRequest, "TestClient");
    EXPECT_TRUE(isValid);
}

TEST_F(TradingEngineTest, RiskManagerInvalidOrder) {
    nlohmann::json invalidOrder;
    invalidOrder[JSON_TOKEN] = 12345;
    invalidOrder[JSON_PRICE] = "-100.50";  // Negative price
    invalidOrder[JSON_QUANTITY] = 50;

    bool isValid = riskManager->validateOrder(invalidOrder, "TestClient");
    EXPECT_FALSE(isValid);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}