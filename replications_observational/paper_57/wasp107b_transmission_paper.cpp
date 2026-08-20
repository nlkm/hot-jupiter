// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #57: WASP-107b Puffy Neptune JWST Transmission Spectroscopy Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #57: WASP-107b PUFFY SUPER-NEPTUNE TRANSMISSION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP107bPuffyNeptuneModel model;

  const double r_planet_rjup = model.planet_radius_rjup(); // 0.94 R_Jup
  const double r_star_rsun = 0.66;                         // K-dwarf host


  // Calculate synthetic transmission spectrum across JWST NIRCam/MIRI wavelength range (0.8 - 12.0 um)
  std::ofstream out("replications_observational/paper_57/wasp107b_transmission_spectrum.csv");
  out << "wavelength_um,transit_depth_ppm,sigma_err_ppm\n";

  for (double lam = 0.8; lam <= 12.0; lam += 0.1) {
    // Base geometric transit depth (Rp/Rstar)^2
    double base_depth = std::pow((r_planet_rjup * 0.10045) / r_star_rsun, 2.0) * 1.0e6; // ~ 20500 ppm

    // SO2 photochemical absorption feature at 4.05 um and 7.3 um
    double delta_so2 = 0.0;
    if (std::abs(lam - 4.05) < 0.3) {
      delta_so2 = 450.0 * std::exp(-std::pow((lam - 4.05) / 0.15, 2.0));
    } else if (std::abs(lam - 7.35) < 0.6) {
      delta_so2 = 650.0 * std::exp(-std::pow((lam - 7.35) / 0.35, 2.0));
    }

    // H2O absorption features at 1.4, 1.9, 2.7 um
    double delta_h2o = 0.0;
    if (std::abs(lam - 1.40) < 0.15) delta_h2o = 350.0 * std::exp(-std::pow((lam - 1.40) / 0.08, 2.0));
    if (std::abs(lam - 1.90) < 0.20) delta_h2o = 400.0 * std::exp(-std::pow((lam - 1.90) / 0.10, 2.0));
    if (std::abs(lam - 2.70) < 0.30) delta_h2o = 550.0 * std::exp(-std::pow((lam - 2.70) / 0.15, 2.0));

    // CO2 feature at 4.3 um
    double delta_co2 = 0.0;
    if (std::abs(lam - 4.30) < 0.20) delta_co2 = 600.0 * std::exp(-std::pow((lam - 4.30) / 0.10, 2.0));

    // Silicate cloud deck plateau longward of 9 um
    double delta_silicate = (lam >= 9.0) ? 250.0 * (1.0 - std::exp(-(lam - 9.0) / 1.5)) : 0.0;

    double total_depth = base_depth + delta_so2 + delta_h2o + delta_co2 + delta_silicate;
    double sigma_err = 35.0; // JWST precision per bin

    out << lam << "," << total_depth << "," << sigma_err << "\n";
  }
  out.close();

  std::cout << "Generated WASP-107b JWST Transmission Spectrum data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
