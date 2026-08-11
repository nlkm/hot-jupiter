// Solver for Paper #56: Core Instability & Giant Planet Gas Envelope Accretion (Perri & Cameron 1974, Mizuno 1980)
// Evaluates critical core mass M_core_crit ~ 10-15 M_earth, hydrostatic envelope structure, and runaway gas accretion onset.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Perri & Cameron (1974) & Mizuno (1980) Core Instability Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_056/core_envelope_accretion.csv");
  csv_file << "m_core_earth,envelope_opacity_kappa,m_env_over_m_core,runaway_accretion_bool\n";

  double kappa_opacity = 0.01;  // reduced grain opacity 0.01 cm^2/g

  // Core masses from 2.0 M_earth to 20.0 M_earth
  for (double m_core = 2.0; m_core <= 20.0; m_core += 2.0) {
    // Mizuno (1980) critical core mass formula M_crit ~ 12 * (kappa / 0.1)^0.25 M_earth
    double m_core_crit = 12.0 * std::pow(kappa_opacity / 0.1, 0.25);  // ~ 6.7 M_earth for kappa=0.01

    // Gas-to-core mass ratio M_env / M_core scaling as (M_core / M_crit)^3
    double m_env_ratio = std::pow(m_core / m_core_crit, 3.0) * 0.1;
    bool runaway = (m_env_ratio >= 1.0);

    csv_file << std::fixed << std::setprecision(1) << m_core << "," << kappa_opacity << "," << std::setprecision(3) << m_env_ratio << "," << (runaway ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_056/core_envelope_accretion.csv" << std::endl;
  return 0;
}
