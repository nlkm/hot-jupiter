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

EXPORT_API void solve_planet_structure_c(
    double M_p_kg, double M_c_kg, double S_env, double P_surf,
    C_PlanetStructureResult* out_result
);

EXPORT_API double evaluate_saumon_chabrier_density_c(double P_pascal, double T_kelvin, double X);

#ifdef __cplusplus
}
#endif

#endif // HOT_JUPITER_C_API_H
