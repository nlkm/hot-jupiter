#include "c_api.h"
#include "interior.hpp"
#include "eos.hpp"

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
    return eos.density_from_PS(P_pascal, 1.0e8);
}

}
