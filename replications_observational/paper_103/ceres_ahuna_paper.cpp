// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #103: Ceres Ahuna Mons Cryovolcanic Dome Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #103: CERES AHUNA MONS CRYOVOLCANIC DOME EXTRUSION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::CeresAhunaMonsCryovolcanismModel model;

  const double h_dome = model.dome_height_km();                    // ~ 4.0 km
  const double d_base = model.base_diameter_km();                  // ~ 20.0 km
  const double tau_yield = model.brine_yield_stress_pa();          // ~ 1.5e4 Pa
  const double frac_carbonate = model.sodium_carbonate_mass_fraction(); // ~ 0.20 (20%)

  std::cout << "Ahuna Mons Summit Peak Relief: " << h_dome << " km" << std::endl;
  std::cout << "Dome Basal Footprint Diameter: " << d_base << " km" << std::endl;
  std::cout << "Cryomagma Brine Slurry Yield Stress: " << tau_yield << " Pa" << std::endl;
  std::cout << "Sodium Carbonate (Na2CO3) Fraction: " << (frac_carbonate * 100.0) << " %" << std::endl;

  // Track Topographic Profile across Radial Distance r = -15 km to +15 km (linear spatial scale):
  // Peak summit at r = 0 km, base boundary at |r| = 10 km (radius = 10 km)
  std::ofstream out("replications_observational/paper_103/ahuna_mons_topography.csv");
  out << "radial_distance_km,elevation_km,flank_slope_deg,relaxation_elevation_200myr_km\n";

  const double r_base_km = d_base / 2.0; // 10.0 km

  for (double r_km = -15.0; r_km <= 15.0; r_km += 0.25) {
    double abs_r = std::abs(r_km);
    double elev_km = 0.0;
    double slope_deg = 0.0;
    double elev_relaxed_km = 0.0;

    if (abs_r < r_base_km) {
      // Bingham plastic dome profile: h(r) = H_0 * (1 - (r/R)^2)^0.7
      double norm_r = abs_r / r_base_km;
      elev_km = h_dome * std::pow(1.0 - std::pow(norm_r, 1.8), 0.75);
      slope_deg = std::atan(std::abs(h_dome * 1.35 * std::pow(norm_r, 0.8) / r_base_km)) * (180.0 / M_PI);

      // Viscoelastic relaxation after 200 Myr (flattening of summit and basal spreading)
      elev_relaxed_km = elev_km * std::exp(-0.15) - 0.10 * (1.0 - norm_r);
      if (elev_relaxed_km < 0.0) elev_relaxed_km = 0.0;
    } else {
      elev_km = 0.0;
      slope_deg = 1.5; // Surrounding cratered terrain
      elev_relaxed_km = 0.0;
    }

    out << r_km << "," << elev_km << "," << slope_deg << "," << elev_relaxed_km << "\n";
  }
  out.close();

  std::cout << "Generated Ceres Ahuna Mons Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
