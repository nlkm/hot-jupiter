// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #96: Miranda Verona Rupes Extensional Tectonics Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #96: MIRANDA VERONA RUPES EXTENSIONAL TECTONICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::MirandaVeronaRupesModel model;

  const double h_cliff_km = model.cliff_vertical_relief_km(); // ~ 20.0 km
  const double g_miranda = model.surface_gravity_m_s2();      // ~ 0.079 m/s^2
  const double dip_deg = model.dip_angle_degrees();            // ~ 65.0 deg
  const double t_fall_min = model.fall_duration_freefall_minutes(); // ~ 12.0 min

  std::cout << "Verona Rupes Vertical Relief: " << h_cliff_km << " km (Tallest Cliff in Solar System)" << std::endl;
  std::cout << "Miranda Surface Gravity: " << g_miranda << " m/s^2" << std::endl;
  std::cout << "Normal Fault Dip Angle: " << dip_deg << " deg" << std::endl;
  std::cout << "Freefall Duration from Cliff Edge: " << t_fall_min << " minutes" << std::endl;

  // Track Freefall Kinematics down 20 km cliff over 0.0 to 12.0 minutes (linear time scale):
  // z(t) = 0.5 * g * t^2, v(t) = g * t
  std::ofstream out("replications_observational/paper_96/miranda_freefall_evolution.csv");
  out << "time_minutes,freefall_distance_km,freefall_velocity_km_h,remaining_altitude_km\n";

  const double t_total_s = std::sqrt(2.0 * (h_cliff_km * 1.0e3) / g_miranda); // ~ 712 s (11.87 min)

  for (double t_min = 0.0; t_min <= 12.0; t_min += 0.2) {
    double t_s = t_min * 60.0;
    if (t_s > t_total_s) t_s = t_total_s;

    double dist_m = 0.5 * g_miranda * t_s * t_s;
    double dist_km = dist_m / 1000.0;
    double vel_ms = g_miranda * t_s;
    double vel_km_h = vel_ms * 3.6;
    double alt_km = h_cliff_km - dist_km;

    out << t_min << "," << dist_km << "," << vel_km_h << "," << alt_km << "\n";
  }
  out.close();

  std::cout << "Generated Miranda Verona Rupes Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
