// Solver for Paper #17: 3D Hill Regime Pebble Accretion Dynamics (Lambrechts & Johansen 2012)
// Evaluates pebble capture rates and core growth timescales in protoplanetary disks.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Lambrechts & Johansen (2012) Pebble Accretion Solver ===" << std::endl;

  hot_jupiter::PebbleAccretionModel pebble_model;

  std::ofstream csv_file("replications_ss/paper_017/pebble_accretion_rates.csv");
  csv_file << "m_core_earth,r_hill_au,mdot_pebble_kg_s_st01,mdot_pebble_kg_s_st001\n";

  // Core masses from 0.01 M_earth to 10.0 M_earth at 5 AU
  double a_m = 5.0 * hot_jupiter::AU;
  double sigma_pebbles = 10.0;  // kg/m^2

  for (double m_earth = 0.01; m_earth <= 10.0; m_earth += 0.5) {
    double m_kg = m_earth * 5.972e24;
    double r_h_au = pebble_model.hill_radius_m(m_kg, a_m) / hot_jupiter::AU;
    double mdot_st01 = pebble_model.pebble_accretion_rate_kg_s(m_kg, a_m, sigma_pebbles, 0.1);
    double mdot_st001 = pebble_model.pebble_accretion_rate_kg_s(m_kg, a_m, sigma_pebbles, 0.01);

    csv_file << std::fixed << std::setprecision(2) << m_earth << "," << std::setprecision(5) << r_h_au << "," << std::scientific << mdot_st01 << "," << mdot_st001 << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_017/pebble_accretion_rates.csv" << std::endl;
  return 0;
}
