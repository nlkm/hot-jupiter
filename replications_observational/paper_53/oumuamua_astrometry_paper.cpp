// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #53: Interstellar Object 1I/'Oumuamua Non-Gravitational Acceleration Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/interstellar_outgassing_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #53: 1I/'OUMUAMUA NON-GRAVITATIONAL ACCELERATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // 1I/'Oumuamua: R_eff = 100 m, aspect ratio a/b = 6.0, bulk density = 300 kg/m^3, porosity = 0.75, tensile = 10 Pa
  hot_jupiter::InterstellarOutgassingDiscoveryEngine oumuamua(
      100.0, 6.0, 300.0, 0.75, 10.0, hot_jupiter::VolatileIceType::N2_NITROGEN);

  // Evolve trajectory across flyby
  auto states = oumuamua.EvolveFlyby(0.255, 8.14, 120.0, 0.5);


  std::ofstream out("replications_observational/paper_53/oumuamua_trajectory_track.csv");
  out << "r_au,temp_k,sublimation_kg_m2_s,non_grav_accel_m_s2,spin_hours,centrifugal_stress_pa,disrupted\n";
  for (const auto& s : states) {
    out << s.heliocentric_dist_au << "," << s.surface_temp_k << "," << s.sublimation_rate_kg_m2_s << ","
        << s.non_grav_accel_m_s2 << "," << s.spin_period_hours << ","
        << s.centrifugal_stress_pa << "," << (s.is_tensile_disrupted ? 1 : 0) << "\n";
  }
  out.close();

  std::cout << "Generated 1I/'Oumuamua non-gravitational trajectory simulation data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
