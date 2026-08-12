// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #16: GJ 436b Extended Hydrogen Cloud & Atmospheric Escape Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #16: GJ 436b EXTENDED HYDROGEN CLOUD ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::GJ436bHydrogenCloudModel cloud_model;
  double mdot_model = cloud_model.mass_loss_rate_g_s();
  double depth_model = cloud_model.lyman_alpha_transit_depth_percent();
  double duration_model = cloud_model.lyman_alpha_transit_duration_hours();

  double hst_mdot_obs = 2.2e10; // g/s (Ehrenreich et al. 2015, Bourrier et al. 2016)
  double hst_depth_obs = 56.3; // %
  double hst_duration_obs = 22.0; // hours

  std::cout << std::scientific << std::setprecision(4);
  std::cout << "Hydrodynamic Mass Loss Rate (Model) = " << mdot_model << " g/s (Observed: " << hst_mdot_obs << " g/s)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Peak Ly-alpha Transit Depth (Model) = " << depth_model << " % (Observed: " << hst_depth_obs << " %)" << std::endl;
  std::cout << "Extended Transit Duration (Model)   = " << duration_model << " hours (Observed: " << hst_duration_obs << " hours)" << std::endl;
  std::cout << "Relative Mass Loss Rate Model Error = " << std::abs((mdot_model - hst_mdot_obs) / hst_mdot_obs) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
