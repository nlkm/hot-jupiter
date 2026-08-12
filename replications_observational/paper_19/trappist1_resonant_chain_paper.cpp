// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #19: TRAPPIST-1 7-Planet Resonant Chain & TTV Dynamics

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #19: TRAPPIST-1 RESONANT CHAIN ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TRAPPIST1ResonantChainModel trap_model;
  double ttv_amp_model = trap_model.ttv_chopping_amplitude_minutes();
  double libration_model = trap_model.laplace_resonant_angle_libration_deg();
  double mass_e_model = trap_model.trappist1e_mass_mearth();

  double obs_ttv_amp = 38.5; // minutes (Gillon et al. 2017, Agol et al. 2021)
  double obs_libration = 1.2; // degrees
  double obs_mass_e = 0.692; // M_Earth

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "TTV Chopping Amplitude (Model)     = " << ttv_amp_model << " minutes (Observed: " << obs_ttv_amp << " min)" << std::endl;
  std::cout << "Laplace Angle Libration (Model)    = " << libration_model << " deg (Observed: " << obs_libration << " deg)" << std::endl;
  std::cout << std::setprecision(3);
  std::cout << "TRAPPIST-1e Dynamical Mass (Model)  = " << mass_e_model << " M_Earth (Observed: " << obs_mass_e << " M_Earth)" << std::endl;
  std::cout << std::setprecision(2);
  std::cout << "Relative TTV Chopping Error        = " << std::abs((ttv_amp_model - obs_ttv_amp) / obs_ttv_amp) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
