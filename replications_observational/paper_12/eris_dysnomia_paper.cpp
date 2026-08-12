// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #12: Eris-Dysnomia Mutual Binary Orbit & Density Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #12: ERIS-DYSNOMIA MUTUAL BINARY ORBIT ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::ErisDysnomiaModel ed_model;
  double P_model = ed_model.orbital_period_days();
  double rho_model = ed_model.eris_bulk_density_kg_m3();

  double alma_hst_P_obs = 15.7740; // days (Brown & Schaller 2007, Holler et al. 2021)
  double alma_hst_rho_obs = 2520.0; // kg/m^3

  std::cout << std::fixed << std::setprecision(5);
  std::cout << "Mutual Orbital Period (Model)        = " << P_model << " days" << std::endl;
  std::cout << "ALMA & HST Observed Period          = " << alma_hst_P_obs << " days" << std::endl;
  std::cout << "Eris Bulk Density (Model)           = " << rho_model << " kg/m^3 (Observed: " << alma_hst_rho_obs << " kg/m^3)" << std::endl;
  std::cout << "Relative Period Model Agreement     = " << std::abs((P_model - alma_hst_P_obs) / alma_hst_P_obs) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
