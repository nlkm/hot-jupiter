// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #51: Saturn's Moon Tethys Ithaca Chasma Graben Extension Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/cryosphere_fracture_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #51: TETHYS ITHACA CHASMA EXTENSIONAL TECTONICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // Tethys: R = 531.1 km, g = 0.145 m/s^2, density = 984 kg/m^3, shear modulus = 3.2 GPa, tensile = 2.0 MPa
  hot_jupiter::CryosphereFractureDiscoveryEngine tethys(531.1, 0.145, 984.0, 3.2, 2.0);

  // Evolve cryosphere freezing across 150 Myr
  auto hist = tethys.EvolveFreezingCryosphere(25.0, 60.0, 110.0, 0.08, 150.0, 1.0);


  std::ofstream out("replications_observational/paper_51/tethys_graben_track.csv");
  out << "time_myr,ocean_thick_km,ice_thick_km,overpressure_mpa,hoop_stress_mpa,is_fractured\n";
  for (const auto& s : hist) {
    out << s.time_myr << "," << s.ocean_thickness_km << "," << s.ice_shell_thickness_km << ","
        << s.ocean_overpressure_mpa << "," << s.surface_hoop_stress_mpa << ","
        << (s.is_fractured ? 1 : 0) << "\n";
  }
  out.close();


  std::cout << "Generated Tethys Ithaca Chasma Graben simulation data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
