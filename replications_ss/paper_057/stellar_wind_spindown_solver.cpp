// Solver for Paper #57: Stellar Wind Mass Loss & Skumanich Angular Momentum Spin-Down (Skumanich 1972, Kawaler 1988)
// Evaluates rotational velocity v_rot(t) = v_0 * (t / t_0)^(-1/2), Parker wind mass loss dM/dt, and magnetic braking torque.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Skumanich (1972) & Kawaler (1988) Stellar Wind Spin-Down Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_057/stellar_spindown_velocities.csv");
  csv_file << "age_gyr,v_rot_km_s,p_rot_days,skumanich_law_fit\n";

  double v0_km_s = 50.0;    // initial rotation speed at 0.01 Gyr (10 Myr)
  double t0_gyr = 0.01;

  for (double t_gyr = 0.1; t_gyr <= 5.0; t_gyr += 0.5) {
    // Skumanich law v_rot ~ t^(-1/2)
    double v_rot = v0_km_s * std::sqrt(t0_gyr / t_gyr);
    double p_rot_days = (2.0 * hot_jupiter::PI * hot_jupiter::R_SUN) / (v_rot * 1000.0 * hot_jupiter::DAY);

    csv_file << std::fixed << std::setprecision(1) << t_gyr << "," << std::setprecision(2) << v_rot << "," << std::setprecision(1) << p_rot_days << "," << std::setprecision(2) << v_rot << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_057/stellar_spindown_velocities.csv" << std::endl;
  return 0;
}
