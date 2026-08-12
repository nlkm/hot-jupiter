// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #2: Cassini CIRS/CDA Enceladus Tidal Heat Flux & Ice Shell Analysis
// First-principles replication of Spencer et al. (2006, 2018) & Tobie et al. (2008)

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::EnceladusTidalAnalysisModel model;

  // Cassini Observational Dataset Values
  double heat_flux_obs_gw = 15.8; // Spencer et al. (2006, 2018), Howett et al. (2011)

  // 1. Model Tidal Dissipation Power
  double power_calc_gw = model.tidal_dissipation_power_gw();

  // 2. Model Global Ice Shell Conductive Heat Loss (d = 20 km)
  double global_heat_calc_gw = model.conductive_heat_flux_gw(20.0);

  // 3. Model South Polar Thin Ice Shell Conductive Heat Loss (d = 5 km, south polar area fraction ~ 0.1)
  double south_polar_heat_calc_gw = 0.1 * model.conductive_heat_flux_gw(5.0);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #2: CASSINI CIRS ENCELADUS TIDAL DISSIPATION & HEAT FLUX" << std::endl;
  std::cout << "================================================================================" << std::endl;
  std::cout << "Cassini Observed CIRS Heat Flux: " << heat_flux_obs_gw << " GW" << std::endl;
  std::cout << "Model Tidal Dissipation Power:   " << power_calc_gw << " GW" << std::endl;
  std::cout << "Model Global Ice Shell Flux:     " << global_heat_calc_gw << " GW (d_avg = 20 km)" << std::endl;
  std::cout << "Model South Polar Thin Shell:    " << south_polar_heat_calc_gw << " GW (d_south = 5 km)" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
