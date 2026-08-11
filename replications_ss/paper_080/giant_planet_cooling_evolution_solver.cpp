// Solver for Paper #80: Giant Exoplanet Core-Envelope Thermal Evolution & Cooling (Burrows 1997, Baraffe 2003, Fortney 2007)
// Evaluates luminosity decay L(t) ~ t^-1.2, contraction radius R(t), and effective temperature T_eff evolution over 10 Gyr.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Burrows (1997) & Baraffe (2003) Giant Planet Cooling Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_080/giant_planet_cooling_tracks.csv");
  csv_file << "age_gyr,luminosity_solar,radius_rjup,t_eff_k\n";

  double m_planet_jup = 1.0;

  // Ages t from 0.01 Gyr to 10.0 Gyr
  for (double age_gyr = 0.01; age_gyr <= 10.0; age_gyr += 0.5) {
    // Burrows et al. (1997) analytical cooling track scalings:
    // Luminosity L / L_sun = 1e-6 * (m / M_jup)^1.8 * (age / 1 Gyr)^(-1.2)
    double l_solar = 1.0e-6 * std::pow(m_planet_jup, 1.8) * std::pow(age_gyr, -1.2);
    double l_watts = l_solar * hot_jupiter::L_SUN;

    // Radius evolution R(t) / R_jup = 1.0 + 0.3 * (age / 1 Gyr)^(-0.15)
    double r_rjup = 1.0 + 0.3 * std::pow(age_gyr, -0.15);
    double r_m = r_rjup * hot_jupiter::R_JUP;

    // Effective temperature T_eff = (L / (4 pi R^2 sigma_SB))^(1/4)
    double t_eff_k = std::pow(l_watts / (4.0 * hot_jupiter::PI * r_m * r_m * hot_jupiter::SIGMA_SB), 0.25);

    csv_file << std::fixed << std::setprecision(2) << age_gyr << "," << std::scientific << std::setprecision(3) << l_solar << "," << std::fixed << std::setprecision(3) << r_rjup << "," << std::setprecision(1) << t_eff_k << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_080/giant_planet_cooling_tracks.csv" << std::endl;
  return 0;
}
