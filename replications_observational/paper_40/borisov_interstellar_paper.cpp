// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #40: Comet 2I/Borisov Interstellar Origin & Extreme CO Abundance

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #40: 2I/BORISOV INTERSTELLAR COMET CO ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::BorisovInterstellarCometModel model;
  double ecc = model.orbital_eccentricity();
  double co_ratio = model.co_to_water_ratio();
  double q_h2o = model.water_production_2au_molecules_s();
  double t_form = model.formation_temperature_k();
  double a1 = model.non_grav_radial_a1_au_day2();

  // ALMA & HST observations (Bodewits et al. 2020 Nature, Cordiner et al. 2020 Nature)
  double obs_ecc = 3.36;    // Hyperbolic eccentricity e = 3.356 +/- 0.001
  double obs_ratio = 1.45;  // Extreme CO/H2O ratio ~ 1.3 - 1.7 (highest ever seen!)
  double obs_qh2o = 2.0e27; // molecules/s at 2 AU
  double obs_tform = 20.0;  // Inferred ultracold formation temperature (< 25 K)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Hyperbolic Orbital Eccentricity     = " << ecc << " (Observed: " << obs_ecc << ")" << std::endl;
  std::cout << "Carbon Monoxide CO/H_2O Ratio       = " << co_ratio << " (Observed: " << obs_ratio << ")" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Water Production Rate at 2 AU       = " << q_h2o << " molec/s (Observed: " << obs_qh2o << " molec/s)" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Inferred Formation Temperature      = " << t_form << " K (Observed: " << obs_tform << " K)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Non-Gravitational Radial A_1        = " << a1 << " AU/day^2" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Relative Ratio Discrepancy          = " << std::abs((co_ratio - obs_ratio) / obs_ratio) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
