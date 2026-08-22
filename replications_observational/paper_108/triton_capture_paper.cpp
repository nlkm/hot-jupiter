// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #108: Neptune Triton Retrograde Capture & Tidal Resurfacing Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #108: NEPTUNE TRITON RETROGRADE CAPTURE & TIDAL HEATING" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TritonRetrogradeCaptureModel model;

  const double inc_deg = model.retrograde_inclination_deg();           // ~ 156.8 deg (Retrograde)
  const double e_init = model.post_capture_eccentricity();             // ~ 0.99
  const double tau_circ_myr = model.circularization_timescale_myr();   // ~ 100.0 Myr
  const double f_tide_peak = model.peak_tidal_circularization_flux_w_m2(); // ~ 1.2e4 W/m^2
  const double r_orb_km = model.present_orbital_radius_km();           // ~ 354760 km

  std::cout << "Retrograde Orbital Inclination: " << inc_deg << " deg" << std::endl;
  std::cout << "Initial Post-Capture Eccentricity: " << e_init << std::endl;
  std::cout << "Tidal Circularization Timescale: " << tau_circ_myr << " Myr" << std::endl;
  std::cout << "Peak Tidal Resurfacing Heat Flux: " << f_tide_peak << " W/m^2" << std::endl;
  std::cout << "Present Circularized Semi-Major Axis: " << r_orb_km << " km" << std::endl;

  // Track Orbital Eccentricity and Tidal Heat Flux over 0 to 120 Myr (linear time scale):
  std::ofstream out("replications_observational/paper_108/triton_capture_evolution.csv");
  out << "time_myr,orbital_eccentricity,tidal_heat_flux_w_m2,semi_major_axis_1000km\n";

  for (double t_myr = 0.0; t_myr <= 120.0; t_myr += 2.0) {
    // Viscoelastic tidal eccentricity decay: e(t) = e0 * (1 - (t/tau)^2)^0.5
    double e_t = 0.0;
    double a_t = r_orb_km;
    double f_tide = 0.0;

    if (t_myr < tau_circ_myr) {
      double frac = t_myr / tau_circ_myr;
      e_t = e_init * std::sqrt(std::max(0.0, 1.0 - frac * frac));
      a_t = r_orb_km / (1.0 - e_t * e_t); // Constant angular momentum: a(1-e^2) = a_final
      f_tide = f_tide_peak * std::pow(e_t / e_init, 2.0) * std::pow(r_orb_km / a_t, 6.0);
    } else {
      e_t = 0.000016; // Modern nearly circular orbit
      a_t = r_orb_km;
      f_tide = 0.005; // Modern radiogenic baseline
    }

    out << t_myr << "," << e_t << "," << f_tide << "," << (a_t / 1000.0) << "\n";
  }
  out.close();

  std::cout << "Generated Triton Retrograde Capture Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
