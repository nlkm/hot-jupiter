// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #74: Mercury Relativistic Precession Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #74: MERCURY RELATIVISTIC PERIHELION PRECESSION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::MercuryRelativisticPrecessionModel model;

  const double gr_rate = model.gr_precession_arcsec_century();        // ~ 42.98 arcsec/century
  const double j2_rate = model.j2_sun_precession_arcsec_century();    // ~ 0.03 arcsec/century
  const double newtonian_rate = 531.63;                               // ~ 531.63 arcsec/century (Venus, Earth, Jupiter, etc.)

  const double total_rate = gr_rate + j2_rate + newtonian_rate;      // ~ 574.64 arcsec/century

  // Track accumulated perihelion precession over 200 years (1900 to 2100, linear scale)
  std::ofstream out("replications_observational/paper_74/mercury_precession_evolution.csv");
  out << "time_years,accumulated_shift_gr_arcsec,accumulated_shift_total_arcsec\n";

  for (double t_yr = 0.0; t_yr <= 200.0; t_yr += 5.0) {
    double shift_gr = (gr_rate / 100.0) * t_yr;
    double shift_total = (total_rate / 100.0) * t_yr;

    out << t_yr << "," << shift_gr << "," << shift_total << "\n";
  }
  out.close();

  std::cout << "Generated Mercury Perihelion Precession Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
