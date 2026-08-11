// Solver for Paper #15: Planet Nine Secular Perihelion Alignment & Orbital Precession (Batygin & Brown 2016)
// Evaluates secular precession rates exerted by Planet Nine on distant Trans-Neptunian Objects (TNOs).

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Batygin & Brown (2016) Planet Nine Secular Solver ===" << std::endl;

  hot_jupiter::PlanetNineSecularModel p9_model;

  std::ofstream csv_file("replications_ss/paper_015/planet_nine_secular_rates.csv");
  csv_file << "a_tno_au,precession_rad_yr,precession_arcsec_yr\n";

  for (double a_tno = 150.0; a_tno <= 450.0; a_tno += 10.0) {
    double rad_yr = p9_model.planet_nine_secular_precession_rad_yr(a_tno, 500.0, 10.0);
    double arcsec_yr = (rad_yr * 180.0 * 3600.0) / M_PI;

    csv_file << std::fixed << std::setprecision(2) << a_tno << "," << std::scientific << rad_yr << "," << std::fixed << std::setprecision(6) << arcsec_yr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_015/planet_nine_secular_rates.csv" << std::endl;
  return 0;
}
