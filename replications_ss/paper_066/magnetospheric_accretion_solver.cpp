// Solver for Paper #66: Magnetospheric Accretion & Star-Disk Magnetic Coupling (Ghosh & Lamb 1979, Koenigl 1991)
// Evaluates magnetospheric truncation radius R_in = (B_*^4 R_*^12 / (2 G M_* M_dot^2))^(1/7), co-rotation radius R_co, and magnetic torque spin equilibrium.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Ghosh & Lamb (1979) & Koenigl (1991) Magnetospheric Accretion Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_066/magnetospheric_radii.csv");
  csv_file << "magnetic_field_gauss,mdot_solar_yr,r_in_rstar,r_co_rstar,spin_equilibrium_bool\n";

  double m_star_solar = 1.0;     // T Tauri star mass 1.0 M_sun
  double r_star_solar = 2.0;     // T Tauri star radius 2.0 R_sun
  double p_spin_days = 8.0;      // Stellar rotation period 8 days

  double m_star_kg = m_star_solar * hot_jupiter::M_SUN;
  double r_star_m = r_star_solar * hot_jupiter::R_SUN;
  double omega_star = 2.0 * hot_jupiter::PI / (p_spin_days * hot_jupiter::DAY);

  // Co-rotation radius R_co = (G M_* / Omega_*^2)^(1/3)
  double r_co_m = std::pow(hot_jupiter::G * m_star_kg / (omega_star * omega_star), 1.0 / 3.0);
  double r_co_rstar = r_co_m / r_star_m;

  // Dipole magnetic fields B_* from 500 Gauss to 3000 Gauss
  for (double b_gauss = 500.0; b_gauss <= 3000.0; b_gauss += 500.0) {
    double b_tesla = b_gauss * 1.0e-4;
    double mdot_solar_yr = 1.0e-8;  // Accretion rate 1e-8 M_sun/yr
    double mdot_kg_s = mdot_solar_yr * hot_jupiter::M_SUN / hot_jupiter::YEAR;

    // Koenigl (1991) magnetospheric truncation radius R_in = (B_*^4 R_*^12 / (2 G M_* M_dot^2))^(1/7)
    double r_in_m = std::pow(std::pow(b_tesla, 4.0) * std::pow(r_star_m, 12.0) / (2.0 * hot_jupiter::G * m_star_kg * mdot_kg_s * mdot_kg_s), 1.0 / 7.0);
    double r_in_rstar = r_in_m / r_star_m;

    // Spin equilibrium when truncation radius matches co-rotation radius R_in / R_co ~ 0.5-1.0
    bool spin_eq = (r_in_rstar <= r_co_rstar);

    csv_file << std::fixed << std::setprecision(0) << b_gauss << "," << std::scientific << std::setprecision(1) << mdot_solar_yr << "," << std::fixed << std::setprecision(2) << r_in_rstar << "," << r_co_rstar << "," << (spin_eq ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_066/magnetospheric_radii.csv" << std::endl;
  return 0;
}
