// Solver for Paper #20: Streaming Instability Planetesimal Formation (Youdin & Goodman 2005, Johansen et al. 2007)
// Evaluates critical dust-to-gas ratio and initial planetesimal mass scales.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Youdin & Goodman (2005) Streaming Instability Solver ===" << std::endl;

  hot_jupiter::StreamingInstabilityModel si_model;

  std::ofstream csv_file("replications_ss/paper_020/streaming_growth_rates.csv");
  csv_file << "stokes_number,z_crit,m_planetesimal_km_eq\n";

  // Stokes numbers from 0.001 to 1.0
  for (double st = 0.001; st <= 1.0; st += 0.05) {
    double z_crit = si_model.critical_dust_to_gas_ratio(st);
    double m_kg = si_model.planetesimal_initial_mass_kg(3.0 * hot_jupiter::AU);
    double radius_km = std::pow((3.0 * m_kg) / (4.0 * M_PI * 2000.0), 1.0 / 3.0) / 1000.0;

    csv_file << std::fixed << std::setprecision(3) << st << "," << std::setprecision(4) << z_crit << "," << std::setprecision(2) << radius_km << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_020/streaming_growth_rates.csv" << std::endl;
  return 0;
}
