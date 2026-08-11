// Solver for Paper #64: Resonant Ring-Moon Interactions & Density Wave Damping in Saturn's Rings (Goldreich & Tremaine 1978, 1982, Cuzzi et al. 1984)
// Evaluates spiral density wave wavelength scaling lambda = 4 * pi^2 * G * Sigma / (3 * (m-1) * Omega^2 * |r - r_res|) and damping length x_damp.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Goldreich & Tremaine (1978, 1982) Saturn Density Wave Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_064/density_wave_wavelengths.csv");
  csv_file << "distance_from_res_km,surface_density_g_cm2,wavelength_km,wave_amplitude_relative\n";

  double r_res_km = 136500.0;     // Prometheus 2:1 inner Lindblad resonance location in A Ring
  double sigma_ring = 450.0;      // Ring surface density ~ 45 g/cm^2 (450 kg/m^2)
  double m_res = 2.0;             // 2:1 resonance m = 2
  double omega_s = std::sqrt(hot_jupiter::G * hot_jupiter::M_SUN / std::pow(r_res_km * 1000.0, 3.0));

  // Radial distance from resonance delta_r from 5 km to 100 km
  for (double delta_r_km = 5.0; delta_r_km <= 100.0; delta_r_km += 5.0) {
    double delta_r_m = delta_r_km * 1000.0;

    // Spiral density wave dispersion relation wavelength lambda = 4 * pi^2 * G * Sigma / (3 * (m - 1) * Omega^2 * delta_r)
    double lambda_m = 4.0 * hot_jupiter::PI * hot_jupiter::PI * hot_jupiter::G * sigma_ring / (3.0 * (m_res - 1.0) * omega_s * omega_s * delta_r_m);
    double lambda_km = lambda_m / 1000.0;

    // Viscous damping amplitude decay A(r) ~ exp(-(delta_r / x_damp)^3)
    double x_damp_km = 40.0;
    double amp_rel = std::exp(-std::pow(delta_r_km / x_damp_km, 3.0));

    csv_file << std::fixed << std::setprecision(1) << delta_r_km << "," << std::setprecision(1) << (sigma_ring / 10.0) << "," << std::setprecision(3) << lambda_km << "," << std::setprecision(4) << amp_rel << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_064/density_wave_wavelengths.csv" << std::endl;
  return 0;
}
