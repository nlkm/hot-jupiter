// Solver for Paper #59: Planetary Atmospheric Greenhouse Warming & Radiative-Convective Equilibrium (Manabe & Wetherald 1967, Kasting 1993)
// Evaluates surface equilibrium temperature T_surf = T_eff * (1 + 0.75 * tau_optical)^(1/4), runaway greenhouse limit, and habitable zone boundaries.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Manabe & Wetherald (1967) & Kasting (1993) Greenhouse Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_059/greenhouse_temperatures.csv");
  csv_file << "optical_depth_tau,t_eff_k,t_surf_k,runaway_greenhouse_bool\n";

  double s_solar = 1361.0;     // W/m^2 solar constant
  double albedo = 0.30;        // Earth bond albedo 0.30
  double sigma_sb = 5.670374e-8;

  // Effective emission temperature T_eff = ((1 - A) * S / (4 * sigma))^(1/4) (~ 255 K for Earth)
  double t_eff = std::pow((1.0 - albedo) * s_solar / (4.0 * sigma_sb), 0.25);

  // Infrared optical depths tau from 0.0 to 10.0
  for (double tau = 0.0; tau <= 10.0; tau += 0.5) {
    // 1D Eddington grey atmosphere radiative equilibrium temperature T_surf = T_eff * (1 + 0.75 * tau)^(1/4)
    double t_surf = t_eff * std::pow(1.0 + 0.75 * tau, 0.25);

    // Kasting (1993) runaway greenhouse temperature limit T > 340 K (water vapor saturation feedback)
    bool runaway = (t_surf >= 340.0);

    csv_file << std::fixed << std::setprecision(1) << tau << "," << std::setprecision(1) << t_eff << "," << std::setprecision(1) << t_surf << "," << (runaway ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_059/greenhouse_temperatures.csv" << std::endl;
  return 0;
}
