// Solver for Paper #143: (25143) Itokawa Rubble-Pile Porosity, Mass, & YORP Acceleration (Fujiwara 2006, Abe 2006, Scheeres 2007, Lowry 2014)
// Evaluates Hayabusa rendezvous sub-kilometer S-type asteroid Itokawa (535 x 294 x 209 m, mean R ~ 162 m), mass M = 3.51 x 10^10 kg, low bulk density rho_bulk = 1.90 +- 0.13 g/cm^3, macro-porosity P_macro = 40.6% (loosely bound re-accumulated rubble-pile), and YORP thermal recoil acceleration domega/dt = 4.5 x 10^-8 rad/day^2.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Fujiwara et al. (2006) & Scheeres et al. (2007) (25143) Itokawa Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_143/itokawa_rubble_pile.csv");
  csv_file << "porosity_pct,bulk_density_g_cm3,mass_10_10_kg,yorp_accel_rad_day2,spin_period_h\n";

  // Bulk porosity P_macro % from 20% to 60%
  for (double p_pct = 20.0; p_pct <= 60.0; p_pct += 5.0) {
    double rho_grain = 3.20;  // LL chondrite grain density (g/cm^3)
    double rho_bulk = rho_grain * (1.0 - p_pct / 100.0);

    double volume_m3 = 1.84e7;  // Itokawa volume (Fujiwara et al. 2006)
    double mass_kg = rho_bulk * 1000.0 * volume_m3;
    double mass_10_10 = mass_kg / 1.0e10;

    // YORP spin acceleration domega/dt (rad/day^2):
    double yorp_accel = 4.5e-8 * (1.90 / rho_bulk);

    // Current spin period P = 12.13 hr:
    double period_h = 12.13;

    csv_file << std::fixed << std::setprecision(1) << p_pct << "," << std::setprecision(2) << rho_bulk << "," << std::setprecision(2) << mass_10_10 << "," << std::scientific << std::setprecision(2) << yorp_accel << "," << std::fixed << std::setprecision(2) << period_h << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_143/itokawa_rubble_pile.csv" << std::endl;
  return 0;
}
