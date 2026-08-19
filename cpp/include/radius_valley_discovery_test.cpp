// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Unit tests for Frontier 1: Radius Valley Discovery Engine

#include "cpp/include/radius_valley_discovery.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   TEST SUITE: RADIUS VALLEY DISCOVERY & POPULATION SYNTHESIS ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::RadiusValleyDiscoveryEngine engine;

  // 1. Test Photoevaporation Mass Loss Rate
  double mdot_photo = engine.PhotoevaporativeMassLossRate(5.0, 0.03, 0.05, 1.0, 0.05);
  assert(mdot_photo > 0.0);
  std::cout << "Photoevaporative Mass Loss Rate (Young G-dwarf) = " << mdot_photo << " M_Earth/Gyr" << std::endl;

  // 2. Test Core-Powered Mass Loss Rate
  double mdot_core = engine.CorePoweredMassLossRate(5.0, 0.03, 0.05, 1.0, 1.0);
  assert(mdot_core > 0.0);
  std::cout << "Core-Powered Mass Loss Rate (1 Gyr G-dwarf)      = " << mdot_core << " M_Earth/Gyr" << std::endl;

  // 3. Test Composite Planet Radius
  double r_bare = engine.ComputePlanetRadius(5.0, 0.0, 0.0, 0.05, 1.0, 5.0);
  double r_gaseous = engine.ComputePlanetRadius(5.0, 0.03, 0.0, 0.05, 1.0, 5.0);
  double r_water = engine.ComputePlanetRadius(5.0, 0.0, 0.50, 0.05, 1.0, 5.0);
  assert(r_gaseous > r_water);
  assert(r_water > r_bare);
  std::cout << "Bare Rock Radius (5 M_Earth)                    = " << r_bare << " R_Earth" << std::endl;
  std::cout << "Water World Radius (50% H2O)                    = " << r_water << " R_Earth" << std::endl;
  std::cout << "Gaseous Sub-Neptune (3% H/He)                   = " << r_gaseous << " R_Earth" << std::endl;

  // 4. Test Population Synthesis Generation
  auto pop_photo = engine.GeneratePopulation(500, hot_jupiter::ValleyMechanism::PHOTOEVAPORATION);
  auto pop_core = engine.GeneratePopulation(500, hot_jupiter::ValleyMechanism::CORE_POWERED_MASS_LOSS);
  auto pop_water = engine.GeneratePopulation(500, hot_jupiter::ValleyMechanism::PRIMORDIAL_WATER_WORLDS);
  assert(pop_photo.size() == 500);
  assert(pop_core.size() == 500);
  assert(pop_water.size() == 500);

  // 5. Test Valley Slope Signatures
  double slope_p_photo = engine.ValleySlopeDLogRDLogP(hot_jupiter::ValleyMechanism::PHOTOEVAPORATION);
  double slope_p_core = engine.ValleySlopeDLogRDLogP(hot_jupiter::ValleyMechanism::CORE_POWERED_MASS_LOSS);
  double slope_p_water = engine.ValleySlopeDLogRDLogP(hot_jupiter::ValleyMechanism::PRIMORDIAL_WATER_WORLDS);
  assert(std::abs(slope_p_photo - (-0.11)) < 0.01);
  assert(std::abs(slope_p_core - (-0.06)) < 0.01);
  assert(std::abs(slope_p_water - 0.00) < 0.01);

  double slope_m_photo = engine.ValleySlopeDLogRDLogMStar(hot_jupiter::ValleyMechanism::PHOTOEVAPORATION);
  double slope_m_core = engine.ValleySlopeDLogRDLogMStar(hot_jupiter::ValleyMechanism::CORE_POWERED_MASS_LOSS);
  assert(std::abs(slope_m_photo - 0.25) < 0.01);
  assert(std::abs(slope_m_core - 0.35) < 0.01);

  std::cout << "Valley Slope dlog(R)/dlog(P) [Photoevap]         = " << slope_p_photo << std::endl;
  std::cout << "Valley Slope dlog(R)/dlog(P) [Core-Powered]     = " << slope_p_core << std::endl;
  std::cout << "Valley Slope dlog(R)/dlog(M_star) [Photoevap]   = " << slope_m_photo << std::endl;
  std::cout << "Valley Slope dlog(R)/dlog(M_star) [Core-Powered]= " << slope_m_core << std::endl;

  std::cout << "================================================================================" << std::endl;
  std::cout << "✅ ALL RADIUS VALLEY DISCOVERY TESTS PASSED!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
