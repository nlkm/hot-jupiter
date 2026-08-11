// Solver for Paper #77: Giant Planet Core Instability & Runaway Gas Accretion (Pollack 1996, Ikoma 2000, Bodenheimer 2000)
// Evaluates critical core mass M_critical = 10 (M_dot_core / 1e-6 M_earth/yr)^0.25 (kappa / 1 cm^2/g)^0.25 M_earth and crossover timescale t_crossover.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Pollack (1996) & Ikoma (2000) Runaway Gas Accretion Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_077/runaway_accretion_rates.csv");
  csv_file << "core_mass_earth,dust_opacity_cm2_g,critical_mass_earth,runaway_timescale_myr\n";

  // Solid core accretion rate 1e-6 M_earth/yr
  double mdot_core = 1.0e-6;

  // Dust opacity kappa from 0.01 cm^2/g (grain settling) to 1.0 cm^2/g (interstellar)
  for (double kappa = 0.01; kappa <= 1.0; kappa += 0.05) {
    double m_core_earth = 10.0;

    // Ikoma et al. (2000) analytical critical core mass formula:
    // M_crit = 10 * (Mdot_core / 1e-6)^0.25 * (kappa / 1.0)^0.25 M_earth
    double m_crit_earth = 10.0 * std::pow(mdot_core / 1.0e-6, 0.25) * std::pow(kappa / 1.0, 0.25);

    // Ikoma (2000) runaway accretion onset timescale:
    // tau_runaway = 10^8 * (M_core / M_earth)^(-2.5) * (kappa / 1.0) years
    double tau_runaway_myr = (1.0e8 * std::pow(m_core_earth, -2.5) * kappa) / 1.0e6;

    csv_file << std::fixed << std::setprecision(1) << m_core_earth << "," << std::setprecision(2) << kappa << "," << std::setprecision(2) << m_crit_earth << "," << std::setprecision(3) << tau_runaway_myr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_077/runaway_accretion_rates.csv" << std::endl;
  return 0;
}
