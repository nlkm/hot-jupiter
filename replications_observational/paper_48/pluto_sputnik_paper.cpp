// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #48: Pluto Sputnik Planitia Solid-State Nitrogen Convection

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #48: PLUTO SPUTNIK PLANITIA CONVECTION ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::PlutoSputnikPlanitiaConvectionModel model;
  double d_cell = model.cell_diameter_km();
  double tau_turn = model.overturning_timescale_years();
  double h_layer = model.nitrogen_ice_thickness_km();
  double ra_num = model.rayleigh_number();

  // New Horizons LORRI & LEISA observations (McKinnon 2016 Nature, Stern 2015 Science)
  double obs_dcell = 30.0;     // km polygonal cell width (20 - 40 km)
  double obs_tauturn = 5.0e5;  // years glacier overturning timescale (~ 500,000 yr)
  double obs_hlayer = 6.0;     // km nitrogen ice layer depth (4 - 8 km)
  double obs_ra = 1.0e7;       // Rayleigh number (10^6 - 10^8)

  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Polygonal Cell Diameter (Model)     = " << d_cell << " km (Observed: " << obs_dcell << " km)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Glacial Overturning Timescale       = " << tau_turn << " years (Observed: " << obs_tauturn << " years)" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Nitrogen Ice Sheet Thickness        = " << h_layer << " km (Observed: " << obs_hlayer << " km)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Rayleigh Convection Number          = " << ra_num << " (Observed: " << obs_ra << ")" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Relative Cell Width Discrepancy     = " << std::abs((d_cell - obs_dcell) / obs_dcell) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
