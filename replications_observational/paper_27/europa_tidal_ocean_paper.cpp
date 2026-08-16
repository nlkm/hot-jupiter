// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #27: Europa Subsurface Ocean & Conductive Ice Shell Tidal Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #27: EUROPA SUBSURFACE OCEAN & ICE SHELL TIDAL ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::EuropaTidalOceanModel model;
  double t_surf = model.surface_temp_k();
  double t_melt = model.basal_melt_temp_k();
  double d_ice = model.ice_shell_thickness_km();
  double f_cond = model.conductive_heat_flux_mw_m2();
  double p_tide = model.tidal_dissipation_power_tw();
  double d_ocean = model.ocean_layer_thickness_km();
  double b_ind = model.induced_magnetic_dipole_nt();

  // Galileo Magnetometer & NIMS observational measurements (Kivelson et al. 2000, Greeley et al. 2004)
  double obs_b_ind = 220.0; // nT (Galileo MAG flybys E4, E14, E26)
  double obs_d_ice = 20.0;  // km (Cycloid & lenticulae geomorphology)
  double obs_p_tide = 0.80; // TW (Laplace resonance equilibrium)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Surface Temperature T_surf          = " << t_surf << " K" << std::endl;
  std::cout << "Basal Ice Melting Temperature T_melt = " << t_melt << " K" << std::endl;
  std::cout << "Ice Shell Thickness D_ice (Model)   = " << d_ice << " km (Observed: " << obs_d_ice << " km)" << std::endl;
  std::cout << "Conductive Heat Flux F_cond (Model) = " << f_cond << " mW/m^2" << std::endl;
  std::cout << "Tidal Dissipation Power (Model)     = " << p_tide << " TW (Observed: " << obs_p_tide << " TW)" << std::endl;
  std::cout << "Subsurface Ocean Depth D_ocean      = " << d_ocean << " km" << std::endl;
  std::cout << "Induced Magnetic Dipole B_ind       = " << b_ind << " nT (Observed: " << obs_b_ind << " nT)" << std::endl;
  std::cout << "Relative Magnetic Fit Discrepancy   = " << std::abs((b_ind - obs_b_ind) / obs_b_ind) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
