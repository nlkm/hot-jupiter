// Solver for Paper #46: Kozai-Lidov Oscillations in Hierarchical Triple Systems (Lidov 1962, Kozai 1962, Naoz 2016)
// Evaluates Kozai constant L_z = sqrt(1 - e^2) * cos(i), maximum eccentricity e_max = sqrt(1 - (5/3)*cos^2(i_0)), and Kozai timescale t_KL.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Kozai (1962), Lidov (1962), & Naoz (2016) Kozai-Lidov Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_046/kozai_lidov_eccentricities.csv");
  csv_file << "initial_inc_deg,kozai_lz_const,e_max_theoretical,t_kl_myr\n";

  double m_primary = hot_jupiter::M_SUN;
  double m_perturber = 0.5 * hot_jupiter::M_SUN;  // binary stellar companion
  double a_inner_au = 5.0;
  double a_outer_au = 100.0;
  double p_inner_yr = std::pow(a_inner_au, 1.5);
  double p_outer_yr = std::sqrt(std::pow(a_outer_au, 3.0) / (m_primary / hot_jupiter::M_SUN + m_perturber / hot_jupiter::M_SUN));

  // Kozai timescale t_KL ~ (m_primary / m_perturber) * (P_outer^2 / P_inner) * (1 - e_outer^2)^1.5
  double t_kl_yr = (m_primary / m_perturber) * (p_outer_yr * p_outer_yr / p_inner_yr);
  double t_kl_myr = t_kl_yr / 1.0e6;

  // Initial mutual inclinations from 40 deg to 85 deg (Kozai regime i > 39.23 deg)
  for (double i0_deg = 40.0; i0_deg <= 85.0; i0_deg += 5.0) {
    double i0_rad = i0_deg * hot_jupiter::PI / 180.0;
    double cos_i0 = std::cos(i0_rad);
    double l_z = cos_i0;  // assuming initial e_0 = 0

    // Theoretical maximum eccentricity for quadrupolar Kozai-Lidov mechanism: e_max = sqrt(1 - (5/3)*cos^2(i_0))
    double e_max_sq = 1.0 - (5.0 / 3.0) * cos_i0 * cos_i0;
    double e_max = (e_max_sq > 0.0) ? std::sqrt(e_max_sq) : 0.0;

    csv_file << std::fixed << std::setprecision(1) << i0_deg << "," << std::setprecision(4) << l_z << "," << std::setprecision(4) << e_max << "," << std::setprecision(2) << t_kl_myr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_046/kozai_lidov_eccentricities.csv" << std::endl;
  return 0;
}
