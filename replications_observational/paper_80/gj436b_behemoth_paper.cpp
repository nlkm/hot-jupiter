// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #80: GJ 436b "The Behemoth" Extended Hydrogen Cloud Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #80: GJ 436b 'THE BEHEMOTH' HYDROGEN CLOUD" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::GJ436bHydrogenCloudModel model;

  const double mdot_g_s = model.mass_loss_rate_g_s(); // ~ 1.9e10 g/s (~ 19,000 kg/s)
  const double peak_depth_pct = model.lyman_alpha_transit_depth_percent(); // ~ 56.3%
  const double duration_hr = model.lyman_alpha_transit_duration_hours();   // ~ 22.0 hr

  std::cout << "GJ 436b Mass Loss Rate: " << mdot_g_s << " g/s (" << (mdot_g_s / 1e6) << " tons/s)" << std::endl;
  std::cout << "Peak Lyman-Alpha Transit Depth: " << peak_depth_pct << " %" << std::endl;
  std::cout << "Lyman-Alpha Transit Duration: " << duration_hr << " hours" << std::endl;

  // Transit light curve simulation over -6 to +18 hours (linear time scale):
  // Optical transit duration ~ 1.0 hr (semi-duration 0.5 hr), depth 0.69%
  // Lyman-alpha giant cloud transit duration ~ 22 hr, peak depth 56.3%
  std::ofstream out("replications_observational/paper_80/gj436b_behemoth_transit.csv");
  out << "time_hours_from_midtransit,optical_relative_flux,lyman_alpha_relative_flux\n";

  for (double t_hr = -6.0; t_hr <= +18.0; t_hr += 0.2) {
    // Optical light curve
    double optical_flux = 1.0;
    if (std::abs(t_hr) <= 0.5) {
      optical_flux = 1.0 - 0.0069; // 0.69% optical transit depth
    }

    // Lyman-alpha light curve with gigantic asymmetric cometary tail extending to +16 hr
    double ly_a_flux = 1.0;
    
    // Cloud head (ingress starts at t ~ -3.5 hr, peak near t ~ 0 to +1.5 hr)
    double head_absorption = 0.563 * std::exp(-std::pow((t_hr - 0.5) / 2.5, 2.0));
    
    // Massive cometary tail lagging to t ~ +16 hr
    double tail_absorption = (t_hr > 0.0) ? 0.35 * (t_hr / 6.0) * std::exp(-t_hr / 6.5) * std::exp(1.0) : 0.0;
    
    double total_absorption = head_absorption + tail_absorption;
    if (total_absorption > 0.563) total_absorption = 0.563;

    ly_a_flux = 1.0 - total_absorption;

    out << t_hr << "," << optical_flux << "," << ly_a_flux << "\n";
  }
  out.close();

  std::cout << "Generated GJ 436b 'The Behemoth' Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
