#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include "constants.hpp"
#include "heating.hpp"
#include "mass_loss.hpp"

using namespace thermal_evolution;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << "  C++ ECCENTRIC TIDAL HEATING + ROCHE LOBE OVERFLOW MASS-LOSS BENCHMARK  " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    RocheLobeMassLoss rlof;
    HeatingModel heating;

    double M_p = 1.0 * M_JUP;
    double M_star = 1.0 * M_SUN;
    double a = 0.020 * AU;
    double e = 0.35;
    double R_p = 1.65 * R_JUP;

    double dt = 5.0e6 * YEAR;
    int num_steps = 400;

    std::cout << "Evolving planet undergoing tidal heating & periastron RLOF over 2.0 Gyr..." << std::endl;
    for (int step = 0; step < num_steps; ++step) {
        double r_peri = a * (1.0 - e);
        auto [dM_dt, da_dt_rlof] = rlof.evaluate_mass_loss_rate(R_p, r_peri, M_p, M_star);
        double P_tidal = heating.compute_tidal_power(M_p, M_star, a, e, R_p);

        // Tidal circularization
        double n = std::sqrt(G * M_star / (a * a * a));
        double scale_tide = 2.0e-5 * (M_star / M_p) * std::pow(R_p / a, 5) * n;
        double de_dt_tide = - 10.5 * scale_tide * e;

        M_p += dM_dt * dt;
        e = std::max(0.0, e + de_dt_tide * dt);
        a += da_dt_rlof * dt;

        if (step % 80 == 0 || step == num_steps - 1) {
            double r_roche_peri = RocheLobeMassLoss::roche_lobe_radius(r_peri, M_p, M_star);
            double fill_peri = R_p / r_roche_peri;
            double dM_dt_earth_gyr = std::abs(dM_dt) / (M_EARTH / GYR);

            std::cout << "Time: " << std::fixed << std::setprecision(2) << (step * dt / GYR) << " Gyr"
                      << " | M_p = " << std::setprecision(3) << (M_p / M_JUP) << " M_Jup"
                      << " | e = " << std::setprecision(4) << e
                      << " | Fill_peri = " << std::setprecision(2) << fill_peri
                      << " | dM/dt = " << std::setprecision(1) << dM_dt_earth_gyr << " M_E/Gyr"
                      << " | P_tide = " << std::scientific << std::setprecision(2) << P_tidal << " W" << std::endl;
        }
    }

    std::cout << "\nEccentric RLOF C++ simulation clean success.\n" << std::endl;
    return 0;
}
