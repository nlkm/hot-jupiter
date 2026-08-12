// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #22: HAT-P-11b Metastable Helium He I 10830A Escape Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #22: HAT-P-11b METASTABLE HELIUM ESCAPE ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::HATP11bHeliumEscapeModel hat_model;
  double mdot_model = hat_model.mass_loss_rate_g_s();
  double hei_depth_model = hat_model.hei_10830_excess_depth_percent();
  double tail_r_model = hat_model.helium_tail_radius_rp();

  double obs_mdot = 2.50e10; // g/s (Mansfield et al. 2018)
  double obs_hei_depth = 1.08; // % (HST WFC3 / Keck HIRES)

  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Mass Loss Rate (Model)              = " << mdot_model << " g/s (Observed: " << obs_mdot << " g/s)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "He I 10830A Excess Depth (Model)    = " << hei_depth_model << " % (Observed: " << obs_hei_depth << " %)" << std::endl;
  std::cout << "Helium Cloud Tail Extent (Model)    = " << tail_r_model << " R_p" << std::endl;
  std::cout << "Relative Mass Loss Error            = " << std::abs((mdot_model - obs_mdot) / obs_mdot) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
