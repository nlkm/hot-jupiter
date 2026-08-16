// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #37: Enceladus CDA Sodium Salt Fractionation & Ocean Chemistry

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #37: ENCELADUS CDA SALT FRACTIONATION ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::EnceladusCDASaltFractionationModel model;
  double salt_frac = model.sodium_salt_mass_fraction();
  double mdot_dust = model.dust_mass_production_rate_kg_s();
  double ph = model.ocean_ph_value();
  double v_grain = model.e_ring_grain_velocity_m_s();

  // Cassini Cosmic Dust Analyzer (CDA) observations (Postberg et al. 2009, 2011 Nature)
  double obs_salt = 0.015;  // 0.5 - 2.0% Na/K salts in Type III ice grains
  double obs_dust = 5.0;    // kg/s dust production rate
  double obs_ph = 9.5;      // Inferred alkaline pH (9-11) from carbonate/bicarbonate
  double obs_vgrain = 250.0;// m/s mean grain ejection velocity

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Sodium Salt Mass Fraction (Model)   = " << salt_frac * 100.0 << " % (Observed: " << obs_salt * 100.0 << " %)" << std::endl;
  std::cout << "E-Ring Dust Mass Production Rate    = " << mdot_dust << " kg/s (Observed: " << obs_dust << " kg/s)" << std::endl;
  std::cout << "Subsurface Ocean Alkaline pH        = " << ph << " (Observed: " << obs_ph << ")" << std::endl;
  std::cout << "Mean Grain Ejection Velocity        = " << v_grain << " m/s (Observed: " << obs_vgrain << " m/s)" << std::endl;
  std::cout << "Relative Salinity Discrepancy       = " << std::abs((salt_frac - obs_salt) / obs_salt) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
