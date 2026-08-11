// Solver for Paper #12: Enceladus Subsurface Ocean & Tidal Heating Dynamics (Spencer et al. 2006)
// Evaluates viscoelastic tidal heat production as a function of orbital eccentricity and tidal dissipation factor k2/Q.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Spencer et al. (2006) Enceladus Ocean Tidal Solver ===" << std::endl;

  hot_jupiter::TidalDissipationModel model;

  std::ofstream csv_file("replications_ss/paper_012/enceladus_tidal_heat.csv");
  csv_file << "eccentricity,power_gw_k2q_001,power_gw_k2q_005,power_gw_k2q_010\n";

  for (double e = 0.000; e <= 0.015; e += 0.0005) {
    double p1 = model.enceladus_tidal_power_gw(e, 0.001);
    double p5 = model.enceladus_tidal_power_gw(e, 0.005);
    double p10 = model.enceladus_tidal_power_gw(e, 0.010);

    csv_file << std::fixed << std::setprecision(6) << e << "," << p1 << "," << p5 << "," << p10 << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_012/enceladus_tidal_heat.csv" << std::endl;
  return 0;
}
