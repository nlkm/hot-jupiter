// Solver for Paper #34: Poynting-Robertson Drag & Dust Grain Orbital Decay (Burns et al. 1979, Gustafson 1994)
// Evaluates dust orbital decay timescale t_PR = (4 * pi * rho * s * c * a^2) / (3 * L_sun * Q_pr).

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Burns et al. (1979) Poynting-Robertson Drag Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_034/pr_drag_decay_timescales.csv");
  csv_file << "grain_size_um,initial_a_au,t_pr_years,t_pr_kyr\n";

  double speed_of_light = 2.99792458e8;  // m/s
  double rho_grain = 2000.0;              // kg/m^3 (silicate grain density)
  double q_pr = 1.0;                      // radiation pressure efficiency factor

  // Grain sizes from 1 um to 100 um at 1 AU initial semi-major axis
  for (double grain_um = 1.0; grain_um <= 100.0; grain_um += 5.0) {
    double grain_m = grain_um * 1.0e-6;
    double a_init_m = hot_jupiter::AU;

    // t_PR = (4 * pi * rho * s * c * a^2) / (3 * L_sun * Q_pr)
    double t_pr_sec = (4.0 * hot_jupiter::PI * rho_grain * grain_m * speed_of_light * a_init_m * a_init_m) /
                       (3.0 * hot_jupiter::L_SUN * q_pr);
    double t_pr_years = t_pr_sec / hot_jupiter::YEAR;
    double t_pr_kyr = t_pr_years / 1000.0;

    csv_file << std::fixed << std::setprecision(1) << grain_um << ",1.0," << std::setprecision(1) << t_pr_years << "," << std::setprecision(2) << t_pr_kyr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_034/pr_drag_decay_timescales.csv" << std::endl;
  return 0;
}
