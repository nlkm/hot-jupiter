// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #33: TOI-849b Chthonian Remnant Core & Extreme Envelope Stripping

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #33: TOI-849b CHTHONIAN REMNANT CORE ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TOI849bStrippedCoreModel model;
  double m_p = model.planet_mass_mearth();
  double r_p = model.planet_radius_rearth();
  double rho = model.bulk_density_g_cm3();
  double f_env = model.envelope_mass_fraction_max();
  double mdot = model.photoevaporation_mass_loss_g_s();

  // TESS & HARPS radial velocity observations (Armstrong et al. 2020 Nature)
  double obs_m = 39.1;   // M_Earth (39.1 +/- 2.5 M_Earth)
  double obs_r = 3.44;   // R_Earth (3.44 +/- 0.12 R_Earth)
  double obs_rho = 5.50; // g/cm^3 (Earth-like density for a giant core!)
  double obs_f = 0.038;  // max 3.8% H/He envelope mass fraction

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Planetary Mass M_p (Model)          = " << m_p << " M_Earth (Observed: " << obs_m << " M_Earth)" << std::endl;
  std::cout << "Planetary Radius R_p (Model)        = " << r_p << " R_Earth (Observed: " << obs_r << " R_Earth)" << std::endl;
  std::cout << "Bulk Mean Density rho (Model)       = " << rho << " g/cm^3 (Observed: " << obs_rho << " g/cm^3)" << std::endl;
  std::cout << "Max H/He Envelope Mass Fraction     = " << f_env * 100.0 << " % (Observed: " << obs_f * 100.0 << " %)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Photoevaporative Mass Loss Rate     = " << mdot << " g/s" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Relative Mass Discrepancy           = " << std::abs((m_p - obs_m) / obs_m) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
