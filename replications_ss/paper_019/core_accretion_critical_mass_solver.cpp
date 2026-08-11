// Solver for Paper #19: Core Accretion Critical Core Mass (Mizuno 1980, Stevenson 1982)
// Evaluates critical core mass M_crit required for runaway gas accretion in protoplanetary disks.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Mizuno (1980) & Stevenson (1982) Critical Core Mass Solver ===" << std::endl;

  hot_jupiter::CoreAccretionModel core_model;

  std::ofstream csv_file("replications_ss/paper_019/critical_core_masses.csv");
  csv_file << "mdot_planetesimal_earth_yr,opacity_cm2_g,m_crit_earth\n";

  // Planetesimal accretion rates 1e-7 to 1e-5 M_earth/yr
  for (double mdot_val = 1.0e-7; mdot_val <= 1.0e-5; mdot_val += 1.0e-6) {
    double opacity = 0.1;  // Standard grain opacity cm^2/g
    double m_crit_kg = core_model.critical_core_mass_kg(mdot_val, opacity);
    double m_crit_earth = m_crit_kg / 5.972e24;

    csv_file << std::scientific << mdot_val << "," << std::fixed << std::setprecision(2) << opacity << "," << m_crit_earth << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_019/critical_core_masses.csv" << std::endl;
  return 0;
}
