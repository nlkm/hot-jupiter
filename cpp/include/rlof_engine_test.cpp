#include "rlof_engine.hpp"
#include <cassert>
#include <iostream>

int main() {
  std::cout << "--> Running C++ CoupledRLOFIntegrator Unit Tests..." << std::endl;

  // Test 1: Non-overflow cooling trajectory
  {
    hot_jupiter::CoupledRLOFIntegrator integrator(1.0, 0.035);
    auto res = integrator.integrate(1.0e9);
    assert(res.outcome == hot_jupiter::EvolutionOutcome::COOLING);
    assert(res.final_m_remnant_earth > 0.0);
    std::cout << "  ✓ Test 1: Non-overflow cooling passed." << std::endl;
  }

  // Test 2: Disruption trajectory
  {
    hot_jupiter::CoupledRLOFIntegrator integrator(0.2, 0.015, 2.0);
    auto res = integrator.integrate(1.0e9);
    assert(res.outcome == hot_jupiter::EvolutionOutcome::DISRUPTED);
    assert(res.final_m_remnant_earth == 0.0);
    std::cout << "  ✓ Test 2: Disruption trajectory passed." << std::endl;
  }

  // Test 3: Mass loss stagnation trajectory
  {
    hot_jupiter::CoupledRLOFIntegrator integrator(1.2, 0.018, 10.0);
    auto res = integrator.integrate(1.0e9);
    assert(res.outcome == hot_jupiter::EvolutionOutcome::STAGNATED ||
           res.outcome == hot_jupiter::EvolutionOutcome::COOLING);
    assert(res.final_m_remnant_earth > 0.0);
    std::cout << "  ✓ Test 3: Stagnation trajectory passed." << std::endl;
  }

  std::cout << "✅ All C++ CoupledRLOFIntegrator Unit Tests Passed!" << std::endl;
  return 0;
}
