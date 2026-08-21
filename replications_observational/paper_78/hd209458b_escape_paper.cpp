// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #78: HD 209458b Lyman-Alpha Hydrodynamic Escape Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #78: HD 209458b HYDRODYNAMIC ATMOSPHERIC ESCAPE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::HD209458bPhotoevaporationModel model;

  const double mdot_g_s = model.mass_loss_rate_g_s(); // ~ 4.85e10 g/s (~ 5e5 kg/s)
  const double ly_a_depth_pct = model.lyman_alpha_transit_depth_percent(); // ~ 15.0%

  std::cout << "HD 209458b Mass Loss Rate: " << mdot_g_s << " g/s (" << (mdot_g_s / 1e6) << " tons/s)" << std::endl;
  std::cout << "Lyman-Alpha Transit Depth: " << ly_a_depth_pct << " %" << std::endl;

  // Transit light curve simulation over -4 to +4 hours (linear time scale):
  // Optical transit duration ~ 3.0 hr (semi-duration 1.5 hr), depth 1.5%
  // Lyman-alpha exospheric cloud + cometary tail duration ~ 6.0 hr, depth 15.0%
  std::ofstream out("replications_observational/paper_78/hd209458b_lyman_alpha_transit.csv");
  out << "time_hours_from_midtransit,optical_relative_flux,lyman_alpha_relative_flux\n";

  for (double t_hr = -4.0; t_hr <= +4.0; t_hr += 0.1) {
    // Optical light curve
    double optical_flux = 1.0;
    if (std::abs(t_hr) <= 1.5) {
      optical_flux = 1.0 - 0.015; // 1.5% transit depth
    }

    // Lyman-alpha light curve with asymmetric cometary tail tailing post-transit
    double ly_a_flux = 1.0;
    // Exospheric head (centered at t=0, half-width ~ 1.8 hr) + tail (lagging to t ~ +3.5 hr)
    double head_absorption = 0.15 * std::exp(-std::pow(t_hr / 1.5, 2.0));
    double tail_absorption = (t_hr > 0.0) ? 0.08 * (t_hr / 3.0) * std::exp(-t_hr / 2.0) : 0.0;
    double total_absorption = head_absorption + tail_absorption;
    if (total_absorption > 0.15) total_absorption = 0.15;

    ly_a_flux = 1.0 - total_absorption;

    out << t_hr << "," << optical_flux << "," << ly_a_flux << "\n";
  }
  out.close();

  std::cout << "Generated HD 209458b Transit & Exospheric Escape Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
