#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

#include "constants.hpp"
#include "eos.hpp"
#include "interior.hpp"
#include "atmosphere.hpp"
#include "heating.hpp"
#include "orbital.hpp"
#include "mass_loss.hpp"
#include "multi_planet.hpp"

using namespace thermal_evolution;

// 1. Fundamental Constants Test
void test_constants() {
    std::cout << "[Test 01/16] Fundamental Physical Constants..." << std::endl;
    assert(G > 6.67e-11 && G < 6.68e-11);
    assert(M_SUN > 1.98e30);
    assert(R_SUN > 6.95e8);
    assert(M_JUP > 1.89e27);
    assert(R_JUP > 7.14e7);
    assert(M_EARTH > 5.97e24);
    assert(AU > 1.49e11);
    assert(KB > 1.38e-23);
    assert(HBAR > 1.05e-34);
    assert(SIGMA_SB > 5.67e-8);
    std::cout << "  -> PASSED." << std::endl;
}

// 2. Hydrogen-Helium Equation of State Test
void test_hhe_eos() {
    std::cout << "[Test 02/16] HydrogenHeliumEOS Pressure & Density..." << std::endl;
    HydrogenHeliumEOS eos;
    double rho_low = eos.density_from_PS(1.0 * BAR, 1.34e5);
    double rho_high = eos.density_from_PS(1.0e11, 1.34e5);
    assert(rho_high > rho_low);

    double T_env = eos.temperature_from_PS(1.0e7, 1.34e5);
    assert(T_env > 10.0);
    std::cout << "  -> PASSED." << std::endl;
}

// 3. Birch-Murnaghan Core Equation of State Test
void test_core_eos() {
    std::cout << "[Test 03/16] BirchMurnaghanCoreEOS..." << std::endl;
    BirchMurnaghanCoreEOS core_eos;
    double rho_0 = core_eos.rho_0;
    double rho_p1 = core_eos.density(1.0e10);
    double rho_p2 = core_eos.density(1.0e11);

    assert(rho_p1 > rho_0);
    assert(rho_p2 > rho_p1);
    std::cout << "  -> PASSED." << std::endl;
}

// 4. 1D Hydrostatic Interior Solver Test
void test_interior_solver() {
    std::cout << "[Test 04/16] InteriorSolver 1D Hydrostatic Structure..." << std::endl;
    InteriorSolver solver;
    PlanetStructure st = solver.solve_structure(1.0 * M_JUP, 12.0 * M_EARTH, 1.34e5);
    assert(st.R_p > 0.5 * R_JUP && st.R_p < 2.5 * R_JUP);
    assert(st.P_c > 1.0e4);
    assert(st.T_c > 100.0);
    assert(st.rho.size() > 10);
    std::cout << "  -> PASSED." << std::endl;
}

// 5. Time-Varying Stellar Luminosity Test
void test_stellar_luminosity() {
    std::cout << "[Test 05/16] TimeVaryingStellarLuminosity..." << std::endl;
    TimeVaryingStellarLuminosity lum;
    double L_early = lum.luminosity_at_time(0.1 * GYR);
    double L_present = lum.luminosity_at_time(4.56 * GYR);
    assert(L_present > L_early);

    double F_inc = lum.incident_flux(1.0 * AU, 4.56 * GYR);
    assert(F_inc > 1000.0 && F_inc < 1500.0);
    std::cout << "  -> PASSED." << std::endl;
}

// 6. Guillot Atmosphere Radiative Boundary Test
void test_guillot_atmosphere() {
    std::cout << "[Test 06/16] GuillotAtmosphere Radiative Boundary..." << std::endl;
    GuillotAtmosphere atm;
    double F_inc = 1.0e5; // W/m^2
    double T_irr = atm.T_irr_from_flux(F_inc, atm.A_b);
    assert(T_irr > 500.0);

    double T_surface = atm.T_at_tau(2.0 / 3.0, 100.0, T_irr);
    double T_deep = atm.T_at_tau(10.0, 100.0, T_irr);
    assert(T_deep > T_surface);
    std::cout << "  -> PASSED." << std::endl;
}

// 7. JWST Atmospheric Scale Height Test
void test_scale_height() {
    std::cout << "[Test 07/16] GuillotAtmosphere Scale Height..." << std::endl;
    GuillotAtmosphere atm;
    double H_m = atm.compute_scale_height(1500.0, 1.0 * M_JUP, 1.4 * R_JUP);
    assert(H_m > 300.0 * 1000.0 && H_m < 1200.0 * 1000.0);
    std::cout << "  -> PASSED." << std::endl;
}

// 8. JWST Spectroscopic Transit Depth Signal Test
void test_transit_depth_signal() {
    std::cout << "[Test 08/16] GuillotAtmosphere Transit Depth Variation..." << std::endl;
    GuillotAtmosphere atm;
    double H_base = atm.compute_scale_height(1500.0, 1.0 * M_JUP, 1.0 * R_JUP);
    double H_inflated = atm.compute_scale_height(1500.0, 1.0 * M_JUP, 1.4 * R_JUP);

    double ppm_base = atm.compute_transit_depth_variation_ppm(1.0 * R_JUP, 1.0 * R_SUN, H_base, 5);
    double ppm_inflated = atm.compute_transit_depth_variation_ppm(1.4 * R_JUP, 1.0 * R_SUN, H_inflated, 5);

    assert(ppm_inflated > ppm_base);
    std::cout << "  -> PASSED." << std::endl;
}

// 9. Quadrupolar Tidal Dissipation Power Test
void test_tidal_power() {
    std::cout << "[Test 09/16] HeatingModel Tidal Power..." << std::endl;
    HeatingModel heating;
    double P_tidal = heating.compute_tidal_power(1.0 * M_JUP, 1.0 * M_SUN, 0.04 * AU, 0.20, 1.35 * R_JUP);
    assert(P_tidal > 1.0e18);

    double n = std::sqrt(G * M_SUN / std::pow(0.04 * AU, 3.0));
    double P_tidal_zero_e = heating.compute_tidal_power(1.0 * M_JUP, 1.0 * M_SUN, 0.04 * AU, 0.0, 1.35 * R_JUP, n, 0.0);
    assert(P_tidal_zero_e == 0.0);
    std::cout << "  -> PASSED." << std::endl;
}

// 10. MHD Ohmic Dissipation Power Test
void test_ohmic_power() {
    std::cout << "[Test 10/16] HeatingModel Ohmic Power..." << std::endl;
    HeatingModel heating;
    double P_ohmic_hot = heating.compute_ohmic_power(1.35 * R_JUP, 1.0e6);
    double P_ohmic_cool = heating.compute_ohmic_power(1.35 * R_JUP, 1.0e4);

    assert(P_ohmic_hot > P_ohmic_cool);
    std::cout << "  -> PASSED." << std::endl;
}

// 11. Tidal Orbital & Spin Rates Test
void test_orbital_spin_rates() {
    std::cout << "[Test 11/16] TidalOrbitalSpinRates..." << std::endl;
    TidalOrbitalSpinRates rates;
    double n = std::sqrt(G * M_SUN / std::pow(0.04 * AU, 3.0));
    auto [da_dt, de_dt, dOmega_dt, dobl_dt] = rates.evaluate_rates(1.0 * M_JUP, 1.35 * R_JUP, 1.0 * M_SUN, 0.04 * AU, 0.20, n, 0.20);

    assert(de_dt < 0.0);
    assert(dobl_dt < 0.0);
    std::cout << "  -> PASSED." << std::endl;
}

// 12. Stellar Rotation Tidal Migration Test
void test_stellar_tidal_rates() {
    std::cout << "[Test 12/16] StellarTidalRates (Sub vs Super-synchronous)..." << std::endl;
    StellarTidalRates star_rates;
    double Omega_sub = 2.0 * M_PI / (25.0 * DAY); // n > Omega_*
    double Omega_super = 2.0 * M_PI / (1.5 * DAY); // n < Omega_*

    auto [da_dt_sub, dOmega_sub] = star_rates.evaluate_stellar_rates(1.0 * M_JUP, 1.0 * M_SUN, 0.03 * AU, Omega_sub);
    auto [da_dt_super, dOmega_super] = star_rates.evaluate_stellar_rates(1.0 * M_JUP, 1.0 * M_SUN, 0.03 * AU, Omega_super);

    assert(da_dt_sub < 0.0);   // Inward decay
    assert(da_dt_super > 0.0); // Outward expansion
    std::cout << "  -> PASSED." << std::endl;
}

// 13. Eggleton Roche Lobe Radius & Filling Factor Test
void test_roche_lobe_radius() {
    std::cout << "[Test 13/16] RocheLobeMassLoss Radius & Filling Factor..." << std::endl;
    RocheLobeMassLoss rlof;
    double r_roche = RocheLobeMassLoss::roche_lobe_radius(0.02 * AU, 1.0 * M_JUP, 1.0 * M_SUN);
    assert(r_roche > 0.0 && r_roche < 0.02 * AU);

    double fill_under = rlof.roche_lobe_filling_factor(0.8 * r_roche, 0.02 * AU, 1.0 * M_JUP, 1.0 * M_SUN);
    double fill_over = rlof.roche_lobe_filling_factor(1.1 * r_roche, 0.02 * AU, 1.0 * M_JUP, 1.0 * M_SUN);
    assert(fill_under < 1.0);
    assert(fill_over > 1.0);
    std::cout << "  -> PASSED." << std::endl;
}

// 14. Energy-Limited XUV Photoevaporative Mass Loss Test
void test_photoevaporative_mdot() {
    std::cout << "[Test 14/16] RocheLobeMassLoss XUV Photoevaporation..." << std::endl;
    RocheLobeMassLoss rlof;
    double dM_dt_xuv = rlof.compute_photoevaporative_mdot(10.0, 1.4 * R_JUP, 1.0 * M_JUP);
    assert(dM_dt_xuv < 0.0);
    assert(std::abs(dM_dt_xuv) > 1.0e7); // kg/s
    std::cout << "  -> PASSED." << std::endl;
}

// 15. Hydrodynamic RLOF Mass Loss & Angular Momentum Feedback Test
void test_rlof_mass_loss() {
    std::cout << "[Test 15/16] RocheLobeMassLoss RLOF & Angular Momentum Feedback..." << std::endl;
    RocheLobeMassLoss rlof;
    double r_roche = RocheLobeMassLoss::roche_lobe_radius(0.018 * AU, 1.0 * M_JUP, 1.0 * M_SUN);
    auto [dM_dt, da_dt] = rlof.evaluate_mass_loss_rate(1.05 * r_roche, 0.018 * AU, 1.0 * M_JUP, 1.0 * M_SUN, 5.0);

    assert(dM_dt < 0.0);
    assert(da_dt != 0.0);
    std::cout << "  -> PASSED." << std::endl;
}

// 16. Multi-Planet Secular Interaction Matrix Test
void test_secular_matrix() {
    std::cout << "[Test 16/16] MultiPlanetSystem Secular Interaction Matrix..." << std::endl;
    MultiPlanetSystem system;
    PlanetSystemMember b, c, d;
    b.M_p = 1.0 * M_JUP; b.a = 0.04 * AU; b.e = 0.15;
    c.M_p = 0.3 * M_JUP; c.a = 0.12 * AU; c.e = 0.08;
    d.M_p = 1.5 * M_JUP; d.a = 0.50 * AU; d.e = 0.04;
    system.planets = {b, c, d};

    auto matrix = system.compute_secular_matrix();
    assert(matrix.size() == 3);
    assert(matrix[0].size() == 3);

    auto de_dt = system.evaluate_secular_de_dt();
    assert(de_dt.size() == 3);
    std::cout << "  -> PASSED." << std::endl;
}

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << " RUNNING EXHAUSTIVE C++ SUITE UNIT TESTS (16/16)                          " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    test_constants();
    test_hhe_eos();
    test_core_eos();
    test_interior_solver();
    test_stellar_luminosity();
    test_guillot_atmosphere();
    test_scale_height();
    test_transit_depth_signal();
    test_tidal_power();
    test_ohmic_power();
    test_orbital_spin_rates();
    test_stellar_tidal_rates();
    test_roche_lobe_radius();
    test_photoevaporative_mdot();
    test_rlof_mass_loss();
    test_secular_matrix();

    std::cout << "==========================================================================" << std::endl;
    std::cout << " ALL 16 EXHAUSTIVE UNIT TESTS PASSED CLEANLY!                             " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    return 0;
}
