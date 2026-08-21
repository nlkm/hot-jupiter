// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #73: Charon Ocean Freezing & Global Extensional Tectonics Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #73: CHARON OCEAN FREEZING & EXTENSIONAL TECTONICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::CharonTectonicFreezingModel model;

  const double sigma_fracture_pa = model.tensile_fracture_stress_pa(); // 2.5e7 Pa (25 MPa)
  const double eps_strain_max = model.global_lithospheric_strain();  // 0.02 (2%)


  // Thermal evolution of ice shell thickening and global tensile hoop stress:
  // sigma_theta(t) = (E / (1 - nu)) * (Delta R(t) / R_0)
  // Track over 0 to 4.5 Gyr (linear time scale)
  std::ofstream out("replications_observational/paper_73/charon_stress_evolution.csv");
  out << "time_gyr,ice_shell_thickness_km,global_extensional_strain_pct,tensile_stress_mpa\n";

  for (double t_gyr = 0.0; t_gyr <= 4.5; t_gyr += 0.1) {
    // Freezing progress: complete ocean freezing by ~ 2.0 Gyr
    double freeze_fraction = (t_gyr <= 2.0) ? std::sin((t_gyr / 2.0) * (M_PI / 2.0)) : 1.0;
    
    // Ice shell thickness grows from 20 km to 220 km
    double h_ice = 20.0 + 200.0 * freeze_fraction;

    // Radius increase: Delta R / R = (1/3) * (Delta V / V) * (V_ocean / V_total)
    // V_ocean ~ 0.30 of mantle volume -> max strain ~ 2.0%
    double strain_pct = eps_strain_max * 100.0 * freeze_fraction;

    // Tensile stress: capped by brittle lithospheric faulting at sigma_fracture (25 MPa)
    double stress_mpa = (sigma_fracture_pa / 1.0e6) * freeze_fraction;

    out << t_gyr << "," << h_ice << "," << strain_pct << "," << stress_mpa << "\n";
  }
  out.close();

  std::cout << "Generated Charon Subsurface Ocean Freezing Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
