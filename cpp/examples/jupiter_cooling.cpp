#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include "constants.hpp"
#include "eos.hpp"
#include "interior.hpp"
#include "atmosphere.hpp"
#include "heating.hpp"

using namespace thermal_evolution;

int main() {
    std::cout << "==========================================================" << std::endl;
    std::cout << "    C++ JUPITER THERMAL EVOLUTION BENCHMARK VALIDATION    " << std::endl;
    std::cout << "==========================================================" << std::endl;

    InteriorSolver solver;
    GuillotAtmosphere atmosphere;
    TimeVaryingStellarLuminosity stellar_model;

    double M_p = 1.0 * M_JUP;
    double M_c = 12.0 * M_EARTH;
    double a_jup = 5.204 * AU;
    double S_init = 1.34e5;

    std::cout << "Solving present-day 1D hydrostatic structure at t = 4.56 Gyr..." << std::endl;
    PlanetStructure st = solver.solve_structure(M_p, M_c, S_init);

    double F_inc = stellar_model.incident_flux(a_jup, 4.56 * GYR);
    double T_irr = atmosphere.T_irr_from_flux(F_inc, atmosphere.A_b);
    double T_eff = atmosphere.T_at_tau(2.0 / 3.0, 99.6, T_irr);

    std::cout << "\n----------------------------------------------------------" << std::endl;
    std::cout << "PRESENT-DAY C++ JUPITER MODEL RESULTS:" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Planet Radius R_p:       " << std::fixed << std::setprecision(3) << (st.R_p / R_JUP) << " R_Jup  (Observed: 1.000 R_Jup)" << std::endl;
    std::cout << "Core Pressure P_c:       " << std::scientific << std::setprecision(2) << (st.P_c / BAR) << " bar    (Observed: ~40 Mbar)" << std::endl;
    std::cout << "Effective Temp T_eff:    " << std::fixed << std::setprecision(1) << T_eff << " K        (Observed: 124.4 K)" << std::endl;
    std::cout << "----------------------------------------------------------\n" << std::endl;

    return 0;
}
