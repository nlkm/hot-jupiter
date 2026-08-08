#include "population_synth.hpp"
#include <cassert>
#include <iostream>

int main() {
  std::cout << "--> Running C++ PopulationSynthesizer Unit Tests..." << std::endl;

  auto results = hot_jupiter::PopulationSynthesizer::run_monte_carlo_sweep(50, 0.1, 3.0, 0.015, 0.030, 2.0, 20.0, 123);
  assert(results.size() == 50);

  int num_disrupted = 0;
  int num_survived = 0;
  for (const auto& p : results) {
    assert(p.m_p_init_jup >= 0.1 && p.m_p_init_jup <= 3.0);
    assert(p.a_init_au >= 0.015 && p.a_init_au <= 0.030);
    if (p.outcome == 0) num_disrupted++;
    else num_survived++;
  }

  assert(num_survived > 0);
  std::cout << "  ✓ Population Monte Carlo Sweep (" << num_survived << " survived, " << num_disrupted << " disrupted) passed." << std::endl;
  std::cout << "✅ All C++ PopulationSynthesizer Unit Tests Passed!" << std::endl;
  return 0;
}
