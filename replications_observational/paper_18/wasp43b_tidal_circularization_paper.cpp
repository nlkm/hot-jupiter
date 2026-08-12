// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #18: WASP-43b Tidal Circularization & Planetary Q'_p Dissipation

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #18: WASP-43b TIDAL CIRCULARIZATION ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP43bTidalCircularizationModel circ_model;
  double tau_e_model = circ_model.circularization_timescale_myr();
  double e_damped_model = circ_model.damped_eccentricity(1.0); // at 1 Gyr system age

  double obs_tau_e = 7.5; // Myr (Hellier et al. 2011, Gillon et al. 2012, Chen et al. 2014)
  double obs_ecc = 0.003; // e ~ 0 (circularized)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Circularization Timescale (Model)   = " << tau_e_model << " Myr (Observed: " << obs_tau_e << " Myr)" << std::endl;
  std::cout << std::scientific << std::setprecision(4);
  std::cout << "Damped Eccentricity at 1 Gyr (Model)= " << e_damped_model << " (Observed: < " << obs_ecc << ")" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Relative Circularization Time Error = " << std::abs((tau_e_model - obs_tau_e) / obs_tau_e) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
