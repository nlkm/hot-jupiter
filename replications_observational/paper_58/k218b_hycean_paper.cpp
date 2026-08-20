// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #58: K2-18b Hycean Ocean World Transmission Spectroscopy Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #58: K2-18b HYCEAN OCEAN WORLD TRANSMISSION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::K218bHyceanAtmosphereModel model;

  const double r_planet_re = model.planet_radius_rearth(); // 2.61 R_earth
  const double r_star_rsun = 0.4445;                       // M2.5V dwarf host
  const double base_depth_ppm = std::pow((r_planet_re * 0.009168) / r_star_rsun, 2.0) * 1.0e6; // ~ 2900 ppm

  // Generate synthetic JWST NIRISS/NIRSpec transmission spectrum (0.9 - 5.2 um)
  std::ofstream out("replications_observational/paper_58/k218b_transmission_spectrum.csv");
  out << "wavelength_um,transit_depth_ppm,sigma_err_ppm\n";

  for (double lam = 0.9; lam <= 5.2; lam += 0.05) {
    // CH4 absorption bands at 1.65, 2.3, 3.3 um
    double delta_ch4 = 0.0;
    if (std::abs(lam - 1.65) < 0.15) delta_ch4 = 85.0 * std::exp(-std::pow((lam - 1.65) / 0.08, 2.0));
    if (std::abs(lam - 2.30) < 0.20) delta_ch4 = 110.0 * std::exp(-std::pow((lam - 2.30) / 0.10, 2.0));
    if (std::abs(lam - 3.35) < 0.30) delta_ch4 = 145.0 * std::exp(-std::pow((lam - 3.35) / 0.15, 2.0));

    // CO2 absorption band at 4.3 um
    double delta_co2 = 0.0;
    if (std::abs(lam - 4.30) < 0.25) delta_co2 = 130.0 * std::exp(-std::pow((lam - 4.30) / 0.12, 2.0));

    // Flat continuum with subtle Rayleigh scattering toward optical
    double delta_rayleigh = 30.0 * std::pow(1.0 / lam, 2.0);

    double total_depth = base_depth_ppm + delta_ch4 + delta_co2 + delta_rayleigh;
    double sigma_err = 18.0; // JWST precision per spectral channel

    out << lam << "," << total_depth << "," << sigma_err << "\n";
  }
  out.close();

  std::cout << "Generated K2-18b JWST Transmission Spectrum simulation data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
