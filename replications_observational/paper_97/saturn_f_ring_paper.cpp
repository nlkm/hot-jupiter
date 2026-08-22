// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #97: Saturn F-Ring Prometheus Perturbations Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #97: SATURN F-RING PROMETHEUS STREAMER-CHANNELS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::SaturnFRingPrometheusModel model;

  const double a_ring = model.ring_core_semimajor_axis_km();     // 140220 km
  const double a_prom = model.prometheus_semimajor_axis_km();    // 139380 km
  const double a_pand = model.pandora_semimajor_axis_km();       // 141720 km
  const double chan_depth = model.streamer_channel_depth_km();   // ~ 50 km
  const double t_syn = model.synodic_encounter_period_hours();    // ~ 68.0 hours

  std::cout << "F-Ring Core Semi-Major Axis: " << a_ring << " km" << std::endl;
  std::cout << "Inner Shepherd Prometheus Semi-Major Axis: " << a_prom << " km" << std::endl;
  std::cout << "Outer Shepherd Pandora Semi-Major Axis: " << a_pand << " km" << std::endl;
  std::cout << "Streamer-Channel Radial Perturbation Depth: " << chan_depth << " km" << std::endl;
  std::cout << "Prometheus Synodic Encounter Period: " << t_syn << " hours" << std::endl;

  // Track Radial Radial Perturbation across Orbital Longitude delta_theta = -180 deg to +180 deg (linear scale):
  // Gravitational impulse from Prometheus at conjunction creates ~ -50 km radial channel and +35 km trailing streamer
  std::ofstream out("replications_observational/paper_97/f_ring_radial_profile.csv");
  out << "orbital_longitude_deg,radial_displacement_km,normal_optical_depth\n";

  for (double deg = -180.0; deg <= 180.0; deg += 2.0) {
    // Asymmetric streamer-channel profile
    double dr_km = -chan_depth * std::exp(-std::pow((deg - 15.0) / 25.0, 2.0))
                   + 35.0 * std::exp(-std::pow((deg - 65.0) / 35.0, 2.0));
    
    // Core baseline optical depth tau ~ 0.12 with depletion in channel and enhancement in streamer
    double tau = 0.12 * (1.0 + dr_km / 60.0);
    if (tau < 0.01) tau = 0.01;

    out << deg << "," << dr_km << "," << tau << "\n";
  }
  out.close();

  std::cout << "Generated Saturn F-Ring Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
