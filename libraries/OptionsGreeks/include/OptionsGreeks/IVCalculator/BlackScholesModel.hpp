#pragma once

namespace OptionsGreeks::IVCalculator {

/**
 * @brief Calculate gamma for European options
 */
double gamma(double S, double K, double r, double v, double T);

/**
 * @brief Calculate vega for European options
 */
double vega(double S, double K, double r, double v, double T);

/**
 * @brief Calculate delta for European options
 */
double delta(double S, double K, double r, double v, double T);

/**
 * @brief Calculate European call option price
 */
double call_price(double S, double K, double r, double v, double T);

/**
 * @brief Calculate European call option theta
 */
double call_theta(double S, double K, double r, double v, double T);

/**
 * @brief Calculate European call option rho
 */
double call_rho(double S, double K, double r, double v, double T);

/**
 * @brief Calculate European put option price
 */
double put_price(double S, double K, double r, double v, double T);

/**
 * @brief Calculate European put option theta
 */
double put_theta(double S, double K, double r, double v, double T);

/**
 * @brief Calculate European put option rho
 */
double put_rho(double S, double K, double r, double v, double T);

/**
 * @brief Calculate implied volatility from option price
 */
double option_price(double S, double K, double r, double T, double P, bool IsCE);

} // namespace OptionsGreeks::IVCalculator