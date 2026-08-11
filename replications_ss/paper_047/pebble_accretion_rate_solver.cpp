// Solver for Paper #47: 2D/3D Pebble Accretion & Rapid Core Growth (Ormel & Klahr 2010, Lambrechts & Johansen 2012)
// Evaluates 3D Hill pebble accretion rate dM/dt = 2 * (St / 0.1)^(2/3) * r_H^2 * Omega * Sigma_p and growth timescale.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Ormel & Klahr (2010) & Lambrechts (2012) Pebble Accretion Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_047/pebble_accretion_growth.csv");
  csv_file << "stokes_number,m_core_earth_masses,dm_dt_earth_per_kyr,t_10earth_myr\n";

  double m_earth = hot_jupiter::M_EARTH;
  double m_sun = hot_jupiter::M_SUN;
  double a_au = 5.0;  // snow line radius
  double omega = std::sqrt(hot_jupiter::G * m_sun / std::pow(a_au * hot_jupiter::AU, 3.0));
  double sigma_pebble = 0.5 * 10.0;  // kg/m^2 pebble surface density

  // Stokes numbers from St = 0.01 to 1.0
  for (double st = 0.01; st <= 1.0; st *= 2.0) {
    double m_core_kg = 1.0 * m_earth;
    double r_hill = a_au * hot_jupiter::AU * std::pow(m_core_kg / (3.0 * m_sun), 1.0 / 3.0);

    // 3D Hill regime accretion rate dM/dt = 2 * (St / 0.1)^(2/3) * r_H^2 * Omega * Sigma_p
    double dm_dt_kg_s = 2.0 * std::pow(st / 0.1, 2.0 / 3.0) * r_hill * r_hill * omega * sigma_pebble;
    double dm_dt_earth_kyr = (dm_dt_kg_s * 1000.0 * hot_jupiter::YEAR) / m_earth;
    double t_10earth_myr = (10.0 * m_earth / dm_dt_kg_s) / (hot_jupiter::YEAR * 1.0e6);

    csv_file << std::fixed << std::setprecision(2) << st << ",1.0," << std::scientific << dm_dt_earth_kyr << "," << std::fixed << std::setprecision(3) << t_10earth_myr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_047/pebble_accretion_growth.csv" << std::endl;
  return 0;
}
