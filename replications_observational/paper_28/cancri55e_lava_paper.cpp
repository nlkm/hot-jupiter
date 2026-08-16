// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #28: 55 Cancri e Ultra-Short-Period Lava World & Atmospheric Radiative Equilibrium

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #28: 55 CANCRI e LAVA WORLD RADIATIVE EQUILIBRIUM" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::Cancri55eLavaAtmosphereModel model;
  double t_sub = model.substellar_temp_k();
  double t_night = model.nightside_temp_k();
  double shift_deg = model.eastward_hotspot_shift_deg();
  double eclipse_flux = model.secondary_eclipse_flux_4_5um_ppm();
  double p_vapor = model.mineral_vapor_pressure_bar();

  // Spitzer IRAC & JWST NIRCam observations (Demory et al. 2016, Zhang et al. 2024)
  double obs_t_sub = 2700.0;    // K (Demory et al. 2016 Nature)
  double obs_t_night = 1380.0;  // K
  double obs_shift = 41.0;      // deg Eastward
  double obs_eclipse = 130.0;   // ppm (Spitzer 4.5 um)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Sub-Stellar Temperature T_sub (Model) = " << t_sub << " K (Observed: " << obs_t_sub << " K)" << std::endl;
  std::cout << "Night-Side Temperature T_night (Model) = " << t_night << " K (Observed: " << obs_t_night << " K)" << std::endl;
  std::cout << "Eastward Hotspot Phase Shift (Model)  = " << shift_deg << " deg (Observed: " << obs_shift << " deg)" << std::endl;
  std::cout << "Secondary Eclipse Flux 4.5um (Model)  = " << eclipse_flux << " ppm (Observed: " << obs_eclipse << " ppm)" << std::endl;
  std::cout << "Mineral Vapor Atmosphere Pressure     = " << p_vapor << " bar" << std::endl;
  std::cout << "Relative Temperature Discrepancy      = " << std::abs((t_sub - obs_t_sub) / obs_t_sub) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
