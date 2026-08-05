#ifndef HOT_JUPITER_EOS_HPP
#define HOT_JUPITER_EOS_HPP

#include <cmath>
#include <tuple>
#include <algorithm>

#include "constants.hpp"

namespace hot_jupiter {

class HydrogenHeliumEOS {
public:
    double K_deg = 1.08e6; // Degeneracy parameter [Pa m^5 kg^-5/3]
    double X = 0.75;
    double Y = 0.25;

    double mu_molecular() const {
        return 1.0 / (X + Y / 4.0);
    }

    double mu_electron() const {
        return 1.0 / X;
    }

    double specific_gas_constant() const {
        return KB / (mu_molecular() * MASS_P);
    }

    double density_from_PS(double P, double S) const {
        double R_spec = specific_gas_constant();
        double mu_e = mu_electron();
        
        double T = 1000.0 * std::pow(P / BAR, 0.285);
        double rho_thermal = P / (R_spec * T);
        double rho_deg = mu_e * std::pow(std::max(0.0, P) / K_deg, 0.6);
        return std::max(1e-4, std::min(30000.0, rho_thermal + rho_deg));
    }

    double temperature_from_PS(double P, double S) const {
        double R_spec = specific_gas_constant();
        double rho = density_from_PS(P, S);
        return std::max(10.0, P / (rho * R_spec));
    }

    std::tuple<double, double, double> get_state_from_PS(double P, double S) const {
        double rho = density_from_PS(P, S);
        double T = temperature_from_PS(P, S);
        double nad = 0.285 + 0.115 / (1.0 + std::exp(-(P - 1e11) / 5e10));
        return {T, rho, nad};
    }
};

class BirchMurnaghanCoreEOS {
public:
    double rho_0 = 5000.0; // Uncompressed reference density [kg/m^3]
    double K_0 = 2.0e11;   // Zero-pressure bulk modulus [Pa]
    double K_0_prime = 4.0;// Pressure derivative

    double density(double P) const {
        if (P <= 0.0) return rho_0;

        double eta = P / K_0;
        double f = 0.5 * std::pow(3.0 * eta, 2.0 / 7.0);
        double rho = rho_0 * std::pow(1.0 + 2.0 * f, 1.5);
        return std::min(35000.0, std::max(rho_0, rho));
    }
};

} // namespace hot_jupiter

#endif // HOT_JUPITER_EOS_HPP
