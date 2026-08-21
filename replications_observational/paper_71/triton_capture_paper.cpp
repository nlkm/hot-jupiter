// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #71: Triton Retrograde Capture & Tidal Heating Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #71: TRITON RETROGRADE CAPTURE & TIDAL HEATING" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TritonRetrogradeCaptureModel model;

  const double e0 = model.post_capture_eccentricity();            // 0.99
  const double tau_circ_myr = model.circularization_timescale_myr(); // 100 Myr
  const double f_peak = model.peak_tidal_circularization_flux_w_m2(); // 1.2e4 W/m^2
  const double a_final_km = model.present_orbital_radius_km();    // 354760 km

  // Tidal circularization and semi-major axis evolution:
  // e(t) = e0 * exp(-t / (tau_circ * 0.45))
  // a(t) = a_final / (1 - e(t)^2)
  // F_tide(t) ~ F_peak * (e(t) / e0)^2 * (a_final / a(t))^6
  std::ofstream out("replications_observational/paper_71/triton_circularization_track.csv");
  out << "time_myr,eccentricity,semimajor_axis_1000km,tidal_flux_w_m2\n";

  for (double t_myr = 0.0; t_myr <= 200.0; t_myr += 5.0) {
    double e_t = e0 * std::exp(-t_myr / (tau_circ_myr * 0.45));
    if (e_t < 1.0e-5) e_t = 1.0e-5;

    double a_km = a_final_km / (1.0 - std::pow(e_t, 2.0));
    double f_tide = f_peak * std::pow(e_t / e0, 2.0) * std::pow(a_final_km / a_km, 6.0);

    out << t_myr << "," << e_t << "," << (a_km / 1000.0) << "," << f_tide << "\n";
  }
  out.close();

  std::cout << "Generated Triton Capture & Circularization Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
