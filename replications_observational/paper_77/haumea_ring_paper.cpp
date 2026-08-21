// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #77: Haumea Ring & Jacobi Ellipsoid Occultation Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #77: HAUMEA TRIAXIAL ELLIPSOID & RING OCCULTATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::HaumeaEllipsoidRingModel model;
  std::cout << "Haumea 3:1 Resonance Ring Radius: " << model.ring_3to1_resonance_radius_km() << " km" << std::endl;

  // Shadow velocity across Earth ~ 26.5 km/s (Ortiz et al. 2017 Nature)
  const double v_shadow_km_s = 26.5;



  // Occultation chord light curve simulation:
  // Ring width ~ 70 km, optical depth tau_ring ~ 0.5 (flux drop ~ 50%)
  // Main body width along chord ~ 1160 km (duration ~ 44 seconds)
  std::ofstream out("replications_observational/paper_77/haumea_occultation_lightcurve.csv");
  out << "time_relative_sec,apparent_relative_flux,stellar_distance_km\n";

  for (double t_s = -80.0; t_s <= +80.0; t_s += 1.0) {
    double x_km = t_s * v_shadow_km_s; // -2120 to +2120 km
    double abs_x = std::abs(x_km);

    double flux = 1.0;

    // Ring occultation: located at |x| ~ 2287 * cos(ring_orientation) ~ 1550 km
    if (std::abs(abs_x - 1550.0) <= 35.0) {
      flux = 0.52;  // Ring absorption dip
    } else if (abs_x <= 580.0) {
      flux = 0.00;  // Complete star blockage
    }


    out << t_s << "," << flux << "," << x_km << "\n";
  }
  out.close();

  std::cout << "Generated Haumea Ring Occultation Light Curve Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
