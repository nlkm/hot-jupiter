// C++ Standalone Replication Solver for Jackson et al. (2017) AJ 154, 77
// Integrates 10-Gyr orbital decay, RLOF mass loss, and remnant core evolution.

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "rlof_engine.hpp"

namespace hot_jupiter {

void run_trajectory_simulation(const std::string& output_csv) {
  std::ofstream out(output_csv);
  out << "time_gyr,a_au,m_mjup,filling_factor,outcome\n";

  double a_init_au = 0.022;
  double m_init_jup = 1.0;
  CoupledRLOFIntegrator integrator(m_init_jup, a_init_au, 10.0);
  auto res = integrator.integrate(1.0e10);

  for (size_t k = 0; k < res.t_arr.size(); ++k) {
    out << res.t_arr[k] / 1.0e9 << "," << res.a_arr[k] / AU << ","
        << res.m_p_arr[k] / M_JUP << "," << res.filling_factor_arr[k] << ","
        << static_cast<int>(res.outcome) << "\n";
  }
  out.close();
  std::cout << "--> Wrote trajectory simulation to " << output_csv << std::endl;
}

void run_bifurcation_grid_sweep(const std::string& output_csv) {
  std::ofstream out(output_csv);
  out << "a_init_au,m_init_jup,outcome,final_m_earth\n";

  int n_a = 15;
  int n_m = 15;
  double a_min = 0.012, a_max = 0.038;
  double m_min = 0.3, m_max = 2.2;

  for (int i = 0; i < n_a; ++i) {
    double a_init = a_min + i * (a_max - a_min) / (n_a - 1);
    for (int j = 0; j < n_m; ++j) {
      double m_init = m_min + j * (m_max - m_min) / (n_m - 1);
      CoupledRLOFIntegrator integrator(m_init, a_init, 10.0);
      auto res = integrator.integrate(1.0e10);
      out << a_init << "," << m_init << "," << static_cast<int>(res.outcome) << ","
          << res.final_m_remnant_earth << "\n";
    }
  }
  out.close();
  std::cout << "--> Wrote 225-point 2D bifurcation sweep to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Replicating Jackson et al. (2017) Numerical Simulations ===" << std::endl;
  hot_jupiter::run_trajectory_simulation("replications/jackson_2017/sim_trajectory.csv");
  hot_jupiter::run_bifurcation_grid_sweep("replications/jackson_2017/sim_bifurcation_grid.csv");
  std::cout << "✅ Jackson et al. (2017) C++ Simulations Completed!" << std::endl;
  return 0;
}
