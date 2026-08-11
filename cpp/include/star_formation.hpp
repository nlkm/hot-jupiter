// C++ Core Library Extension for Star Formation & ISM Dynamics
// Models Jeans Instability, Bonnor-Ebert Sphere, Larson Scaling Laws, and Initial Mass Functions (IMF).

#ifndef HOT_JUPITER_STAR_FORMATION_HPP
#define HOT_JUPITER_STAR_FORMATION_HPP

#include <algorithm>
#include <cmath>
#include <tuple>
#include <vector>

#include "constants.hpp"

namespace hot_jupiter {

// 1. Jeans Instability & Gravitational Collapse (Jeans 1902)
class JeansInstabilityModel {
 public:
  // Sound Speed c_s [m/s] in Isothermal Molecular Gas Cloud
  double sound_speed_m_s(double temp_k, double mean_molecular_weight = 2.3) const {
    double m_h = 1.6735575e-27;
    return std::sqrt((KB * temp_k) / (mean_molecular_weight * m_h));
  }

  // Jeans Length lambda_J [m]
  double jeans_length_m(double temp_k, double density_kg_m3, double mean_molecular_weight = 2.3) const {
    double c_s = sound_speed_m_s(temp_k, mean_molecular_weight);
    return c_s * std::sqrt(M_PI / (G * density_kg_m3));
  }

  // Jeans Mass M_J [kg]
  double jeans_mass_kg(double temp_k, double density_kg_m3, double mean_molecular_weight = 2.3) const {
    double lambda_j = jeans_length_m(temp_k, density_kg_m3, mean_molecular_weight);
    return (M_PI / 6.0) * density_kg_m3 * std::pow(lambda_j, 3.0);
  }
};

// 2. Bonnor-Ebert Sphere Critical Collapse Mass (Ebert 1955, Bonnor 1956)
class BonnorEbertSphereModel {
 public:
  // Critical Bonnor-Ebert Mass M_BE [kg]
  double bonnor_ebert_mass_kg(double temp_k, double external_pressure_pa, double mean_molecular_weight = 2.3) const {
    JeansInstabilityModel jeans;
    double c_s = jeans.sound_speed_m_s(temp_k, mean_molecular_weight);
    double c_be = 1.18;  // Bonnor-Ebert dimensionless constant
    return (c_be * std::pow(c_s, 4.0)) / (std::sqrt(G * G * G * external_pressure_pa));
  }
};

// 3. Larson Scaling Laws for Giant Molecular Clouds (Larson 1981)
class LarsonScalingLawsModel {
 public:
  // Velocity Dispersion sigma_v [m/s] scaling with Cloud Size L [pc]
  double velocity_dispersion_m_s(double cloud_size_pc) const {
    // Larson 1st law: sigma_v = 1.1 km/s * (L / 1 pc)^0.38
    return 1100.0 * std::pow(cloud_size_pc, 0.38);
  }

  // Mean Gas Density rho [kg/m^3] scaling with Cloud Size L [pc]
  double mean_density_kg_m3(double cloud_size_pc) const {
    // Larson 2nd law: n(H2) ~ 1000 cm^-3 * (L / 1 pc)^-1.1
    double n_h2_cm3 = 1000.0 * std::pow(cloud_size_pc, -1.1);
    double m_h2 = 2.0 * 1.6735575e-27;
    return n_h2_cm3 * 1.0e6 * m_h2;
  }
};

// 4. Initial Mass Function (IMF) (Salpeter 1955, Chabrier 2003)
class InitialMassFunctionModel {
 public:
  // Salpeter IMF xi(M) dM ~ M^-2.35
  double salpeter_imf(double M_star_kg) const {
    double m_solar = M_star_kg / M_SUN;
    return std::pow(m_solar, -2.35);
  }

  // Chabrier Log-Normal System IMF for stellar mass M < 1 M_sun
  double chabrier_imf(double M_star_kg) const {
    double m_solar = M_star_kg / M_SUN;
    if (m_solar < 1.0) {
      double m_c = 0.079;
      double sigma = 0.69;
      double log_ratio = std::log10(m_solar / m_c);
      return (0.158 / m_solar) * std::exp(-std::pow(log_ratio, 2.0) / (2.0 * sigma * sigma));
    }
    return 0.044 * std::pow(m_solar, -2.3);
  }
};

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_STAR_FORMATION_HPP
