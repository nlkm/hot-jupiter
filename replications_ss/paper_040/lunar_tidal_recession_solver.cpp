// Solver for Paper #40: Tidal Recession of the Moon & Lunar Orbital Evolution (Goldreich 1966, Touma & Wisdom 1994)
// Evaluates lunar orbital recession da/dt ~ (k_2 / Q) * (M_moon / M_earth) * (R_earth / a)^5 * n * a.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Goldreich (1966) & Touma & Wisdom (1994) Lunar Tidal Recession Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_040/lunar_recession_rates.csv");
  csv_file << "a_rearth,a_km,recession_rate_cm_yr,recession_rate_m_kyr\n";

  double m_earth = hot_jupiter::M_EARTH;
  double m_moon = 7.342e22;          // Moon mass [kg]
  double r_earth = hot_jupiter::R_EARTH;
  double k2_q = 0.30 / 12.0;         // Earth Love number k2 = 0.30, tidal dissipation Q ~ 12

  // Lunar semi-major axis from 10 R_earth (initial post-impact distance) to 60 R_earth (present day)
  for (double a_ratio = 10.0; a_ratio <= 60.0; a_ratio += 5.0) {
    double a_m = a_ratio * r_earth;
    double a_km = a_m / 1000.0;
    double n_moon = std::sqrt(hot_jupiter::G * m_earth / std::pow(a_m, 3.0));

    // da/dt = 3 * (k2 / Q) * (M_moon / M_earth) * (R_earth / a)^5 * n_moon * a
    double da_dt_m_s = 3.0 * k2_q * (m_moon / m_earth) * std::pow(r_earth / a_m, 5.0) * n_moon * a_m;
    double da_dt_cm_yr = da_dt_m_s * 100.0 * hot_jupiter::YEAR;
    double da_dt_m_kyr = da_dt_m_s * 1000.0 * hot_jupiter::YEAR;

    csv_file << std::fixed << std::setprecision(1) << a_ratio << "," << std::setprecision(1) << a_km << "," << std::setprecision(2) << da_dt_cm_yr << "," << std::scientific << da_dt_m_kyr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_040/lunar_recession_rates.csv" << std::endl;
  return 0;
}
