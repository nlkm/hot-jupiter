// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #21: KELT-9b Ultra-Hot Thermosphere & H-alpha Absorption Spectroscopy

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #21: KELT-9b THERMOSPHERE ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::KELT9bUltraHotThermosphereModel kelt_model;
  double scale_height = kelt_model.scale_height_km();
  double r_ratio = kelt_model.thermosphere_radius_ratio();
  double halpha_depth = kelt_model.halpha_excess_depth_percent();

  double obs_r_ratio = 1.32; // R_p (Yan & Henning 2018)
  double obs_halpha_depth = 1.15; // % (CARMENES / HARPS-N)

  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Thermospheric Scale Height (Model)  = " << scale_height << " km" << std::endl;
  std::cout << std::setprecision(2);
  std::cout << "Thermospheric Radius Ratio (Model)  = " << r_ratio << " R_p (Observed: " << obs_r_ratio << " R_p)" << std::endl;
  std::cout << "H-alpha Excess Absorption (Model)   = " << halpha_depth << " % (Observed: " << obs_halpha_depth << " %)" << std::endl;
  std::cout << "Relative H-alpha Line Depth Error  = " << std::abs((halpha_depth - obs_halpha_depth) / obs_halpha_depth) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
