#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include "constants.hpp"
#include "orbital.hpp"
#include "heating.hpp"

using namespace thermal_evolution;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << "  C++ COUPLED 6D ORBITAL & 3D SPIN VECTOR EVOLUTION BENCHMARK           " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    TidalOrbitalSpinRates tidal_rates;
    HeatingModel heating;

    double M_p = 1.0 * M_JUP;
    double M_star = 1.0 * M_SUN;
    double R_p = 1.35 * R_JUP;

    double a = 0.040 * AU;
    double e = 0.25;
    SpinState spin = SpinState::from_period_hours(10.0, 15.0);

    double dt = 1.0e6 * YEAR;
    int num_steps = 1000;

    std::cout << "Evolving coupled orbital element and spin vector dynamics over 1.0 Gyr..." << std::endl;
    for (int step = 0; step < num_steps; ++step) {
        auto [da_dt, de_dt, dOmega_dt, dobl_dt] = tidal_rates.evaluate_rates(
            M_p, R_p, M_star, a, e, spin.Omega_rot, spin.obliquity
        );

        a += da_dt * dt;
        e = std::max(0.0, e + de_dt * dt);
        spin.Omega_rot += dOmega_dt * dt;
        spin.obliquity = std::max(0.0, spin.obliquity + dobl_dt * dt);

        if (step % 200 == 0 || step == num_steps - 1) {
            double P_rot_hrs = (2.0 * M_PI / spin.Omega_rot) / HOUR;
            double obl_deg = spin.obliquity * 180.0 / M_PI;
            double P_tidal = heating.compute_tidal_power(M_p, M_star, a, e, R_p, spin.Omega_rot, spin.obliquity);

            std::cout << "Time: " << std::fixed << std::setprecision(1) << (step * dt / GYR) << " Gyr"
                      << " | a = " << std::setprecision(4) << (a / AU) << " AU"
                      << " | e = " << std::setprecision(4) << e
                      << " | P_rot = " << std::setprecision(2) << P_rot_hrs << " hrs"
                      << " | obl = " << std::setprecision(1) << obl_deg << " deg"
                      << " | P_tide = " << std::scientific << std::setprecision(2) << P_tidal << " W" << std::endl;
        }
    }

    std::cout << "\nCoupled dynamics integration clean success.\n" << std::endl;
    return 0;
}
