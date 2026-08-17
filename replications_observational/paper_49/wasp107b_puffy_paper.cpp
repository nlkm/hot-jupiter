// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #49: WASP-107b Low-Density Puffy Super-Neptune & Photochemistry

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #49: WASP-107b PUFFY SUPER-NEPTUNE ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP107bPuffyNeptuneModel model;
  double m_p = model.planet_mass_mearth();
  double r_p = model.planet_radius_rjup();
  double rho_bulk = model.bulk_density_g_cm3();
  double f_so2 = model.so2_mixing_ratio();
  double f_tide = model.interior_tidal_flux_w_m2();

  // JWST NIRSpec/MIRI & HST observations (Dyrek 2024 Nature, Sing 2024 Nature)
  double obs_m = 30.5;        // M_Earth (30.5 +/- 1.7 M_Earth)
  double obs_r = 0.94;        // R_Jup (0.94 +/- 0.02 R_Jup)
  double obs_rho = 0.13;      // g/cm^3 ultra-low bulk density (0.13 +/- 0.01 g/cm^3)
  double obs_so2 = 2.0e-5;    // SO2 volume mixing ratio (1-4 x 10^-5)
  double obs_ftide = 1.2;     // W/m^2 tidal inflation heating flux

  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Planetary Mass M_p (Model)          = " << m_p << " M_Earth (Observed: " << obs_m << " M_Earth)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Planetary Radius R_p (Model)        = " << r_p << " R_Jup (Observed: " << obs_r << " R_Jup)" << std::endl;
  std::cout << "Bulk Mean Density                   = " << rho_bulk << " g/cm^3 (Observed: " << obs_rho << " g/cm^3)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "SO2 Photochemical Mixing Ratio      = " << f_so2 << " (Observed: " << obs_so2 << ")" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Interior Tidal Heat Flux            = " << f_tide << " W/m^2 (Observed: " << obs_ftide << " W/m^2)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Relative Density Discrepancy        = " << std::abs((rho_bulk - obs_rho) / obs_rho) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
