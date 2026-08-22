// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #68: LHS 3844b Bare Rock Thermal Phase Curve Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #68: LHS 3844b BARE ROCK THERMAL PHASE CURVE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::LHS3844bBareRockModel model;
  const double albedo = model.basalt_surface_albedo(); // 0.05
  const double peak_contrast_ppm = 380.0 * (1.0 - albedo) / 0.95;
  std::ofstream out("replications_observational/paper_68/lhs3844b_phase_curve.csv");

  out << "orbital_phase,planet_flux_contrast_ppm,sigma_err_ppm\n";

  for (double phi = -0.50; phi <= +0.50; phi += 0.02) {
    // Phase angle alpha = 2 * pi * phi
    double alpha = 2.0 * M_PI * phi;

    // Bare rock phase variation: Lambertian sphere with zero atmosphere
    // F_p(alpha) / F_p(0) = (sin(alpha) + (pi - |alpha|)*cos(alpha)) / pi
    double lambert_factor = 0.0;
    if (std::abs(alpha) < M_PI) {
      double abs_a = std::abs(alpha);
      lambert_factor = (std::sin(abs_a) + (M_PI - abs_a) * std::cos(abs_a)) / M_PI;
    }

    double contrast_ppm = peak_contrast_ppm * lambert_factor;
    double sigma_err = 22.0; // Spitzer IRAC Channel 2 photometric precision

    out << phi << "," << contrast_ppm << "," << sigma_err << "\n";
  }
  out.close();

  std::cout << "Generated LHS 3844b Spitzer Thermal Phase Curve Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
