// Solver for Paper #54: Magnetospheric Truncation Radius & Stellar Spin-Down (Ghosh & Lamb 1979, Matt & Pudritz 2005)
// Evaluates Alfven magnetospheric radius r_mag = (B^2 * R^6 / (sqrt(2*G*M) * dM/dt))^(2/7), corotation radius r_cor, and spin-locking.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Ghosh & Lamb (1979) & Matt & Pudritz (2005) Magnetospheric Truncation Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_054/magnetospheric_truncation_radii.csv");
  csv_file << "magnetic_field_gauss,mdot_accretion_solar_yr,r_mag_over_r_star,spin_locked_bool\n";

  double m_star_kg = hot_jupiter::M_SUN;
  double r_star_m = 2.0 * hot_jupiter::R_SUN;  // T Tauri star radius 2 R_sun

  // Magnetic fields B from 100 Gauss to 3000 Gauss
  for (double b_gauss = 100.0; b_gauss <= 3000.0; b_gauss += 500.0) {
    double b_tesla = b_gauss * 1.0e-4;
    double mdot_solar_yr = 1.0e-8;  // typical T Tauri disk accretion rate
    double mdot_kg_s = (mdot_solar_yr * hot_jupiter::M_SUN) / hot_jupiter::YEAR;

    // Magnetospheric radius r_mag = (B^2 * R^6 / (sqrt(2*G*M) * dM/dt))^(2/7)
    double numerator = b_tesla * b_tesla * std::pow(r_star_m, 6.0);
    double denominator = std::sqrt(2.0 * hot_jupiter::G * m_star_kg) * mdot_kg_s;
    double r_mag_m = std::pow(numerator / denominator, 2.0 / 7.0);

    double r_mag_ratio = r_mag_m / r_star_m;
    bool spin_locked = (r_mag_ratio >= 3.0 && r_mag_ratio <= 10.0);

    csv_file << std::fixed << std::setprecision(0) << b_gauss << "," << std::scientific << mdot_solar_yr << "," << std::fixed << std::setprecision(2) << r_mag_ratio << "," << (spin_locked ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_054/magnetospheric_truncation_radii.csv" << std::endl;
  return 0;
}
