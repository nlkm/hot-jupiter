// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #24: WASP-121b Extreme Tidal Deformability & RLOF Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #24: WASP-121b TIDAL DEFORMABILITY & RLOF ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP121bDeformabilityRLOFModel wasp_model;
  double prolate_ratio = wasp_model.prolate_deformation_ratio();
  double rlof_factor = wasp_model.roche_lobe_filling_factor();
  double mdot_model = wasp_model.mass_loss_rate_g_s();
  double fe_ii_depth = wasp_model.nuv_fe_ii_excess_depth_percent();
  double delta_t_model = wasp_model.day_night_temp_contrast_k();

  double obs_prolate = 1.08;
  double obs_mdot = 1.00e11; // g/s (Sing et al. 2019)
  double obs_fe_ii_depth = 0.85; // % (HST STIS)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Prolate Deformation Ratio (Model)   = " << prolate_ratio << " R_p (Observed: " << obs_prolate << " R_p)" << std::endl;
  std::cout << "Roche Lobe Filling Factor (Model)   = " << rlof_factor << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Heavy Metal Mass Loss (Model)       = " << mdot_model << " g/s (Observed: " << obs_mdot << " g/s)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "NUV Fe II Excess Depth (Model)      = " << fe_ii_depth << " % (Observed: " << obs_fe_ii_depth << " %)" << std::endl;
  std::cout << "Day-Night Temp Contrast (Model)     = " << delta_t_model << " K" << std::endl;
  std::cout << "Relative Mass Loss Error            = " << std::abs((mdot_model - obs_mdot) / obs_mdot) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
