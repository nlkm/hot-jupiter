// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #82: WASP-43b Tidal Eccentricity Circularization Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #82: WASP-43b TIDAL CIRCULARIZATION & Q'_p" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP43bTidalCircularizationModel model;

  const double tau_e_myr = model.circularization_timescale_myr(); // ~ 7.52 Myr
  const double q_p_prime = 2.95e6;

  std::cout << "WASP-43b Planetary Tidal Quality Factor Q'_p: " << q_p_prime << std::endl;
  std::cout << "Tidal Eccentricity Circularization Timescale: " << tau_e_myr << " Myr" << std::endl;

  // Track Eccentricity Evolution over 0 to 50 Myr (linear scale):
  // e(t) = e_0 * exp(-t / tau_e)
  std::ofstream out("replications_observational/paper_82/wasp43b_eccentricity_evolution.csv");
  out << "time_myr,orbital_eccentricity,eccentricity_upper_limit_obs\n";

  for (double t_myr = 0.0; t_myr <= 50.0; t_myr += 1.0) {
    double e_val = 0.20 * std::exp(-t_myr / tau_e_myr);
    double e_obs_limit = 0.005; // Modern observational 3-sigma upper limit (Gillon et al. 2012)

    out << t_myr << "," << e_val << "," << e_obs_limit << "\n";
  }
  out.close();

  std::cout << "Generated WASP-43b Tidal Circularization Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
