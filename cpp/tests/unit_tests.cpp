#include <iostream>
#include <cassert>
#include <cmath>

#include "constants.hpp"
#include "eos.hpp"
#include "interior.hpp"
#include "atmosphere.hpp"
#include "heating.hpp"
#include "orbital.hpp"
#include "mass_loss.hpp"
#include "multi_planet.hpp"

using namespace thermal_evolution;

void test_eos() {
    std::cout << "[Test 1/8] HydrogenHeliumEOS & BirchMurnaghanCoreEOS..." << std::endl;
    HydrogenHeliumEOS eos;
    BirchMurnaghanCoreEOS core_eos;

    double rho1 = eos.density_from_PS(1.0 * BAR, 1.34e5);
    double rho2 = eos.density_from_PS(1.0e11, 1.34e5);
    assert(rho2 > rho1);

    double rho_core = core_eos.density(1.0e11);
    assert(rho_core > core_eos.rho_0);
    std::cout << "  -> PASSED." << std::endl;
}

void test_interior_solver() {
    std::cout << "[Test 2/8] InteriorSolver 1D Hydrostatic Solution..." << std::endl;
    InteriorSolver solver;
    PlanetStructure st = solver.solve_structure(1.0 * M_JUP, 12.0 * M_EARTH, 1.34e5);
    assert(st.R_p > 0.5 * R_JUP && st.R_p < 2.5 * R_JUP);
    assert(st.P_c > 0.0);
    std::cout << "  -> PASSED." << std::endl;
}

void test_atmosphere() {
    std::cout << "[Test 3/8] GuillotAtmosphere Radiative Profile..." << std::endl;
    GuillotAtmosphere atm;
    double T_irr = atm.T_irr_from_flux(1.0e5, 0.34);
    double T_surf = atm.T_at_tau(2.0 / 3.0, 100.0, T_irr);
    assert(T_surf > 100.0);
    std::cout << "  -> PASSED." << std::endl;
}

void test_heating() {
    std::cout << "[Test 4/8] HeatingModel (Tidal & Ohmic Dissipation)..." << std::endl;
    HeatingModel heating;
    double P_tide = heating.compute_tidal_power(1.0 * M_JUP, 1.0 * M_SUN, 0.04 * AU, 0.2, 1.3 * R_JUP);
    assert(P_tide > 1.0e18);

    double P_ohmic = heating.compute_ohmic_power(1.3 * R_JUP, 1.0e5);
    assert(P_ohmic > 0.0);
    std::cout << "  -> PASSED." << std::endl;
}

void test_orbital_rates() {
    std::cout << "[Test 5/8] TidalOrbitalSpinRates (Coupled Dynamics)..." << std::endl;
    TidalOrbitalSpinRates rates;
    double n = std::sqrt(G * M_SUN / (0.04 * AU * 0.04 * AU * 0.04 * AU));
    auto [da_dt, de_dt, dOmega_dt, dobl_dt] = rates.evaluate_rates(1.0 * M_JUP, 1.3 * R_JUP, 1.0 * M_SUN, 0.04 * AU, 0.2, n, 0.0);
    assert(de_dt < 0.0);
    std::cout << "  -> PASSED." << std::endl;
}

void test_stellar_tides() {
    std::cout << "[Test 6/8] StellarTidalRates..." << std::endl;
    StellarTidalRates star_rates;
    auto [da_dt_sub, _] = star_rates.evaluate_stellar_rates(1.0 * M_JUP, 1.0 * M_SUN, 0.03 * AU, 2.9e-6);
    assert(da_dt_sub < 0.0);
    std::cout << "  -> PASSED." << std::endl;
}

void test_mass_loss() {
    std::cout << "[Test 7/8] RocheLobeMassLoss..." << std::endl;
    RocheLobeMassLoss rlof;
    double r_roche = RocheLobeMassLoss::roche_lobe_radius(0.018 * AU, 1.0 * M_JUP, 1.0 * M_SUN);
    assert(r_roche > 0.0);

    auto [dM_dt, da_dt] = rlof.evaluate_mass_loss_rate(1.1 * r_roche, 0.018 * AU, 1.0 * M_JUP, 1.0 * M_SUN);
    assert(dM_dt < 0.0);
    std::cout << "  -> PASSED." << std::endl;
}

void test_multi_planet() {
    std::cout << "[Test 8/8] MultiPlanetSystem Secular Perturbations..." << std::endl;
    MultiPlanetSystem system;
    PlanetSystemMember p1, p2;
    p1.M_p = 1.0 * M_JUP; p1.a = 0.05 * AU; p1.e = 0.15;
    p2.M_p = 0.3 * M_JUP; p2.a = 0.18 * AU; p2.e = 0.10;
    system.planets = {p1, p2};

    auto de_dt = system.evaluate_secular_de_dt();
    assert(de_dt.size() == 2);
    std::cout << "  -> PASSED." << std::endl;
}

int main() {
    std::cout << "==========================================================" << std::endl;
    std::cout << "        RUNNING C++ PHYSICAL SUITE UNIT TESTS             " << std::endl;
    std::cout << "==========================================================" << std::endl;

    test_eos();
    test_interior_solver();
    test_atmosphere();
    test_heating();
    test_orbital_rates();
    test_stellar_tides();
    test_mass_loss();
    test_multi_planet();

    std::cout << "==========================================================" << std::endl;
    std::cout << "      ALL C++ UNIT TESTS PASSED CLEANLY (8/8 SUCCESS)     " << std::endl;
    std::cout << "==========================================================" << std::endl;

    return 0;
}
