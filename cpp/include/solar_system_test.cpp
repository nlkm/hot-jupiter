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

  hot_jupiter::RelativisticPrecessionModel gr_model;
  double merc_gr = gr_model.mercury_gr_precession_arcsec_century();
  std::cout << "--> Mercury GR Perihelion Precession: " << merc_gr << " arcsec/century" << std::endl;
  assert(std::abs(merc_gr - 43.0) < 3.0 && "Mercury GR precession should be ~43 arcsec/century!");

  hot_jupiter::PlanetNineSecularModel p9_model;
  double p9_prec = p9_model.planet_nine_secular_precession_rad_yr(250.0);
  std::cout << "--> Planet Nine TNO Precession (250 AU): " << p9_prec << " rad/yr" << std::endl;
  assert(p9_prec > 1.0e-10 && "Planet Nine secular precession should be positive!");

  hot_jupiter::LaplaceLagrangeSecularModel ll_model;
  double g5 = ll_model.jupiter_secular_g5_arcsec_yr();
  double g6 = ll_model.saturn_secular_g6_arcsec_yr();
  std::cout << "--> Secular Eigenfrequencies: g5 = " << g5 << ", g6 = " << g6 << " arcsec/yr" << std::endl;
  assert(std::abs(g5 - 4.257) < 0.01 && "g5 frequency mismatch!");
  assert(std::abs(g6 - 28.245) < 0.01 && "g6 frequency mismatch!");

  hot_jupiter::NiceModelResonanceCrossing nice_model;
  double kick = nice_model.ice_giant_eccentricity_kick(0.0, 35.0);
  std::cout << "--> Nice Model Ice Giant Eccentricity Kick: " << kick << std::endl;
  assert(kick > 0.10 && "Nice Model eccentricity kick out of range!");

  hot_jupiter::SeasonalYarkovskyModel seasonal_model;
  double drift = seasonal_model.seasonal_drift_rate_au_myr(500.0, 2000.0, 2.5, 90.0);
  std::cout << "--> Seasonal Yarkovsky Drift Rate (90 deg obl): " << drift << " AU/Myr" << std::endl;
  assert(drift < 0.0 && "Seasonal Yarkovsky drift rate should be negative!");

  hot_jupiter::SaturnRingLindbladResonanceModel lindblad_model;
  double torque = lindblad_model.lindblad_resonance_torque_nm(1.4e17, 1.3935e8);
  std::cout << "--> Lindblad Ring Torque: " << torque << " N m" << std::endl;
  assert(torque > 1.0e8 && "Lindblad torque should be positive!");

  hot_jupiter::CetoPhorcysBinaryModel ceto_model;
  double ceto_p = ceto_model.orbital_period_days();
  double ceto_rho = ceto_model.system_bulk_density_kg_m3();
  std::cout << "--> Ceto-Phorcys Orbital Period: " << ceto_p << " days, Density: " << ceto_rho << " kg/m^3" << std::endl;
  assert(std::abs(ceto_p - 9.554) < 0.05 && "Ceto orbital period mismatch!");
  assert(std::abs(ceto_rho - 1370.0) < 50.0 && "Ceto density mismatch!");

  std::cout << "✅ All Solar System Dynamics C++ Tests PASSED!" << std::endl;
  return 0;
}
