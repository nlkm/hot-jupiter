// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #31: Titan Methane Thermodynamics & Atmospheric Superrotation

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #31: TITAN METHANE THERMODYNAMICS & SUPERROTATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TitanMethaneAtmosphereModel model;
  double p0 = model.surface_pressure_bar();
  double t0 = model.surface_temp_k();
  double lake_frac = model.methane_lake_fraction();
  double u_jet = model.superrotation_jet_speed_m_s();
  double depth_m = model.kraken_mare_depth_m();

  // Cassini RADAR altimetry & CIRS thermal wind observations (Lorenz et al. 2008, Hayes et al. 2018)
  double obs_p0 = 1.47;     // bar (Huygens HASI)
  double obs_t0 = 94.0;     // K (Huygens HASI)
  double obs_u_jet = 120.0; // m/s (CIRS thermal gradient superrotation)
  double obs_depth = 160.0; // m (Cassini RADAR altimetry Kraken Mare)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Surface Atmospheric Pressure P_0   = " << p0 << " bar (Observed: " << obs_p0 << " bar)" << std::endl;
  std::cout << "Surface Temperature T_0            = " << t0 << " K (Observed: " << obs_t0 << " K)" << std::endl;
  std::cout << "Methane Liquid Fraction in Lakes   = " << lake_frac * 100.0 << " %" << std::endl;
  std::cout << "Equatorial Jet Superrotation Speed = " << u_jet << " m/s (Observed: " << obs_u_jet << " m/s)" << std::endl;
  std::cout << "Kraken Mare Sea Depth              = " << depth_m << " m (Observed: " << obs_depth << " m)" << std::endl;
  std::cout << "Relative Pressure Discrepancy      = " << std::abs((p0 - obs_p0) / obs_p0) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
