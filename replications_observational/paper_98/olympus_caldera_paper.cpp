// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #98: Mars Olympus Mons Caldera Subsidence Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #98: MARS OLYMPUS MONS CALDERA SUBSIDENCE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::MarsOlympusMonsCalderaModel model;

  const double h_summit = model.volcano_summit_elevation_km();    // 21.287 km (Datum)
  const double d_caldera = model.caldera_complex_diameter_km();   // 80.0 km
  const double max_depth = model.maximum_caldera_depth_km();      // 3.2 km
  const double d_magma = model.magma_chamber_depth_km();          // 15.0 km
  const double d_rigidity = model.flexural_rigidity_n_m();        // 2.0e24 N*m

  std::cout << "Olympus Mons Summit Peak Elevation: " << h_summit << " km" << std::endl;
  std::cout << "Nested Caldera Complex Diameter: " << d_caldera << " km" << std::endl;
  std::cout << "Maximum Caldera Floor Subsidence: " << max_depth << " km" << std::endl;
  std::cout << "Magma Chamber Reservoir Depth: " << d_magma << " km" << std::endl;
  std::cout << "Lithospheric Flexural Rigidity D: " << d_rigidity << " N*m" << std::endl;

  // Track Caldera Complex Topography across Radial Distance x = -60 km to +60 km (linear scale):
  // Summit rim at |x| ~ 40 km, Nested caldera collapse floor at |x| < 40 km
  std::ofstream out("replications_observational/paper_98/olympus_caldera_topography.csv");
  out << "distance_from_caldera_center_km,elevation_above_datum_km,piston_subsidence_depth_km\n";

  const double r_rim = d_caldera / 2.0; // 40 km

  for (double x_km = -60.0; x_km <= 60.0; x_km += 1.0) {
    double abs_x = std::abs(x_km);
    double elev_km = 0.0;
    double sub_km = 0.0;

    if (abs_x >= r_rim) {
      // Outer volcano flank slope (~ 5 deg slope)
      elev_km = h_summit - 0.087 * (abs_x - r_rim);
      sub_km = 0.0;
    } else {
      // Stepped nested caldera subsidence profile (6 coalesced collapse episodes)
      double f_step1 = (1.0 / (1.0 + std::exp((abs_x - 38.0) / 1.0)));
      double f_step2 = (1.0 / (1.0 + std::exp((abs_x - 26.0) / 1.0)));
      double f_step3 = (1.0 / (1.0 + std::exp((abs_x - 14.0) / 1.0)));

      sub_km = 1.2 * f_step1 + 1.1 * f_step2 + 0.9 * f_step3; // Total ~ 3.2 km
      elev_km = h_summit - sub_km;
    }

    out << x_km << "," << elev_km << "," << sub_km << "\n";
  }
  out.close();

  std::cout << "Generated Olympus Mons Caldera Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
