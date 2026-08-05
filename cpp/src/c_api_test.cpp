#include <iostream>
#include <cassert>
#include <cmath>

#include "c_api.h"
#include "constants.hpp"

void test_c_api_solve_structure() {
    double M_p = 1.0 * hot_jupiter::M_JUP;
    double M_c = 10.0 * hot_jupiter::M_EARTH;
    double S_env = 1.0e8;
    double P_surf = 1.0 * hot_jupiter::BAR;

    C_PlanetStructureResult res;
    solve_planet_structure_c(M_p, M_c, S_env, P_surf, &res);

    assert(res.R_p > 0.5 * hot_jupiter::R_JUP);
    assert(res.R_p < 3.0 * hot_jupiter::R_JUP);
    assert(res.num_layers == 300);
    assert(res.P_center > 0.0);
    assert(res.T_center > 0.0);
    std::cout << "[PASS] C-API Solve Structure Test" << std::endl;
}

void test_c_api_evaluate_density() {
    double P = 1.0e10; // 10 GPa
    double T = 5000.0;
    double rho = evaluate_saumon_chabrier_density_c(P, T, 0.75);

    assert(rho > 100.0);
    assert(rho < 10000.0);
    std::cout << "[PASS] C-API Evaluate Density Test" << std::endl;
}

int main() {
    test_c_api_solve_structure();
    test_c_api_evaluate_density();
    std::cout << "All C-API unit tests passed successfully!" << std::endl;
    return 0;
}
