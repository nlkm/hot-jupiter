// Solver for Paper #29: Bonnor-Ebert Sphere Critical Mass & ISM Collapse (Bonnor 1956, Ebert 1955)
// Evaluates critical Bonnor-Ebert mass M_BE and pressure equilibrium for isothermal molecular cloud cores.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "star_formation.hpp"

int main() {
  std::cout << "=== Running Bonnor (1956) & Ebert (1955) Sphere Collapse Solver ===" << std::endl;

  hot_jupiter::BonnorEbertSphereModel be_model;

  std::ofstream csv_file("replications_ss/paper_029/bonnor_ebert_critical_masses.csv");
  csv_file << "temp_k,p_ext_k_cm3,m_be_solar\n";

  // External pressure P_ext / k_B from 1e4 to 1e6 K/cm^3
  for (double p_ext_k = 1.0e4; p_ext_k <= 1.0e6; p_ext_k += 5.0e4) {
    double p_ext_pa = p_ext_k * 1.380649e-23 * 1.0e6;
    double temp_k = 10.0;
    double m_be_solar = be_model.bonnor_ebert_mass_kg(temp_k, p_ext_pa) / hot_jupiter::M_SUN;

    csv_file << std::fixed << std::setprecision(1) << temp_k << "," << std::scientific << p_ext_k << "," << std::fixed << std::setprecision(3) << m_be_solar << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_029/bonnor_ebert_critical_masses.csv" << std::endl;
  return 0;
}
