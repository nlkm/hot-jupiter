// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #5: Cassini Grand Finale Saturn Zonal Harmonics J2-J8 & Core Mass Determination
// First-principles replication of Iess et al. (2019), Militzer et al. (2019), & Movshovitz et al. (2020)

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::SaturnCassiniGravityAnalysisModel model;

  // Cassini Grand Finale Radio Science Measurements (Iess et al. 2019)
  double j2_obs = 16290.71;
  double j4_obs = -935.83;
  double j6_obs = 86.14;

  // 1. Model Rotational Parameter q_rot
  double q_rot = model.rotational_q();

  // 2. Model J2 Harmonic
  double j2_calc = model.j2_harmonic_1e6(0.09796, q_rot);

  // 3. Model J4 Harmonic
  double j4_calc = model.j4_harmonic_1e6(0.09796, q_rot);

  // 4. Model J6 Harmonic
  double j6_calc = model.j6_harmonic_1e6(0.09796, q_rot);

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #5: CASSINI GRAND FINALE SATURN ZONAL HARMONICS" << std::endl;
  std::cout << "================================================================================" << std::endl;
  std::cout << "J2 x 10^6: Cassini Obs = " << j2_obs << " | Model = " << j2_calc << std::endl;
  std::cout << "J4 x 10^6: Cassini Obs = " << j4_obs << " | Model = " << j4_calc << std::endl;
  std::cout << "J6 x 10^6: Cassini Obs = " << j6_obs << " | Model = " << j6_calc << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
