// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #105: Jupiter Great Red Spot Deep Root Dynamics Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #105: JUPITER GREAT RED SPOT DEEP ROOT DYNAMICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::JupiterGreatRedSpotJunoModel model;

  const double l_grs = model.grs_length_km();                // ~ 16000 km
  const double w_grs = model.grs_width_km();                 // ~ 12000 km
  const double v_max = model.max_wind_speed_m_s();           // ~ 120.0 m/s
  const double z_depth = model.vertical_root_depth_km();     // ~ 300.0 km
  const double rho_contrast = model.deep_density_contrast(); // ~ 0.0015

  std::cout << "GRS Longitudinal Width: " << l_grs << " km" << std::endl;
  std::cout << "GRS Latitudinal Span: " << w_grs << " km" << std::endl;
  std::cout << "Maximum Anticyclonic Jet Collar Wind Speed: " << v_max << " m/s (" << (v_max * 3.6) << " km/h)" << std::endl;
  std::cout << "Vertical Root Depth (Juno MWR / Gravity): " << z_depth << " km" << std::endl;
  std::cout << "Deep Density Contrast: " << rho_contrast << std::endl;

  // Track Tangential Wind Velocity vs Vertical Depth z = 0 to 500 km (linear depth scale):
  // z = 0 km is 1-bar cloud top, z ~ 300 km is the deep transition base
  std::ofstream out("replications_observational/paper_105/grs_depth_velocity_profile.csv");
  out << "depth_km,wind_velocity_m_s,microwave_brightness_temp_anomaly_k\n";

  for (double z_km = 0.0; z_km <= 500.0; z_km += 10.0) {
    // Vertical velocity attenuation profile based on thermal wind equation:
    // Strong winds persist to ~ 300 km, decaying rapidly below 350 km
    double f_decay = 1.0 / (1.0 + std::exp((z_km - z_depth) / 35.0));
    double v_z = v_max * f_decay;

    // Microwave brightness temperature anomaly (Juno MWR 600 MHz to 22 GHz inversion)
    // Positive anomaly in deep roots due to ammonia gas depletion and downwelling
    double dt_mwr = 3.5 * std::exp(-std::pow((z_km - 150.0) / 100.0, 2.0));

    out << z_km << "," << v_z << "," << dt_mwr << "\n";
  }
  out.close();

  std::cout << "Generated Jupiter Great Red Spot Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
