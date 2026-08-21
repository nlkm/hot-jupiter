// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #81: WASP-12b Tidal Orbital Decay Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #81: WASP-12b TIDAL ORBITAL DECAY" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP12bTidalDecayModel model;

  const double pdot_ms_yr = model.period_decay_rate_ms_yr(); // -29.27 ms/yr
  const double lifetime_myr = model.remaining_lifetime_myr(); // ~ 3.16 Myr
  const double p_days = 1.09142;

  std::cout << "WASP-12b Period Decay Rate: " << pdot_ms_yr << " ms/year" << std::endl;
  std::cout << "Remaining Orbital Lifetime: " << lifetime_myr << " Myr" << std::endl;

  // Track O - C Timing Deviations across Epochs N = 0 to 5000 (linear scale, 2008 to 2023):
  // Delta T(N) = 0.5 * (dP/dN) * N^2 = 0.5 * (pdot_yr / epochs_per_yr) * N^2
  std::ofstream out("replications_observational/paper_81/wasp12b_decay_timing.csv");
  out << "epoch_number,elapsed_years,omc_timing_deviation_minutes,constant_period_baseline_minutes\n";

  for (double epoch_N = 0.0; epoch_N <= 5000.0; epoch_N += 100.0) {
    double t_yr = (epoch_N * p_days) / 365.25;
    double omc_min = model.ttv_omc_minutes(epoch_N);

    out << epoch_N << "," << t_yr << "," << omc_min << "," << 0.0 << "\n";
  }
  out.close();

  std::cout << "Generated WASP-12b Tidal Decay Timing Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
