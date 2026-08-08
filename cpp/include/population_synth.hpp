#ifndef HOT_JUPITER_POPULATION_SYNTH_HPP_
#define HOT_JUPITER_POPULATION_SYNTH_HPP_

#include <vector>
#include <random>
#include <cmath>
#include "rlof_engine.hpp"

namespace hot_jupiter {

struct SyntheticPlanetResult {
  double m_p_init_jup;
  double a_init_au;
  double m_core_earth;
  double final_m_remnant_earth;
  double z_bulk;
  int outcome;
};

class PopulationSynthesizer {
 public:
  static std::vector<SyntheticPlanetResult> run_monte_carlo_sweep(
      int num_planets,
      double m_min_jup = 0.1,
      double m_max_jup = 5.0,
      double a_min_au = 0.012,
      double a_max_au = 0.035,
      double m_core_min_earth = 1.0,
      double m_core_max_earth = 25.0,
      unsigned int seed = 42) {
    std::vector<SyntheticPlanetResult> results(num_planets);
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist_m(m_min_jup, m_max_jup);
    std::uniform_real_distribution<double> dist_a(a_min_au, a_max_au);
    std::uniform_real_distribution<double> dist_core(m_core_min_earth, m_core_max_earth);

    for (int i = 0; i < num_planets; ++i) {
      double m_init = dist_m(rng);
      double a_init = dist_a(rng);
      double m_core = dist_core(rng);

      CoupledRLOFIntegrator integrator(m_init, a_init, m_core);
      auto trajectory = integrator.integrate(5.0e9, 100);

      results[i] = SyntheticPlanetResult{
          m_init,
          a_init,
          m_core,
          trajectory.final_m_remnant_earth,
          trajectory.z_bulk,
          static_cast<int>(trajectory.outcome)
      };
    }
    return results;
  }
};

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_POPULATION_SYNTH_HPP_
