// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #25: LTT 9779b Ultra-Hot Neptune Extreme Albedo & RLOF Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #25: LTT 9779b ULTRA-HOT NEPTUNE ALBEDO & RLOF ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::LTT9779bUltraHotNeptuneModel ltt_model;
  double albedo_model = ltt_model.geometric_albedo();
  double eclipse_depth_model = ltt_model.secondary_eclipse_depth_ppm();
  double mdot_model = ltt_model.mass_loss_rate_g_s();
  double t_day_model = ltt_model.day_side_temperature_k();

  double obs_albedo = 0.80; // (Hoyer et al. 2023)
  double obs_eclipse_depth = 225.0; // ppm (CHEOPS)
  double obs_mdot = 1.80e10; // g/s (Jenkins et al. 2020)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Geometric Albedo A_g (Model)        = " << albedo_model << " (Observed: " << obs_albedo << ")" << std::endl;
  std::cout << "Secondary Eclipse Depth (Model)     = " << eclipse_depth_model << " ppm (Observed: " << obs_eclipse_depth << " ppm)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Mass Loss Rate (Model)              = " << mdot_model << " g/s (Observed: " << obs_mdot << " g/s)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Day-Side Temperature (Model)        = " << t_day_model << " K" << std::endl;
  std::cout << "Relative Albedo Error               = " << std::abs((albedo_model - obs_albedo) / obs_albedo) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
