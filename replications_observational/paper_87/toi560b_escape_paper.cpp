// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #87: TOI-560b Young Sub-Neptune Helium Escape Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #87: TOI-560b YOUNG SUB-NEPTUNE HELIUM ESCAPE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TOI560bSubNeptuneEscapeModel model;

  const double mdot_g_s = model.mass_loss_rate_g_s(); // ~ 4.20e10 g/s (~ 42,000 kg/s)
  const double excess_depth_pct = model.hei_10830_excess_depth_percent(); // ~ 0.68%
  const double v_outflow = model.outflow_velocity_km_s(); // ~ 10.2 km/s

  std::cout << "TOI-560b Mass Loss Rate: " << mdot_g_s << " g/s (" << (mdot_g_s / 1e6) << " tons/s)" << std::endl;
  std::cout << "He I 10830A Excess Absorption Depth: " << excess_depth_pct << " %" << std::endl;
  std::cout << "Hydrodynamic Outflow Velocity: " << v_outflow << " km/s" << std::endl;

  // Track High-Resolution Transmission Spectrum across He I triplet (linear velocity scale -40 to +40 km/s):
  // Young active K-dwarf host star (age ~ 500 Myr) drives strong photoevaporative wind blueshifted by -10.2 km/s
  std::ofstream out("replications_observational/paper_87/toi560b_helium_transmission.csv");
  out << "doppler_velocity_km_s,relative_absorption_depth_pct,continuum_baseline_pct\n";

  const double v_wind = -10.2; // km/s net blueshift
  const double fwhm = 14.8;    // km/s resolved instrumental + thermal FWHM

  for (double v_kms = -40.0; v_kms <= 40.0; v_kms += 1.0) {
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

  std::cout << "Generated TOI-560b He I 10830A Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
