// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #20: Kepler-223 8:6:4:3 Four-Planet Resonant Chain & TTV Astrometry

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #20: KEPLER-223 RESONANT CHAIN ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::Kepler223ResonantChainModel kep_model;
  double ttv_amp_model = kep_model.ttv_chopping_amplitude_minutes();
  double libration_model = kep_model.resonant_angle_libration_deg();
  double mass_c_model = kep_model.kepler223c_mass_mearth();

  double obs_ttv_amp = 14.2; // minutes (Mills et al. 2016)
  double obs_libration = 2.4; // degrees
  double obs_mass_c = 5.1; // M_Earth

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "TTV Chopping Amplitude (Model)     = " << ttv_amp_model << " minutes (Observed: " << obs_ttv_amp << " min)" << std::endl;
  std::cout << "Resonant Angle Libration (Model)   = " << libration_model << " deg (Observed: " << obs_libration << " deg)" << std::endl;
  std::cout << std::setprecision(3);
  std::cout << "Kepler-223c Dynamical Mass (Model)  = " << mass_c_model << " M_Earth (Observed: " << obs_mass_c << " M_Earth)" << std::endl;
  std::cout << std::setprecision(2);
  std::cout << "Relative TTV Chopping Error        = " << std::abs((ttv_amp_model - obs_ttv_amp) / obs_ttv_amp) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
