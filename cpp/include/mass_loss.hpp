#ifndef THERMAL_EVOLUTION_MASS_LOSS_HPP
#define THERMAL_EVOLUTION_MASS_LOSS_HPP

#include <cmath>
#include <tuple>
#include <algorithm>

#include "constants.hpp"

namespace thermal_evolution {

class RocheLobeMassLoss {
public:
    double eta_exponent = 4.0;
    double M_dot_0 = 1.0e11;             // Mass loss onset rate [kg/s]
    double momentum_fraction_beta = 0.5; // Angular momentum retention fraction
    double epsilon_xuv = 0.15;           // XUV photoevaporation efficiency

    static double roche_lobe_radius(double a, double M_p, double M_star) {
        if (a <= 0 || M_p <= 0 || M_star <= 0) return 0.0;
        double q = M_p / M_star;
        double q_13 = std::pow(q, 1.0 / 3.0);
        double q_23 = std::pow(q, 2.0 / 3.0);
        double r_roche_ratio = 0.49 * q_23 / (0.6 * q_23 + std::log(1.0 + q_13));
        return a * r_roche_ratio;
    }

    double roche_lobe_filling_factor(double R_p, double a, double M_p, double M_star) const {
        double r_roche = roche_lobe_radius(a, M_p, M_star);
        return (r_roche > 0) ? R_p / r_roche : 0.0;
    }

    double compute_photoevaporative_mdot(double F_XUV, double R_p, double M_p) const {
        if (M_p <= 0 || R_p <= 0) return 0.0;
        // Energy-limited XUV mass loss: dM/dt = 3 * epsilon * F_XUV / (4 * G * rho_p)
        double V_p = (4.0 / 3.0) * M_PI * std::pow(R_p, 3.0);
        double rho_p = M_p / V_p;
        double dM_dt_xuv = - (3.0 * epsilon_xuv * F_XUV) / (4.0 * G * rho_p);
        return dM_dt_xuv; // kg/s
    }

    std::tuple<double, double> evaluate_mass_loss_rate(double R_p, double a, double M_p, double M_star, double F_XUV = 0.0) const {
        double r_roche = roche_lobe_radius(a, M_p, M_star);
        if (r_roche <= 0 || R_p <= 0 || M_p <= 0) return {0.0, 0.0};

        double filling_factor = R_p / r_roche;
        double dM_dt_rlof = 0.0;

        if (filling_factor >= 0.95) {
            double overflow_excess = std::max(0.0, filling_factor - 1.0);
            dM_dt_rlof = - M_dot_0 * std::exp(eta_exponent * overflow_excess);
            double max_dM_dt = - 0.10 * M_p / (1.0e9 * YEAR);
            dM_dt_rlof = std::max(dM_dt_rlof, max_dM_dt);
        }

        double dM_dt_xuv = compute_photoevaporative_mdot(F_XUV, R_p, M_p);
        double dM_dt_total = dM_dt_rlof + dM_dt_xuv;

        double da_dt_massloss = - 2.0 * a * (dM_dt_total / M_p) * (1.0 - momentum_fraction_beta);
        return {dM_dt_total, da_dt_massloss};
    }
};

} // namespace thermal_evolution

#endif // THERMAL_EVOLUTION_MASS_LOSS_HPP
