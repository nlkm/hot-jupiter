// Solver for Paper #31: Stellar Initial Mass Functions (Salpeter 1955, Chabrier 2003)
// Evaluates mass distributions dN/dM for power-law Salpeter and log-normal Chabrier IMFs.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "star_formation.hpp"

int main() {
  std::cout << "=== Running Salpeter (1955) & Chabrier (2003) IMF Solver ===" << std::endl;

  hot_jupiter::InitialMassFunctionModel imf_model;

  std::ofstream csv_file("replications_ss/paper_031/imf_mass_distributions.csv");
  csv_file << "mass_solar,salpeter_val,chabrier_val\n";

  // Stellar mass spectrum from 0.08 M_sun to 50.0 M_sun
  for (double m_solar = 0.08; m_solar <= 50.0; m_solar *= 1.25) {
    double m_kg = m_solar * hot_jupiter::M_SUN;
    double salpeter_val = imf_model.salpeter_imf(m_kg);
    double chabrier_val = imf_model.chabrier_imf(m_kg);

    csv_file << std::fixed << std::setprecision(3) << m_solar << "," << std::scientific << salpeter_val << "," << chabrier_val << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_031/imf_mass_distributions.csv" << std::endl;
  return 0;
}
