// Solver for Paper #30: Larson Scaling Laws for Giant Molecular Clouds (Larson 1981)
// Evaluates velocity dispersion scaling \sigma_v \propto L^{0.38} and mean density scaling \rho \propto L^{-1.1}.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "star_formation.hpp"

int main() {
  std::cout << "=== Running Larson (1981) Giant Molecular Cloud Scaling Laws Solver ===" << std::endl;

  hot_jupiter::LarsonScalingLawsModel larson_model;

  std::ofstream csv_file("replications_ss/paper_030/larson_scaling_laws.csv");
  csv_file << "cloud_size_pc,sigma_v_m_s,mean_rho_kg_m3\n";

  // Cloud sizes from 0.1 pc to 100.0 pc
  for (double size_pc = 0.1; size_pc <= 100.0; size_pc += 5.0) {
    double sigma_v = larson_model.velocity_dispersion_m_s(size_pc);
    double rho = larson_model.mean_density_kg_m3(size_pc);

    csv_file << std::fixed << std::setprecision(1) << size_pc << "," << std::setprecision(2) << sigma_v << "," << std::scientific << rho << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_030/larson_scaling_laws.csv" << std::endl;
  return 0;
}
