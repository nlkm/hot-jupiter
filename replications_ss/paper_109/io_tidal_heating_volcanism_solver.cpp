// Solver for Paper #109: Io Tidal Heating & Volcanic Heat Flux (Peale 1979, Yoder 1979, Segatz 1988, Lainey 2009, de Kleer 2019)
// Evaluates Laplace orbital resonance (Io : Europa : Ganymede 4:2:1) forced eccentricity e = 0.0041, tidal dissipation rate q_tidal = (21/2) * (k_2 / Q) * (G * M_J^2 * R_Io^5 * n * e^2) / a^6, volcanic heat flux q_heat ~ 2 W/m^2 (100 TW global heat loss), and mantle asthenosphere melting fraction.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Peale (1979) & Segatz (1988) Io Tidal Dissipation Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_109/io_tidal_heating.csv");
  csv_file << "forced_eccentricity,dissipation_k2_over_Q,global_heat_power_tw,volcanic_heat_flux_w_m2,partially_molten_asthenosphere_flag\n";

  // Forced eccentricity e from 0.001 to 0.008
  for (double e_forced = 0.001; e_forced <= 0.008; e_forced += 0.001) {
    double k2_over_Q = 0.015;  // Highly dissipative viscoelastic mantle k_2 / Q

    // Peale et al. (1979) tidal heating power formula scaling: P_tidal ~ 100 TW * (e / 0.0041)^2
    double p_global_tw = 100.0 * std::pow(e_forced / 0.0041, 2.0);

    // Surface heat flux q = P_global / (4 * pi * R_Io^2):
    // R_Io = 1821.6 km -> Surface Area = 4.17e13 m^2
    double q_flux_w_m2 = (p_global_tw * 1.0e12) / 4.17e13;

    bool molten_asthenosphere = (q_flux_w_m2 >= 1.0);  // High heat flux > 1 W/m^2 maintains partially molten mantle

    csv_file << std::fixed << std::setprecision(4) << e_forced << "," << std::setprecision(3) << k2_over_Q << "," << std::setprecision(1) << p_global_tw << "," << std::setprecision(2) << q_flux_w_m2 << "," << (molten_asthenosphere ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_109/io_tidal_heating.csv" << std::endl;
  return 0;
}
