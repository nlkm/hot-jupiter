// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #63: GJ 1214b Aerosol Hazes & Thermal Phase Curve Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #63: GJ 1214b SUPER-EARTH AEROSOL HAZES & PHASE CURVE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::GJ1214bAerosolHazeModel model;

  const double t_day = model.dayside_temp_k();          // 553 K
  const double t_night = model.nightside_temp_k();      // 437 K
  const double r_planet_re = model.planet_radius_rearth(); // 2.74 R_earth
  const double r_star_rsun = 0.216;                     // M4.5V dwarf host
  const double t_star_k = 3030.0;                       // Stellar effective temperature

  // Calculate full-orbit thermal phase curve in JWST MIRI LRS band (5.0 - 12.0 um)
  // Contrast F_p / F_star across orbital phase phi (-0.5 to +0.5)
  std::ofstream out("replications_observational/paper_63/gj1214b_phase_curve.csv");
  out << "orbital_phase,planet_brightness_temp_k,flux_contrast_ppm,sigma_err_ppm\n";

  for (double phi = -0.50; phi <= +0.50; phi += 0.02) {
    // Thermal emission phase variation: T(phi) = T_mid + Delta_T * cos(2*pi*(phi - delta_east))
    double delta_east = 0.05; // 18 degree eastward hotspot offset
    double t_b = 0.5 * (t_day + t_night) + 0.5 * (t_day - t_night) * std::cos(2.0 * M_PI * (phi - delta_east));

    // Mid-IR flux contrast scaling ~ (R_p / R_star)^2 * (B_lam(T_p) / B_lam(T_star))
    double r_ratio_sq = std::pow((r_planet_re * 0.009168) / r_star_rsun, 2.0); // ~ 0.0135 (1.35%)
    double b_ratio = (t_b / t_star_k); // Rayleigh-Jeans limit approximation
    double contrast_ppm = r_ratio_sq * b_ratio * 1.0e6;
    double sigma_err = 12.0; // JWST MIRI phase curve precision

    out << phi << "," << t_b << "," << contrast_ppm << "," << sigma_err << "\n";
  }
  out.close();

  std::cout << "Generated GJ 1214b JWST MIRI Thermal Phase Curve Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
