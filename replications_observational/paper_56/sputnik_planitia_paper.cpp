// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #56: Pluto Sputnik Planitia Solid Nitrogen Convection Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #56: PLUTO SPUTNIK PLANITIA NITROGEN CONVECTION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::PlutoSputnikPlanitiaConvectionModel model;

  const double D_cell = model.cell_diameter_km();      // 30.0 km
  const double tau_overturn = model.overturning_timescale_years(); // 5.0e5 yr


  // Rayleigh-Benard convection cell surface topography profile:
  // delta_z(r) = Delta_h * cos(pi * r / R_cell) where R_cell = D_cell / 2 = 15 km
  // and Delta_h ~ 50 m (elevated center relative to trough margin)
  const double R_cell = D_cell / 2.0;
  const double Delta_h = 0.050; // 50 meters (0.05 km)

  std::ofstream out("replications_observational/paper_56/sputnik_cell_profile.csv");
  out << "r_km,elev_m,flow_velocity_cm_yr,temperature_k\n";

  const double v_max_cm_yr = (D_cell * 1.0e5) / tau_overturn; // ~ 6.0 cm/year

  for (double r = 0.0; r <= R_cell; r += 0.25) {
    double elev_m = Delta_h * 1000.0 * std::cos(M_PI * r / R_cell);
    double flow_v = v_max_cm_yr * std::sin(M_PI * r / R_cell);
    double temp_k = 38.0 + 4.0 * (0.5 + 0.5 * std::cos(M_PI * r / R_cell)); // 38 K margin to 42 K center

    out << r << "," << elev_m << "," << flow_v << "," << temp_k << "\n";
  }
  out.close();

  std::cout << "Generated Sputnik Planitia Convection Cell profile data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
