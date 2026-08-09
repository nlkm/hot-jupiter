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

void test_c_api_rlof_integrate() {
    double t_arr[100], a_arr[100], e_arr[100], m_p_arr[100], r_p_arr[100], ff_arr[100];
    C_TrajectoryResult res;

    rlof_integrate_trajectory_c(1.0, 0.035, 10.0, 1.0, 1.0e9, 100,
                                t_arr, a_arr, e_arr, m_p_arr, r_p_arr, ff_arr, &res);

    assert(res.outcome == 2);  // COOLING
    assert(res.final_m_remnant_earth > 0.0);
    assert(res.num_pts_returned == 100);
    assert(t_arr[0] == 1.0e6);
    assert(a_arr[0] > 0.030);
    std::cout << "[PASS] C-API RLOF Integrate Trajectory Test" << std::endl;
}

void test_c_api_solve_interior_profile_detailed() {
    double r[300], m[300], P[300], rho[300], T[300], nad[300];
    C_PlanetStructureResult res;

    solve_interior_profile_detailed_c(
        1.0 * hot_jupiter::M_JUP, 10.0 * hot_jupiter::M_EARTH, 1.0e8, 1.0 * hot_jupiter::BAR, 300, 0.0, 1.0,
        r, m, P, rho, T, nad, &res
    );

    assert(res.num_layers == 300);
    assert(r[0] > 0.5 * hot_jupiter::R_JUP);
    assert(rho[0] > 0.0);
    assert(P[299] > 0.0);
    std::cout << "[PASS] C-API Detailed Interior Profile Test" << std::endl;
}

int main() {
    test_c_api_solve_structure();
    test_c_api_evaluate_density();
    test_c_api_rlof_integrate();
    test_c_api_solve_interior_profile_detailed();
    std::cout << "All C-API unit tests passed successfully!" << std::endl;
    return 0;
}
