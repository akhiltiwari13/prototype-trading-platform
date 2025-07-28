#include "OptionsGreeks/IVCalculator/BlackScholesModel.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace OptionsGreeks::IVCalculator {

// Constants for better readability
inline constexpr double SQUARE = 2.0;
inline constexpr double HALF   = 0.5;

/**
 * @brief Standard normal probability density function
 */
double norm_pdf(double x) {
    return 1.0 / (std::sqrt(SQUARE * M_PI)) * std::exp(-HALF * std::pow(x, SQUARE));
}

/**
 * @brief Cumulative distribution function for standard normal distribution
 */
double norm_cdf(double x) {
    return HALF * (1.0 + erf(x / sqrt(SQUARE)));
}

/**
 * @brief Calculate d_j term for Black-Scholes formula
 * @param j Index (1 or 2)
 * @param S Current asset price
 * @param K Strike price
 * @param r Risk-free rate
 * @param v Volatility
 * @param T Time to expiration
 */
double d_j(int j, double S, double K, double r, double v, double T) {
    return (std::log(S / K) + (r + pow(-1, j - 1) * HALF * v * v) * T) / (v * sqrt(T));
}

double delta(double S, double K, double r, double v, double T) {
    return norm_cdf(d_j(1, S, K, r, v, T));
}

double call_price(double S, double K, double r, double v, double T) {
    return S * norm_cdf(d_j(1, S, K, r, v, T)) - K * exp(-r * T) * norm_cdf(d_j(2, S, K, r, v, T));
}

double put_price(double S, double K, double r, double v, double T) {
    return -S * norm_cdf(-d_j(1, S, K, r, v, T)) + K * exp(-r * T) * norm_cdf(-d_j(2, S, K, r, v, T));
}

double gamma(double S, double K, double r, double v, double T) {
    return norm_pdf(d_j(1, S, K, r, v, T)) / (S * v * sqrt(T));
}

double vega(double S, double K, double r, double v, double T) { 
    return S * norm_pdf(d_j(1, S, K, r, v, T)) * sqrt(T); 
}

double call_theta(double S, double K, double r, double v, double T) {
    return -(S * norm_pdf(d_j(1, S, K, r, v, T)) * v) / (2 * sqrt(T)) - r * K * exp(-r * T) * norm_cdf(d_j(2, S, K, r, v, T));
}

double call_rho(double S, double K, double r, double v, double T) {
    return K * T * exp(-r * T) * norm_cdf(d_j(2, S, K, r, v, T));
}

double put_rho(double S, double K, double r, double v, double T) {
    return -K * T * exp(-r * T) * norm_cdf(-d_j(2, S, K, r, v, T));
}

double put_theta(double S, double K, double r, double v, double T) {
    return -(S * norm_pdf(d_j(1, S, K, r, v, T)) * v) / (2 * sqrt(T)) + r * K * exp(-r * T) * norm_cdf(-d_j(2, S, K, r, v, T));
}

template<typename Function>
double ImpliedVolatility(Function func, double S, double K, double r, double T, double P) {
    double High = 5;
    double Low  = 0;
    double Mid  = (High + Low) / 2.0;
    do {
        if (func(S, K, r, Mid, T) > P) {
            High = Mid;
        } else {
            Low = Mid;
        }
        Mid = (High + Low) / 2.0;
    } while ((High - Low) > 0.0001);

    return Mid;
}

using FunctionList = double (*)(double S, double K, double r, double v, double T);
constexpr FunctionList List[] = { put_price, call_price };

double option_price(double S, double K, double r, double T, double P, bool IsCE) {
    if (S <= 0) return 0.0;
    return ImpliedVolatility(List[IsCE], S, K, r, T, P);
}

} // namespace OptionsGreeks::IVCalculator