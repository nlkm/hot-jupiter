// Solver for Paper #33: Tidal Disruption of Comets & Rubble Pile Asteroids (Asphaug & Benz 1996, Richardson et al. 1998)
// Evaluates tidal disruption radius r_disrupt = R_p * (2 * rho_p / rho_b)^(1/3) for Shoemaker-Levy 9 and rubble pile encounters.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Asphaug & Benz (1996) Rubble Pile Tidal Disruption Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_033/tidal_disruption_radii.csv");
  csv_file << "body_density_g_cm3,disrupt_radius_rjup,disrupt_distance_km\n";

  double r_jup_m = hot_jupiter::R_JUP;
  double rho_jup = 1326.0;  // Jupiter mean density [kg/m^3]

  // Body densities from 0.5 g/cm^3 (cometary) to 3.5 g/cm^3 (rocky asteroid)
  for (double rho_b_g_cm3 = 0.5; rho_b_g_cm3 <= 3.5; rho_b_g_cm3 += 0.25) {
    double rho_b_kg_m3 = rho_b_g_cm3 * 1000.0;
    // Roche fluid limit / tidal disruption limit: r_d = R_p * (2.0 * rho_p / rho_b)^(1/3)
    double r_d_ratio = std::pow(2.0 * rho_jup / rho_b_kg_m3, 1.0 / 3.0);
    double r_d_km = (r_d_ratio * r_jup_m) / 1000.0;

    csv_file << std::fixed << std::setprecision(2) << rho_b_g_cm3 << "," << std::setprecision(3) << r_d_ratio << "," << std::setprecision(1) << r_d_km << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_033/tidal_disruption_radii.csv" << std::endl;
  return 0;
}
