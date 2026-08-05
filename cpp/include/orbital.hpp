#ifndef HOT_JUPITER_ORBITAL_HPP
#define HOT_JUPITER_ORBITAL_HPP

#include <cmath>
#include <tuple>
#include <algorithm>

#include "constants.hpp"

namespace hot_jupiter {

struct OrbitalState {
    double a;            // Semi-major axis [m]
    double e;            // Eccentricity
    double inc = 0.0;    // Inclination [rad]
    double Omega_node = 0.0;
    double omega_arg = 0.0;
};

struct SpinState {
    double Omega_rot;    // Spin angular frequency [rad/s]
    double obliquity = 0.0;// Obliquity tilt epsilon [rad]

    static SpinState from_period_hours(double period_hrs, double obl_deg) {
        SpinState st;
        st.Omega_rot = (2.0 * M_PI) / (period_hrs * HOUR);
        st.obliquity = obl_deg * M_PI / 180.0;
        return st;
    }
};

class TidalOrbitalSpinRates {
public:
    double k2_over_Q = 2.0e-5;
    double C_moment = 0.25; // Dimensionless moment of inertia I_p / (M_p R_p^2)

    std::tuple<double, double, double, double> evaluate_rates(
        double M_p, double R_p, double M_star, double a, double e, double Omega_rot, double obliquity, double dR_dt = 0.0
    ) const {
        if (a <= 0 || R_p <= 0 || M_p <= 0 || M_star <= 0) return {0.0, 0.0, 0.0, 0.0};

        double n = std::sqrt(G * M_star / (a * a * a));
        double cos_eps = std::cos(obliquity);
        double sin_eps = std::sin(obliquity);

        // Hut (1981) pseudo-synchronous spin rate
        double e2 = e * e;
        double f_ps = (1.0 + 7.5 * e2 + 5.625 * e2 * e2 + 0.3125 * e2 * e2 * e2) / 
                      (std::pow(1.0 - e2, 1.5) * (1.0 + 3.0 * e2 + 0.375 * e2 * e2));
        double Omega_ps = n * f_ps;

        // Rapid spin relaxation towards pseudo-synchronous state
        double tau_spin = 1.0e5 * YEAR;
        double dOmega_dt = (Omega_ps - Omega_rot) / tau_spin;

        double R_over_a_5 = std::pow(R_p / a, 5);
        double scale_tide = k2_over_Q * (M_star / M_p) * R_over_a_5 * n;

        // Hut (1981) tidal eccentricity damping & orbital energy dissipation (da/dt <= 0)
        double de_dt = - 27.0 * scale_tide * e * std::pow(1.0 - e2, -6.5) * (1.0 + 3.75 * e2 + 0.9375 * e2 * e2);
        if (e <= 1.0e-6) de_dt = 0.0;

        // Conservation of angular momentum under circularization: a(1 - e^2) = const => da/dt = 2 a e / (1 - e^2) de/dt
        double da_dt = (2.0 * a * e / (1.0 - e2 + 1.0e-12)) * de_dt;

        // Obliquity damping
        double dobl_dt = - (sin_eps / tau_spin);
        if (obliquity <= 1.0e-6 && dobl_dt < 0) dobl_dt = 0.0;

        return {da_dt, de_dt, dOmega_dt, dobl_dt};
    }
};

class StellarTidalRates {
public:
    double k2_over_Q_star = 1.0e-6;
    double R_star = R_SUN;

    std::tuple<double, double> evaluate_stellar_rates(double M_p, double M_star, double a, double Omega_star, double stellar_obliquity = 0.0) const {
        if (a <= 0 || M_p <= 0 || M_star <= 0) return {0.0, 0.0};

        double n = std::sqrt(G * M_star / (a * a * a));
        double cos_psi = std::cos(stellar_obliquity);

        double scale_star = k2_over_Q_star * (M_p / M_star) * std::pow(R_star / a, 5) * n;
        double da_dt_star = - 3.0 * scale_star * a * (1.0 - (Omega_star / std::max(n, 1e-15)) * cos_psi);

        double I_star = 0.07 * M_star * (R_star * R_star);
        double T_star = 1.5 * k2_over_Q_star * G * (M_p * M_p) * std::pow(R_star, 5) / std::pow(a, 6) * (n * cos_psi - Omega_star);
        double dOmega_star_dt = T_star / std::max(I_star, 1e-10);

        return {da_dt_star, dOmega_star_dt};
    }
};

} // namespace hot_jupiter

#endif // HOT_JUPITER_ORBITAL_HPP
