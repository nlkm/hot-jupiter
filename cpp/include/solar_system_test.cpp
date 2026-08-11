// C++ Unit Test for Solar System Bodies & Orbital Dynamics Library

#include <cassert>
#include <cmath>
#include <iostream>

#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Solar System Dynamics C++ Tests ===" << std::endl;

  hot_jupiter::MoonTidalDynamicsModel moon_model;
  double P_io = moon_model.io_tidal_heating_power_watts();
  std::cout << "--> Io Tidal Heating Power: " << P_io / 1.0e12 << " TW" << std::endl;
  assert(P_io > 1.0e13 && "Io tidal heating power out of expected range!");

  double rate_recession = moon_model.earth_moon_recession_rate_m_s();
  double cm_per_yr = rate_recession * 100.0 * 365.25 * 86400.0;
  std::cout << "--> Earth-Moon Recession Rate: " << cm_per_yr << " cm/yr" << std::endl;
  assert(std::abs(cm_per_yr - 3.8) < 0.5 && "Lunar recession rate should be ~3.8 cm/yr!");

  hot_jupiter::PlanetaryRingModel ring_model;
  double r_saturn_roche = ring_model.roche_limit_m(6.0268e7, 687.0, 1000.0, true);
  std::cout << "--> Saturn Fluid Roche Limit: " << r_saturn_roche / 1.0e6 << " 10^3 km" << std::endl;
  assert(r_saturn_roche > 1.0e8 && "Saturn Roche limit out of range!");

  hot_jupiter::AsteroidDynamicsModel asteroid_model;
  double a_yark = asteroid_model.yarkovsky_acceleration_m_s2(500.0, 2000.0, 2.5, 30.0);
  std::cout << "--> Yarkovsky Acceleration (500m asteroid at 2.5 AU): " << a_yark << " m/s^2" << std::endl;
  assert(a_yark > 1.0e-15 && "Yarkovsky acceleration should be positive!");
  assert(asteroid_model.in_kirkwood_gap(2.50) && "2.50 AU should be in 3:1 Kirkwood gap!");

  hot_jupiter::CometDynamicsModel comet_model;
  double g_1au = comet_model.marsden_sublimation_g_r(1.0);
  std::cout << "--> Marsden Comet Sublimation g(1 AU): " << g_1au << std::endl;
  assert(g_1au > 0.05 && "Marsden sublimation g(r) at 1 AU out of range!");

  std::cout << "✅ All Solar System Dynamics C++ Tests PASSED!" << std::endl;
  return 0;
}
