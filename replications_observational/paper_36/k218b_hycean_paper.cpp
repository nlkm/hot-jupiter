// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #36: K2-18b Hycean Atmosphere & JWST Transmission Spectroscopy

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #36: K2-18b HYCEAN ATMOSPHERE & JWST SPECTROSCOPY" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::K218bHyceanAtmosphereModel model;
  double m_p = model.planet_mass_mearth();
  double r_p = model.planet_radius_rearth();
  double ch4 = model.methane_volume_mixing_ratio();
  double co2 = model.co2_volume_mixing_ratio();
  double nh3 = model.ammonia_upper_limit();

  // JWST NIRISS & NIRSpec G395H observations (Madhusudhan et al. 2023 ApJL)
  double obs_m = 8.63;   // M_Earth (8.63 +/- 1.35 M_Earth)
  double obs_r = 2.61;   // R_Earth (2.61 +/- 0.09 R_Earth)
  double obs_ch4 = 0.01; // ~1% CH4 detection at 3.3 um
  double obs_co2 = 0.01; // ~1% CO2 detection at 4.3 um
  double obs_nh3 = 1e-5; // Ammonia depletion (< 10 ppm)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Planetary Mass M_p (Model)          = " << m_p << " M_Earth (Observed: " << obs_m << " M_Earth)" << std::endl;
  std::cout << "Planetary Radius R_p (Model)        = " << r_p << " R_Earth (Observed: " << obs_r << " R_Earth)" << std::endl;
  std::cout << "Methane CH_4 Mixing Ratio           = " << ch4 * 100.0 << " % (Observed: " << obs_ch4 * 100.0 << " %)" << std::endl;
  std::cout << "Carbon Dioxide CO_2 Mixing Ratio    = " << co2 * 100.0 << " % (Observed: " << obs_co2 * 100.0 << " %)" << std::endl;
  std::cout << std::scientific << std::setprecision(1);
  std::cout << "Ammonia NH_3 Upper Limit            = " << nh3 << " (Observed: < " << obs_nh3 << ")" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Relative Mass Discrepancy           = " << std::abs((m_p - obs_m) / obs_m) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
