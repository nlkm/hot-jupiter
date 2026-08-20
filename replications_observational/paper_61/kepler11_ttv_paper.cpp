// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #61: Kepler-11 Compact Coplanar Resonances & TTV Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #61: KEPLER-11 COMPACT MULTI-PLANET TTV DYNAMICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::Kepler11CompactResonantModel model;

  const double ttv_amp_min = model.ttv_amplitude_minutes(); // 24.5 min
  const double p_c = 13.025;                                // Period planet c (days)
  const double p_d = 22.687;                                // Period planet d (days)


  // Super-period for near-resonant perturbation: P_super = 1 / |j/P_d - k/P_c|
  const double p_super_days = 1.0 / std::abs(5.0 / p_d - 3.0 / p_c); // ~ 100 days

  // Generate TTV sinusoidal signature across 1200 days of Kepler observations
  std::ofstream out("replications_observational/paper_61/kepler11_ttv_track.csv");
  out << "time_bjd_offset,transit_num,ttv_o_c_minutes,sigma_err_min\n";

  int transit_count = 0;
  for (double t = 0.0; t <= 1200.0; t += p_c) {
    transit_count++;
    // TTV O-C oscillation: delta_t = A * sin(2*pi*t / P_super + phase)
    double ttv_val = ttv_amp_min * std::sin(2.0 * M_PI * t / p_super_days + 0.35);
    double sigma_err = 2.5; // Kepler photometric centroid timing error (minutes)

    out << t << "," << transit_count << "," << ttv_val << "," << sigma_err << "\n";
  }
  out.close();

  std::cout << "Generated Kepler-11 TTV Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
