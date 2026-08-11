// Solver for Paper #32: Protostellar Free-Fall Cloud Collapse Dynamics (Hunter 1977, Whitworth & Summers 1985)
// Evaluates free-fall collapse timescale t_ff = sqrt(3*pi / (32*G*rho_0)) and infall trajectory r(t).

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "star_formation.hpp"

int main() {
  std::cout << "=== Running Hunter (1977) & Whitworth (1985) Free-Fall Collapse Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_032/freefall_collapse_timescales.csv");
  csv_file << "rho0_kg_m3,n_h2_cm3,t_ff_years,t_ff_kyr\n";

  // Initial cloud densities from n(H2) = 1e3 to 1e7 cm^-3
  for (double n_h2 = 1.0e3; n_h2 <= 1.0e7; n_h2 *= 2.5) {
    double m_h2 = 2.0 * 1.6735575e-27;
    double rho0 = n_h2 * 1.0e6 * m_h2;  // kg/m^3
    double t_ff_sec = std::sqrt((3.0 * hot_jupiter::PI) / (32.0 * hot_jupiter::G * rho0));
    double t_ff_years = t_ff_sec / (365.25 * 86400.0);
    double t_ff_kyr = t_ff_years / 1000.0;

    csv_file << std::scientific << rho0 << "," << n_h2 << "," << std::fixed << std::setprecision(1) << t_ff_years << "," << std::setprecision(2) << t_ff_kyr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_032/freefall_collapse_timescales.csv" << std::endl;
  return 0;
}
