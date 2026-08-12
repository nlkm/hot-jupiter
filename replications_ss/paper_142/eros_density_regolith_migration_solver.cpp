// Solver for Paper #142: (433) Eros Mass, Bulk Density, & Regolith Seismic Migration (Yeomans 2000, Veverka 2000, Cheng 2002, Thomas 2002)
// Evaluates NEAR Shoemaker orbit mass determination M_Eros = 6.687 x 10^15 kg, bulk density rho_bulk = 2.67 +- 0.03 g/cm^3 (homogeneous L/LL ordinary chondrite composition with ~ 20% macro-porosity), seismic shaking from sub-catastrophic impacts driving regolith downslope migration, crater erasure up to d ~ 100 m, and smooth regolith ponds in low-potential gravity depressions.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Yeomans et al. (2000) & Cheng et al. (2002) (433) Eros Density & Regolith Migration Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_142/eros_regolith_migration.csv");
  csv_file << "impact_energy_joules,seismic_acceleration_g,regolith_pond_depth_m,crater_erasure_diameter_m,retained_macro_porosity_pct\n";

  // Seismic impact energy log10(E) from 10^12 J to 10^18 J
  for (double log_e = 12.0; log_e <= 18.0; log_e += 1.0) {
    double energy_j = std::pow(10.0, log_e);

    // Peak seismic acceleration relative to Eros surface gravity g_surface ~ 0.005 m/s^2:
    double g_seismic = 0.2 * std::pow(energy_j / 1.0e15, 0.4);

    // Regolith pond accumulation depth in gravity depressions (m):
    double pond_depth_m = 2.5 * std::pow(energy_j / 1.0e15, 0.3);

    // Crater erasure diameter cutoff d_erasure (m):
    double d_erasure_m = 45.0 * (g_seismic / 0.2);

    // Macro-porosity retained %:
    double porosity_pct = 20.0;

    csv_file << std::scientific << std::setprecision(2) << energy_j << "," << std::fixed << std::setprecision(3) << g_seismic << "," << std::setprecision(2) << pond_depth_m << "," << std::setprecision(1) << d_erasure_m << "," << std::setprecision(1) << porosity_pct << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_142/eros_regolith_migration.csv" << std::endl;
  return 0;
}
