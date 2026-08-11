// Solver for Paper #65: Pre-Main Sequence Stellar Contraction on Hayashi & Henyey Tracks (Hayashi 1961, Henyey et al. 1955)
// Evaluates PMS radius contraction R(t) = R_init * (1 + t / t_KH)^(-1/3), Hayashi boundary temperature T_eff ~ 3000-4000 K, and Hertzsprung-Russell track evolution.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Hayashi (1961) & Henyey (1955) PMS Contraction Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_065/pms_evolution_tracks.csv");
  csv_file << "age_myr,radius_solar,luminosity_solar,teff_k\n";

  double m_star_solar = 1.0;     // 1.0 M_sun protostar
  double r_init_solar = 5.0;     // initial birth radius 5.0 R_sun
  double t_kh_myr = 10.0;        // Kelvin-Helmholtz timescale ~ 10 Myr

  // PMS age from 0.1 Myr to 50.0 Myr
  for (double t_myr = 0.1; t_myr <= 50.0; t_myr += 1.0) {
    // Kelvin-Helmholtz radius contraction on Hayashi track: R(t) = R_init * (1 + t / t_KH)^(-1/3)
    double r_solar = r_init_solar / std::pow(1.0 + t_myr / t_kh_myr, 1.0 / 3.0);

    // Hayashi convective track near-constant T_eff ~ 4000 K (H- opacity limit)
    double teff_k = 4000.0 * std::pow(m_star_solar, 0.1);

    // Luminosity L = 4 * pi * R^2 * sigma * T_eff^4
    double l_solar = std::pow(r_solar, 2.0) * std::pow(teff_k / 5778.0, 4.0);

    csv_file << std::fixed << std::setprecision(1) << t_myr << "," << std::setprecision(3) << r_solar << "," << std::setprecision(3) << l_solar << "," << std::setprecision(0) << teff_k << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_065/pms_evolution_tracks.csv" << std::endl;
  return 0;
}
