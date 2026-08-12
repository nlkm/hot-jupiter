// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #4: Juno Gravity Science (GS) Zonal Harmonics & Jupiter Dilute Core Model
// First-principles replication of Iess et al. (2018), Durante et al. (2020), & Nettelmann et al. (2021)

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::JupiterJunoGravityAnalysisModel model;

  // Juno Radio Science Observational Dataset (Iess et al. 2018, Durante et al. 2020)
  double j2_obs = 14696.57;
  double j4_obs = -586.61;
  double j6_obs = 34.20;

  // 1. Model Rotational Parameter q_rot
  double q_rot = model.rotational_q();

  // 2. Model J2 Harmonic
  double j2_calc = model.j2_harmonic_1e6(0.06487, q_rot);

  // 3. Model J4 Harmonic
  double j4_calc = model.j4_harmonic_1e6(0.06487, q_rot);

  // 4. Model J6 Harmonic
  double j6_calc = model.j6_harmonic_1e6(0.06487, q_rot);

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #4: JUNO RADIO SCIENCE JUPITER ZONAL HARMONICS" << std::endl;
  std::cout << "================================================================================" << std::endl;
  std::cout << "J2 x 10^6: Juno Obs = " << j2_obs << " | Model = " << j2_calc << std::endl;
  std::cout << "J4 x 10^6: Juno Obs = " << j4_obs << " | Model = " << j4_calc << std::endl;
  std::cout << "J6 x 10^6: Juno Obs = " << j6_obs << " | Model = " << j6_calc << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
