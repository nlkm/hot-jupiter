// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 7 Execution Driver: Interstellar Object Outgassing & Spin Disruption Engine

#include <iostream>
#include <iomanip>
#include <fstream>
#include "cpp/include/interstellar_outgassing_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   FRONTIER 7 DISCOVERY: INTERSTELLAR OBJECT OUTGASSING & SPIN DISRUPTION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // 1. 1I/'Oumuamua Flyby Simulation across perihelion (H2 iceberg model)
  hot_jupiter::InterstellarOutgassingDiscoveryEngine oumuamua_h2(
      100.0, 6.0, 300.0, 0.70, 10.0, hot_jupiter::VolatileIceType::H2_MOLECULAR_HYDROGEN);
  auto track_h2 = oumuamua_h2.EvolveFlyby(0.255, 8.14, 120.0, 0.2);

  std::ofstream out_oumuamua("discovery_campaigns/frontier_07_interstellar_outgassing/oumuamua_flyby_track.csv");
  out_oumuamua << "r_au,temp_k,sublimation_kg_m2_s,a_ng_m_s2,spin_period_hrs,centrifugal_stress_pa,is_disrupted\n";
  for (const auto& s : track_h2) {
    out_oumuamua << s.heliocentric_dist_au << "," << s.surface_temp_k << ","
                 << s.sublimation_rate_kg_m2_s << "," << s.non_grav_accel_m_s2 << ","
                 << s.spin_period_hours << "," << s.centrifugal_stress_pa << ","
                 << (s.is_tensile_disrupted ? 1 : 0) << "\n";
  }
  out_oumuamua.close();

  // 2. Comparison across Ice Compositions (H2, N2, CO, H2O)
  std::ofstream out_comp("discovery_campaigns/frontier_07_interstellar_outgassing/ice_composition_comparison.csv");
  out_comp << "r_au,a_ng_h2,a_ng_n2,a_ng_co,a_ng_h2o\n";

  hot_jupiter::InterstellarOutgassingDiscoveryEngine eng_n2(100.0, 6.0, 500.0, 0.60, 10.0, hot_jupiter::VolatileIceType::N2_NITROGEN);
  hot_jupiter::InterstellarOutgassingDiscoveryEngine eng_co(100.0, 6.0, 500.0, 0.60, 10.0, hot_jupiter::VolatileIceType::CO_CARBON_MONOXIDE);
  hot_jupiter::InterstellarOutgassingDiscoveryEngine eng_h2o(100.0, 6.0, 600.0, 0.50, 10.0, hot_jupiter::VolatileIceType::H2O_WATER);

  for (double r = 0.25; r <= 3.5; r += 0.05) {
    double a_h2 = oumuamua_h2.ComputeNonGravAcceleration(r, 0.20);
    double a_n2 = eng_n2.ComputeNonGravAcceleration(r, 0.20);
    double a_co = eng_co.ComputeNonGravAcceleration(r, 0.20);
    double a_h2o = eng_h2o.ComputeNonGravAcceleration(r, 0.20);
    out_comp << r << "," << a_h2 << "," << a_n2 << "," << a_co << "," << a_h2o << "\n";
  }
  out_comp.close();

  // 3. 2D Spin Disruption Phase Grid: Aspect Ratio (a/b) vs Tensile Strength (Pa)
  std::ofstream out_grid("discovery_campaigns/frontier_07_interstellar_outgassing/spin_disruption_grid.csv");
  out_grid << "aspect_ratio,tensile_strength_pa,is_disrupted,final_spin_hrs\n";

  for (double aspect = 1.0; aspect <= 10.0; aspect += 0.5) {
    for (double tensile = 0.5; tensile <= 50.0; tensile += 2.0) {
      hot_jupiter::InterstellarOutgassingDiscoveryEngine sim(100.0, aspect, 300.0, 0.70, tensile, hot_jupiter::VolatileIceType::H2_MOLECULAR_HYDROGEN);
      auto hist = sim.EvolveFlyby(0.255, 8.14, 120.0, 0.5);
      bool disrupted = hist.back().is_tensile_disrupted;
      out_grid << aspect << "," << tensile << "," << (disrupted ? 1 : 0) << "," << hist.back().spin_period_hours << "\n";
    }
  }
  out_grid.close();

  std::cout << "Successfully generated Interstellar Object Outgassing Tracks and Stability Grids!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
