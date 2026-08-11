// Solver for Paper #84: Post-Main-Sequence Stellar Mass Loss & Planetary Orbital Expansion (Villaver & Livio 2007, Mustill & Villaver 2012, Adams 2013)
// Evaluates adiabatic orbital expansion a(t) M_*(t) = const, RGB/AGB tidal engulfment radius r_engulf, and planetary survival thresholds.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Villaver & Livio (2007) Post-MS Orbital Expansion Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_084/post_ms_orbital_expansion.csv");
  csv_file << "initial_semi_au,final_stellar_mass_solar,expanded_semi_au,max_rgb_radius_au,survival_flag\n";

  double m_star_init_solar = 1.0;
  double r_max_rgb_au = 1.0;  // Solar RGB maximum radius ~ 1.0 AU (215 R_sun)

  // Planetary initial semi-major axis a_init from 0.5 AU to 5.0 AU
  for (double a_init_au = 0.5; a_init_au <= 5.0; a_init_au += 0.25) {
    // Final white dwarf mass M_final = 0.54 M_sun
    double m_star_final_solar = 0.54;

    // Adiabatic mass loss orbital expansion: a_final = a_init * (M_init / M_final)
    double a_expanded_au = a_init_au * (m_star_init_solar / m_star_final_solar);

    // Survival criterion: Initial pericenter a_init > r_max_rgb_au (plus tidal decay threshold)
    bool survived = (a_init_au >= 1.20 * r_max_rgb_au);

    csv_file << std::fixed << std::setprecision(2) << a_init_au << "," << std::setprecision(2) << m_star_final_solar << "," << std::setprecision(2) << a_expanded_au << "," << std::setprecision(2) << r_max_rgb_au << "," << (survived ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_084/post_ms_orbital_expansion.csv" << std::endl;
  return 0;
}
