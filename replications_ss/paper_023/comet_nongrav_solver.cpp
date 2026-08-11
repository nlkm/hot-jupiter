// Solver for Paper #23: Comet Outgassing & Non-Gravitational Torques (Whipple 1950, Marsden et al. 1973)
// Evaluates non-gravitational acceleration components (A1 radial, A2 transverse, A3 normal) for sublimating comets.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Whipple (1950) & Marsden et al. (1973) Comet Non-Grav Solver ===" << std::endl;

  hot_jupiter::CometDynamicsModel comet_model;

  std::ofstream csv_file("replications_ss/paper_023/comet_nongrav_accelerations.csv");
  csv_file << "heliocentric_r_au,g_r_function,a1_radial_m_s2,a2_transverse_m_s2\n";

  // Heliocentric distances from 0.5 AU to 5.0 AU (e.g. 67P / Halley)
  for (double r_au = 0.5; r_au <= 5.0; r_au += 0.25) {
    double g_r = comet_model.marsden_sublimation_g_r(r_au);
    double a1 = comet_model.non_gravitational_acceleration_m_s2(r_au, 1.0e-8);
    double a2 = comet_model.non_gravitational_acceleration_m_s2(r_au, 2.0e-9);

    csv_file << std::fixed << std::setprecision(2) << r_au << "," << std::scientific << g_r << "," << a1 << "," << a2 << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_023/comet_nongrav_accelerations.csv" << std::endl;
  return 0;
}
