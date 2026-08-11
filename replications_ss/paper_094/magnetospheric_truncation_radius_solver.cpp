// Solver for Paper #94: Magnetospheric Accretion Truncation & Inner Disk Cavity Radii (Ghosh & Lamb 1979, Koenigl 1991, Shu 1994, Bouvier 2007)
// Evaluates Alfvén radius r_A = (mu^2 / (2 * M_dot * sqrt(2 * G * M_star)))^(2/7), corotation radius r_co, and stellar magnetic field truncation in Classical T Tauri Stars (CTTS).

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "star_formation.hpp"

int main() {
  std::cout << "=== Running Koenigl (1991) Magnetospheric Truncation Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_094/magnetospheric_cavity_radii.csv");
  csv_file << "magnetic_field_gauss,accretion_rate_solar_yr,alfven_truncation_radius_stellar,corotation_radius_stellar,funnel_flow_flag\n";

  double m_star_solar = 1.0;
  double r_star_solar = 2.0;       // Young CTTS stellar radius 2.0 R_sun
  double period_rot_days = 8.0;    // CTTS rotation period 8 days

  // Corotation radius r_co = (G * M_star * P_rot^2 / (4 * pi^2))^(1/3)
  double p_rot_sec = period_rot_days * 86400.0;
  double r_co_m = std::pow(hot_jupiter::G * (m_star_solar * hot_jupiter::M_SUN) * std::pow(p_rot_sec, 2.0) / (4.0 * hot_jupiter::PI * hot_jupiter::PI), 1.0 / 3.0);
  double r_co_stellar = r_co_m / (r_star_solar * hot_jupiter::R_SUN);

  // Stellar surface magnetic dipole field B_star from 500 G to 3000 G
  for (double b_gauss = 500.0; b_gauss <= 3000.0; b_gauss += 500.0) {
    double m_dot_solar_yr = 1.0e-8;  // CTTS accretion rate 10^-8 M_sun/yr

    // Koenigl (1991) Alfvén truncation radius scaling:
    // r_in / R_star ~ 5.0 * (B / 1000 G)^(4/7) * (M_dot / 10^-8)^(-2/7)
    double r_in_stellar = 5.0 * std::pow(b_gauss / 1000.0, 4.0 / 7.0) * std::pow(m_dot_solar_yr / 1.0e-8, -2.0 / 7.0);

    bool funnel_flow = (r_in_stellar <= r_co_stellar);

    csv_file << std::fixed << std::setprecision(0) << b_gauss << "," << std::scientific << std::setprecision(1) << m_dot_solar_yr << "," << std::fixed << std::setprecision(2) << r_in_stellar << "," << std::setprecision(2) << r_co_stellar << "," << (funnel_flow ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_094/magnetospheric_cavity_radii.csv" << std::endl;
  return 0;
}
