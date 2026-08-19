// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Unit tests for Frontier 5: Resonant Chain Stability & Chaos Discovery Engine

#include "cpp/include/resonant_chain_discovery.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   TEST SUITE: RESONANT CHAIN STABILITY & CHAOS DISCOVERY ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // TRAPPIST-1 analog (M_star = 0.09 M_sun, M1 = 1.0 M_E, M2 = 1.3 M_E, M3 = 0.9 M_E)
  hot_jupiter::ResonantChainDiscoveryEngine engine(0.09, 1.0, 1.3, 0.9);

  // 1. Test Resonance Width Calculation
  double w_32 = engine.ResonanceWidth(2.0, 1.0, 0.015, 1.3);
  assert(w_32 > 1.0e-5 && w_32 < 1.0e-2);
  std::cout << "3:2 MMR width delta_a at 0.015 AU = " << w_32 << " AU" << std::endl;

  // 2. Test Chirikov Critical Overlap Separation
  double delta_a_crit = engine.CriticalOverlapSeparation(0.015, 1.0, 1.3);
  assert(delta_a_crit > 1.0e-4 && delta_a_crit < 5.0e-3);
  std::cout << "Chirikov critical separation delta_a_crit = " << delta_a_crit << " AU" << std::endl;

  // 3. Test Equilibrium Eccentricity
  double e_eq = engine.EquilibriumEccentricity(100.0, 1.0);  // K = 100
  assert(e_eq > 0.01 && e_eq < 0.10);
  std::cout << "Equilibrium eccentricity e_eq (K = 100) = " << e_eq << std::endl;

  // 4. Test Resonant Chain Evolution & Capture
  auto history = engine.EvolveResonantChain(0.012, 0.018, 50.0, 100.0, 100.0, 0.1);
  assert(!history.empty());
  std::cout << "Simulated " << history.size() << " timesteps of resonant chain migration." << std::endl;
  std::cout << "Initial Period Ratio = " << history.front().period_ratio << std::endl;
  std::cout << "Final Period Ratio   = " << history.back().period_ratio << " (Target ~ 1.50)" << std::endl;

  assert(std::abs(history.back().period_ratio - 1.50) < 0.05);

  std::cout << "================================================================================" << std::endl;
  std::cout << "✅ ALL RESONANT CHAIN DISCOVERY TESTS PASSED!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
