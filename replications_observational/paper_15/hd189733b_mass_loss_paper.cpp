// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #15: HD 189733b X-Ray Driven Mass Loss & Stellar Flare Response

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #15: HD 189733b X-RAY MASS LOSS ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::HD189733bMassLossModel mass_model;
  double mdot_quiescent = mass_model.quiescent_mass_loss_rate_g_s();
  double mdot_flare = mass_model.flare_mass_loss_rate_g_s();
  double depth_flare = mass_model.flare_lyman_alpha_transit_depth_percent();

  double hst_xmm_mdot_quiescent_obs = 4.8e10; // g/s (Lecavelier des Etangs et al. 2012, Bourrier et al. 2013)
  double hst_xmm_mdot_flare_obs = 4.5e11; // g/s
  double hst_depth_flare_obs = 14.4; // %

  std::cout << std::scientific << std::setprecision(4);
  std::cout << "Quiescent Mass Loss Rate (Model)    = " << mdot_quiescent << " g/s (Observed: " << hst_xmm_mdot_quiescent_obs << " g/s)" << std::endl;
  std::cout << "Flare-Enhanced Mass Loss (Model)    = " << mdot_flare << " g/s (Observed: " << hst_xmm_mdot_flare_obs << " g/s)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Flare Ly-alpha Transit Depth (Model) = " << depth_flare << " % (Observed: " << hst_depth_flare_obs << " %)" << std::endl;
  std::cout << "Relative Flare Mass Loss Model Error= " << std::abs((mdot_flare - hst_xmm_mdot_flare_obs) / hst_xmm_mdot_flare_obs) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
