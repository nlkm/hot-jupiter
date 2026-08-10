#ifndef HOT_JUPITER_ATMOSPHERE_HPP
#define HOT_JUPITER_ATMOSPHERE_HPP

#include <cmath>
#include <algorithm>

#include "constants.hpp"

namespace hot_jupiter {

class TimeVaryingStellarLuminosity {
public:
    double L_star_0 = L_SUN; // Present-day luminosity [W]

    double luminosity_at_time(double t_sec) const {
        double t_gyr = t_sec / GYR;
        // Solar-type main sequence luminosity evolution: L_*(t) = L_0 * [1 + 0.4 * (1 - t/t_sun)]^-1
        double l_ratio = 1.0 / (1.0 + 0.4 * (1.0 - t_gyr / 4.56));
        return L_star_0 * std::max(0.2, std::min(3.0, l_ratio));
    }

    double incident_flux(double a, double t_sec) const {
        if (a <= 0) return 0.0;
        double L_star = luminosity_at_time(t_sec);
        return L_star / (4.0 * M_PI * a * a);
    }
};

class GuillotAtmosphere {
public:
    double gamma = 0.1;       // Opacity ratio kappa_vis / kappa_th
    double kappa_th = 0.01;   // Thermal opacity [m^2/kg]
    double A_b = 0.34;        // Bond albedo
    double mu_atm = 2.3 * MASS_P; // Mean molecular weight [kg] (H2/He)

    double T_irr_from_flux(double F_inc, double albedo) const {
        return std::pow((1.0 - albedo) * F_inc / (4.0 * SIGMA_SB), 0.25);
    }

    double T_at_tau(double tau, double T_int, double T_irr) const {
        double T_int_4 = std::pow(T_int, 4);
        double T_irr_4 = std::pow(T_irr, 4);

        double term1 = 0.75 * T_int_4 * (tau + 2.0 / 3.0);
        double term2 = 0.75 * T_irr_4 * (2.0 / 3.0 + (2.0 / (3.0 * gamma)) * (1.0 + (0.5 * gamma * tau - 1.0) * std::exp(-gamma * tau)));

        return std::pow(std::max(0.0, term1 + term2), 0.25);
    }

    double compute_scale_height(double T_eq, double M_p, double R_p, double R_roche = 0.0) const {
        double g_iso = G * M_p / (R_p * R_p);
        double f_tide = (R_roche > 0.0 && R_p < R_roche) ? (1.0 - std::pow(R_p / R_roche, 3.0)) : 1.0;
        double g_eff = std::max(1.0e-5, g_iso * f_tide);
        return (KB * T_eq) / (mu_atm * g_eff); // Scale height in meters
    }

    double compute_transit_depth_variation_ppm(double R_p, double R_star, double H_m, int n_scale_heights = 5) const {
        double delta_area = 2.0 * M_PI * R_p * (n_scale_heights * H_m);
        double star_area = M_PI * R_star * R_star;
        return (delta_area / star_area) * 1.0e6; // Signal amplitude in ppm
    }
};

} // namespace hot_jupiter

#endif // HOT_JUPITER_ATMOSPHERE_HPP
