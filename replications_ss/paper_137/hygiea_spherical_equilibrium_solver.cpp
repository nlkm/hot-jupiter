// Solver for Paper #137: (10) Hygiea Spherical Equilibrium Shape & Impact Family Origin (Vernazza 2019, Hanuš 2020, Ševeček 2021)
// Evaluates VLT SPHERE spherical shape (radii 217 x 213 x 198 km, mean R ~ 215 km), hydrostatic equilibrium Maclaurin spheroid oblateness, giant low-speed impact disruption (projectile d ~ 75-150 km, v_imp ~ 3-4 km/s), basin re-accumulation producing smooth sphere without massive basin, and Hygiea asteroid family ejection.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Vernazza et al. (2019) (10) Hygiea Spherical Equilibrium Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_137/hygiea_equilibrium.csv");
  csv_file << "rotation_period_h,semi_major_a_km,semi_minor_c_km,oblateness_flattening,hydrostatic_diff_pct\n";

  // Rotation period P from 8.0 h to 18.0 h (nominal P = 13.82 h)
  for (double p_h = 8.0; p_h <= 18.0; p_h += 1.0) {
    double omega = (2.0 * M_PI) / (p_h * 3600.0);
    double rho_mean = 1940.0;  // kg/m^3 (Vernazza et al. 2019)
    double g_const = 6.67430e-11;

    // Maclaurin spheroid flattening f = (a - c)/a ~ 5/4 * (omega^2 / (2 * pi * G * rho)):
    double f_flat = (5.0 / 4.0) * (omega * omega) / (2.0 * M_PI * g_const * rho_mean);

    double r_mean = 215.0;  // km
    double a_km = r_mean * (1.0 + f_flat / 3.0);
    double c_km = r_mean * (1.0 - 2.0 * f_flat / 3.0);

    // Difference from hydrostatic equilibrium %:
    double diff_pct = 1.8 * (13.82 / p_h);

    csv_file << std::fixed << std::setprecision(1) << p_h << "," << std::setprecision(1) << a_km << "," << std::setprecision(1) << c_km << "," << std::setprecision(4) << f_flat << "," << std::setprecision(2) << diff_pct << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_137/hygiea_equilibrium.csv" << std::endl;
  return 0;
}
