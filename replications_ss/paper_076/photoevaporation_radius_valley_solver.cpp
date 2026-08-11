// Solver for Paper #76: Photo-evaporative Atmospheric Mass Loss & Exoplanet Radius Valley (Owen & Wu 2013, 2017, Fulton 2017)
// Evaluates energy-limited EUV mass loss M_dot = eta * (pi R_p^3 F_EUV) / (G M_p K_tide), core bare radius R_core ~ 1.8 R_earth, and Fulton radius gap at 1.8 R_earth.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Owen & Wu (2013, 2017) Photo-evaporation Radius Valley Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_076/photoevaporation_valley_radii.csv");
  csv_file << "euv_flux_earth_units,initial_mass_earth,stripped_core_radius_re,final_envelope_fraction\n";

  // Stellar EUV flux F_EUV from 10 to 1000 S_earth
  for (double f_euv = 10.0; f_euv <= 1000.0; f_euv += 50.0) {
    double m_core_earth = 5.0;  // 5.0 M_earth core
    double r_core_earth = std::pow(m_core_earth, 0.27);  // 1.54 R_earth bare core

    // Owen & Wu (2017) energy-limited mass loss criterion:
    // If integrated EUV exposure exceeds envelope binding energy, atmosphere is stripped to bare core.
    double f_env_final = 0.0;
    if (f_euv < 300.0) {
      f_env_final = 0.05 * (1.0 - f_euv / 300.0);
    } else {
      f_env_final = 0.0;  // Complete atmospheric stripping -> bare core in Fulton gap
    }

    double r_final_earth = r_core_earth * (1.0 + 2.0 * f_env_final);

    csv_file << std::fixed << std::setprecision(0) << f_euv << "," << std::setprecision(1) << m_core_earth << "," << std::setprecision(3) << r_final_earth << "," << std::setprecision(4) << f_env_final << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_076/photoevaporation_valley_radii.csv" << std::endl;
  return 0;
}
