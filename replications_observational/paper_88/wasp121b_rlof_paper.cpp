// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #88: WASP-121b Tidal Deformability & Near-RLOF Metal Escape Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #88: WASP-121b TIDAL DEFORMABILITY & RLOF ESCAPE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP121bDeformabilityRLOFModel model;

  const double prolate_ratio = model.prolate_deformation_ratio(); // ~ 1.08
  const double rlof_factor = model.roche_lobe_filling_factor();    // ~ 0.92
  const double mdot_metals = model.mass_loss_rate_g_s();           // ~ 1.0e11 g/s (100,000 tons/s)
  const double nuv_excess = model.nuv_fe_ii_excess_depth_percent();// ~ 0.85%

  std::cout << "WASP-121b Prolate Tidal Deformation: " << prolate_ratio << std::endl;
  std::cout << "Roche Lobe Filling Factor: " << rlof_factor << std::endl;
  std::cout << "Heavy Metal Mass Loss Rate: " << mdot_metals << " g/s (" << (mdot_metals / 1e6) << " tons/s)" << std::endl;
  std::cout << "NUV Metal Excess Absorption Depth: " << nuv_excess << " %" << std::endl;

  // Track Near-UV Transmission Spectrum from 2200 A to 2900 A (linear wavelength scale):
  // Continuum optical transit depth ~ 1.55%
  // Prominent Fe II forest (2300-2600 A) and Mg II resonance doublet (2796 A, 2803 A)
  std::ofstream out("replications_observational/paper_88/wasp121b_nuv_transmission.csv");
  out << "wavelength_angstrom,transit_depth_percent,continuum_transit_depth_percent\n";

  for (double lam = 2200.0; lam <= 2900.0; lam += 5.0) {
    double depth = 1.55; // Optical baseline transit depth

    // Fe II absorption blend (2300 - 2650 A)
    if (lam >= 2300.0 && lam <= 2650.0) {
      double fe_env = 0.85 * std::sin((lam - 2300.0) / 350.0 * M_PI);
      double fine_structure = 0.15 * std::cos(lam * 0.2);
      depth += (fe_env + fine_structure);
    }

    // Mg II h & k doublet (2796 A & 2803 A)
    if (lam >= 2770.0 && lam <= 2830.0) {
      double mg_peak = 0.92 * std::exp(-std::pow((lam - 2800.0) / 12.0, 2.0));
      depth += mg_peak;
    }

    out << lam << "," << depth << "," << 1.55 << "\n";
  }
  out.close();

  std::cout << "Generated WASP-121b NUV Transmission Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
