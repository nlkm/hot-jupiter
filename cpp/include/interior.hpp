#ifndef HOT_JUPITER_INTERIOR_HPP
#define HOT_JUPITER_INTERIOR_HPP

#include <iostream>
#include <tuple>
#include <vector>

#include "constants.hpp"
#include "eos.hpp"

namespace hot_jupiter {

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

    PlanetStructure solve_structure(double M_p, double M_c, double S_env,
                                    double P_surf = 1.0 * BAR, int num_pts = 300,
                                    double a_au = 0.0, double m_star_sun = 1.0);
};

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_INTERIOR_HPP
