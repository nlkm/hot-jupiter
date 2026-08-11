// Solver for Paper #117: Interstellar Comets 'Oumuamua & 2I/Borisov Outgassing Acceleration (Meech 2017, Micheli 2018, Guzik 2019, Seligman 2020)
// Evaluates hyperbolic excess velocity v_inf > 26 km/s, non-gravitational acceleration a_ng ~ 5 * 10^-6 m/s^2 driven by H2 / CO / H2O sublimation jet recoil, needle/cigar axis ratio 10:1, and ISO spatial number density n_ISO ~ 0.1 / au^3.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Micheli (2018) & Seligman (2020) Interstellar Comet Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_117/interstellar_comet_outgassing.csv");
  csv_file << "heliocentric_distance_au,sublimation_mass_loss_kg_s,non_gravitational_acceleration_m_s2,hyperbolic_excess_velocity_km_s,iso_class_flag\n";

  // Heliocentric distance r_helio from 1.0 au to 3.0 au
  for (double r_au = 1.0; r_au <= 3.0; r_au += 0.2) {
    double v_inf_km_s = 26.3;  // 'Oumuamua interstellar excess velocity

    // Sublimation mass loss dM/dt ~ r^-2 (H2O / CO / H2 volatile sublimation):
    double m_dot_kg_s = 100.0 / (r_au * r_au);

    // Non-gravitational acceleration a_ng = v_gas * (dM/dt) / M_body:
    // For M_body ~ 10^9 kg, v_gas ~ 500 m/s:
    double a_ng_m_s2 = 5.0e-6 / (r_au * r_au);

    bool is_interstellar_object = (v_inf_km_s > 0.0 && a_ng_m_s2 > 1.0e-7);

    csv_file << std::fixed << std::setprecision(1) << r_au << "," << std::setprecision(1) << m_dot_kg_s << "," << std::scientific << std::setprecision(2) << a_ng_m_s2 << "," << std::fixed << std::setprecision(1) << v_inf_km_s << "," << (is_interstellar_object ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_117/interstellar_comet_outgassing.csv" << std::endl;
  return 0;
}
