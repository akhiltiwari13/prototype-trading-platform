#include "OptionsGreeks/OptionsGreeks.hpp"
#include "OptionsGreeks/IVCalculator/BlackScholesModel.hpp"

#include <ctime>

namespace OptionsGreeks {

double GetDelta(double S, double K, double v, double r, double T, bool IsCall) {
    return IVCalculator::delta(S, K, r, v, T) - (not IsCall);
}

double GetGamma(double s, double k, double v, double r, double t, [[maybe_unused]] bool IsCall) {
    // Identical to call by put-call parity
    return IVCalculator::gamma(s, k, r, v, t) * 100.0;
}

double GetVega(double s, double k, double v, double r, double t, [[maybe_unused]] bool IsCall) {
    return IVCalculator::vega(s, k, r, v, t) / 10000.0;
}

double GetRho(double s, double k, double v, double r, double t, bool IsCall) {
    if (IsCall) {
        return IVCalculator::call_rho(s, k, r, v, t);
    } else {
        return IVCalculator::put_rho(s, k, r, v, t);
    }
}

double GetTheta(double s, double k, double v, double r, double t, bool IsCall) {
    if (IsCall) {
        return IVCalculator::call_theta(s, k, r, v, t) / 36500.0;
    } else {
        return IVCalculator::put_theta(s, k, r, v, t) / 36500.0;
    }
}

double GetIV(double S, double K, double r, double T, double P, bool IsCE) {
    return IVCalculator::option_price(S, K, r, T, P, IsCE);
}

double GetOptionPrice(double s, double k, double v, double r, double t, bool IsCall) {
    if (IsCall) {
        return IVCalculator::call_price(s, k, r, v, t);
    } else {
        return IVCalculator::put_price(s, k, r, v, t);
    }
}

double GetExpiryGap(uint32_t Time) {
    const long Cur_time = time(nullptr);
    const long expiry   = Time + 3600;
    const long diff     = (expiry + 315513000L - Cur_time);
    double     gap      = static_cast<double>(diff) / (365.25 * 24.0 * 60.0 * 60.0);
    return gap;
}

} // namespace OptionsGreeks