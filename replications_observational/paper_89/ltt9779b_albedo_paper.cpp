// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #89: LTT 9779b Ultra-Hot Neptune Albedo & Reflected Light Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #89: LTT 9779b ULTRA-HOT NEPTUNE ALBEDO" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::LTT9779bUltraHotNeptuneModel model;

  const double albedo_g = model.geometric_albedo();               // ~ 0.80
  const double eclipse_depth_ppm = model.secondary_eclipse_depth_ppm(); // ~ 225 ppm
  const double t_day_k = model.day_side_temperature_k();           // ~ 2300 K
  const double mdot_g_s = model.mass_loss_rate_g_s();             // ~ 1.8e10 g/s

  std::cout << "LTT 9779b Geometric Albedo A_g: " << albedo_g << std::endl;
  std::cout << "Secondary Eclipse Depth: " << eclipse_depth_ppm << " ppm" << std::endl;
  std::cout << "Dayside Temperature with Reflective Clouds: " << t_day_k << " K" << std::endl;
  std::cout << "Photoevaporation Mass Loss Rate: " << mdot_g_s << " g/s (" << (mdot_g_s / 1e6) << " tons/s)" << std::endl;

  // Track Optical Eclipse Light Curve over -3.0 to +3.0 hours from mid-occultation (linear time scale):
  // Eclipse duration ~ 1.8 hr (semi-duration 0.9 hr, ingress/egress ~ 0.18 hr)
  std::ofstream out("replications_observational/paper_89/ltt9779b_eclipse_lightcurve.csv");
  out << "time_hours_from_mideclipse,relative_flux_ppm,baseline_flux_ppm\n";

  const double tau_ing = 0.18; // hours
  const double tau_tot = 0.90; // hours

  for (double t_hr = -3.0; t_hr <= 3.0; t_hr += 0.05) {
    double abs_t = std::abs(t_hr);
    double occulted_fraction = 0.0;

    if (abs_t <= (tau_tot - tau_ing)) {
      occulted_fraction = 1.0;
    } else if (abs_t < tau_tot) {
      occulted_fraction = (tau_tot - abs_t) / tau_ing;
    } else {
      occulted_fraction = 0.0;
    }

    double flux_ppm = -eclipse_depth_ppm * occulted_fraction;

    out << t_hr << "," << flux_ppm << "," << 0.0 << "\n";
  }
  out.close();

  std::cout << "Generated LTT 9779b Eclipse Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
