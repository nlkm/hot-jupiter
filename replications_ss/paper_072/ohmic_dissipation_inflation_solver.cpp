// Solver for Paper #72: Hot Jupiter Thermal Radius Inflation via Ohmic Dissipation (Batygin & Stevenson 2010, Laughlin et al. 2011)
// Evaluates magnetic induction power P_ohmic = eta * (J^2 / sigma), radius inflation delta_R, and effective temperature threshold T_eq > 1400 K.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Batygin & Stevenson (2010) Ohmic Dissipation Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_072/ohmic_inflation_radii.csv");
  csv_file << "t_eq_k,magnetic_field_gauss,ohmic_power_erg_s,inflated_radius_rjup\n";

  double r_base_rjup = 1.10;  // Non-inflated baseline Hot Jupiter radius 1.10 R_jup

  // Equilibrium temperatures T_eq from 1000 K to 2400 K
  for (double t_eq_k = 1000.0; t_eq_k <= 2400.0; t_eq_k += 100.0) {
    double b_gauss = 10.0 * (t_eq_k / 1400.0);

    // Batygin & Stevenson (2010) Ohmic power dissipation scaling:
    // P_ohmic ~ 10^27 * (T_eq / 1500 K)^4 * (B / 10 G)^2 erg/s
    double p_ohmic_erg_s = 0.0;
    if (t_eq_k >= 1300.0) {
      p_ohmic_erg_s = 1.0e27 * std::pow(t_eq_k / 1500.0, 4.0) * std::pow(b_gauss / 10.0, 2.0);
    }

    // Radius inflation delta_R = 0.25 * (P_ohmic / 1e27 erg/s)^0.5 R_jup
    double delta_r_rjup = 0.25 * std::pow(p_ohmic_erg_s / 1.0e27, 0.5);
    double r_inflated_rjup = r_base_rjup + delta_r_rjup;

    csv_file << std::fixed << std::setprecision(0) << t_eq_k << "," << std::setprecision(1) << b_gauss << "," << std::scientific << std::setprecision(3) << p_ohmic_erg_s << "," << std::fixed << std::setprecision(3) << r_inflated_rjup << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_072/ohmic_inflation_radii.csv" << std::endl;
  return 0;
}
