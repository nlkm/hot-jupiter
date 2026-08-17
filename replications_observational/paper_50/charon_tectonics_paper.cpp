// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #50: Pluto Moon Charon Ocean Freezing Extensional Tectonics

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #50: CHARON OCEAN FREEZING TECTONICS ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::CharonTectonicFreezingModel model;
  double delta_v = model.volumetric_expansion_fraction();
  double d_chasma = model.canyon_chasma_depth_km();
  double sigma_crit = model.tensile_fracture_stress_pa();
  double eps_strain = model.global_lithospheric_strain();

  // New Horizons LORRI & MVIC observations (Beyer 2017 Icarus, Moore 2016 Science)
  double obs_deltav = 0.07;       // Volumetric expansion on freezing liquid water to Ice I (+7%)
  double obs_dchasma = 8.0;       // km Serenity Chasma relief depth (7.0 - 9.0 km)
  double obs_sigmacrit = 2.5e7;   // Pa critical tensile fracture strength (~ 25 MPa)
  double obs_strain = 0.02;       // Lithospheric extensional strain (1 - 3%)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Water Freezing Volumetric Expansion = " << delta_v * 100.0 << " % (Observed: " << obs_deltav * 100.0 << " %)" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Serenity Chasma Graben Depth        = " << d_chasma << " km (Observed: " << obs_dchasma << " km)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Ice Lithosphere Tensile Strength    = " << sigma_crit << " Pa (Observed: " << obs_sigmacrit << " Pa)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Global Extensional Strain           = " << eps_strain * 100.0 << " % (Observed: " << obs_strain * 100.0 << " %)" << std::endl;
  std::cout << "Relative Graben Depth Discrepancy   = " << std::abs((d_chasma - obs_dchasma) / obs_dchasma) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
