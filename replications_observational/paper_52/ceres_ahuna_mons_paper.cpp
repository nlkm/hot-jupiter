// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #52: Ceres Ahuna Mons Cryovolcanic Dome Extrusion Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #52: CERES AHUNA MONS CRYOVOLCANIC DOME EXTRUSION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::CeresAhunaMonsCryovolcanismModel model;

  // Bingham plastic / Herschel-Bulkley dome extrusion profile: h(r) = H_0 * (1 - (r/R_0)^2)^(1/2)
  // for a yield-stress slurry dome under Ceres low gravity (g = 0.28 m/s^2)
  const double H_0 = model.dome_height_km();      // 4.0 km
  const double R_0 = model.base_diameter_km() / 2.0; // 10.0 km
  const double g_ceres = 0.28;                    // m/s^2
  const double rho_slurry = 1350.0;               // kg/m^3 (mud + hydrated salts + ice)
  const double tau_0 = model.brine_yield_stress_pa(); // 15 kPa

  std::ofstream out("replications_observational/paper_52/ahuna_mons_profile.csv");
  out << "radius_km,elevation_km,yield_stress_kpa,slope_deg\n";

  for (double r = 0.0; r <= R_0; r += 0.2) {
    double h = H_0 * std::sqrt(std::max(0.0, 1.0 - (r / R_0) * (r / R_0)));
    double dh_dr = (r > 0.0 && r < R_0) ? -(H_0 / R_0) * (r / R_0) / std::sqrt(1.0 - (r / R_0) * (r / R_0)) : 0.0;
    double slope_deg = std::atan(std::abs(dh_dr)) * 180.0 / M_PI;
    double tau_calc = (rho_slurry * g_ceres * (H_0 - h) * 1000.0 * 0.02) / 1000.0 + (tau_0 / 1000.0);

    out << r << "," << h << "," << tau_calc << "," << slope_deg << "\n";
  }
  out.close();

  std::cout << "Generated Ceres Ahuna Mons Cryovolcanic Dome profile data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
