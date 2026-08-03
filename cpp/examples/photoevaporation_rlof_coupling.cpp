#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "constants.hpp"
#include "mass_loss.hpp"

using namespace thermal_evolution;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << " C++ PHOTOEVAPORATION & RLOF COUPLED MASS LOSS BENCHMARK                  " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    RocheLobeMassLoss mass_loss_solver;

    double M_p = 0.8 * M_JUP;
    double M_star = 1.0 * M_SUN;
    double a = 0.022 * AU;

    std::ofstream csv("outputs/photoevaporation_mass_loss.csv");
    csv << "t_gyr,M_p_Mjup,M_dot_rlof_kg_s,M_dot_xuv_kg_s,M_dot_total_kg_s,filling_factor\n";

    double dt = 4.56e6 * YEAR;
    int steps = 1000;

    for (int i = 0; i < steps; ++i) {
        double t_gyr = (i * dt) / GYR;

        // Radius evolution with early thermal expansion
        double R_p = (1.55 - 0.25 * std::exp(-t_gyr / 1.0)) * R_JUP;

        // Time-decaying stellar XUV flux: F_XUV(t) = F_0 * (t / 10 Myr)^-1.5
        double t_myr = std::max(10.0, t_gyr * 1000.0);
        double F_XUV = 10.0 * std::pow(t_myr / 10.0, -1.5); // W/m^2

        double filling_factor = mass_loss_solver.roche_lobe_filling_factor(R_p, a, M_p, M_star);
        auto [dM_dt_total, da_dt] = mass_loss_solver.evaluate_mass_loss_rate(R_p, a, M_p, M_star, F_XUV);

        double dM_dt_xuv = mass_loss_solver.compute_photoevaporative_mdot(F_XUV, R_p, M_p);
        double dM_dt_rlof = dM_dt_total - dM_dt_xuv;

        // Integrate mass loss
        M_p = std::max(0.1 * M_JUP, M_p + dM_dt_total * dt);

        csv << t_gyr << ","
            << (M_p / M_JUP) << ","
            << std::abs(dM_dt_rlof) << ","
            << std::abs(dM_dt_xuv) << ","
            << std::abs(dM_dt_total) << ","
            << filling_factor << "\n";
    }

    csv.close();
    std::cout << "Photoevaporation & RLOF coupling benchmark written to outputs/photoevaporation_mass_loss.csv" << std::endl;
    return 0;
}
