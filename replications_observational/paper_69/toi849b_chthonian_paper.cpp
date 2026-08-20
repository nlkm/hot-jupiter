// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #69: TOI-849b Remnant Chthonian Core Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #69: TOI-849b CHTHONIAN REMNANT CORE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TOI849bStrippedCoreModel model;

  const double m_planet_me = model.planet_mass_mearth();    // 39.1 M_earth


  // Interior structure mass-radius envelope fraction grid:
  // R(f_env) = R_core * (1 + 0.55 * log10(1 + f_env / f_0))
  // for a 39.1 M_earth solid Earth-like core (R_core ~ 3.10 R_earth)
  const double r_core_re = 3.10;

  std::ofstream out("replications_observational/paper_69/toi849b_envelope_fraction_grid.csv");
  out << "envelope_fraction_pct,planet_radius_rearth,bulk_density_g_cm3\n";

  for (double f_pct = 0.0; f_pct <= 10.0; f_pct += 0.25) {
    double f_val = f_pct * 0.01;
    double r_synth = r_core_re * std::pow(1.0 + 3.5 * f_val, 0.45);
    
    // Bulk density: rho ~ M / R^3
    double rho_synth = (m_planet_me / std::pow(r_synth, 3.0)) * 5.515; // in g/cm^3

    out << f_pct << "," << r_synth << "," << rho_synth << "\n";
  }
  out.close();

  std::cout << "Generated TOI-849b Chthonian Envelope Grid Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
