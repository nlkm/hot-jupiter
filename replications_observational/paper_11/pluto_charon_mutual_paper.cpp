// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #11: Pluto-Charon Mutual Binary Orbit & Mass Ratio Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #11: PLUTO-CHARON MUTUAL BINARY ORBIT ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::PlutoCharonMutualModel pc_model;
  double P_model = pc_model.orbital_period_days();
  double r_bary_model = pc_model.barycenter_distance_km();
  double q_model = pc_model.mass_ratio();

  double new_horizons_P_obs = 6.38723; // days (Stern et al. 2015, Brozovic et al. 2015)
  double new_horizons_r_bary_obs = 2127.0; // km
  double new_horizons_q_obs = 0.1217;

  std::cout << std::fixed << std::setprecision(5);
  std::cout << "Mutual Orbital Period (Model)        = " << P_model << " days" << std::endl;
  std::cout << "New Horizons & HST Observed Period   = " << new_horizons_P_obs << " days" << std::endl;
  std::cout << "Barycenter Distance from Pluto (km)  = " << r_bary_model << " km (Observed: " << new_horizons_r_bary_obs << " km)" << std::endl;
  std::cout << "Charon / Pluto Mass Ratio q          = " << q_model << " (Observed: " << new_horizons_q_obs << ")" << std::endl;
  std::cout << "Relative Period Model Agreement      = " << std::abs((P_model - new_horizons_P_obs) / new_horizons_P_obs) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
