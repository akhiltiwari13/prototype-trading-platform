#include <gtest/gtest.h>
#include <OptionsGreeks/OptionsGreeks.hpp>

TEST(SimpleTest, BasicFunctionality) {
    double price = OptionsGreeks::GetOptionPrice(100.0, 100.0, 0.2, 0.05, 0.25, true);
    EXPECT_GT(price, 0.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
