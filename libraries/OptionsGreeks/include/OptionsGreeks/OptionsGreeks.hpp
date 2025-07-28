#pragma once

#include <cstdint>

namespace OptionsGreeks {

/**
 * @brief Calculate option delta (price sensitivity to underlying asset price)
 * @param S Current asset price
 * @param K Strike price
 * @param v Volatility
 * @param r Risk-free rate
 * @param T Time to expiration
 * @param IsCall True for call option, false for put option
 * @return Delta value
 */
double GetDelta(double S, double K, double v, double r, double T, bool IsCall);

/**
 * @brief Calculate option gamma (delta sensitivity to underlying asset price)
 * @param S Current asset price
 * @param K Strike price
 * @param v Volatility
 * @param r Risk-free rate
 * @param T Time to expiration
 * @param IsCall True for call option, false for put option
 * @return Gamma value
 */
double GetGamma(double S, double K, double v, double r, double T, bool IsCall);

/**
 * @brief Calculate option vega (price sensitivity to volatility)
 * @param S Current asset price
 * @param K Strike price
 * @param v Volatility
 * @param r Risk-free rate
 * @param T Time to expiration
 * @param IsCall True for call option, false for put option
 * @return Vega value
 */
double GetVega(double S, double K, double v, double r, double T, bool IsCall);

/**
 * @brief Calculate option rho (price sensitivity to interest rate)
 * @param S Current asset price
 * @param K Strike price
 * @param v Volatility
 * @param r Risk-free rate
 * @param T Time to expiration
 * @param IsCall True for call option, false for put option
 * @return Rho value
 */
double GetRho(double S, double K, double v, double r, double T, bool IsCall);

/**
 * @brief Calculate option theta (price sensitivity to time decay)
 * @param S Current asset price
 * @param K Strike price
 * @param v Volatility
 * @param r Risk-free rate
 * @param T Time to expiration
 * @param IsCall True for call option, false for put option
 * @return Theta value
 */
double GetTheta(double S, double K, double v, double r, double T, bool IsCall);

/**
 * @brief Calculate implied volatility from option price
 * @param s Current asset price
 * @param K Strike price
 * @param r Risk-free rate
 * @param t Time to expiration
 * @param p Option price
 * @param IsCall True for call option, false for put option
 * @return Implied volatility
 */
double GetIV(double s, double K, double r, double t, double p, bool IsCall);

/**
 * @brief Calculate theoretical option price using Black-Scholes
 * @param S Current asset price
 * @param K Strike price
 * @param v Volatility
 * @param r Risk-free rate
 * @param t Time to expiration
 * @param IsCall True for call option, false for put option
 * @return Theoretical option price
 */
double GetOptionPrice(double S, double K, double v, double r, double t, bool IsCall);

/**
 * @brief Calculate time gap until expiry
 * @param Time Expiration time as uint32_t
 * @return Time gap in years
 */
double GetExpiryGap(uint32_t Time);

} // namespace OptionsGreeks