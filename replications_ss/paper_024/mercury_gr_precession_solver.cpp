// Solver for Paper #24: General Relativistic Perihelion Precession of Mercury (Einstein 1915, Laskar 2009)
// Evaluates GR Schwarzschild perihelion precession rate \dot{\varpi}_GR across solar system inner planets.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Einstein (1915) & Laskar (2009) GR Precession Solver ===" << std::endl;

  hot_jupiter::RelativisticPrecessionModel gr_model;

  std::ofstream csv_file("replications_ss/paper_024/mercury_gr_rates.csv");
  csv_file << "planet,a_au,eccentricity,gr_precession_arcsec_century\n";

  struct Planet {
    std::string name;
    double a_au;
    double e;
  };

  std::vector<Planet> planets = {
      {"Mercury", 0.387098, 0.20563},
      {"Venus", 0.723332, 0.00677},
      {"Earth", 1.000000, 0.01671},
      {"Mars", 1.523679, 0.09340}
  };

  for (const auto& p : planets) {
    double a_m = p.a_au * hot_jupiter::AU;
    double rad_s = gr_model.gr_perihelion_precession_rad_s(hot_jupiter::M_SUN, a_m, p.e);
    double arcsec_century = rad_s * ((180.0 * 3600.0) / M_PI) * (100.0 * 365.25 * 86400.0);

    csv_file << p.name << "," << std::fixed << std::setprecision(4) << p.a_au << "," << p.e << "," << std::setprecision(3) << arcsec_century << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_024/mercury_gr_rates.csv" << std::endl;
  return 0;
}
