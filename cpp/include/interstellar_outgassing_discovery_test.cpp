// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Unit tests for Frontier 7: Interstellar Object Outgassing & Spin Disruption Engine

#include "cpp/include/interstellar_outgassing_discovery.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   TEST SUITE: INTERSTELLAR OUTGASSING & SPIN DISRUPTION DISCOVERY ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // 1I/'Oumuamua analog (R = 100 m, a/b = 6:1, rho = 300 kg/m^3, H2 ice)
  hot_jupiter::InterstellarOutgassingDiscoveryEngine oumuamua(
      100.0, 6.0, 300.0, 0.70, 10.0, hot_jupiter::VolatileIceType::H2_MOLECULAR_HYDROGEN);

  // 1. Test Sublimation Flux & Thermal Exhaust Velocity
  double z_1au = oumuamua.SublimationFluxKgM2S(1.0);
  double v_th = oumuamua.ThermalExhaustVelocityMS(280.0);
  assert(z_1au > 1.0e-4 && z_1au < 1.0e-1);
  assert(v_th > 500.0 && v_th < 2500.0);
  std::cout << "H2 sublimation flux at 1 AU = " << z_1au << " kg/m^2/s, v_th = " << v_th << " m/s" << std::endl;

  // 2. Test Non-Gravitational Acceleration at 1 AU
  double a_ng_1au = oumuamua.ComputeNonGravAcceleration(1.0, 0.25);
  assert(a_ng_1au > 1.0e-7 && a_ng_1au < 1.0e-4);
  std::cout << "Non-gravitational acceleration at 1 AU = " << a_ng_1au << " m/s^2" << std::endl;

  // 3. Test Flyby Simulation & Spin Evolution
  auto history = oumuamua.EvolveFlyby(0.255, 8.14, 60.0, 0.5);
  assert(!history.empty());
  std::cout << "Simulated " << history.size() << " steps across 'Oumuamua perihelion passage." << std::endl;
  std::cout << "Initial Spin Period = " << history.front().spin_period_hours << " hrs" << std::endl;
  std::cout << "Perihelion Accel   = " << history[history.size() / 2].non_grav_accel_m_s2 << " m/s^2" << std::endl;
  std::cout << "Final Spin Period   = " << history.back().spin_period_hours << " hrs" << std::endl;

  assert(history[history.size() / 2].non_grav_accel_m_s2 > a_ng_1au);

  std::cout << "================================================================================" << std::endl;
  std::cout << "✅ ALL INTERSTELLAR OUTGASSING DISCOVERY TESTS PASSED!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
