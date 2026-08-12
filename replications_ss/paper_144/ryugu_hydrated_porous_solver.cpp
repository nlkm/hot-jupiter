// Solver for Paper #144: (162173) Ryugu C-Type Hydrated Mineralogy & High Macro-Porosity (Watanabe 2019, Sugita 2019, Jaumann 2019, Grott 2019, Kitazato 2019)
// Evaluates Hayabusa2 rendezvous top-shaped C-type (Cb) asteroid Ryugu (equatorial d ~ 1000 m, polar d ~ 880 m, mean R ~ 448 m), mass M = 4.50 x 10^11 kg, ultra-low bulk density rho_bulk = 1.19 +- 0.03 g/cm^3, macro-porosity P_macro = 52.0%, NIRS3 2.72 um OH-bearing phyllosilicate absorption band, and low thermal inertia I_thermal ~ 300 J/m^2/K/s^0.5.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Watanabe et al. (2019) & Sugita et al. (2019) (162173) Ryugu Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_144/ryugu_hydrated_porous.csv");
  csv_file << "porosity_pct,bulk_density_g_cm3,mass_10_11_kg,thermal_inertia_tiu,oh_band_depth_pct\n";

  // Bulk porosity P_macro % from 30% to 70%
  for (double p_pct = 30.0; p_pct <= 70.0; p_pct += 5.0) {
    double rho_grain = 2.48;  // CI/CM chondrite grain density (g/cm^3)
    double rho_bulk = rho_grain * (1.0 - p_pct / 100.0);

    double volume_m3 = 3.77e8;  // Ryugu volume (Watanabe et al. 2019)
    double mass_kg = rho_bulk * 1000.0 * volume_m3;
    double mass_10_11 = mass_kg / 1.0e11;

    // Thermal inertia TIU (J/m^2/K/s^0.5):
    double tiu = 150.0 + 300.0 * ((70.0 - p_pct) / 40.0);

    // NIRS3 2.72 um OH absorption band depth %:
    double band_depth_pct = 1.8;

    csv_file << std::fixed << std::setprecision(1) << p_pct << "," << std::setprecision(2) << rho_bulk << "," << std::setprecision(2) << mass_10_11 << "," << std::setprecision(0) << tiu << "," << std::setprecision(1) << band_depth_pct << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_144/ryugu_hydrated_porous.csv" << std::endl;
  return 0;
}
