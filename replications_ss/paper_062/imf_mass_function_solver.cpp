// Solver for Paper #62: Stellar Initial Mass Function (IMF) & Power-Law Multi-Segment Scaling (Salpeter 1955, Kroupa 2001, Chabrier 2003)
// Evaluates Salpeter alpha = 2.35 power law, Kroupa multi-segment power laws, and Chabrier log-normal distribution for brown dwarfs and low-mass stars.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "star_formation.hpp"

int main() {
  std::cout << "=== Running Salpeter (1955), Kroupa (2001) & Chabrier (2003) IMF Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_062/imf_mass_spectrum.csv");
  csv_file << "mass_solar,salpeter_dn_dm,kroupa_dn_dm,chabrier_dn_dm\n";

  // Stellar masses from 0.01 M_sun to 10.0 M_sun
  for (double m = 0.01; m <= 10.0; m *= 1.25) {
    // Salpeter (1955): dN/dM ~ M^-2.35
    double dn_dm_salpeter = std::pow(m, -2.35);

    // Kroupa (2001) segmented power law:
    // alpha = 0.3 for m < 0.08 M_sun, 1.3 for 0.08 <= m < 0.5 M_sun, 2.3 for m >= 0.5 M_sun
    double dn_dm_kroupa = 0.0;
    if (m < 0.08) {
      dn_dm_kroupa = std::pow(m / 0.08, -0.3) * std::pow(0.08 / 0.5, -1.3) * std::pow(0.5, -2.3);
    } else if (m < 0.5) {
      dn_dm_kroupa = std::pow(m / 0.5, -1.3) * std::pow(0.5, -2.3);
    } else {
      dn_dm_kroupa = std::pow(m, -2.3);
    }

    // Chabrier (2003) log-normal distribution for m <= 1 M_sun:
    // dN/dlnM ~ exp(-(ln(M/0.08))^2 / (2 * 0.55^2)) => dN/dM = (1/M) * exp(...)
    double dn_dm_chabrier = 0.0;
    if (m <= 1.0) {
      double log_m_ratio = std::log(m / 0.08);
      dn_dm_chabrier = (1.0 / m) * std::exp(-(log_m_ratio * log_m_ratio) / (2.0 * 0.55 * 0.55));
    } else {
      dn_dm_chabrier = std::pow(m, -2.3);
    }

    csv_file << std::scientific << std::setprecision(4) << m << "," << dn_dm_salpeter << "," << dn_dm_kroupa << "," << dn_dm_chabrier << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_062/imf_mass_spectrum.csv" << std::endl;
  return 0;
}
