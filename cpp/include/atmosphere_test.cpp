#include <iostream>
#include <cassert>
#include "atmosphere.hpp"

using namespace hot_jupiter;

int main() {
    std::cout << "[Unit Test] Atmosphere & JWST Scale Height..." << std::endl;
    TimeVaryingStellarLuminosity lum;
    double L_early = lum.luminosity_at_time(0.1 * GYR);
    double L_present = lum.luminosity_at_time(4.56 * GYR);
    assert(L_present > L_early);

    GuillotAtmosphere atm;
    double F_inc = 1.0e5;
    double T_irr = atm.T_irr_from_flux(F_inc, atm.A_b);
    assert(T_irr > 500.0);

    double H_base = atm.compute_scale_height(1500.0, 1.0 * M_JUP, 1.0 * R_JUP);
    double H_inflated = atm.compute_scale_height(1500.0, 1.0 * M_JUP, 1.4 * R_JUP);
    assert(H_inflated > H_base);

    double ppm_base = atm.compute_transit_depth_variation_ppm(1.0 * R_JUP, 1.0 * R_SUN, H_base, 5);
    double ppm_inflated = atm.compute_transit_depth_variation_ppm(1.4 * R_JUP, 1.0 * R_SUN, H_inflated, 5);
    assert(ppm_inflated > ppm_base);

    std::cout << "  -> PASSED." << std::endl;
    return 0;
}
