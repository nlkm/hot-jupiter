#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "constants.hpp"
#include "orbital.hpp"
#include "heating.hpp"

using namespace thermal_evolution;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << " C++ HIGH OBLIQUITY SPIN AXIS TILT BENCHMARK                             " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    TidalOrbitalSpinRates rates;
    HeatingModel heating;

    double M_p = 1.0 * M_JUP;
    double M_star = 1.0 * M_SUN;
    double R_p = 1.40 * R_JUP;
    double a = 0.040 * AU;
    double e = 0.10;
    SpinState spin = SpinState::from_period_hours(6.0, 45.0);

    std::ofstream csv("outputs/obliquity_tilted_spin_evolution.csv");
    csv << "t_gyr,obliquity_deg,P_rot_hrs,P_obliquity_W,R_p_Rjup\n";

    double dt = 1.0e6 * YEAR;
    int steps = 1000;

    for (int i = 0; i < steps; ++i) {
        double t_gyr = (i * dt) / GYR;
        auto [da_dt, de_dt, dOmega_dt, dobl_dt] = rates.evaluate_rates(
            M_p, R_p, M_star, a, e, spin.Omega_rot, spin.obliquity
        );

        spin.Omega_rot += dOmega_dt * dt;
        spin.obliquity = std::max(0.0, spin.obliquity + dobl_dt * dt);

        double obl_deg = spin.obliquity * 180.0 / M_PI;
        double P_rot_hrs = (2.0 * M_PI / spin.Omega_rot) / HOUR;
        double P_obl = heating.compute_tidal_power(M_p, M_star, a, e, R_p, spin.Omega_rot, spin.obliquity);
        double R_p_curr = (1.20 + 0.25 * (spin.obliquity / (45.0 * M_PI / 180.0))) * R_JUP;

        csv << t_gyr << "," << obl_deg << "," << P_rot_hrs << "," << P_obl << "," << (R_p_curr / R_JUP) << "\n";
    }

    csv.close();
    std::cout << "CSV data written to outputs/obliquity_tilted_spin_evolution.csv" << std::endl;
    return 0;
}
