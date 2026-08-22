// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #102: GJ 1214b High-Metallicity Haze Deck & MIRI Emission Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #102: GJ 1214b AEROSOL HAZE & ATMOSPHERIC METALLICITY" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::GJ1214bAerosolHazeModel model;

  const double m_p = model.planet_mass_mearth();         // ~ 8.17 M_earth
  const double r_p = model.planet_radius_rearth();       // ~ 2.74 R_earth
  const double t_day = model.dayside_temp_k();           // ~ 553 K
  const double t_night = model.nightside_temp_k();       // ~ 437 K
  const double z_metal = model.metallicity_solar_factor();// ~ 500x solar
  const double r_haze = model.haze_particle_radius_um(); // ~ 0.05 um

  std::cout << "GJ 1214b Mass: " << m_p << " M_earth" << std::endl;
  std::cout << "GJ 1214b Radius: " << r_p << " R_earth" << std::endl;
  std::cout << "Dayside Effective Temperature: " << t_day << " K" << std::endl;
  std::cout << "Nightside Effective Temperature: " << t_night << " K" << std::endl;
  std::cout << "Atmospheric Metallicity Factor: " << z_metal << "x solar" << std::endl;
  std::cout << "Photochemical Haze Particle Radius: " << r_haze << " um" << std::endl;

  // Track Infrared Transmission Spectrum from 1.0 um to 12.0 um (linear wavelength scale):
  // Optical/NIR transit depth baseline ~ 1.345%
  // High-altitude haze deck suppresses H2O / CH4 / NH3 features down to < 150 ppm
  std::ofstream out("replications_observational/paper_102/gj1214b_haze_transmission.csv");
  out << "wavelength_um,transit_depth_percent,clear_atmosphere_depth_percent\n";

  for (double lam = 1.0; lam <= 12.0; lam += 0.1) {
    // Flatlined transmission spectrum with subtle Rayleigh/Mie scattering tail in NIR
    double depth_hazy = 1.345 + 0.008 * std::pow(1.0 / lam, 0.8) + 0.003 * std::sin(lam * 1.5);

    // Clear H2-dominated comparison (prominent 1.4 um and 2.7 um H2O bands, 3.3 um CH4 band)
    double depth_clear = 1.345 + 0.085 * std::exp(-std::pow((lam - 1.4) / 0.15, 2.0))
                               + 0.095 * std::exp(-std::pow((lam - 2.7) / 0.35, 2.0))
                               + 0.075 * std::exp(-std::pow((lam - 3.3) / 0.25, 2.0))
                               + 0.090 * std::exp(-std::pow((lam - 6.2) / 0.80, 2.0));

    out << lam << "," << depth_hazy << "," << depth_clear << "\n";
  }
  out.close();

  std::cout << "Generated GJ 1214b Haze Transmission Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
