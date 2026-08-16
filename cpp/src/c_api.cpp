#include "c_api.h"
#include "interior.hpp"
#include "eos.hpp"
#include "rlof_engine.hpp"
#include "population_synth.hpp"

extern "C" {

void solve_planet_structure_c(
    double M_p_kg, double M_c_kg, double S_env, double P_surf,
    C_PlanetStructureResult* out_result
) {
    if (!out_result) return;
    
    hot_jupiter::InteriorSolver solver;
    hot_jupiter::PlanetStructure sys = solver.solve_structure(M_p_kg, M_c_kg, S_env, P_surf);
    
    out_result->R_p = sys.R_p;
    out_result->M_p = sys.M_p;
    out_result->M_c = sys.M_c;
    out_result->S_env = sys.S_env;
    out_result->T_center = sys.T_c;
    out_result->P_center = sys.P_c;
    out_result->rho_center = sys.rho.empty() ? 0.0 : sys.rho.back();
    out_result->num_layers = sys.r.size();
}

double evaluate_saumon_chabrier_density_c(double P_pascal, double T_kelvin, double X) {
    hot_jupiter::HydrogenHeliumEOS eos;
    eos.X = X;
    eos.Y = 1.0 - X;
    return eos.density_from_PT(P_pascal, T_kelvin);
}

void rlof_integrate_trajectory_c(
    double m_p_init_jup,
    double a_init_au,
    double m_core_earth,
    double m_star_sun,
    double t_max_yr,
    int num_pts,
    double* out_t_arr,
    double* out_a_arr,
    double* out_e_arr,
    double* out_m_p_arr,
    double* out_r_p_arr,
    double* out_ff_arr,
    C_TrajectoryResult* out_result
) {
    hot_jupiter::CoupledRLOFIntegrator integrator(m_p_init_jup, a_init_au, m_core_earth, m_star_sun, 0.15);
    auto res = integrator.integrate(t_max_yr, num_pts);

    if (out_result) {
        out_result->final_m_remnant_earth = res.final_m_remnant_earth;
        out_result->z_bulk = res.z_bulk;
        out_result->outcome = static_cast<int>(res.outcome);
        out_result->num_pts_returned = static_cast<int>(res.t_arr.size());
    }

    for (size_t i = 0; i < res.t_arr.size() && i < static_cast<size_t>(num_pts); ++i) {
        if (out_t_arr) out_t_arr[i] = res.t_arr[i];
        if (out_a_arr) out_a_arr[i] = res.a_arr[i];
        if (out_e_arr) out_e_arr[i] = res.e_arr[i];
        if (out_m_p_arr) out_m_p_arr[i] = res.m_p_arr[i];
        if (out_r_p_arr) out_r_p_arr[i] = res.r_p_arr[i];
        if (out_ff_arr) out_ff_arr[i] = res.filling_factor_arr[i];
    }
}

void rlof_sweep_grid_c(
    const double* m_grid,
    int n_m,
    const double* a_grid,
    int n_a,
    double m_core_earth,
    double m_star_sun,
    double t_max_yr,
    int num_pts,
    int* out_outcomes_matrix
) {
    if (!m_grid || !a_grid || !out_outcomes_matrix) return;
    for (int i = 0; i < n_m; ++i) {
        for (int j = 0; j < n_a; ++j) {
            double mp_val = m_grid[i];
            double a_val = a_grid[j];
            hot_jupiter::CoupledRLOFIntegrator integrator(mp_val, a_val, m_core_earth, m_star_sun, 0.15);
            auto res = integrator.integrate(t_max_yr, num_pts);
            out_outcomes_matrix[i * n_a + j] = static_cast<int>(res.outcome);
        }
    }
}

void solve_interior_profile_detailed_c(
    double M_p_kg,
    double M_c_kg,
    double S_env,
    double P_surf,
    int num_pts,
    double a_au,
    double m_star_sun,
    double* out_r,
    double* out_m,
    double* out_P,
    double* out_rho,
    double* out_T,
    double* out_nabla_ad,
    C_PlanetStructureResult* out_result
) {
    hot_jupiter::InteriorSolver solver;
    hot_jupiter::PlanetStructure sys = solver.solve_structure(M_p_kg, M_c_kg, S_env, P_surf, num_pts, a_au, m_star_sun);

    if (out_result) {
        out_result->R_p = sys.R_p;
        out_result->M_p = sys.M_p;
        out_result->M_c = sys.M_c;
        out_result->S_env = sys.S_env;
        out_result->T_center = sys.T_c;
        out_result->P_center = sys.P_c;
        out_result->rho_center = sys.rho.empty() ? 0.0 : sys.rho.back();
        out_result->num_layers = static_cast<int>(sys.r.size());
    }

    for (size_t i = 0; i < sys.r.size() && i < static_cast<size_t>(num_pts); ++i) {
        if (out_r) out_r[i] = sys.r[i];
        if (out_m) out_m[i] = sys.m[i];
        if (out_P) out_P[i] = sys.P[i];
        if (out_rho) out_rho[i] = sys.rho[i];
        if (out_T) out_T[i] = sys.T[i];
        if (out_nabla_ad) out_nabla_ad[i] = sys.nabla_ad[i];
    }
}

void simulate_population_c(
    int num_planets,
    double m_min_jup,
    double m_max_jup,
    double a_min_au,
    double a_max_au,
    double m_core_min_earth,
    double m_core_max_earth,
    unsigned int seed,
    double* out_m_init,
    double* out_a_init,
    double* out_m_core,
    double* out_m_remnant,
    double* out_z_bulk,
    int* out_outcome
) {
    auto res = hot_jupiter::PopulationSynthesizer::run_monte_carlo_sweep(
        num_planets, m_min_jup, m_max_jup, a_min_au, a_max_au,
        m_core_min_earth, m_core_max_earth, seed
    );

    for (int i = 0; i < num_planets && i < static_cast<int>(res.size()); ++i) {
        if (out_m_init) out_m_init[i] = res[i].m_p_init_jup;
        if (out_a_init) out_a_init[i] = res[i].a_init_au;
        if (out_m_core) out_m_core[i] = res[i].m_core_earth;
        if (out_m_remnant) out_m_remnant[i] = res[i].final_m_remnant_earth;
        if (out_z_bulk) out_z_bulk[i] = res[i].z_bulk;
        if (out_outcome) out_outcome[i] = res[i].outcome;
    }
}

}
