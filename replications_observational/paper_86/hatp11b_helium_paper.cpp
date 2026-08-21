// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #86: HAT-P-11b Metastable Helium He I 10830A Escape Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #86: HAT-P-11b METASTABLE HELIUM He I 10830A ESCAPE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::HATP11bHeliumEscapeModel model;

  const double mdot_g_s = model.mass_loss_rate_g_s(); // ~ 2.5e10 g/s (~ 25,000 kg/s)
  const double excess_depth_pct = model.hei_10830_excess_depth_percent(); // ~ 1.08%
  const double tail_rp = model.helium_tail_radius_rp(); // ~ 2.5 R_p

  std::cout << "HAT-P-11b Mass Loss Rate: " << mdot_g_s << " g/s (" << (mdot_g_s / 1e6) << " tons/s)" << std::endl;
  std::cout << "He I 10830A Excess Absorption Depth: " << excess_depth_pct << " %" << std::endl;
  std::cout << "Escaping Helium Cloud Tail Radius: " << tail_rp << " R_p" << std::endl;

  // Track High-Resolution Transmission Spectrum across the He I triplet (linear velocity scale -50 to +50 km/s):
  // Triplet components: Tr1 at -29.6 km/s (relative), Tr2 & Tr3 blended near 0 km/s
  // Net exospheric blueshift ~ -3.0 km/s due to stellar wind interaction
  std::ofstream out("replications_observational/paper_86/hatp11b_helium_transmission.csv");
  out << "doppler_velocity_km_s,relative_absorption_depth_pct,continuum_baseline_pct\n";

  const double v_wind = -3.0; // km/s blueshift
  const double fwhm = 16.5;   // km/s resolved instrumental + thermal FWHM

  for (double v_kms = -50.0; v_kms <= 50.0; v_kms += 1.0) {
    // Blended major doublet component (Tr2 & Tr3 at lambda = 10830.25 A and 10830.34 A)
    double v_diff_main = v_kms - v_wind;
    double prof_main = (8.0 / 9.0) * excess_depth_pct * std::exp(-std::pow(v_diff_main / (fwhm / 1.665), 2.0));

    // Minor singlet component (Tr1 at lambda = 10829.09 A, offset by -32.0 km/s)
    double v_diff_weak = v_kms - (v_wind - 32.0);
    double prof_weak = (1.0 / 9.0) * excess_depth_pct * std::exp(-std::pow(v_diff_weak / (fwhm / 1.665), 2.0));

    double total_prof = prof_main + prof_weak;

    out << v_kms << "," << total_prof << "," << 0.0 << "\n";
  }
  out.close();

  std::cout << "Generated HAT-P-11b He I 10830A Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
