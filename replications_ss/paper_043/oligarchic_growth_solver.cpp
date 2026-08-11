// Solver for Paper #43: Runaway Accretion & Oligarchic Growth of Planetesimals (Kokubo & Ida 1998, 2000)
// Evaluates isolation mass M_iso = 2.7 * (b * a / 10)^1.5 * (Sigma_m / 10)^1.5 * M_sun and growth rate dM/dt.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Kokubo & Ida (1998, 2000) Oligarchic Growth Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_043/oligarchic_isolation_masses.csv");
  csv_file << "a_au,sigma_solid_g_cm2,m_iso_earth_masses,t_growth_myr\n";

  double m_earth = hot_jupiter::M_EARTH;

  // Semi-major axes from 0.5 AU to 5.0 AU (terrestrial to giant planet region)
  for (double a_au = 0.5; a_au <= 5.0; a_au += 0.5) {
    // Solid surface density MMSN: Sigma_s(a) = 10.0 * (a / 1 AU)^-1.5 g/cm^2
    double sigma_solid_g_cm2 = 10.0 * std::pow(a_au, -1.5);

    // Kokubo & Ida (2000) isolation mass M_iso = 2.7 * (b / 10)^1.5 * (Sigma_s / 10)^1.5 * (a / 1 AU)^3 * M_earth
    double b_feeding_width = 10.0;  // 10 Hill radii feeding zone width
    double m_iso_kg = 2.7 * std::pow(b_feeding_width / 10.0, 1.5) * std::pow(sigma_solid_g_cm2 / 10.0, 1.5) * std::pow(a_au, 3.0) * (0.01 * m_earth);
    double m_iso_earth = m_iso_kg / m_earth;

    // Growth timescale t_growth ~ 0.1 * (a / 1 AU)^2.5 / (Sigma_s / 10) Myr
    double t_growth_myr = 0.1 * std::pow(a_au, 2.5) / (sigma_solid_g_cm2 / 10.0);

    csv_file << std::fixed << std::setprecision(1) << a_au << "," << std::setprecision(2) << sigma_solid_g_cm2 << "," << std::setprecision(3) << m_iso_earth << "," << std::setprecision(2) << t_growth_myr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_043/oligarchic_isolation_masses.csv" << std::endl;
  return 0;
}
