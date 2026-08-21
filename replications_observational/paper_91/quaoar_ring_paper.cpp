// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #91: Quaoar Dense Ring System Beyond the Roche Limit Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #91: QUAOAR DENSE RING SYSTEM BEYOND ROCHE LIMIT" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::QuaoarRingSystemModel model;

  const double r_roche = model.classical_roche_limit_km();       // ~ 1720 km (3.1 R_Q)
  const double r_q1r = model.q1r_ring_radius_km();               // 4100 km (7.4 R_Q)
  const double r_q2r = model.q2r_ring_radius_km();               // 2520 km (4.5 R_Q)
  const double r_res_6_1 = model.spin_orbit_resonance_radius_km();// ~ 4190 km (6:1 spin-orbit resonance)

  std::cout << "Quaoar Classical Roche Limit: " << r_roche << " km" << std::endl;
  std::cout << "Primary Ring Q1R Radius: " << r_q1r << " km (Ratio to Roche: " << (r_q1r / r_roche) << ")" << std::endl;
  std::cout << "Secondary Ring Q2R Radius: " << r_q2r << " km" << std::endl;
  std::cout << "6:1 Spin-Orbit Resonance Radius: " << r_res_6_1 << " km" << std::endl;

  // Track Stellar Occultation Light Curve across radial distance r = 3950 to 4250 km (linear scale):
  std::ofstream out("replications_observational/paper_91/quaoar_ring_occultation.csv");
  out << "radial_distance_km,optical_depth_tau,relative_occultation_flux\n";

  for (double r_km = 3950.0; r_km <= 4250.0; r_km += 2.0) {
    double tau = model.occultation_optical_depth(r_km);
    double flux = std::exp(-tau);

    out << r_km << "," << tau << "," << flux << "\n";
  }
  out.close();

  std::cout << "Generated Quaoar Ring Occultation Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
