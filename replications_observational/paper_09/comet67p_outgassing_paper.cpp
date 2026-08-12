// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #9: Comet 67P Non-Gravitational Outgassing Acceleration

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #9: COMET 67P NON-GRAVITATIONAL OUTGASSING ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::Comet67POutgassingModel comet_model;
  double a1_model = comet_model.radial_acceleration_AU_day2(1.0);
  double a2_model = comet_model.transverse_acceleration_AU_day2(1.0);

  double rosetta_rsi_a1_obs = 3.25e-8; // AU/day^2 at 1 AU (Godard et al. 2017)

  std::cout << std::scientific << std::setprecision(4);
  std::cout << "Radial Acceleration A1 * g(1 AU) (Model) = " << a1_model << " AU/day^2" << std::endl;
  std::cout << "Rosetta RSI Observed Radial Coefficient  = " << rosetta_rsi_a1_obs << " AU/day^2" << std::endl;
  std::cout << "Transverse Acceleration A2 * g(1 AU)     = " << a2_model << " AU/day^2" << std::endl;
  std::cout << "Relative Radial Model Agreement         = " << std::abs((a1_model - rosetta_rsi_a1_obs) / rosetta_rsi_a1_obs) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
