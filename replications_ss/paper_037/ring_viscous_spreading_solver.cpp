// Solver for Paper #37: Planetary Ring Viscous Evolution & Spreading (Goldreich & Tremaine 1982, Borderies et al. 1983)
// Evaluates viscous ring spreading timescale t_visc ~ (Delta r)^2 / nu and surface density evolution.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Goldreich & Tremaine (1982) Planetary Ring Viscous Spreading Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_037/ring_viscous_spreading_timescales.csv");
  csv_file << "particle_size_cm,kinematic_viscosity_m2_s,t_visc_years,t_visc_myr\n";

  double ring_radius_m = 1.2e8;     // B-ring radius (~2 R_saturn)
  double ring_width_m = 1.0e7;      // ring width [m]
  double m_saturn = 5.6834e26;      // Saturn mass [kg]
  double n_saturn = std::sqrt(hot_jupiter::G * m_saturn / std::pow(ring_radius_m, 3.0));  // orbital frequency

  // Particle sizes from 1 cm to 10 m
  for (double size_cm = 1.0; size_cm <= 1000.0; size_cm *= 2.5) {
    double size_m = size_cm / 100.0;
    // Kinematic viscosity nu ~ s^2 * n
    double nu = size_m * size_m * n_saturn;

    // Viscous spreading timescale t_visc ~ (Delta r)^2 / nu
    double t_visc_sec = (ring_width_m * ring_width_m) / nu;
    double t_visc_years = t_visc_sec / hot_jupiter::YEAR;
    double t_visc_myr = t_visc_years / 1.0e6;

    csv_file << std::fixed << std::setprecision(1) << size_cm << "," << std::scientific << nu << "," << std::scientific << t_visc_years << "," << std::fixed << std::setprecision(2) << t_visc_myr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_037/ring_viscous_spreading_timescales.csv" << std::endl;
  return 0;
}
