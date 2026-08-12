// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #23: TOI-560b Young Sub-Neptune Hydrodynamic Escape Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #23: TOI-560b YOUNG SUB-NEPTUNE ESCAPE ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TOI560bSubNeptuneEscapeModel toi_model;
  double mdot_model = toi_model.mass_loss_rate_g_s();
  double hei_depth_model = toi_model.hei_10830_excess_depth_percent();
  double v_outflow_model = toi_model.outflow_velocity_km_s();

  double obs_mdot = 4.20e10; // g/s (Zhang et al. 2022)
  double obs_hei_depth = 0.68; // % (Keck HIRES / JWST NIRSpec)
  double obs_v_outflow = 10.0; // km/s

  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Mass Loss Rate (Model)              = " << mdot_model << " g/s (Observed: " << obs_mdot << " g/s)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "He I 10830A Excess Depth (Model)    = " << hei_depth_model << " % (Observed: " << obs_hei_depth << " %)" << std::endl;
  std::cout << "Outflow Velocity (Model)            = " << v_outflow_model << " km/s (Observed: " << obs_v_outflow << " km/s)" << std::endl;
  std::cout << "Relative Mass Loss Error            = " << std::abs((mdot_model - obs_mdot) / obs_mdot) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
