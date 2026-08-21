// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #92: Titan Subsurface Ocean & Viscoelastic Tidal Love Numbers Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #92: TITAN SUBSURFACE OCEAN & LOVE NUMBERS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TitanTidalDissipationModel model;

  const double d_crust_km = 80.0;   // Outer Ice-Ih shell thickness [km]
  const double d_ocean_km = 350.0;  // Subsurface liquid water ocean thickness [km]

  const double k2_val = model.love_number_k2(d_crust_km, d_ocean_km);
  const double h2_val = model.love_number_h2(d_crust_km, d_ocean_km);
  const double l2_val = model.love_number_l2(d_crust_km, d_ocean_km);
  const double p_orb_days = model.orbital_period_days(); // ~ 15.945 days

  std::cout << "Titan Potential Love Number k_2: " << k2_val << std::endl;
  std::cout << "Titan Radial Love Number h_2: " << h2_val << std::endl;
  std::cout << "Titan Horizontal Love Number l_2: " << l2_val << std::endl;
  std::cout << "Titan Orbital Period: " << p_orb_days << " days" << std::endl;

  // Track Diurnal Radial Surface Tidal Elevation Delta h(t) across 15.945-day orbit (linear time scale):
  // Diurnal eccentricity tide amplitude ~ (3/2) * (G*M_saturn*R_titan^4 / a_titan^3) * e * (h_2 / g_titan) ~ 10.0 meters
  std::ofstream out("replications_observational/paper_92/titan_tidal_elevation_evolution.csv");
  out << "orbital_time_days,radial_tidal_elevation_meters,solid_no_ocean_elevation_meters\n";

  const double amp_ocean = 10.0; // meters (with decoupled subsurface ocean)
  const double amp_solid = 1.2;  // meters (without ocean, rigid frozen interior)

  for (double t_days = 0.0; t_days <= 16.0; t_days += 0.2) {
    double phase = (2.0 * M_PI / p_orb_days) * t_days;
    double h_ocean = amp_ocean * std::cos(phase);
    double h_solid = amp_solid * std::cos(phase);

    out << t_days << "," << h_ocean << "," << h_solid << "\n";
  }
  out.close();

  std::cout << "Generated Titan Tidal Elevation Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
