// C++ Unit Test for Multi-Domain Astrophysics Libraries

#include <cassert>
#include <cmath>
#include <iostream>

#include "planet_formation.hpp"
#include "stellar_evolution.hpp"
#include "star_formation.hpp"

using namespace hot_jupiter;

int main() {
  std::cout << "=== Running Multi-Domain Astrophysics C++ Tests ===" << std::endl;

  // 1. Planet Formation Tests
  hot_jupiter::CoreAccretionModel core_model;
  double m_crit = core_model.critical_core_mass_kg(1.0e-6 * 5.972e24 / (365.25 * 86400.0));
  double m_crit_earth = m_crit / 5.972e24;
  std::cout << "--> Critical Core Mass: " << m_crit_earth << " M_earth" << std::endl;
  assert(std::abs(m_crit_earth - 10.0) < 1.0 && "Critical core mass should be ~10 M_earth!");

  hot_jupiter::DiskMigrationModel migration_model;
  double t_mig1 = migration_model.type_i_migration_timescale_yr(5.972e24, 1.0 * AU);
  std::cout << "--> Earth Type I Migration Timescale: " << t_mig1 / 1.0e5 << " 10^5 yr" << std::endl;
  assert(t_mig1 > 1.0e4 && "Type I migration timescale should be positive!");

  // 2. Stellar Evolution Tests
  hot_jupiter::StellarMainSequenceModel stellar_model;
  double L_sun_calc = stellar_model.zams_luminosity_watts(M_SUN);
  std::cout << "--> ZAMS Solar Luminosity: " << L_sun_calc / 1.0e26 << " 10^26 W" << std::endl;
  assert(std::abs(L_sun_calc - 3.828e26) < 1.0e25 && "ZAMS solar luminosity mismatch!");

  hot_jupiter::EddingtonLimitModel eddington_model;
  double L_edd = eddington_model.eddington_luminosity_watts(M_SUN);
  std::cout << "--> Solar Eddington Luminosity Limit: " << L_edd / 1.0e31 << " 10^31 W" << std::endl;
  assert(L_edd > 1.0e31 && "Eddington limit should exceed 1e31 W!");

  // 3. Star Formation Tests
  hot_jupiter::JeansInstabilityModel jeans_model;
  double M_J = jeans_model.jeans_mass_kg(10.0, 1.0e-16);
  double M_J_solar = M_J / M_SUN;
  std::cout << "--> Jeans Mass at 10K (1e-16 kg/m^3): " << M_J_solar << " M_sun" << std::endl;
  assert(M_J_solar > 0.1 && M_J_solar < 100.0 && "Jeans mass should be of order Solar mass!");

  hot_jupiter::LarsonScalingLawsModel larson_model;
  double sigma_v = larson_model.velocity_dispersion_m_s(1.0);
  std::cout << "--> Larson Velocity Dispersion at 1 pc: " << sigma_v << " m/s" << std::endl;
  assert(std::abs(sigma_v - 1100.0) < 50.0 && "Larson velocity dispersion at 1 pc should be ~1.1 km/s!");

  std::cout << "✅ All Multi-Domain Astrophysics C++ Tests PASSED!" << std::endl;
  return 0;
}
