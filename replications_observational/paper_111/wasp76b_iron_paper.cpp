// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #111: WASP-76b Asymmetric Iron Rain Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #111: WASP-76b ASYMMETRIC IRON CONDENSATION & RAIN" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP76bIronRainModel model;

  const double t_day = model.dayside_temp_k();                           // ~ 2500 K
  const double t_night = model.nightside_temp_k();                       // ~ 1400 K
  const double abs_eve = model.evening_terminator_fe_absorption_percent(); // ~ 0.45 %
  const double abs_morn = model.morning_terminator_fe_absorption_percent(); // ~ 0.00 %
  const double t_cond = model.iron_condensation_temp_k();                // ~ 1800 K

  std::cout << "Dayside Temperature: " << t_day << " K" << std::endl;
  std::cout << "Nightside Temperature: " << t_night << " K" << std::endl;
  std::cout << "Evening Terminator Fe I Absorption: " << abs_eve << " %" << std::endl;
  std::cout << "Morning Terminator Fe I Absorption: " << abs_morn << " %" << std::endl;
  std::cout << "Fe Condensation Threshold Temperature: " << t_cond << " K" << std::endl;

  // Track Fe I Transmission Absorption Signal vs Transit Time t from -2.5 to +2.5 hours (linear time scale):
  // t = 0 is mid-transit. Ingress is t = -1.8 hr, Egress is t = +1.8 hr.
  // In the first half (evening limb leading), Fe vapor is abundant (0.45%).
  // In the second half (morning limb following), Fe has condensed on the nightside (0.00%).
  std::ofstream out("replications_observational/paper_111/wasp76b_fe_transit_evolution.csv");
  out << "transit_time_hours,fe_absorption_percent,doppler_blueshift_km_s,atmospheric_temp_k\n";

  for (double t_hr = -2.5; t_hr <= 2.5; t_hr += 0.1) {
    double abs_fe = 0.0;
    double doppler_v = 0.0;
    double temp_local = 0.0;

    // Out of transit: |t| > 1.8 hr
    if (std::abs(t_hr) > 1.8) {
      abs_fe = 0.0;
      doppler_v = 0.0;
      temp_local = t_night;
    } else {
      // In-transit asymmetric transition from evening to morning limb
      // Sigmoid transition near mid-transit (t ~ 0)
      double w_eve = 1.0 / (1.0 + std::exp((t_hr + 0.1) / 0.45));
      abs_fe = abs_eve * w_eve;

      // Strong day-to-night eastward jet blueshift (-11 km/s)
      doppler_v = -11.0 * w_eve;

      // Local limb temperature interpolation
      temp_local = t_night + (t_day - t_night) * w_eve;
    }

    out << t_hr << "," << abs_fe << "," << doppler_v << "," << temp_local << "\n";
  }
  out.close();

  std::cout << "Generated WASP-76b Iron Rain Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
