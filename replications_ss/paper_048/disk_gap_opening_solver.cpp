// Solver for Paper #48: Giant Planet Gap Opening & Viscous Type II Migration (Lin & Papaloizou 1986, Crida et al. 2006)
// Evaluates Crida gap parameter C_crida = 1.1 * (H/r_H) * (q/3)^(-1/3) + 50 * (alpha * (H/r)^2 / q) and gap opening threshold C_crida < 1.0.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Lin & Papaloizou (1986) & Crida et al. (2006) Disk Gap Opening Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_048/disk_gap_opening_crida.csv");
  csv_file << "planet_mass_jup,aspect_ratio_h_r,alpha_viscosity,crida_parameter,gap_opened_bool\n";

  double h_over_r = 0.05;  // typical disk aspect ratio H/r = 0.05
  double alpha = 1.0e-3;   // Shakura-Sunyaev viscosity alpha

  // Planet masses from 0.1 M_jup to 5.0 M_jup
  for (double m_jup = 0.1; m_jup <= 5.0; m_jup *= 1.5) {
    double q = (m_jup * hot_jupiter::M_JUP) / hot_jupiter::M_SUN;
    double r_hill_over_r = std::pow(q / 3.0, 1.0 / 3.0);

    // Crida et al. (2006) gap parameter C_crida:
    // C_crida = 1.1 * (H / r_H) + 50 * (alpha * (H/r)^2 / q)
    double term1 = 1.1 * (h_over_r / r_hill_over_r);
    double term2 = 50.0 * (alpha * h_over_r * h_over_r / q);
    double c_crida = term1 + term2;

    bool gap_opened = (c_crida <= 1.0);

    csv_file << std::fixed << std::setprecision(2) << m_jup << "," << h_over_r << "," << std::scientific << alpha << "," << std::fixed << std::setprecision(3) << c_crida << "," << (gap_opened ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_048/disk_gap_opening_crida.csv" << std::endl;
  return 0;
}
