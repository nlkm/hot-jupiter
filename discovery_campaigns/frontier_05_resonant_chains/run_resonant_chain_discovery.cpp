// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 5 Execution Driver: Resonant Chain Stability & Chaos in Compact Systems

#include <iostream>
#include <iomanip>
#include <fstream>
#include "cpp/include/resonant_chain_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   FRONTIER 5 DISCOVERY: RESONANT CHAIN STABILITY & CHAOS ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::ResonantChainDiscoveryEngine engine(0.09, 1.0, 1.3, 0.9);

  // 1. High-resolution time series of resonant angles (TRAPPIST-1 b-c analog)
  auto track = engine.EvolveResonantChain(0.012, 0.018, 50.0, 100.0, 150.0, 0.1);
  std::ofstream out_track("discovery_campaigns/frontier_05_resonant_chains/resonant_evolution_track.csv");
  out_track << "time_kyr,a1_au,a2_au,e1,e2,period_ratio,resonant_angle_deg,laplace_angle_deg,is_librating\n";

  for (const auto& s : track) {
    out_track << s.time_kyr << "," << s.semimajor_axis_1_au << "," << s.semimajor_axis_2_au << ","
              << s.eccentricity_1 << "," << s.eccentricity_2 << "," << s.period_ratio << ","
              << s.resonant_angle_deg << "," << s.laplace_angle_deg << "," << (s.is_librating ? 1 : 0) << "\n";
  }
  out_track.close();

  // 2. 2D Stability Phase Space Grid: tau_mig vs K_damp = tau_mig / tau_e
  std::ofstream out_grid("discovery_campaigns/frontier_05_resonant_chains/chain_stability_grid.csv");
  out_grid << "tau_mig_kyr,k_damp,fate,final_period_ratio,final_e2\n";

  for (double tau_mig = 10.0; tau_mig <= 200.0; tau_mig += 10.0) {
    for (double k_damp = 5.0; k_damp <= 200.0; k_damp += 10.0) {
      auto hist = engine.EvolveResonantChain(0.012, 0.018, tau_mig, k_damp, 100.0, 0.2);
      auto fate = engine.ClassifyFate(hist);
      const auto& f = hist.back();
      std::string fate_str = (fate == hot_jupiter::ResonantChainFate::STABLE_RESONANT_LIBRATION) ? "stable_resonant" :
                             ((fate == hot_jupiter::ResonantChainFate::CHAOTIC_RESONANCE_OVERLAP) ? "chaotic_overlap" : "collision");
      out_grid << tau_mig << "," << k_damp << "," << fate_str << "," << f.period_ratio << "," << f.eccentricity_2 << "\n";
    }
  }
  out_grid.close();

  std::cout << "Successfully generated Resonant Chain Evolution Tracks and Stability Grid!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
