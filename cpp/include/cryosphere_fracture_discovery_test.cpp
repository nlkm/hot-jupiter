// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Unit tests for Frontier 6: Ocean-Freezing Pressurization & Cryosphere Fracture Engine

#include "cpp/include/cryosphere_fracture_discovery.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   TEST SUITE: CRYOSPHERE FRACTURE & OCEAN FREEZING DISCOVERY ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // Charon analog (R = 606 km, g = 0.288 m/s^2, density = 1700 kg/m^3)
  hot_jupiter::CryosphereFractureDiscoveryEngine engine(606.0, 0.288, 1700.0, 3.5, 2.0);

  // 1. Test Ice Viscosity & Maxwell Timescale
  double eta_260k = engine.IceViscosityPaS(260.0);
  double tau_m_yr = engine.MaxwellRelaxationTimeYears(260.0);
  assert(eta_260k > 1.0e13 && eta_260k < 1.0e17);
  assert(tau_m_yr > 1.0e-5 && tau_m_yr < 1.0e4);
  std::cout << "Ice viscosity at 260 K = " << eta_260k << " Pa s, tau_M = " << tau_m_yr << " yr" << std::endl;


  // 2. Test Ocean Overpressure Calculation
  double delta_p = engine.ComputeOceanOverpressureMPa(50.0, 100.0, 5.0);
  assert(delta_p > 0.1 && delta_p < 50.0);
  std::cout << "Ocean overpressure from freezing 5 km ice = " << delta_p << " MPa" << std::endl;

  // 3. Test Hoop Stress
  double hoop_stress = engine.ComputeSurfaceHoopStressMPa(delta_p, 50.0);
  assert(hoop_stress > delta_p);
  std::cout << "Surface hoop stress sigma_theta = " << hoop_stress << " MPa" << std::endl;

  // 4. Test Full Evolution & Charon Tensile Rupture (Canyon Formation)
  auto history = engine.EvolveFreezingCryosphere(30.0, 80.0, 120.0, 0.1, 500.0, 1.0);
  assert(!history.empty());

  auto failure = engine.ClassifyFailure(history);

  std::cout << "Simulated " << history.size() << " Myr of cryosphere cooling." << std::endl;
  std::cout << "Final Ice Thickness  = " << history.back().ice_shell_thickness_km << " km" << std::endl;
  std::cout << "Max Surface Stress   = " << history.back().surface_hoop_stress_mpa << " MPa" << std::endl;
  std::cout << "Fracture Status      = " << (history.back().is_fractured ? "FRACTURED (Canyons)" : "INTACT") << std::endl;

  assert(failure == hot_jupiter::CryosphereFailureMode::BRITTLE_TENSILE_RUPTURE);

  std::cout << "================================================================================" << std::endl;
  std::cout << "✅ ALL CRYOSPHERE FRACTURE DISCOVERY TESTS PASSED!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
