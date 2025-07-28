#include <gtest/gtest.h>
#include <DatabaseLayer/DatabaseLayer.hpp>
#include <filesystem>
#include <fstream>

class DatabaseLayerTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDbPath = "test_contracts.db";
        // Clean up any existing test database
        if (std::filesystem::exists(testDbPath)) {
            std::filesystem::remove(testDbPath);
        }
    }

    void TearDown() override {
        // Clean up test database
        if (std::filesystem::exists(testDbPath)) {
            std::filesystem::remove(testDbPath);
        }
    }

    std::string testDbPath;
};

TEST_F(DatabaseLayerTest, ContractFetcherCreation) {
    EXPECT_NO_THROW({
        DatabaseLayer::ContractFetcher fetcher(testDbPath);
    });
}

TEST_F(DatabaseLayerTest, BasicDatabaseOperations) {
    DatabaseLayer::ContractFetcher fetcher(testDbPath);
    
    // Test basic query execution
    EXPECT_NO_THROW({
        fetcher.ExecuteQuery("CREATE TABLE test (id INTEGER, name TEXT)");
        fetcher.ExecuteQuery("INSERT INTO test VALUES (1, 'test')");
    });
    
    // Test query with results
    auto results = fetcher.GetResultWithColumnName("SELECT * FROM test");
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0]["id"], "1");
    EXPECT_EQ(results[0]["name"], "test");
}

TEST_F(DatabaseLayerTest, ContractInsertion) {
    DatabaseLayer::ContractFetcher fetcher(testDbPath);
    
    DatabaseLayer::TableWithColumnIndexT testData = {
        {"12345", "NIFTY", "NIFTY 50", "NSE", "FUTURE", "NONE", "0", "1640995200", "50"},
        {"12346", "BANKNIFTY", "BANK NIFTY", "NSE", "FUTURE", "NONE", "0", "1640995200", "25"}
    };
    
    EXPECT_NO_THROW({
        fetcher.Insert(testData);
    });
    
    // Verify insertion
    auto results = fetcher.GetResultWithColumnName("SELECT COUNT(*) as count FROM contracts");
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0]["count"], "2");
}

TEST_F(DatabaseLayerTest, EnumConversions) {
    EXPECT_EQ(DatabaseLayer::ToString(DatabaseLayer::Exchange_NSE_EQUITY), "NSE_EQUITY");
    EXPECT_EQ(DatabaseLayer::ToString(DatabaseLayer::Instrument_OPTION), "Instrument_OPTION");
    EXPECT_EQ(DatabaseLayer::ToString(DatabaseLayer::Side_BUY), "Side_BUY");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}