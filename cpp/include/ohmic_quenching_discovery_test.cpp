// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Unit tests for Frontier 2: Hot Jupiter Ohmic Quenching Discovery Engine

#include "cpp/include/ohmic_quenching_discovery.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   TEST SUITE: OHMIC DISSIPATION & DYNAMO QUENCHING DISCOVERY ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::OhmicQuenchingDiscoveryEngine engine(5.0, 1.0);  // 5 Gauss, 1 M_Jup

  // 1. Test Electrical Conductivity Scaling with Temperature
  double sigma_1200 = engine.AtmosphericConductivity(1200.0);
  double sigma_2200 = engine.AtmosphericConductivity(2200.0);
  assert(sigma_2200 > sigma_1200);
  std::cout << "Atmospheric Conductivity at 1200 K = " << sigma_1200 << " S/m" << std::endl;
  std::cout << "Atmospheric Conductivity at 2200 K = " << sigma_2200 << " S/m" << std::endl;

  // 2. Test Lorentz Drag & Wind Deceleration
  double v_1200 = engine.SelfConsistentWindSpeed(1200.0, sigma_1200);
  double v_2500 = engine.SelfConsistentWindSpeed(2500.0, engine.AtmosphericConductivity(2500.0));
  std::cout << "Equatorial Jet Speed at 1200 K     = " << v_1200 << " m/s" << std::endl;
  std::cout << "Equatorial Jet Speed at 2500 K     = " << v_2500 << " m/s" << std::endl;
  // High ionization creates Lorentz drag that brakes the jet
  assert(v_2500 < v_1200);

  // 3. Test Ohmic Dissipation Power Peak (Non-monotonicity)
  double p_1200 = engine.OhmicDissipationPower(1200.0);
  double p_1800 = engine.OhmicDissipationPower(1800.0);
  double p_2600 = engine.OhmicDissipationPower(2600.0);
  std::cout << "Ohmic Dissipation Power at 1200 K  = " << p_1200 << " Watts" << std::endl;
  std::cout << "Ohmic Dissipation Power at 1800 K  = " << p_1800 << " Watts" << std::endl;
  std::cout << "Ohmic Dissipation Power at 2600 K  = " << p_2600 << " Watts" << std::endl;
  assert(p_1800 > p_1200);
  assert(p_1800 > p_2600);

  // 4. Test State Generation & Radius Inflation Curve
  auto states = engine.GenerateHeatingCurve(50);
  assert(states.size() == 50);

  std::cout << "================================================================================" << std::endl;
  std::cout << "✅ ALL OHMIC QUENCHING DISCOVERY TESTS PASSED!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
