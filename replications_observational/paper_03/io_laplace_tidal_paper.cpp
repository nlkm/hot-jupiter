// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #3: Galileo NIMS / Juno JIRAM Io Tidal Heat Flow & Laplace Resonance
// First-principles replication of Peale et al. (1979), Spencer et al. (2000), & Veeder et al. (2012)

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::IoLaplaceTidalAnalysisModel model;

  // Galileo & Juno Observational Values
  double power_obs_tw = 105.0; // 1.05 x 10^14 W (Spencer et al. 2000, Veeder et al. 2012)
  double flux_obs_w_m2 = 2.52; // 2.52 W/m^2 surface average

  // 1. Model Tidal Power [TW]
  double power_calc_tw = model.io_tidal_power_tw();

  // 2. Model Surface Heat Flux [W/m^2]
  double flux_calc_w_m2 = model.surface_heat_flux_w_m2(power_calc_tw);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #3: GALILEO/JUNO IO TIDAL HEAT FLOW & LAPLACE RESONANCE" << std::endl;
  std::cout << "================================================================================" << std::endl;
  std::cout << "Galileo/Juno Observed Thermal Power: " << power_obs_tw << " TW | Model = " << power_calc_tw << " TW" << std::endl;
  std::cout << "Observed Surface Heat Flux:          " << flux_obs_w_m2 << " W/m^2 | Model = " << flux_calc_w_m2 << " W/m^2" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
