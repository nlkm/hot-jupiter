#include <iostream>
#include <fstream>
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
    StellarTidalRates stellar_rates;
    HeatingModel heating;

    double M_p = 1.0 * M_JUP;
    double M_star = 1.0 * M_SUN;
    double R_p = 1.35 * R_JUP;

    double a = 0.040 * AU;
    double e = 0.25;
    SpinState spin = SpinState::from_period_hours(10.0, 15.0);

    std::ofstream csv("outputs/hot_jupiter_coupled_orbital_spin_evolution.csv");
    csv << "t_gyr,a_AU,e,P_rot_hrs,obliquity_deg,R_p_Rjup,P_tidal_W\n";

    double dt_macro = 1.0e6 * YEAR;
    int num_steps = 1000;
    int sub_steps = 100;
    double dt_sub = dt_macro / sub_steps;

    for (int step = 0; step < num_steps; ++step) {
        double t_gyr = (step * dt_macro) / GYR;

        // Output current macro state
        double P_rot_hrs = (2.0 * M_PI / spin.Omega_rot) / HOUR;
        double obl_deg = spin.obliquity * 180.0 / M_PI;
        double P_tidal = heating.compute_tidal_power(M_p, M_star, a, e, R_p, spin.Omega_rot, spin.obliquity);
        double R_p_curr = (1.25 + 0.15 * std::exp(-t_gyr / 0.5)) * R_JUP;

        csv << t_gyr << "," << (a / AU) << "," << e << "," << P_rot_hrs << "," << obl_deg << "," << (R_p_curr / R_JUP) << "," << P_tidal << "\n";

        // Sub-step integration to resolve fast spin-synchronization timescale cleanly
        for (int sub = 0; sub < sub_steps; ++sub) {
            auto [da_dt_p, de_dt, dOmega_dt, dobl_dt] = tidal_rates.evaluate_rates(
                M_p, R_p, M_star, a, e, spin.Omega_rot, spin.obliquity
            );
            auto [da_dt_star, dOmega_star] = stellar_rates.evaluate_stellar_rates(
                M_p, M_star, a, (2.0 * M_PI) / (25.0 * DAY), 0.0
            );

            double da_dt = da_dt_p + da_dt_star;

            a = std::max(0.015 * AU, a + da_dt * dt_sub);
            e = std::max(0.0, e + de_dt * dt_sub);
            spin.Omega_rot = std::max(1.0e-6, spin.Omega_rot + dOmega_dt * dt_sub);
            spin.obliquity = std::max(0.0, spin.obliquity + dobl_dt * dt_sub);
        }
    }

    csv.close();
    std::cout << "CSV data written to outputs/hot_jupiter_coupled_orbital_spin_evolution.csv" << std::endl;
    return 0;
}
