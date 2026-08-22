// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #113: 2I/Borisov Interstellar Comet CO Enrichment Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #113: 2I/BORISOV INTERSTELLAR COMET CO ENRICHMENT" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::BorisovInterstellarCometModel model;

  const double ecc = model.orbital_eccentricity();                     // ~ 3.36 (Hyperbolic)
  const double ratio_co_h2o = model.co_to_water_ratio();               // ~ 1.45 (Extreme CO dominance)
  const double q_h2o_2au = model.water_production_2au_molecules_s();   // ~ 2.0e27 molecules/s
  const double t_form = model.formation_temperature_k();               // ~ 20.0 K (Ultra-cold frost line)
  const double a1_nongrav = model.non_grav_radial_a1_au_day2();        // ~ 4.0e-8 AU/day^2

  std::cout << "Orbital Eccentricity (Hyperbolic Unbound): " << ecc << std::endl;
  std::cout << "CO / H2O Production Ratio at 2 AU: " << ratio_co_h2o << std::endl;
  std::cout << "H2O Outgassing Production at 2 AU: " << q_h2o_2au << " molec/s" << std::endl;
  std::cout << "Primordial Disk Formation Temperature: " << t_form << " K" << std::endl;
  std::cout << "Radial Non-Gravitational Parameter A1: " << a1_nongrav << " AU/day^2" << std::endl;

  // Track Gas Production Rates Q_CO and Q_H2O vs Heliocentric Distance r_h from 2.0 to 4.0 AU (linear scale):
  std::ofstream out("replications_observational/paper_113/borisov_production_rates.csv");
  out << "heliocentric_distance_au,co_production_rate_1e27_s,water_production_rate_1e27_s,co_to_water_ratio\n";

  for (double rh = 2.0; rh <= 4.0; rh += 0.05) {
    // Water sublimation drops steeply beyond ~ 2.5 AU due to water ice sublimation threshold
    double q_h2o = (q_h2o_2au / 1.0e27) * std::pow(2.0 / rh, 2.0) * std::exp(-std::pow(rh / 2.8, 4.0));

    // CO is super-volatile and sublimates freely at all r_h < 10 AU with ~ r_h^-2 scaling
    double q_co = (q_h2o_2au * ratio_co_h2o / 1.0e27) * std::pow(2.0 / rh, 1.8);

    double ratio_local = (q_h2o > 1e-4) ? (q_co / q_h2o) : 100.0;

    out << rh << "," << q_co << "," << q_h2o << "," << ratio_local << "\n";
  }
  out.close();

  std::cout << "Generated 2I/Borisov Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
