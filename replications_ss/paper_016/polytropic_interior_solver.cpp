// Solver for Paper #16: Polytropic Stellar Interiors & Lane-Emden Structure (Kippenhahn & Weigert 1990)
// Evaluates central pressure, central density, and internal pressure profiles for n=1.5 and n=3.0 polytropes.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Kippenhahn & Weigert (1990) Polytropic Stellar Interior Solver ===" << std::endl;

  hot_jupiter::PolytropicStellarInteriorModel interior_model;

  std::ofstream csv_file("replications_ss/paper_016/polytropic_interior_profiles.csv");
  csv_file << "m_solar,pc_pa_n15,rhoc_kg_m3_n15,pc_pa_n30,rhoc_kg_m3_n30\n";

  // Stellar masses from 0.1 M_sun to 10.0 M_sun
  for (double m_solar = 0.1; m_solar <= 10.0; m_solar += 0.5) {
    double m_kg = m_solar * hot_jupiter::M_SUN;
    hot_jupiter::StellarMainSequenceModel ms_model;
    double r_m = ms_model.zams_radius_m(m_kg);

    double p15 = interior_model.central_pressure_pa(m_kg, r_m, 1.5);
    double rho15 = interior_model.central_density_kg_m3(m_kg, r_m, 1.5);
    double p30 = interior_model.central_pressure_pa(m_kg, r_m, 3.0);
    double rho30 = interior_model.central_density_kg_m3(m_kg, r_m, 3.0);

    csv_file << std::fixed << std::setprecision(2) << m_solar << "," << std::scientific << p15 << "," << rho15 << "," << p30 << "," << rho30 << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_016/polytropic_interior_profiles.csv" << std::endl;
  return 0;
}
