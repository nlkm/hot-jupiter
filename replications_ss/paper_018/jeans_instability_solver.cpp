// Solver for Paper #18: Protoplanetary Cloud Collapse & Jeans Instability (Jeans 1902, Larson 1969)
// Evaluates Jeans mass and critical collapse length scale in molecular clouds across gas temperatures and densities.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "star_formation.hpp"

int main() {
  std::cout << "=== Running Jeans (1902) & Larson (1969) Cloud Collapse Solver ===" << std::endl;

  hot_jupiter::JeansInstabilityModel jeans_model;

  std::ofstream csv_file("replications_ss/paper_018/jeans_collapse_scales.csv");
  csv_file << "temp_k,rho_kg_m3,jeans_mass_solar,jeans_length_pc\n";

  // Temperatures 10K to 100K in molecular cloud core
  for (double temp_k = 10.0; temp_k <= 100.0; temp_k += 5.0) {
    double rho_kg_m3 = 1.0e-16;  // Standard dense core density
    double m_j_solar = jeans_model.jeans_mass_kg(temp_k, rho_kg_m3) / hot_jupiter::M_SUN;
    double l_j_pc = jeans_model.jeans_length_m(temp_k, rho_kg_m3) / (3.086e16);

    csv_file << std::fixed << std::setprecision(1) << temp_k << "," << std::scientific << rho_kg_m3 << "," << std::fixed << std::setprecision(3) << m_j_solar << "," << std::setprecision(4) << l_j_pc << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_018/jeans_collapse_scales.csv" << std::endl;
  return 0;
}
