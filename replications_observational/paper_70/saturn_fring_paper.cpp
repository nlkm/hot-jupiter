// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #70: Saturn F-Ring Prometheus Shepherd Dynamics Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #70: SATURN F-RING PROMETHEUS SHEPHERD DYNAMICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::SaturnFRingPrometheusModel model;

  const double h_channel_km = model.streamer_channel_depth_km();  // 50.0 km


  // Radial perturbation amplitude across orbital longitude:
  // Delta r(theta) = - h_channel * exp(-(theta / w_streamer)^2) * sin(k * theta)
  std::ofstream out("replications_observational/paper_70/saturn_fring_streamer_profile.csv");
  out << "orbital_longitude_deg,radial_displacement_km,optical_depth_tau\n";

  for (double lon_deg = -30.0; lon_deg <= +30.0; lon_deg += 0.5) {
    double lon_rad = lon_deg * M_PI / 180.0;
    // Streamer channel formation by Prometheus gravitational encounter
    double delta_r = -h_channel_km * std::exp(-std::pow(lon_deg / 8.0, 2.0)) * std::cos(lon_rad * 4.0);
    // Unperturbed core optical depth ~ 0.8, dropping in channel to ~ 0.1, clumping at edge to ~ 1.4
    double tau = 0.80 + 0.60 * (delta_r / h_channel_km);
    if (tau < 0.05) tau = 0.05;

    out << lon_deg << "," << delta_r << "," << tau << "\n";
  }
  out.close();

  std::cout << "Generated Saturn F-Ring Prometheus Streamer Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
