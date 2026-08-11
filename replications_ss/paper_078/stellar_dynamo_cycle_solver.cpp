// Solver for Paper #78: Stellar Alpha-Omega Dynamo & Magnetic Activity Cycles (Parker 1955, Steenbeck 1966, Noyes 1984)
// Evaluates Rossby number Ro = P_rot / tau_conv, magnetic activity indicator R'_HK ~ Ro^-1.2, and dynamo cycle period P_cyc.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Parker (1955) & Noyes (1984) Stellar Dynamo Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_078/stellar_dynamo_activity.csv");
  csv_file << "period_rot_days,convective_overturn_days,rossby_number,activity_r_hk_1e5,dynamo_cycle_yr\n";

  double tau_conv_days = 20.0;  // Solar convective turnover time 20 days

  // Rotation periods from 1.0 day to 35.0 days
  for (double p_rot_days = 1.0; p_rot_days <= 35.0; p_rot_days += 2.0) {
    // Noyes et al. (1984) Rossby number Ro = P_rot / tau_conv
    double rossby_no = p_rot_days / tau_conv_days;

    // Activity indicator R'_HK scaling: R'_HK * 1e5 = 1.5 * Ro^(-1.2)
    double r_hk_1e5 = 1.5 * std::pow(rossby_no, -1.2);

    // Dynamo activity cycle period P_cyc ~ 11.0 * (P_rot / 25.0)^0.5 years (Soon et al. 1995)
    double p_cyc_yr = 11.0 * std::pow(p_rot_days / 25.0, 0.5);

    csv_file << std::fixed << std::setprecision(1) << p_rot_days << "," << std::setprecision(1) << tau_conv_days << "," << std::setprecision(3) << rossby_no << "," << std::setprecision(3) << r_hk_1e5 << "," << std::setprecision(2) << p_cyc_yr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_078/stellar_dynamo_activity.csv" << std::endl;
  return 0;
}
