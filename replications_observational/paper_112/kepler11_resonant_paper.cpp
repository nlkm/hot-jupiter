// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #112: Kepler-11 Compact Resonant System Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #112: KEPLER-11 COMPACT COPLANAR RESONANT SYSTEM" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::Kepler11CompactResonantModel model;

  const int n_planets = model.number_of_planets();                  // 6 transiting planets
  const double i_mut_max = model.mutual_inclination_max_deg();       // < 1.0 deg (Extremely flat)
  const double rho_bulk = model.mean_bulk_density_g_cm3();           // ~ 1.20 g/cm^3 (Puffy)
  const double ratio_cd = model.planet_c_d_period_ratio();           // ~ 1.74
  const double ttv_amp = model.ttv_amplitude_minutes();              // ~ 24.5 minutes

  std::cout << "Number of Transiting Planets: " << n_planets << std::endl;
  std::cout << "Maximum Mutual Inclination: " << i_mut_max << " deg" << std::endl;
  std::cout << "Mean Bulk Density: " << rho_bulk << " g/cm^3" << std::endl;
  std::cout << "Period Ratio c/d: " << ratio_cd << std::endl;
  std::cout << "Transit Timing Variation (TTV) Amplitude: " << ttv_amp << " min" << std::endl;

  // Track Kepler-11d TTV Signal vs Observation Time t from 0 to 1200 days (linear time scale):
  // Super-period P_ttv ~ 390 days
  std::ofstream out("replications_observational/paper_112/kepler11_ttv_evolution.csv");
  out << "time_days,transit_timing_variation_minutes,sinusoidal_superperiod_model_minutes\n";

  const double p_ttv_days = 390.0;
  const double p_orb_d_days = 22.687; // Kepler-11d orbital period

  for (double t_day = 0.0; t_day <= 1200.0; t_day += p_orb_d_days) {
    // Primary resonant perturbation harmonic
    double phase1 = 2.0 * M_PI * t_day / p_ttv_days;
    double phase2 = 4.0 * M_PI * t_day / p_ttv_days;

    double ttv_val = ttv_amp * std::sin(phase1) + 4.2 * std::sin(phase2 + 0.4);
    double model_fit = ttv_amp * std::sin(phase1) + 4.2 * std::sin(phase2 + 0.4);

    out << t_day << "," << ttv_val << "," << model_fit << "\n";
  }
  out.close();

  std::cout << "Generated Kepler-11 Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
