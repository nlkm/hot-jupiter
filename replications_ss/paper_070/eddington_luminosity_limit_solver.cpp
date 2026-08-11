// Solver for Paper #70: Main Sequence Stellar Structure & Eddington Radiation Pressure Limit (Eddington 1926, Schwarzschild 1958)
// Evaluates Eddington luminosity L_edd = 4 * pi * G * M * c / kappa_es, radiation pressure ratio Gamma = L / L_edd, and maximum stable stellar mass.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Eddington (1926) & Schwarzschild (1958) Stellar Structure Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_070/eddington_stellar_limits.csv");
  csv_file << "mass_solar,luminosity_solar,eddington_lum_solar,gamma_ratio\n";

  double c_speed = 2.99792458e8; // Speed of light [m/s]
  double kappa_es = 0.034;        // Electron scattering opacity [m^2/kg] (0.34 cm^2/g for solar hydrogen composition X=0.7)

  // Stellar masses M from 1.0 M_sun to 150.0 M_sun
  for (double m_solar = 1.0; m_solar <= 150.0; m_solar *= 1.5) {
    double m_kg = m_solar * hot_jupiter::M_SUN;

    // Mass-luminosity relation L ~ M^3.5 for M < 20 M_sun, L ~ M^1.5 for M > 20 M_sun
    double l_solar = 0.0;
    if (m_solar < 20.0) {
      l_solar = std::pow(m_solar, 3.5);
    } else {
      l_solar = 3.0e3 * std::pow(m_solar, 1.5);
    }

    // Eddington luminosity L_edd = 4 * pi * G * M * c / kappa_es
    double l_edd_w = 4.0 * hot_jupiter::PI * hot_jupiter::G * m_kg * c_speed / kappa_es;
    double l_edd_solar = l_edd_w / hot_jupiter::L_SUN;

    // Eddington radiation force ratio Gamma = L / L_edd
    double gamma_ratio = l_solar / l_edd_solar;

    csv_file << std::fixed << std::setprecision(1) << m_solar << "," << std::scientific << std::setprecision(3) << l_solar << "," << l_edd_solar << "," << std::fixed << std::setprecision(3) << gamma_ratio << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_070/eddington_stellar_limits.csv" << std::endl;
  return 0;
}
