// C++ Core Library Extension for Planet Formation & Protoplanetary Disk Physics
// Models Core Accretion, Pebble Accretion, Type I/II Migration, and Streaming Instability.

#ifndef HOT_JUPITER_PLANET_FORMATION_HPP
#define HOT_JUPITER_PLANET_FORMATION_HPP

#include <algorithm>
#include <cmath>
#include <tuple>
#include <vector>

#include "constants.hpp"

namespace hot_jupiter {

// 1. Core Accretion & Critical Core Mass (Pollack et al. 1996, Rafikov 2006)
class CoreAccretionModel {
 public:
  // Critical Core Mass for Runaway Gas Accretion [kg]
  double critical_core_mass_kg(double planetesimal_accretion_rate_kg_s, double opacity_cm2_g = 0.1) const {
    // Rafikov (2006) M_crit scaling: M_crit ~ (M_dot_planetesimal / 1e-6 M_earth/yr)^0.25 * (kappa / 0.1)^0.25
    double M_earth_yr_to_kg_s = 5.972e24 / (365.25 * 86400.0);
    double m_dot_normalized = planetesimal_accretion_rate_kg_s / (1.0e-6 * M_earth_yr_to_kg_s);
    double m_crit_earth = 10.0 * std::pow(std::max(1.0e-4, m_dot_normalized), 0.25) * std::pow(opacity_cm2_g / 0.1, 0.25);
    return m_crit_earth * 5.972e24;
  }

  // Planetesimal Accretion Rate onto Protoplanet (Safronov 1972) [kg/s]
  double planetesimal_accretion_rate_kg_s(double M_core_kg, double R_core_m, double surface_density_planetesimals_kg_m2, double safronov_number = 5.0) const {
    double n = std::sqrt(G * M_SUN / std::pow(1.0 * AU, 3.0));
    double cross_section_factor = 1.0 + 2.0 * safronov_number;
    return M_PI * R_core_m * R_core_m * surface_density_planetesimals_kg_m2 * n * cross_section_factor;
  }
};

// 2. Pebble Accretion Dynamics (Lambrechts & Johansen 2012)
class PebbleAccretionModel {
 public:
  // Hill radius for pebble capture [m]
  double hill_radius_m(double M_core_kg, double a_m, double M_star_kg = M_SUN) const {
    return a_m * std::pow(M_core_kg / (3.0 * M_star_kg), 1.0 / 3.0);
  }

  // Pebble Accretion Rate [kg/s] (3D Hill Regime)
  double pebble_accretion_rate_kg_s(double M_core_kg, double a_m, double surface_density_pebbles_kg_m2, double stokes_number = 0.1) const {
    double n = std::sqrt(G * M_SUN / std::pow(a_m, 3.0));
    double r_h = hill_radius_m(M_core_kg, a_m);
    double accretion_cross_section = r_h * r_h * std::pow(stokes_number, 2.0 / 3.0);
    return accretion_cross_section * surface_density_pebbles_kg_m2 * n;
  }
};

// 3. Disk Migration (Type I / Type II) (Ward 1997, Paardekooper et al. 2010)
class DiskMigrationModel {
 public:
  // Type I Migration Timescale [years]
  double type_i_migration_timescale_yr(double M_planet_kg, double a_m, double surface_density_gas_kg_m2 = 1000.0, double aspect_ratio = 0.05) const {
    double n = std::sqrt(G * M_SUN / std::pow(a_m, 3.0));
    double q = M_planet_kg / M_SUN;
    double gamma_type1 = (q / std::pow(aspect_ratio, 2.0)) * (surface_density_gas_kg_m2 * a_m * a_m / M_SUN);
    double t_migration_s = (1.0 / (gamma_type1 * n));
    return t_migration_s / (365.25 * 86400.0);
  }

  // Gap Opening Condition for Type II Migration (Lin & Papaloizou 1986)
  bool opens_gap(double M_planet_kg, double a_m, double aspect_ratio = 0.05, double alpha_viscosity = 1.0e-3) const {
    double q = M_planet_kg / M_SUN;
    double thermal_criterion = q / std::pow(aspect_ratio, 3.0);
    double viscous_criterion = q / std::sqrt(alpha_viscosity * std::pow(aspect_ratio, 5.0));
    return (thermal_criterion > 1.0) && (viscous_criterion > 1.0);
  }
};

// 4. Streaming Instability & Planetesimal Formation (Youdin & Goodman 2005)
class StreamingInstabilityModel {
 public:
  // Critical Dust-to-Gas Ratio for Streaming Instability Trigger
  double critical_dust_to_gas_ratio(double stokes_number = 0.1) const {
    // Youdin & Goodman (2005): Z_crit ~ 0.015 for St ~ 0.1
    return 0.01 + 0.05 * std::pow(stokes_number - 0.1, 2.0);
  }

  // Characteristic Initial Mass of Planetesimals Formed via Streaming Instability [kg]
  double planetesimal_initial_mass_kg(double a_m, double surface_density_gas_kg_m2 = 1000.0) const {
    double h_gas = 0.05 * a_m;
    double lambda_si = 0.1 * h_gas;  // Fastest growing wavelength
    double mass = surface_density_gas_kg_m2 * lambda_si * lambda_si;
    return mass;
  }
};

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_PLANET_FORMATION_HPP
