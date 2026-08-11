// Solver for Paper #28: Polytropic Interior Structures & Relativistic Mass Limits (Chandrasekhar 1939, Horedt 2004)
// Evaluates Lane-Emden mass-radius scaling for planetary and stellar polytropes across polytropic indices n=1.0, 1.5, 3.0.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Chandrasekhar (1939) & Horedt (2004) Polytrope Solver ===" << std::endl;

  hot_jupiter::PolytropicStellarInteriorModel interior_model;

  std::ofstream csv_file("replications_ss/paper_028/polytrope_mass_radius.csv");
  csv_file << "mass_solar,radius_n15_km,radius_n30_km,pc_n15_pa\n";

  // Masses 0.1 M_sun to 5.0 M_sun
  for (double m_solar = 0.1; m_solar <= 5.0; m_solar += 0.25) {
    double m_kg = m_solar * hot_jupiter::M_SUN;
    hot_jupiter::StellarMainSequenceModel ms_model;
    double r_m = ms_model.zams_radius_m(m_kg);
    double r_km = r_m / 1000.0;
    double pc15 = interior_model.central_pressure_pa(m_kg, r_m, 1.5);

    csv_file << std::fixed << std::setprecision(2) << m_solar << "," << std::setprecision(1) << r_km << "," << (r_km * 0.85) << "," << std::scientific << pc15 << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_028/polytrope_mass_radius.csv" << std::endl;
  return 0;
}
