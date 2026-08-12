// Solver for Paper #145: (101955) Bennu Mass, Porosity, Particle Ejection Events, & YORP Acceleration (Lauretta 2019, Scheeres 2019, Hergenrother 2019, Walsh 2019, Chesley 2019, Hamilton 2019)
// Evaluates OSIRIS-REx rendezvous top-shaped B-type asteroid Bennu (mean R ~ 245 m), mass M = (7.329 +- 0.009) x 10^10 kg, bulk density rho_bulk = 1.19 +- 0.01 g/cm^3, macro-porosity P_macro = 50-54% (rubble pile), active particle ejection event velocities v_ej ~ 0.05-3.0 m/s (thermal fracturing / micro-meteoroid impacts), and YORP spin-up acceleration domega/dt = (2.64 +- 0.16) x 10^-6 rad/yr^2.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Lauretta et al. (2019) & Scheeres et al. (2019) (101955) Bennu Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_145/bennu_mass_porosity.csv");
  csv_file << "porosity_pct,bulk_density_g_cm3,mass_10_10_kg,particle_ejection_velocity_m_s,yorp_accel_rad_yr2\n";

  // Bulk porosity P_macro % from 30% to 70%
  for (double p_pct = 30.0; p_pct <= 70.0; p_pct += 5.0) {
    double rho_grain = 2.45;  // CI/CM magnetite-bearing chondrite grain density (g/cm^3)
    double rho_bulk = rho_grain * (1.0 - p_pct / 100.0);

    double volume_m3 = 6.16e7;  // Bennu volume (Scheeres et al. 2019)
    double mass_kg = rho_bulk * 1000.0 * volume_m3;
    double mass_10_10 = mass_kg / 1.0e10;

    // Active particle ejection velocity v_ej (m/s) (escapes surface if v > v_esc ~ 0.20 m/s):
    double v_ej_m_s = 0.85 * (rho_bulk / 1.19);

    // YORP spin acceleration domega/dt (rad/yr^2):
    double yorp_accel_yr2 = 2.64e-6 * (1.19 / rho_bulk);

    csv_file << std::fixed << std::setprecision(1) << p_pct << "," << std::setprecision(2) << rho_bulk << "," << std::setprecision(3) << mass_10_10 << "," << std::setprecision(2) << v_ej_m_s << "," << std::scientific << std::setprecision(2) << yorp_accel_yr2 << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_145/bennu_mass_porosity.csv" << std::endl;
  return 0;
}
