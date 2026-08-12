// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #14: HD 209458b Hydrodynamic Escape & STIS Ly-alpha Spectroscopy

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #14: HD 209458b HYDRODYNAMIC ESCAPE ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::HD209458bPhotoevaporationModel photo_model;
  double mdot_model = photo_model.mass_loss_rate_g_s();
  double depth_model = photo_model.lyman_alpha_transit_depth_percent();

  double hst_mdot_obs = 5.0e10; // g/s (Vidal-Madjar et al. 2003, Murray-Clay et al. 2009)
  double hst_depth_obs = 15.0; // %

  std::cout << std::scientific << std::setprecision(4);
  std::cout << "Hydrodynamic Mass Loss Rate (Model) = " << mdot_model << " g/s" << std::endl;
  std::cout << "HST STIS Inferred Mass Loss Rate   = " << hst_mdot_obs << " g/s" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Lyman-alpha Transit Depth (Model)   = " << depth_model << " % (Observed: " << hst_depth_obs << " %)" << std::endl;
  std::cout << "Relative Mass Loss Rate Model Error = " << std::abs((mdot_model - hst_mdot_obs) / hst_mdot_obs) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
