// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #117: Mars Subsurface Glacial Scarp & SHARAD Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #117: MARS SUBSURFACE GLACIAL SCARP & SHARAD STRATIGRAPHY" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::MarsSubsurfaceGlacialScarpModel model;

  const double h_ice = model.exposed_ice_thickness_m();       // ~ 130.0 m
  const double z_lag = model.dry_lag_thickness_m();          // ~ 1.5 m
  const double purity_pct = model.ice_volume_purity_percent(); // ~ 95.0 %
  const double eps_r = model.dielectric_permittivity();        // ~ 3.15 (Pure ice)
  const double scarp_deg = model.scarp_slope_degrees();        // ~ 45.0 deg

  std::cout << "Exposed Pure Ice Sheet Thickness: " << h_ice << " m" << std::endl;
  std::cout << "Dry Protective Regolith Lag: " << z_lag << " m" << std::endl;
  std::cout << "Ice Volumetric Purity: " << purity_pct << " %" << std::endl;
  std::cout << "SHARAD Dielectric Permittivity: " << eps_r << std::endl;
  std::cout << "Scarp Slope Angle: " << scarp_deg << " deg" << std::endl;

  // Track Ice Volume Fraction and Radar Permittivity vs Vertical Depth z from 0 to 160 m (linear depth scale):
  std::ofstream out("replications_observational/paper_117/mars_glacier_depth_profile.csv");
  out << "depth_m,ice_volume_fraction,dielectric_permittivity,radar_reflectivity_db\n";

  for (double z_m = 0.0; z_m <= 160.0; z_m += 1.0) {
    double f_ice = 0.0;
    double eps = 5.5; // Basaltic regolith default permittivity
    double refl_db = -40.0;

    if (z_m < z_lag) {
      // Dry lag layer
      f_ice = 0.0;
      eps = 5.5;
      refl_db = -35.0;
    } else if (z_m <= z_lag + h_ice) {
      // Massive ice sheet with cyclic obliquity dust layers (wavelength ~ 15 m)
      double depth_in_ice = z_m - z_lag;
      double dust_mod = 0.04 * std::sin(2.0 * M_PI * depth_in_ice / 15.0);
      f_ice = 0.95 + dust_mod;
      eps = eps_r + 0.15 * std::sin(2.0 * M_PI * depth_in_ice / 15.0);

      // Radar reflections at internal stratigraphic boundaries
      refl_db = -18.0 + 6.0 * std::sin(2.0 * M_PI * depth_in_ice / 15.0);
    } else {
      // Basal bedrock interface below 131.5 m
      f_ice = 0.0;
      eps = 7.0; // Basal rocky bedrock
      refl_db = -10.0; // Strong basal radar reflection
    }

    out << z_m << "," << f_ice << "," << eps << "," << refl_db << "\n";
  }
  out.close();

  std::cout << "Generated Mars Glacial Scarp Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
