// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #72: Mars Olympus Mons Caldera Subsidence Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #72: MARS OLYMPUS MONS CALDERA SUBSIDENCE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::MarsOlympusMonsCalderaModel model;

  const double z_summit = model.volcano_summit_elevation_km();   // 21.287 km
  const double h_depth = model.maximum_caldera_depth_km();       // 3.2 km


  // Multi-ring nested piston collapse profile:
  // z(r) = z_summit - h_depth * [ 0.40 * (1 / (1 + exp((|r| - 35)/2))) + 0.35 * (1 / (1 + exp((|r| - 25)/2))) + 0.25 * (1 / (1 + exp((|r| - 12)/2))) ]
  std::ofstream out("replications_observational/paper_72/olympus_caldera_topography.csv");
  out << "radial_distance_km,elevation_datum_km,subsidence_depth_km\n";

  for (double r = -60.0; r <= +60.0; r += 1.0) {
    double abs_r = std::abs(r);
    double step1 = 1.0 / (1.0 + std::exp((abs_r - 38.0) / 1.5));
    double step2 = 1.0 / (1.0 + std::exp((abs_r - 26.0) / 1.5));
    double step3 = 1.0 / (1.0 + std::exp((abs_r - 14.0) / 1.5));
    
    double sub_frac = 0.40 * step1 + 0.35 * step2 + 0.25 * step3;
    double depth_km = h_depth * sub_frac;
    double z_profile = z_summit - depth_km;

    out << r << "," << z_profile << "," << depth_km << "\n";
  }
  out.close();

  std::cout << "Generated Olympus Mons Caldera Topographic Profile Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
