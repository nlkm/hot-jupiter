// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #47: Dwarf Planet Ceres Ahuna Mons Cryovolcanic Dome & Rheology

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #47: CERES AHUNA MONS CRYOVOLCANISM ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::CeresAhunaMonsCryovolcanismModel model;
  double h_dome = model.dome_height_km();
  double d_base = model.base_diameter_km();
  double tau_yield = model.brine_yield_stress_pa();
  double f_salt = model.sodium_carbonate_mass_fraction();

  // Dawn Framing Camera & VIR spectrometer observations (Ruesch 2016 Science, Krohn 2016)
  double obs_hdome = 4.0;       // km relief height (4.0 +/- 0.3 km)
  double obs_dbase = 20.0;      // km basal footprint (20.0 +/- 1.0 km)
  double obs_yield = 1.5e4;     // Pa Bingham plastic yield strength (1.0 - 2.0 x 10^4 Pa)
  double obs_salt = 0.20;       // Na2CO3 sodium carbonate / chloride brine mass fraction

  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Ahuna Mons Dome Height (Model)      = " << h_dome << " km (Observed: " << obs_hdome << " km)" << std::endl;
  std::cout << "Basal Footprint Diameter (Model)    = " << d_base << " km (Observed: " << obs_dbase << " km)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Bingham Slurry Yield Stress         = " << tau_yield << " Pa (Observed: " << obs_yield << " Pa)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Sodium Carbonate Mass Fraction      = " << f_salt << " (Observed: " << obs_salt << ")" << std::endl;
  std::cout << "Relative Dome Height Discrepancy    = " << std::abs((h_dome - obs_hdome) / obs_hdome) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
