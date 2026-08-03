#include <iostream>
#include <cassert>
#include "heating.hpp"

using namespace thermal_evolution;

int main() {
    std::cout << "[Unit Test] Heating Model (Tidal & Ohmic)..." << std::endl;
    HeatingModel heating;
    double P_tidal = heating.compute_tidal_power(1.0 * M_JUP, 1.0 * M_SUN, 0.04 * AU, 0.20, 1.35 * R_JUP);
    assert(P_tidal > 1.0e18);

    double n = std::sqrt(G * M_SUN / std::pow(0.04 * AU, 3.0));
    double P_tidal_zero_e = heating.compute_tidal_power(1.0 * M_JUP, 1.0 * M_SUN, 0.04 * AU, 0.0, 1.35 * R_JUP, n, 0.0);
    assert(P_tidal_zero_e == 0.0);

    double P_ohmic_hot = heating.compute_ohmic_power(1.35 * R_JUP, 1.0e6);
    double P_ohmic_cool = heating.compute_ohmic_power(1.35 * R_JUP, 1.0e4);
    assert(P_ohmic_hot > P_ohmic_cool);

    std::cout << "  -> PASSED." << std::endl;
    return 0;
}
