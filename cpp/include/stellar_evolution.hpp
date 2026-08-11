// C++ Core Library Extension for Stellar Evolution & Stellar Interiors
// Models ZAMS Luminosity, Polytropic Interiors, Eddington Limit, Reimers Mass Loss, and Hayashi Track.

#ifndef HOT_JUPITER_STELLAR_EVOLUTION_HPP
#define HOT_JUPITER_STELLAR_EVOLUTION_HPP

#include <algorithm>
#include <cmath>
#include <tuple>
#include <vector>

#include "constants.hpp"

namespace hot_jupiter {

// 1. ZAMS Mass-Luminosity & Mass-Radius Scaling (Kippenhahn & Weigert 1990)
class StellarMainSequenceModel {
 public:
  // Zero-Age Main Sequence (ZAMS) Luminosity [Watts]
  double zams_luminosity_watts(double M_star_kg) const {
    double m_solar = M_star_kg / M_SUN;
    double l_solar = 3.828e26;
    double ratio = 1.0;
    if (m_solar < 0.43) {
      ratio = 0.23 * std::pow(m_solar, 2.3);
    } else if (m_solar < 2.0) {
      ratio = std::pow(m_solar, 4.0);
    } else if (m_solar < 20.0) {
      ratio = 1.5 * std::pow(m_solar, 3.5);
    } else {
      ratio = 32000.0 * m_solar;
    }
    return ratio * l_solar;
  }

  // ZAMS Stellar Radius [m]
  double zams_radius_m(double M_star_kg) const {
    double m_solar = M_star_kg / M_SUN;
    double r_solar = R_SUN;
    double ratio = 1.0;
    if (m_solar < 1.0) {
      ratio = std::pow(m_solar, 0.8);
    } else {
      ratio = std::pow(m_solar, 0.57);
    }
    return ratio * r_solar;
  }

  // Effective Surface Temperature T_eff [K]
  double effective_temperature_k(double M_star_kg) const {
    double L = zams_luminosity_watts(M_star_kg);
    double R = zams_radius_m(M_star_kg);
    double stefan_boltzmann = 5.670374419e-8;
    return std::pow(L / (4.0 * M_PI * R * R * stefan_boltzmann), 0.25);
  }
};

// 2. Eddington Luminosity Limit (Eddington 1926)
class EddingtonLimitModel {
 public:
  // Critical Eddington Luminosity for Radiation Pressure Equilibrium [Watts]
  double eddington_luminosity_watts(double M_star_kg, double opacity_electron_scattering = 0.04) const {
    // L_edd = (4 pi G M c) / kappa
    double c = 299792458.0;
    return (4.0 * M_PI * G * M_star_kg * c) / opacity_electron_scattering;
  }

  // Eddington Mass-Loss Rate Limit [kg/s]
  double eddington_mass_loss_rate_kg_s(double M_star_kg, double R_star_m) const {
    double L_edd = eddington_luminosity_watts(M_star_kg);
    double v_esc = std::sqrt(2.0 * G * M_star_kg / R_star_m);
    return L_edd / (v_esc * v_esc);
  }
};

// 3. Reimers Stellar Wind Mass-Loss (Reimers 1975)
class ReimersStellarWindModel {
 public:
  // Reimers Red Giant / RGB Mass-Loss Rate [kg/s]
  double reimers_mass_loss_rate_kg_s(double M_star_kg, double R_star_m, double L_star_watts, double eta_reimers = 0.5) const {
    double m_solar = M_star_kg / M_SUN;
    double r_solar = R_star_m / R_SUN;
    double l_solar = L_star_watts / 3.828e26;
    // Reimers formula: M_dot = 4e-13 * eta * (L * R / M) M_sun/yr
    double m_dot_sun_yr = 4.0e-13 * eta_reimers * (l_solar * r_solar / std::max(0.1, m_solar));
    double kg_per_sun_yr = M_SUN / (365.25 * 86400.0);
    return m_dot_sun_yr * kg_per_sun_yr;
  }
};

// 4. Polytropic Stellar Interior (Lane-Emden n=1.5 & n=3.0)
class PolytropicStellarInteriorModel {
 public:
  // Central Pressure P_c [Pa] for Polytrope index n
  double central_pressure_pa(double M_star_kg, double R_star_m, double n_index = 1.5) const {
    double C_p = (n_index == 1.5) ? 0.7701 : 11.05;  // Polytropic constant C_p
    return C_p * G * M_star_kg * M_star_kg / std::pow(R_star_m, 4.0);
  }

  // Central Density rho_c [kg/m^3] for Polytrope index n
  double central_density_kg_m3(double M_star_kg, double R_star_m, double n_index = 1.5) const {
    double mean_density = M_star_kg / ((4.0 / 3.0) * M_PI * std::pow(R_star_m, 3.0));
    double ratio_rho_c = (n_index == 1.5) ? 5.991 : 54.18;
    return mean_density * ratio_rho_c;
  }
};

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_STELLAR_EVOLUTION_HPP
