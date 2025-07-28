#include <gtest/gtest.h>
#include <OptionsGreeks/OptionsGreeks.hpp>
#include <cmath>
#include <ctime>

class OptionsGreeksTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Standard test parameters
    S = 100.0; // Current stock price
    K = 100.0; // Strike price
    r = 0.05;  // Risk-free rate (5%)
    v = 0.2;   // Volatility (20%)
    T = 0.25;  // Time to expiration (3 months)
  }

  double S, K, r, v, T;
  const double tolerance = 1e-6;
};

TEST_F(OptionsGreeksTest, CallOptionPrice) {
  double callPrice = OptionsGreeks::GetOptionPrice(S, K, v, r, T, true);

  // For ATM call with these parameters, price should be around 4.0
  EXPECT_GT(callPrice, 3.0);
  EXPECT_LT(callPrice, 5.0);
  EXPECT_GT(callPrice, 0.0);
}

TEST_F(OptionsGreeksTest, PutOptionPrice) {
  double putPrice = OptionsGreeks::GetOptionPrice(S, K, v, r, T, false);

  // For ATM put with these parameters, price should be around 2.8
  EXPECT_GT(putPrice, 2.0);
  EXPECT_LT(putPrice, 4.0);
  EXPECT_GT(putPrice, 0.0);
}

TEST_F(OptionsGreeksTest, PutCallParity) {
  double callPrice = OptionsGreeks::GetOptionPrice(S, K, v, r, T, true);
  double putPrice = OptionsGreeks::GetOptionPrice(S, K, v, r, T, false);

  // Put-Call Parity: C - P = S - K*e^(-r*T)
  double leftSide = callPrice - putPrice;
  double rightSide = S - K * std::exp(-r * T);

  EXPECT_NEAR(leftSide, rightSide, tolerance);
}

TEST_F(OptionsGreeksTest, CallDelta) {
  double delta = OptionsGreeks::GetDelta(S, K, v, r, T, true);

  // ATM call delta should be around 0.5
  EXPECT_GT(delta, 0.4);
  EXPECT_LT(delta, 0.6);
}

TEST_F(OptionsGreeksTest, PutDelta) {
  double delta = OptionsGreeks::GetDelta(S, K, v, r, T, false);

  // ATM put delta should be around -0.5
  EXPECT_GT(delta, -0.6);
  EXPECT_LT(delta, -0.4);
}

TEST_F(OptionsGreeksTest, Gamma) {
  double callGamma = OptionsGreeks::GetGamma(S, K, v, r, T, true);
  double putGamma = OptionsGreeks::GetGamma(S, K, v, r, T, false);

  // Gamma should be the same for calls and puts
  EXPECT_NEAR(callGamma, putGamma, tolerance);
  EXPECT_GT(callGamma, 0.0);
}

TEST_F(OptionsGreeksTest, Vega) {
  double callVega = OptionsGreeks::GetVega(S, K, v, r, T, true);
  double putVega = OptionsGreeks::GetVega(S, K, v, r, T, false);

  // Vega should be the same for calls and puts
  EXPECT_NEAR(callVega, putVega, tolerance);
  EXPECT_GT(callVega, 0.0);
}

TEST_F(OptionsGreeksTest, CallRho) {
  double rho = OptionsGreeks::GetRho(S, K, v, r, T, true);

  // Call rho should be positive
  EXPECT_GT(rho, 0.0);
}

TEST_F(OptionsGreeksTest, PutRho) {
  double rho = OptionsGreeks::GetRho(S, K, v, r, T, false);

  // Put rho should be negative
  EXPECT_LT(rho, 0.0);
}

TEST_F(OptionsGreeksTest, CallTheta) {
  double theta = OptionsGreeks::GetTheta(S, K, v, r, T, true);

  // Call theta should be negative (time decay)
  EXPECT_LT(theta, 0.0);
}

TEST_F(OptionsGreeksTest, PutTheta) {
  double theta = OptionsGreeks::GetTheta(S, K, v, r, T, false);

  // Put theta should be negative (time decay)
  EXPECT_LT(theta, 0.0);
}

TEST_F(OptionsGreeksTest, ImpliedVolatility) {
  // First get a theoretical price
  double theoreticalPrice = OptionsGreeks::GetOptionPrice(S, K, v, r, T, true);

  // Then calculate implied volatility from that price
  double impliedVol = OptionsGreeks::GetIV(S, K, r, T, theoreticalPrice, true);

  // Should recover the original volatility
  EXPECT_NEAR(impliedVol, v, 0.01); // 1% tolerance for numerical precision
}

TEST_F(OptionsGreeksTest, ExpiryGap) {
  // Test with a future timestamp (1 hour from now)
  uint32_t futureTime = static_cast<uint32_t>(std::time(nullptr)) + 3600;
  double gap = OptionsGreeks::GetExpiryGap(futureTime);

  // Should be a small positive number (close to 0 for 1 hour)
  EXPECT_GT(gap, 0.0);
  EXPECT_LT(gap, 50.0); // Less than 50 years (reasonable upper bound)
}

TEST_F(OptionsGreeksTest, MoneynessBehavior) {
  // Test ITM call
  double itmCallDelta = OptionsGreeks::GetDelta(S, K - 10, v, r, T, true);
  EXPECT_GT(itmCallDelta, 0.5);

  // Test OTM call
  double otmCallDelta = OptionsGreeks::GetDelta(S, K + 10, v, r, T, true);
  EXPECT_LT(otmCallDelta, 0.5);

  // Test ITM put
  double itmPutDelta = OptionsGreeks::GetDelta(S, K + 10, v, r, T, false);
  EXPECT_LT(itmPutDelta, -0.5);

  // Test OTM put
  double otmPutDelta = OptionsGreeks::GetDelta(S, K - 10, v, r, T, false);
  EXPECT_GT(otmPutDelta, -0.5);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}