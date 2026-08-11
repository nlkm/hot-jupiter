// Solver for Paper #85: Stellar Dynamo Saturation & X-Ray Emission in Fast Rotators (Vilhu 1984, Saar 1989, Wright 2011)
// Evaluates X-ray activity ratio L_X / L_bol as a function of Rossby number Ro, identifying saturation plateau (L_X / L_bol)_sat ~ 10^-3 for Ro < 0.13.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Wright (2011) Dynamo Saturation Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_085/dynamo_saturation_levels.csv");
  csv_file << "period_rot_days,rossby_number,lx_lbol_ratio,saturation_flag\n";

  double tau_conv_days = 15.0;  // G-dwarf convective turnover time 15 days

  // Rotation periods from 0.1 day to 30.0 days
  for (double p_rot_days = 0.1; p_rot_days <= 30.0; p_rot_days += 1.0) {
    double rossby_no = p_rot_days / tau_conv_days;

    // Wright et al. (2011) Rossby saturation law:
    // For Ro < 0.13: (L_X / L_bol)_sat = 1.0e-3 (Saturated regime)
    // For Ro >= 0.13: (L_X / L_bol) = 1.0e-3 * (Ro / 0.13)^(-2.1) (Unsaturated regime)
    double lx_lbol = 0.0;
    bool is_saturated = false;

    if (rossby_no < 0.13) {
      lx_lbol = 1.0e-3;
      is_saturated = true;
    } else {
      lx_lbol = 1.0e-3 * std::pow(rossby_no / 0.13, -2.1);
      is_saturated = false;
    }

    csv_file << std::fixed << std::setprecision(1) << p_rot_days << "," << std::setprecision(3) << rossby_no << "," << std::scientific << std::setprecision(3) << lx_lbol << "," << (is_saturated ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_085/dynamo_saturation_levels.csv" << std::endl;
  return 0;
}
