// Solver for Paper #41: Atmospheric Escape & Hydrodynamic Photo-Evaporation (Watson et al. 1981, Tian et al. 2005)
// Evaluates energy-limited mass loss rate dM/dt = (epsilon * pi * F_euv * R_p^3) / (G * M_p * K_tide).

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Watson (1981) & Tian (2005) Hydrodynamic Escape Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_041/hydrodynamic_escape_rates.csv");
  csv_file << "f_euv_w_m2,dm_dt_kg_s,dm_dt_earth_masses_gyr\n";

  double m_p = hot_jupiter::M_EARTH;
  double r_p = hot_jupiter::R_EARTH;
  double epsilon = 0.15;  // EUV heating efficiency

  // EUV flux levels from 1e-4 W/m^2 (modern Earth) to 1.0 W/m^2 (young Sun / early Earth)
  for (double f_euv = 1.0e-4; f_euv <= 1.0; f_euv *= 4.0) {
    // Energy-limited mass loss rate dM/dt = (epsilon * pi * F_euv * R_p^3) / (G * M_p)
    double dm_dt_kg_s = (epsilon * hot_jupiter::PI * f_euv * std::pow(r_p, 3.0)) / (hot_jupiter::G * m_p);
    double dm_dt_gyr = (dm_dt_kg_s * hot_jupiter::GYR) / m_p;

    csv_file << std::scientific << f_euv << "," << dm_dt_kg_s << "," << std::fixed << std::setprecision(4) << dm_dt_gyr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_041/hydrodynamic_escape_rates.csv" << std::endl;
  return 0;
}
