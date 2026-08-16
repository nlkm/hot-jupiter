// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #26: WASP-39b JWST Transmission Spectroscopy & Photochemical SO2 Production

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #26: WASP-39b JWST TRANSMISSION SPECTROSCOPY ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP39bTransmissionModel model;
  double r_p = model.planet_radius_rjup();
  double t_eq = model.equilibrium_temperature_k();
  double h_km = model.atmospheric_scale_height_km();
  double co2_ppm = model.co2_transit_depth_ppm();
  double h2o_ppm = model.h2o_transit_depth_ppm();
  double so2_ppm = model.so2_transit_depth_ppm();
  double z_sol = model.atmospheric_metallicity_solar();

  // Observational JWST NIRSpec measurements (JWST ERS Team, Nature 2023; Rustamkulov et al. 2023)
  double obs_co2 = 22350.0; // ppm (4.3 um)
  double obs_so2 = 21420.0; // ppm (4.05 um)
  double obs_h2o = 21500.0; // ppm (1.4 um)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Planetary Radius R_p (Model)        = " << r_p << " R_Jup" << std::endl;
  std::cout << "Equilibrium Temperature T_eq        = " << t_eq << " K" << std::endl;
  std::cout << "Atmospheric Scale Height H          = " << h_km << " km" << std::endl;
  std::cout << "Atmospheric Metallicity [Z/Z_sun]   = " << z_sol << "x Solar" << std::endl;
  std::cout << "CO2 4.3um Transit Depth (Model)     = " << co2_ppm << " ppm (Observed: " << obs_co2 << " ppm)" << std::endl;
  std::cout << "SO2 4.05um Transit Depth (Model)    = " << so2_ppm << " ppm (Observed: " << obs_so2 << " ppm)" << std::endl;
  std::cout << "H2O 1.4um Transit Depth (Model)     = " << h2o_ppm << " ppm (Observed: " << obs_h2o << " ppm)" << std::endl;
  std::cout << "Relative Spectral Fit Discrepancy   = " << std::abs((co2_ppm - obs_co2) / obs_co2) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
