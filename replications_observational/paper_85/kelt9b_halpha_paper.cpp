// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #85: KELT-9b Ultra-Hot Thermosphere & H-Alpha Absorption Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #85: KELT-9b ULTRA-HOT THERMOSPHERE & H-ALPHA" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::KELT9bUltraHotThermosphereModel model;

  const double h_scale_km = model.scale_height_km();      // ~ 8314 km
  const double r_ratio = model.thermosphere_radius_ratio(); // ~ 1.32 R_p
  const double excess_depth_pct = model.halpha_excess_depth_percent(); // ~ 1.15%

  std::cout << "KELT-9b Thermospheric Scale Height: " << h_scale_km << " km" << std::endl;
  std::cout << "Thermospheric Extent Radius: " << r_ratio << " R_p" << std::endl;
  std::cout << "H-Alpha Excess Absorption Depth: " << excess_depth_pct << " %" << std::endl;

  // Track High-Resolution Transmission Spectrum around H-alpha line (linear velocity scale -80 to +80 km/s):
  // Doppler broadened Voigt / Gaussian core at T ~ 10,000 K (v_thermal ~ 12.8 km/s) + planetary rotation & day-to-night wind (-4.0 km/s)
  std::ofstream out("replications_observational/paper_85/kelt9b_halpha_transmission.csv");
  out << "doppler_velocity_km_s,relative_transmission_depth_pct,continuum_baseline_pct\n";

  const double v_wind = -4.0; // km/s net day-to-night thermospheric wind blueshift
  const double v_fwhm = 24.5; // km/s resolved line FWHM

  for (double v_kms = -80.0; v_kms <= 80.0; v_kms += 1.0) {
    double v_diff = v_kms - v_wind;
    double profile = excess_depth_pct * std::exp(-std::pow(v_diff / (v_fwhm / 1.665), 2.0));

    out << v_kms << "," << profile << "," << 0.0 << "\n";
  }
  out.close();

  std::cout << "Generated KELT-9b H-Alpha Transmission Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
