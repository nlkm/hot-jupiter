#ifndef THERMAL_EVOLUTION_INTERIOR_HPP
#define THERMAL_EVOLUTION_INTERIOR_HPP

#include <vector>
#include <tuple>
#include <iostream>

#include "constants.hpp"
#include "eos.hpp"

namespace thermal_evolution {

struct PlanetStructure {
    double M_p;
    double M_c;
    double S_env;
    double R_p;
    double R_c;
    double P_c;
    double T_c;
    
    std::vector<double> r;
    std::vector<double> m;
    std::vector<double> P;
    std::vector<double> rho;
    std::vector<double> T;
    std::vector<double> nabla_ad;
};

class InteriorSolver {
public:
    HydrogenHeliumEOS envelope_eos;
    BirchMurnaghanCoreEOS core_eos;

    PlanetStructure solve_structure(double M_p, double M_c, double S_env, double P_surf = 1.0 * BAR, int num_pts = 300);

private:
    double mass_residual(double R_p_try, double M_p, double M_c, double S_env, double P_surf);
};

} // namespace thermal_evolution

#endif // THERMAL_EVOLUTION_INTERIOR_HPP
