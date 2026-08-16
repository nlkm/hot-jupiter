// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #41: TRAPPIST-1e Habitability & Atmosphere Retention Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #41: TRAPPIST-1e HABITABILITY & ATMOSPHERE ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::Trappist1eHabitabilityAtmosphereModel model;
  double m_p = model.planet_mass_mearth();
  double r_p = model.planet_radius_rearth();
  double flux = model.incident_flux_relative();
  double t_day = model.dayside_temp_k();
  double p_co2 = model.co2_surface_pressure_bar();

  // Spitzer, Kepler/K2, and JWST MIRI observations (Gillon 2017, Agol 2021, Greene 2023)
  double obs_m = 0.692;   // M_Earth (0.692 +/- 0.022 M_Earth)
  double obs_r = 0.920;   // R_Earth (0.920 +/- 0.012 R_Earth)
  double obs_flux = 0.662;// Relative to Earth (0.662 S_Earth)
  double obs_tday = 245.0;// K dayside emission temperature limit
  double obs_pco2 = 1.0;  // bar CO2 atmospheric baseline

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Planetary Mass M_p (Model)          = " << m_p << " M_Earth (Observed: " << obs_m << " M_Earth)" << std::endl;
  std::cout << "Planetary Radius R_p (Model)        = " << r_p << " R_Earth (Observed: " << obs_r << " R_Earth)" << std::endl;
  std::cout << "Incident Stellar Flux S             = " << flux << " S_Earth (Observed: " << obs_flux << " S_Earth)" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Dayside Equilibrium Temperature     = " << t_day << " K (Observed: " << obs_tday << " K)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "CO2 Surface Pressure Baseline       = " << p_co2 << " bar (Observed: " << obs_pco2 << " bar)" << std::endl;
  std::cout << "Relative Mass Discrepancy           = " << std::abs((m_p - obs_m) / obs_m) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
