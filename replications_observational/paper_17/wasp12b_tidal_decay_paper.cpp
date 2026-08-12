// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #17: WASP-12b Tidal Orbital Decay & Stellar Dissipation Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #17: WASP-12b TIDAL ORBITAL DECAY ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP12bTidalDecayModel decay_model;
  double pdot_model = decay_model.period_decay_rate_ms_yr();
  double omc_5000 = decay_model.ttv_omc_minutes(5000.0);
  double lifetime_model = decay_model.remaining_lifetime_myr();

  double obs_pdot = -29.0; // ms/year (Maciejewski et al. 2016, Yee et al. 2019, Wong et al. 2022)
  double obs_lifetime = 3.2; // Myr

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Orbital Period Decay Rate (Model) = " << pdot_model << " ms/year (Observed: " << obs_pdot << " ms/yr)" << std::endl;
  std::cout << "TTV O-C Deviation at N=5000       = " << omc_5000 << " minutes" << std::endl;
  std::cout << "Remaining Lifetime to Merger       = " << lifetime_model << " Myr (Expected: " << obs_lifetime << " Myr)" << std::endl;
  std::cout << "Relative Period Decay Rate Error   = " << std::abs((pdot_model - obs_pdot) / obs_pdot) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
