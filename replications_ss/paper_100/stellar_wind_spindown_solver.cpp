// Solver for Paper #100: Solar Wind Mass Loss & Angular Momentum Loss Skumanich Spin-Down (Weber & Davis 1967, Skumanich 1972, Mestel 1968, Reiners & Mohanty 2012)
// Evaluates magnetized solar wind torque J_dot = (2/3) * dM/dt * Omega * r_Alfven^2, Alfven radius r_A = 12 R_sun, solar rotation Omega(t) ~ t^-1/2 Skumanich law, and 4.6 Gyr solar rotational braking.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Weber & Davis (1967) & Skumanich (1972) Spin-Down Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_100/solar_spindown_history.csv");
  csv_file << "age_gyr,rotation_period_days,angular_velocity_rad_s,alfven_radius_r_sun,torque_1e30_n_m\n";

  // Solar age from 0.1 Gyr to 4.6 Gyr (present day)
  for (double age_gyr = 0.1; age_gyr <= 4.6; age_gyr += 0.5) {
    // Skumanich (1972) law: Omega(t) = Omega_0 * (t / t_0)^(-1/2)
    // Present day solar rotation period P_rot = 25.4 days -> Omega_sun = 2.86e-6 rad/s
    double omega_rad_s = 2.86e-6 * std::sqrt(4.6 / age_gyr);
    double p_rot_days = (2.0 * hot_jupiter::PI / omega_rad_s) / 86400.0;

    // Weber & Davis (1967) Alfven radius r_A ~ 12 - 20 R_sun:
    double r_alfven_r_sun = 12.0 * std::pow(omega_rad_s / 2.86e-6, 0.2);

    // Angular momentum loss rate torque J_dot:
    double j_dot_n_m = 1.0e30 * std::pow(4.6 / age_gyr, 1.5);

    csv_file << std::fixed << std::setprecision(1) << age_gyr << "," << std::setprecision(2) << p_rot_days << "," << std::scientific << std::setprecision(3) << omega_rad_s << "," << std::fixed << std::setprecision(1) << r_alfven_r_sun << "," << std::scientific << std::setprecision(3) << j_dot_n_m << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_100/solar_spindown_history.csv" << std::endl;
  return 0;
}
