// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Unit tests for Frontier 3: Ultra-Short-Period (USP) RLOF & Super-Mercury Formation

#include "cpp/include/usp_rlof_discovery.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   TEST SUITE: USP TIDAL DECAY & ROCHE LOBE STRIPPING DISCOVERY ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::USPRLOFDiscoveryEngine engine(1.0, 1.0, 1.0e-6);

  // 1. Test Roche Radius Calculation
  double a_roche = engine.RocheRadius(5.0, 1.6);
  assert(a_roche > 0.001 && a_roche < 0.03);
  std::cout << "Roche Radius for 5 M_Earth rocky planet = " << a_roche << " AU" << std::endl;


  // 2. Test Tidal Orbital Decay
  double da_dt = engine.TidalDecayRate(0.015, 5.0);
  assert(da_dt < 0.0);
  std::cout << "Tidal Decay Rate at 0.015 AU             = " << da_dt << " AU/Myr" << std::endl;

  // 3. Test RLOF Mass Loss Activation
  double mdot_inside = engine.RLOFMassLossRate(a_roche * 0.95, 5.0, 1.6);
  double mdot_outside = engine.RLOFMassLossRate(a_roche * 1.05, 5.0, 1.6);
  assert(mdot_inside > 0.0);
  assert(mdot_outside == 0.0);
  std::cout << "RLOF Mass Loss Rate inside Roche Lobe   = " << mdot_inside << " M_Earth/Myr" << std::endl;

  // 4. Test Coupled Evolution & Super-Mercury Remnant Formation
  // 10 M_Earth progenitor (4 M_Earth Iron Core + 6 M_Earth Silicate Mantle) starting at a = 0.018 AU
  auto history = engine.EvolveSystem(4.0, 6.0, 0.018, 2000.0, 0.5);
  assert(!history.empty());

  auto fate = engine.ClassifyFate(history);
  std::cout << "Evolution Steps Simulated                = " << history.size() << std::endl;
  std::cout << "Initial Total Mass                       = " << history.front().planet_mass_mearth << " M_Earth" << std::endl;
  std::cout << "Final Total Mass                         = " << history.back().planet_mass_mearth << " M_Earth" << std::endl;
  std::cout << "Final Mantle Mass                        = " << history.back().mantle_mass_mearth << " M_Earth" << std::endl;
  std::cout << "Final Core Mass                          = " << history.back().core_mass_mearth << " M_Earth" << std::endl;
  std::cout << "Final Period                             = " << history.back().orbital_period_hours << " hours" << std::endl;

  assert(fate == hot_jupiter::USPFate::STABLE_ROCHE_STRIPPED_REMNANT || fate == hot_jupiter::USPFate::STABLE_ORBITAL_PARKING);

  std::cout << "================================================================================" << std::endl;
  std::cout << "✅ ALL USP TIDAL RLOF DISCOVERY TESTS PASSED!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
