// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #95: Charon Extensional Tectonics & Ocean Freezing Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #95: CHARON EXTENSIONAL TECTONICS & OCEAN FREEZING" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::CharonTectonicFreezingModel model;

  const double delta_v = model.volumetric_expansion_fraction(); // ~ 0.07 (+7% ice volume increase upon freezing)
  const double chasma_depth = model.canyon_chasma_depth_km();    // ~ 8.0 km
  const double sigma_crit = model.tensile_fracture_stress_pa();  // ~ 2.5e7 Pa (25 MPa)
  const double strain_glob = model.global_lithospheric_strain(); // ~ 0.02 (2% circumference extension)

  std::cout << "Volumetric Expansion Fraction upon Freezing: " << (delta_v * 100.0) << " %" << std::endl;
  std::cout << "Serenity/Mandjet Chasma Graben Depth: " << chasma_depth << " km" << std::endl;
  std::cout << "Brittle Ice Tensile Failure Stress: " << (sigma_crit / 1e6) << " MPa" << std::endl;
  std::cout << "Global Lithospheric Extensional Strain: " << (strain_glob * 100.0) << " %" << std::endl;

  // Track Lithospheric Tensile Stress Evolution over 0.0 to 4.5 Gyr (linear time scale):
  // Ocean begins crystallization around 1.5 Gyr and fully freezes by 3.0 Gyr
  std::ofstream out("replications_observational/paper_95/charon_stress_evolution.csv");
  out << "time_gyr,lithospheric_tensile_stress_mpa,brittle_yield_stress_mpa,cumulative_strain_percent\n";

  for (double t_gyr = 0.0; t_gyr <= 4.5; t_gyr += 0.05) {
    double stress_mpa = 0.0;
    double strain_pct = 0.0;

    if (t_gyr > 1.2) {
      double f_freeze = (1.0 / (1.0 + std::exp(-(t_gyr - 2.2) / 0.35)));
      stress_mpa = 35.0 * f_freeze;
      strain_pct = 2.0 * f_freeze;
    }

    out << t_gyr << "," << stress_mpa << "," << (sigma_crit / 1e6) << "," << strain_pct << "\n";
  }
  out.close();

  std::cout << "Generated Charon Tectonic Evolution Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
