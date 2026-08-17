// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #46: GJ 1214b Super-Earth Aerosol Haze & Atmospheric Metallicity

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #46: GJ 1214b AEROSOL HAZE & METALLICITY ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::GJ1214bAerosolHazeModel model;
  double m_p = model.planet_mass_mearth();
  double r_p = model.planet_radius_rearth();
  double t_day = model.dayside_temp_k();
  double t_night = model.nightside_temp_k();
  double z_met = model.metallicity_solar_factor();
  double r_haze = model.haze_particle_radius_um();

  // HST WFC3 & JWST MIRI 5-12 micron phase curve (Bean 2010 Nature, Kempton 2023 Nature)
  double obs_m = 8.17;       // M_Earth (8.17 +/- 0.43 M_Earth)
  double obs_r = 2.74;       // R_Earth (2.74 +/- 0.05 R_Earth)
  double obs_tday = 553.0;   // K dayside brightness temperature (553 +/- 12 K)
  double obs_tnight = 437.0; // K nightside brightness temperature (437 +/- 19 K)
  double obs_z = 500.0;      // Solar metallicity factor (> 100x solar)
  double obs_rhaze = 0.05;   // um photochemical hydrocarbon soot radius

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Planetary Mass (Model)              = " << m_p << " M_Earth (Observed: " << obs_m << " M_Earth)" << std::endl;
  std::cout << "Planetary Radius (Model)            = " << r_p << " R_Earth (Observed: " << obs_r << " R_Earth)" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Dayside Temperature (Model)         = " << t_day << " K (Observed: " << obs_tday << " K)" << std::endl;
  std::cout << "Nightside Temperature (Model)       = " << t_night << " K (Observed: " << obs_tnight << " K)" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Atmospheric Metallicity             = " << z_met << "x Solar (Observed: " << obs_z << "x Solar)" << std::endl;
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Aerosol Haze Particle Radius        = " << r_haze << " um (Observed: " << obs_rhaze << " um)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Relative Dayside Discrepancy        = " << std::abs((t_day - obs_tday) / obs_tday) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
