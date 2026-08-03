#ifndef THERMAL_EVOLUTION_HEATING_HPP
#define THERMAL_EVOLUTION_HEATING_HPP

#include <cmath>

#include "constants.hpp"

namespace thermal_evolution {

class HeatingModel {
public:
    double k2_over_Q = 2.0e-5;
    double epsilon_ohmic_max = 0.03; // 3% max Ohmic conversion efficiency

    double compute_tidal_power(double M_p, double M_star, double a, double e, double R_p, double Omega_rot = 0.0, double obliquity = 0.0) const {
        if (a <= 0 || R_p <= 0 || e < 0) return 0.0;
        
        double n = std::sqrt(G * M_star / (a * a * a));
        double factor = 0.5 * (k2_over_Q) * G * (M_star * M_star) * std::pow(R_p, 5) / std::pow(a, 6);

        double P_eccentricity = 21.0 * factor * (e * e);
        double spin_diff = Omega_rot - n * std::cos(obliquity);
        double P_spin = 3.0 * factor * (spin_diff * spin_diff);

        return P_eccentricity + P_spin;
    }

    double compute_ohmic_power(double R_p, double F_inc, double A_b = 0.34) const {
        if (R_p <= 0 || F_inc <= 0) return 0.0;
        double T_eq = std::pow(F_inc * (1.0 - A_b) / (4.0 * SIGMA_SB), 0.25);
        double exp_factor = std::exp(- std::pow(T_eq - 1600.0, 2) / (2.0 * 300.0 * 300.0));
        return epsilon_ohmic_max * exp_factor * M_PI * (R_p * R_p) * F_inc * (1.0 - A_b);
    }

    double compute_total_power(double M_p, double M_star, double a, double e, double R_p, double F_inc, double Omega_rot = 0.0, double obliquity = 0.0) const {
        return compute_tidal_power(M_p, M_star, a, e, R_p, Omega_rot, obliquity) + compute_ohmic_power(R_p, F_inc);
    }
};

} // namespace thermal_evolution

#endif // THERMAL_EVOLUTION_HEATING_HPP
