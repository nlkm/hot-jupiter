// Solver for Paper #55: Exoplanetary Transmission Spectroscopy & Rayleigh Scattering Slope (Seager & Sasselov 2000, Charbonneau et al. 2002)
// Evaluates effective transit radius R_eff(lambda) = R_0 + H * ln(tau_0 * (sigma(lambda)/sigma_0)) and Rayleigh slope dR_eff / d(ln lambda) = -4 * H.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Seager & Sasselov (2000) Transmission Spectroscopy Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_055/transmission_spectrum_slopes.csv");
  csv_file << "wavelength_um,transit_depth_ppm,delta_r_over_rstar_ppm\n";

  double r_star = hot_jupiter::R_SUN;
  double r_planet = hot_jupiter::R_JUP;
  double scale_height_h_km = 500.0;  // 500 km scale height H = k_B * T / (mu * g)

  double h_m = scale_height_h_km * 1000.0;
  double baseline_depth_ppm = (r_planet * r_planet) / (r_star * r_star) * 1.0e6;

  // Wavelengths from 0.3 um (UV) to 1.0 um (NIR)
  for (double lambda_um = 0.3; lambda_um <= 1.0; lambda_um += 0.05) {
    // Rayleigh scattering cross section sigma ~ lambda^-4 -> delta R = -4 * H * ln(lambda / lambda_ref)
    double delta_r_m = -4.0 * h_m * std::log(lambda_um / 0.5);
    double r_eff_m = r_planet + delta_r_m;
    double depth_ppm = (r_eff_m * r_eff_m) / (r_star * r_star) * 1.0e6;

    csv_file << std::fixed << std::setprecision(2) << lambda_um << "," << std::setprecision(1) << depth_ppm << "," << std::setprecision(1) << (depth_ppm - baseline_depth_ppm) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_055/transmission_spectrum_slopes.csv" << std::endl;
  return 0;
}
