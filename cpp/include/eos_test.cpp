#include <iostream>
#include <cassert>
#include "eos.hpp"

using namespace thermal_evolution;

int main() {
    std::cout << "[Unit Test] Equation of State (H/He & Core)..." << std::endl;
    HydrogenHeliumEOS eos;
    double rho_low = eos.density_from_PS(1.0 * BAR, 1.34e5);
    double rho_high = eos.density_from_PS(1.0e11, 1.34e5);
    assert(rho_high > rho_low);

    double T_env = eos.temperature_from_PS(1.0e7, 1.34e5);
    assert(T_env > 10.0);

    BirchMurnaghanCoreEOS core_eos;
    double rho_0 = core_eos.rho_0;
    double rho_p1 = core_eos.density(1.0e10);
    double rho_p2 = core_eos.density(1.0e11);
    assert(rho_p1 > rho_0);
    assert(rho_p2 > rho_p1);

    std::cout << "  -> PASSED." << std::endl;
    return 0;
}
