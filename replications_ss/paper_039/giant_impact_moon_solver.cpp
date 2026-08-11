// Solver for Paper #39: Giant Impact Moon Formation & Debris Disk Accretion (Cameron & Ward 1976, Canup & Asphaug 2001)
// Evaluates impactor mass ratio gamma = M_imp / M_tot, debris disk mass M_disk, and angular momentum budget J_tot.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Cameron & Ward (1976) & Canup (2001) Giant Impact Moon Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_039/giant_impact_disk_masses.csv");
  csv_file << "impactor_mass_ratio,impactor_mass_mars,m_disk_lunar_masses,j_tot_j_earth_moon\n";

  double m_earth = hot_jupiter::M_EARTH;
  double m_moon = 7.342e22;  // Moon mass [kg]
  double m_mars = 6.4171e23; // Mars mass [kg]
  double r_earth = hot_jupiter::R_EARTH;

  // Impactor mass ratios gamma = M_impactor / M_protoearth from 0.05 to 0.25 (Mars-sized impactors)
  for (double gamma = 0.05; gamma <= 0.25; gamma += 0.025) {
    double m_impactor = gamma * m_earth;
    double m_imp_mars = m_impactor / m_mars;

    // Canup (2001) disk mass fraction fit M_disk / M_impactor ~ 0.15 * (4 * gamma)^0.8
    double m_disk_kg = 0.15 * std::pow(4.0 * gamma, 0.8) * m_impactor;
    double m_disk_lunar = m_disk_kg / m_moon;

    // Total angular momentum in units of current Earth-Moon system J_EM ~ 3.5e34 kg m^2 / s
    double j_em = 3.5e34;
    double v_imp = std::sqrt(hot_jupiter::G * (m_earth + m_impactor) / r_earth);  // mutual escape speed
    double b_param = 0.707;  // 45 degree impact angle (sin b = 0.707)
    double j_impact = gamma * m_earth * v_imp * r_earth * b_param;
    double j_ratio = j_impact / j_em;

    csv_file << std::fixed << std::setprecision(3) << gamma << "," << std::setprecision(2) << m_imp_mars << "," << std::setprecision(2) << m_disk_lunar << "," << std::setprecision(2) << j_ratio << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_039/giant_impact_disk_masses.csv" << std::endl;
  return 0;
}
