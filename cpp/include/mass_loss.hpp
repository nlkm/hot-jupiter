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

    std::tuple<double, double> evaluate_mass_loss_rate(double R_p, double a, double M_p, double M_star) const {
        double r_roche = roche_lobe_radius(a, M_p, M_star);
        if (r_roche <= 0 || R_p <= 0 || M_p <= 0) return {0.0, 0.0};

        double filling_factor = R_p / r_roche;
        if (filling_factor < 0.95) return {0.0, 0.0};

        double overflow_excess = std::max(0.0, filling_factor - 1.0);
        double dM_dt = - M_dot_0 * std::exp(eta_exponent * overflow_excess);

        // Cap maximum mass loss rate to 10% of planet mass per Gyr
        double max_dM_dt = - 0.10 * M_p / (1.0e9 * YEAR);
        dM_dt = std::max(dM_dt, max_dM_dt);

        double da_dt_rlof = - 2.0 * a * (dM_dt / M_p) * (1.0 - momentum_fraction_beta);
        return {dM_dt, da_dt_rlof};
    }
};

} // namespace thermal_evolution

#endif // THERMAL_EVOLUTION_MASS_LOSS_HPP
