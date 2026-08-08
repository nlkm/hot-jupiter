#ifndef HOT_JUPITER_C_API_H
#define HOT_JUPITER_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define EXPORT_API __attribute__((visibility("default")))

typedef struct {
    double R_p;
    double M_p;
    double M_c;
    double S_env;
    double T_center;
    double P_center;
    double rho_center;
    int num_layers;
} C_PlanetStructureResult;

typedef struct {
    double final_m_remnant_earth;
    double z_bulk;
    int outcome;  // 0: DISRUPTED, 1: STAGNATED, 2: COOLING, 3: ENGULFED
    int num_pts_returned;
} C_TrajectoryResult;

EXPORT_API void solve_planet_structure_c(
    double M_p_kg, double M_c_kg, double S_env, double P_surf,
    C_PlanetStructureResult* out_result
);

EXPORT_API double evaluate_saumon_chabrier_density_c(double P_pascal, double T_kelvin, double X);

EXPORT_API void rlof_integrate_trajectory_c(
    double m_p_init_jup,
    double a_init_au,
    double m_core_earth,
    double m_star_sun,
    double t_max_yr,
    int num_pts,
    double* out_t_arr,
    double* out_a_arr,
    double* out_m_p_arr,
    double* out_r_p_arr,
    double* out_ff_arr,
    C_TrajectoryResult* out_result
);

EXPORT_API void solve_interior_profile_detailed_c(
    double M_p_kg,
    double M_c_kg,
    double S_env,
    double P_surf,
    int num_pts,
    double* out_r,
    double* out_m,
    double* out_P,
    double* out_rho,
    double* out_T,
    double* out_nabla_ad,
    C_PlanetStructureResult* out_result
);

EXPORT_API void simulate_population_c(
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
);

#ifdef __cplusplus
}
#endif

#endif // HOT_JUPITER_C_API_H
