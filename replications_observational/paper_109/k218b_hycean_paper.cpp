// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #109: K2-18b Hycean Atmosphere & Ocean Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #109: K2-18b HYCEAN ATMOSPHERE & WATER OCEAN" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::K218bHyceanAtmosphereModel model;

  const double m_p = model.planet_mass_mearth();              // ~ 8.63 M_earth
  const double r_p = model.planet_radius_rearth();            // ~ 2.61 R_earth
  const double vmr_ch4 = model.methane_volume_mixing_ratio();  // ~ 0.010 (1%)
  const double vmr_co2 = model.co2_volume_mixing_ratio();      // ~ 0.010 (1%)
  const double vmr_nh3 = model.ammonia_upper_limit();          // ~ 1.0e-5 (< 10 ppm)
  const double f_water = model.ocean_water_mass_fraction();    // ~ 0.50 (50% water layer)

  std::cout << "K2-18b Mass: " << m_p << " M_earth" << std::endl;
  std::cout << "K2-18b Radius: " << r_p << " R_earth" << std::endl;
  std::cout << "CH4 Volume Mixing Ratio: " << (vmr_ch4 * 100.0) << " %" << std::endl;
  std::cout << "CO2 Volume Mixing Ratio: " << (vmr_co2 * 100.0) << " %" << std::endl;
  std::cout << "NH3 Volume Mixing Ratio Upper Limit: " << (vmr_nh3 * 1e6) << " ppm" << std::endl;
  std::cout << "Bulk Water Interior Fraction: " << (f_water * 100.0) << " %" << std::endl;

  // Track Transmission Spectrum from 0.8 um to 5.2 um (linear wavelength scale):
  // Baseline transit depth ~ 2730 ppm (0.273%)
  std::ofstream out("replications_observational/paper_109/k218b_jwst_transmission.csv");
  out << "wavelength_um,transit_depth_ppm,ch4_band_contribution_ppm,co2_band_contribution_ppm\n";

  for (double lam = 0.8; lam <= 5.2; lam += 0.04) {
    double depth_base = 2730.0;

    // Rayleigh scattering tail in visible/optical
    double rayleigh = 45.0 * std::pow(1.0 / lam, 4.0);

    // Methane bands at 1.4, 1.66, 2.3, 3.3 um
    double ch4_14 = 85.0 * std::exp(-std::pow((lam - 1.40) / 0.08, 2.0));
    double ch4_16 = 110.0 * std::exp(-std::pow((lam - 1.66) / 0.09, 2.0));
    double ch4_23 = 165.0 * std::exp(-std::pow((lam - 2.32) / 0.15, 2.0));
    double ch4_33 = 240.0 * std::exp(-std::pow((lam - 3.35) / 0.22, 2.0));
    double ch4_tot = ch4_14 + ch4_16 + ch4_23 + ch4_33;

    // CO2 band at 4.3 um
    double co2_43 = 210.0 * std::exp(-std::pow((lam - 4.30) / 0.18, 2.0));

    // Total transmission model
    double depth_tot = depth_base + rayleigh + ch4_tot + co2_43;

    out << lam << "," << depth_tot << "," << ch4_tot << "," << co2_43 << "\n";
  }
  out.close();

  std::cout << "Generated K2-18b Hycean Transmission Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
