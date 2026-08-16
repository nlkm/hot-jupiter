#ifndef HOT_JUPITER_EOS_HPP
#define HOT_JUPITER_EOS_HPP

#include <algorithm>
#include <cmath>
#include <tuple>

#include "constants.hpp"

namespace hot_jupiter {

class HydrogenHeliumEOS {
 public:
    double K_deg = 1.08e6;  // Degeneracy parameter [Pa m^5 kg^-5/3]
    double X = 0.75;
    double Y = 0.25;
    double gamma = 1.4;

    double mu_molecular() const {
        return 1.0 / (X / 2.0 + Y / 4.0);
    }

    double mu_electron() const {
        return 2.0 / (1.0 + X);
    }

    double specific_gas_constant() const {
        return KB / (mu_molecular() * MASS_P);
    }

    double temperature_from_PS(double P, double S) const {
        double R_spec = specific_gas_constant();
        double ln_T = ((gamma - 1.0) / gamma) *
                      (S / R_spec + std::log(std::max(1.0, P) / R_spec) - 20.0);
        return std::max(10.0, std::min(5.0e5, std::exp(ln_T)));
    }

    double density_from_PT(double P, double T) const {
        double R_spec = specific_gas_constant();
        double mu_e = mu_electron();
        double rho = P / (R_spec * std::max(10.0, T));
        for (int iter = 0; iter < 10; ++iter) {
            double p_th = rho * R_spec * T;
            double p_deg = K_deg * std::pow(rho / mu_e, 5.0 / 3.0);
            double f = p_th + p_deg - P;
            double df = R_spec * T +
                        (5.0 / 3.0) * (K_deg / mu_e) * std::pow(rho / mu_e, 2.0 / 3.0);
            double delta = f / df;
            rho = std::max(1e-6, rho - delta);
            if (std::abs(delta) < 1e-6 * rho) break;
        }
        return rho;
    }

    double density_from_PS(double P, double S) const {
        double T = temperature_from_PS(P, S);
        return density_from_PT(P, T);
    }

    std::tuple<double, double, double> get_state_from_PS(double P, double S) const {
        double T = temperature_from_PS(P, S);
        double rho = density_from_PT(P, T);
        double nad = (gamma - 1.0) / gamma;  // 2/7 ≈ 0.2857
        return {T, rho, nad};
    }
};

class BirchMurnaghanCoreEOS {
 public:
    double rho_0 = 5000.0;   // Uncompressed reference density [kg/m^3]
    double K_0 = 2.0e11;     // Zero-pressure bulk modulus [Pa]
    double K_0_prime = 4.0;  // Pressure derivative

    double density(double P) const {
        if (P <= 0.0) return rho_0;

        double eta = P / K_0;
        double f = 0.5 * std::pow(3.0 * eta, 2.0 / 7.0);
        double rho = rho_0 * std::pow(1.0 + 2.0 * f, 1.5);
        return std::min(35000.0, std::max(rho_0, rho));
    }
};

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_EOS_HPP
