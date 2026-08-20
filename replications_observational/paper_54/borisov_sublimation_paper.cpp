// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #54: Interstellar Comet 2I/Borisov Extreme CO Sublimation Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #54: 2I/BORISOV EXTREME CO SUBLIMATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::BorisovInterstellarCometModel borisov;

  // Track CO and H2O sublimation production rates across heliocentric distance (2.0 to 3.0 AU)
  const double q_h2o_2au = borisov.water_production_2au_molecules_s(); // 2.0e27 molec/s
  const double co_h2o_ratio = borisov.co_to_water_ratio();            // 1.45

  std::ofstream out("replications_observational/paper_54/borisov_production_rates.csv");
  out << "r_au,q_h2o_molec_s,q_co_molec_s,ratio_co_h2o\n";

  for (double r = 2.0; r <= 3.0; r += 0.05) {
    // Water sublimation scales steeply ~ r^-5 to r^-6 outside the water ice line (r > 2.5 AU)
    double q_h2o = q_h2o_2au * std::pow(2.0 / r, 5.0);
    // CO sublimation remains active at large distance, scaling ~ r^-2
    double q_co = (q_h2o_2au * co_h2o_ratio) * std::pow(2.0 / r, 2.0);
    double current_ratio = q_co / q_h2o;

    out << r << "," << q_h2o << "," << q_co << "," << current_ratio << "\n";
  }
  out.close();

  std::cout << "Generated 2I/Borisov CO & H2O production rate simulation data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
