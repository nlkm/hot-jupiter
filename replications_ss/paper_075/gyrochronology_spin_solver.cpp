// Solver for Paper #75: Stellar Rotation & Gyrochronology Age-Spin-Mass Relations (Skumanich 1972, Barnes 2007, Mamajek & Hillenbrand 2008)
// Evaluates rotational period evolution P_rot(t, B-V) = a * t^n * (B-V - c)^b, Skumanich law v_rot ~ t^-0.5, and stellar age dating.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Barnes (2007) & Mamajek (2008) Gyrochronology Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_075/gyrochronology_periods.csv");
  csv_file << "age_gyr,period_sun_days,period_k_dwarf_days,period_m_dwarf_days\n";

  // Barnes (2007) coefficients: a = 0.77, n = 0.52, b = 0.60, c = 0.40
  double a_coeff = 0.77;
  double n_exponent = 0.52;
  double b_exponent = 0.60;
  double c_offset = 0.40;

  // B-V color indices: Sun (G2V) = 0.65, K-dwarf (K5V) = 1.15, M-dwarf (M2V) = 1.50
  double bv_sun = 0.65;
  double bv_k = 1.15;
  double bv_m = 1.50;

  // Stellar ages t from 0.1 Gyr to 10.0 Gyr
  for (double age_gyr = 0.1; age_gyr <= 10.0; age_gyr += 0.5) {
    double age_myr = age_gyr * 1000.0;

    // Barnes (2007) P_rot(t, B-V) = a * t_Myr^n * (B-V - c)^b days
    double p_sun = a_coeff * std::pow(age_myr, n_exponent) * std::pow(bv_sun - c_offset, b_exponent);
    double p_k = a_coeff * std::pow(age_myr, n_exponent) * std::pow(bv_k - c_offset, b_exponent);
    double p_m = a_coeff * std::pow(age_myr, n_exponent) * std::pow(bv_m - c_offset, b_exponent);

    csv_file << std::fixed << std::setprecision(1) << age_gyr << "," << std::setprecision(2) << p_sun << "," << p_k << "," << p_m << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_075/gyrochronology_periods.csv" << std::endl;
  return 0;
}
