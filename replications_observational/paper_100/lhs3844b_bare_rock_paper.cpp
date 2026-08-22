// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #100: LHS 3844b Bare Rock Thermal Phase Curve Driver (Century Milestone)

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #100: LHS 3844b BARE ROCK THERMAL PHASE CURVE" << std::endl;
  std::cout << "   *** CENTURY MILESTONE: 100 OBSERVATIONAL REPLICATIONS COMPLETE ***" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::LHS3844bBareRockModel model;

  const double t_day = model.dayside_temp_k();                 // ~ 1040 K
  const double t_night = model.nightside_temp_k();             // ~ 20 K
  const double eps_redis = model.heat_redistribution_efficiency(); // 0.00 (Zero atmospheric circulation)
  const double albedo_basalt = model.basalt_surface_albedo();  // ~ 0.05 (Dark volcanic basalt)

  std::cout << "LHS 3844b Dayside Temperature: " << t_day << " K" << std::endl;
  std::cout << "LHS 3844b Nightside Temperature: " << t_night << " K" << std::endl;
  std::cout << "Heat Redistribution Efficiency: " << eps_redis << std::endl;
  std::cout << "Basaltic Surface Bond Albedo: " << albedo_basalt << std::endl;

  // Track 4.5 um Thermal Phase Curve across Orbital Period 11.1 hours (linear time scale):
  // P_orb = 0.463 days = 11.11 hours
  // Mid-transit at t = 0.0 hr (phi = 0 deg), Secondary eclipse at t = 5.55 hr (phi = 180 deg)
  std::ofstream out("replications_observational/paper_100/lhs3844b_thermal_phase_curve.csv");
  out << "orbital_time_hours,orbital_phase_deg,relative_flux_ppm,thick_atmosphere_flux_ppm\n";

  const double p_orb_hr = 11.11;
  const double amp_bare_rock = 380.0; // ppm at 4.5 um
  const double amp_thick_atm = 90.0;  // ppm (if 1 bar CO2 / N2 atmosphere existed)

  for (double t_hr = 0.0; t_hr <= 11.11; t_hr += 0.15) {
    double phase_deg = (t_hr / p_orb_hr) * 360.0;
    double phase_rad = phase_deg * M_PI / 180.0;

    // Bare rock instantaneous Lambertian thermal emission
    double cos_phase = -std::cos(phase_rad); // Minimum at transit, maximum at occultation
    double f_bare = (cos_phase > 0.0) ? (amp_bare_rock * std::pow(cos_phase, 1.2)) : 0.0;

    // Thick atmosphere recirculated thermal emission (smooth sinusoidal curve)
    double f_atm = amp_thick_atm * (0.5 * (1.0 + cos_phase));

    out << t_hr << "," << phase_deg << "," << f_bare << "," << f_atm << "\n";
  }
  out.close();

  std::cout << "Generated LHS 3844b Thermal Phase Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
