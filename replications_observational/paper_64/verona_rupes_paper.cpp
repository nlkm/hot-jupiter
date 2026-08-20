// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #64: Miranda Verona Rupes Extensional Tectonics Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #64: MIRANDA VERONA RUPES EXTENSIONAL TECTONICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::MirandaVeronaRupesModel model;

  const double h_cliff = model.cliff_vertical_relief_km(); // 20.0 km
  const double g_miranda = model.surface_gravity_m_s2();   // 0.079 m/s^2
  const double theta_deg = model.dip_angle_degrees();      // 65.0 deg
  const double theta_rad = theta_deg * M_PI / 180.0;

  // Extensional normal fault scarp profile:
  // z(x) = H * (1 / (1 + exp(-x / w_scarp)))
  // where fault throw H = 20 km, width w_scarp = H / tan(theta) ~ 9.3 km
  const double w_scarp = h_cliff / std::tan(theta_rad);

  std::ofstream out("replications_observational/paper_64/verona_rupes_topography.csv");
  out << "x_cross_scarp_km,elevation_km,scarp_slope_deg,freefall_time_s\n";

  for (double x = -20.0; x <= 20.0; x += 0.5) {
    double z_elev = h_cliff / (1.0 + std::exp(-x / (w_scarp * 0.35)));
    // Scarp slope angle
    double dz_dx = (h_cliff / (w_scarp * 0.35)) * std::exp(-x / (w_scarp * 0.35)) / std::pow(1.0 + std::exp(-x / (w_scarp * 0.35)), 2.0);
    double slope_deg = std::atan(dz_dx) * 180.0 / M_PI;

    // Free-fall time from cliff crest to current height: t = sqrt(2 * (H - z) / g)
    double delta_drop_m = (h_cliff - z_elev) * 1.0e3;
    double t_fall = (delta_drop_m > 0.0) ? std::sqrt(2.0 * delta_drop_m / g_miranda) : 0.0;

    out << x << "," << z_elev << "," << slope_deg << "," << t_fall << "\n";
  }
  out.close();

  std::cout << "Generated Miranda Verona Rupes Topographic Profile Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
