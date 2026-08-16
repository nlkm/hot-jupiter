// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #39: Kepler-11 Compact Coplanar Resonant Architecture & TTV Inversion

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #39: KEPLER-11 COMPACT MULTI-PLANET TTV ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::Kepler11CompactResonantModel model;
  int n_planets = model.number_of_planets();
  double inc_max = model.mutual_inclination_max_deg();
  double rho = model.mean_bulk_density_g_cm3();
  double pratio_cd = model.planet_c_d_period_ratio();
  double ttv_amp = model.ttv_amplitude_minutes();

  // Kepler transit photometry & TTV dynamical inversions (Lissauer et al. 2011, 2013 Nature)
  int obs_n = 6;            // 6 transiting coplanar planets
  double obs_inc = 1.0;     // Mutual inclination < 1 deg
  double obs_rho = 1.20;    // Low bulk density ~ 0.5 - 1.7 g/cm^3
  double obs_pratio = 1.74; // P_d / P_c ~ 1.74 (near 7:4 MMR)
  double obs_ttv = 24.5;    // minutes TTV chopping amplitude for planet d

  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Number of Coplanar Planets          = " << n_planets << " (Observed: " << obs_n << ")" << std::endl;
  std::cout << "Max Mutual Inclination              = " << inc_max << " deg (Observed: < " << obs_inc << " deg)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Mean Planet Bulk Density            = " << rho << " g/cm^3 (Observed: " << obs_rho << " g/cm^3)" << std::endl;
  std::cout << "Period Ratio P_d / P_c              = " << pratio_cd << " (Observed: " << obs_pratio << ")" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Planet d TTV Amplitude              = " << ttv_amp << " min (Observed: " << obs_ttv << " min)" << std::endl;
  std::cout << "Relative Density Discrepancy        = " << std::abs((rho - obs_rho) / obs_rho) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
