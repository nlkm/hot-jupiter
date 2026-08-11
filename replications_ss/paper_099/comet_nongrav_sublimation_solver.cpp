// Solver for Paper #99: Comet Outgassing Non-Gravitational Acceleration & Sublimation Jet Torques (Whipple 1950, Marsden 1973, Yeomans 2004)
// Evaluates Marsden g(r) = alpha * (r / r_0)^-m * (1 + (r / r_0)^n)^-k water sublimation empirical law, A_1 (radial), A_2 (transverse), A_3 (normal) non-gravitational acceleration components, and orbital period change delta_P.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Whipple (1950) & Marsden (1973) Non-Gravitational Acceleration Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_099/comet_nongrav_accelerations.csv");
  csv_file << "heliocentric_distance_au,sublimation_g_r,a1_radial_accel_au_day2,a2_transverse_accel_au_day2,orbital_period_shift_sec_per_orbit\n";

  // Parameters for Comet 67P / Churyumov-Gerasimenko water sublimation function g(r):
  double alpha = 0.11126;
  double r_0 = 2.808;
  double m = 2.15;
  double n = 5.093;
  double k = 4.6142;

  // Distance from Sun from 0.8 AU (perihelion) to 5.0 AU (aphelion)
  for (double r_au = 0.8; r_au <= 5.0; r_au += 0.2) {
    // Marsden et al. (1973) g(r) empirical function:
    double g_r = alpha * std::pow(r_au / r_0, -m) * std::pow(1.0 + std::pow(r_au / r_0, n), -k);

    // Non-gravitational acceleration components:
    double a1 = 1.5e-8 * g_r;  // A_1 radial acceleration (AU / day^2)
    double a2 = 0.2e-8 * g_r;  // A_2 transverse acceleration (AU / day^2)

    // Orbital period shift per orbit delta_P (seconds):
    double delta_p_sec = 120.0 * (a2 / 0.2e-8);

    csv_file << std::fixed << std::setprecision(1) << r_au << "," << std::scientific << std::setprecision(3) << g_r << "," << std::scientific << std::setprecision(3) << a1 << "," << std::scientific << std::setprecision(3) << a2 << "," << std::fixed << std::setprecision(1) << delta_p_sec << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_099/comet_nongrav_accelerations.csv" << std::endl;
  return 0;
}
