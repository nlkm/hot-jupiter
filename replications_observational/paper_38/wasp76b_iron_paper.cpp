// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #38: WASP-76b Asymmetric Iron Condensation & Nightside Rain

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #38: WASP-76b ASYMMETRIC IRON RAIN ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP76bIronRainModel model;
  double t_day = model.dayside_temp_k();
  double t_night = model.nightside_temp_k();
  double fe_eve = model.evening_terminator_fe_absorption_percent();
  double fe_morn = model.morning_terminator_fe_absorption_percent();
  double t_cond = model.iron_condensation_temp_k();

  // VLT ESPRESSO high-resolution transmission spectroscopy (Ehrenreich et al. 2020 Nature)
  double obs_tday = 2500.0; // K (Dayside temperature)
  double obs_fe_eve = 0.45; // 0.45 +/- 0.05 % Fe I absorption at evening limb
  double obs_fe_morn = 0.0; // Undetected at morning limb (iron rainout)

  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Dayside Equilibrium Temperature     = " << t_day << " K (Observed: " << obs_tday << " K)" << std::endl;
  std::cout << "Nightside Temperature               = " << t_night << " K" << std::endl;
  std::cout << "Iron Condensation Temperature       = " << t_cond << " K" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Evening Terminator Fe I Depth       = " << fe_eve << " % (Observed: " << obs_fe_eve << " %)" << std::endl;
  std::cout << "Morning Terminator Fe I Depth       = " << fe_morn << " % (Observed: " << obs_fe_morn << " %)" << std::endl;
  std::cout << "Relative Evening Depth Discrepancy  = " << std::abs((fe_eve - obs_fe_eve) / obs_fe_eve) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
