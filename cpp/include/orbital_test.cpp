#include <iostream>
#include <cassert>
#include "orbital.hpp"

using namespace hot_jupiter;

int main() {
    std::cout << "[Unit Test] Orbital & Spin Rates..." << std::endl;
    TidalOrbitalSpinRates rates;
    double n = std::sqrt(G * M_SUN / std::pow(0.04 * AU, 3.0));
    auto [da_dt, de_dt, dOmega_dt, dobl_dt] = rates.evaluate_rates(1.0 * M_JUP, 1.35 * R_JUP, 1.0 * M_SUN, 0.04 * AU, 0.20, n, 0.20);

    assert(de_dt < 0.0);
    assert(dobl_dt < 0.0);

    StellarTidalRates star_rates;
    double Omega_sub = 2.0 * M_PI / (25.0 * DAY);
    double Omega_super = 2.0 * M_PI / (1.5 * DAY);

    auto [da_dt_sub, dOmega_sub] = star_rates.evaluate_stellar_rates(1.0 * M_JUP, 1.0 * M_SUN, 0.03 * AU, Omega_sub);
    auto [da_dt_super, dOmega_super] = star_rates.evaluate_stellar_rates(1.0 * M_JUP, 1.0 * M_SUN, 0.03 * AU, Omega_super);

    assert(da_dt_sub < 0.0);
    assert(da_dt_super > 0.0);

    std::cout << "  -> PASSED." << std::endl;
    return 0;
}
