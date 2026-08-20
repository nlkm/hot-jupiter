// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #67: Asteroid (101955) Bennu Particle Ejection Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #67: BENNU REGOLITH THERMAL PARTICLE EJECTION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::BennuParticleEjectionModel model;

  const double v_ej_base = model.particle_ejection_velocity_m_s(); // 0.50 m/s
  const double r_particle_cm = model.mean_particle_radius_cm();    // 1.5 cm
  const double v_escape_bennu = 0.20; // ~ 0.20 m/s escape speed


  // Particle size distribution and launch velocity spectrum:
  // v(r) ~ v_0 * sqrt(r_0 / r) due to thermal elastic strain energy release
  std::ofstream out("replications_observational/paper_67/bennu_particle_spectrum.csv");
  out << "particle_radius_cm,launch_velocity_m_s,escaped_fraction,kinetic_energy_microjoules\n";

  for (double r_cm = 0.2; r_cm <= 6.0; r_cm += 0.2) {
    double v_launch = v_ej_base * std::sqrt(r_particle_cm / r_cm);
    double esc_frac = (v_launch > v_escape_bennu) ? (1.0 - (v_escape_bennu / v_launch)) : 0.0;
    
    // Mass m = (4/3)*pi*r^3 * rho (2000 kg/m^3)
    double m_kg = (4.0 / 3.0) * M_PI * std::pow(r_cm * 0.01, 3.0) * 2000.0;
    double ke_uj = 0.5 * m_kg * std::pow(v_launch, 2.0) * 1.0e6;

    out << r_cm << "," << v_launch << "," << esc_frac << "," << ke_uj << "\n";
  }
  out.close();

  std::cout << "Generated Bennu Particle Ejection Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
