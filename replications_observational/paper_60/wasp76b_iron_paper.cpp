// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #60: WASP-76b Asymmetric Iron Condensation & Rain Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #60: WASP-76b ASYMMETRIC IRON CONDENSATION & RAIN" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP76bIronRainModel model;

  const double t_day = model.dayside_temp_k();          // 2500 K
  const double t_night = model.nightside_temp_k();      // 1400 K
  const double t_condense = model.iron_condensation_temp_k(); // 1800 K
  const double fe_evening = model.evening_terminator_fe_absorption_percent(); // 0.45% (4500 ppm)


  // Track Neutral Iron (Fe I) absorption across transit orbital phase (-0.03 to +0.03)
  // Phase < 0 probes leading/morning terminator (condensed, depleted Fe)
  // Phase > 0 probes trailing/evening terminator (hot, vaporized Fe from dayside)
  std::ofstream out("replications_observational/paper_60/wasp76b_fe_transit_track.csv");
  out << "orbital_phase,doppler_shift_km_s,fe_absorption_ppm,temperature_k\n";

  for (double phi = -0.030; phi <= +0.030; phi += 0.002) {
    // Doppler velocity blueshift across transit (equatorial jet ~ -5 km/s + orbital v)
    double v_dop = -5.0 + 110.0 * std::sin(2.0 * M_PI * phi);

    // Terminator temperature transition
    double temp_local = t_night + (t_day - t_night) / (1.0 + std::exp(-phi / 0.008));

    // Fe I gas fraction vanishes when T < 1800 K (condenses into liquid iron droplets)
    double fe_ppm = 0.0;
    if (temp_local > t_condense) {
      fe_ppm = (fe_evening * 1.0e4) * (1.0 - std::exp(-(temp_local - t_condense) / 200.0));
    }

    out << phi << "," << v_dop << "," << fe_ppm << "," << temp_local << "\n";
  }
  out.close();

  std::cout << "Generated WASP-76b Asymmetric Iron Condensation simulation data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
