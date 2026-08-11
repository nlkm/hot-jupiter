// Solver for Paper #27: Laplace-Lagrange Secular Perturbation Theory (Laskar 1988, 1989)
// Evaluates secular eigenfrequencies g5, g6 and long-term eccentricity oscillations of Jupiter and Saturn.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Laskar (1988, 1989) Laplace-Lagrange Secular Solver ===" << std::endl;

  hot_jupiter::LaplaceLagrangeSecularModel ll_model;

  std::ofstream csv_file("replications_ss/paper_027/laplace_lagrange_eccentricities.csv");
  csv_file << "time_kyr,e_jupiter_analytical\n";

  // Secular time series from 0 to 1000 kyr
  for (double t_kyr = 0.0; t_kyr <= 1000.0; t_kyr += 25.0) {
    double t_yr = t_kyr * 1000.0;
    double e_jup = ll_model.jupiter_eccentricity_at_time_yr(t_yr);

    csv_file << std::fixed << std::setprecision(1) << t_kyr << "," << std::setprecision(4) << e_jup << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_027/laplace_lagrange_eccentricities.csv" << std::endl;
  return 0;
}
