// C++ Standalone Replication Solver for Kozai (1962) AJ 67, 591
// Computes Kozai-Lidov secular Hamiltonian phase space (omega, e) and e_max(i0).

#include <cmath>
#include <fstream>
#include <iostream>

#include "constants.hpp"
#include "orbital.hpp"

namespace hot_jupiter {

void run_kozai_phase_space(const std::string& output_csv) {
  std::ofstream out(output_csv);
  out << "omega_deg,eccentricity\n";

  double i0_rad = 65.0 * M_PI / 180.0;
  double hz = std::cos(i0_rad);  // H_z for e0 -> 0

  double e0 = 0.10;
  double hz2 = hz * hz;
  double c2i_0 = hz2 / (1.0 - e0 * e0);
  double s2i_0 = 1.0 - c2i_0;
  double theta_0 = (2.0 + 3.0 * e0 * e0) * (3.0 * c2i_0 - 1.0) + 15.0 * e0 * e0 * s2i_0;

  for (double deg = 0.0; deg <= 180.0; deg += 1.0) {
    double omega = deg * M_PI / 180.0;
    double cos_2w = std::cos(2.0 * omega);
    double best_e = 0.0;
    double min_diff = 1e9;

    for (double e_val = 0.01; e_val < 0.99; e_val += 0.001) {
      double c2i = hz2 / (1.0 - e_val * e_val);
      double s2i = 1.0 - c2i;
      if (s2i < 0.0) continue;
      double th = (2.0 + 3.0 * e_val * e_val) * (3.0 * c2i - 1.0) + 15.0 * e_val * e_val * s2i * cos_2w;
      double diff = std::abs(th - theta_0);
      if (diff < min_diff) {
        min_diff = diff;
        best_e = e_val;
      }
    }
    out << deg << "," << best_e << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kozai (1962) Phase Space dataset to " << output_csv << std::endl;
}

void run_max_eccentricity_sweep(const std::string& output_csv) {
  std::ofstream out(output_csv);
  out << "initial_inc_deg,max_eccentricity\n";

  for (double inc_deg = 39.23; inc_deg <= 90.0; inc_deg += 1.0) {
    double inc_rad = inc_deg * M_PI / 180.0;
    double cos_i0 = std::cos(inc_rad);
    double e_max = std::sqrt(1.0 - (5.0 / 3.0) * cos_i0 * cos_i0);
    out << inc_deg << "," << e_max << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kozai (1962) Max Eccentricity dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Kozai (1962) C++ Secular Dynamics Solver ===" << std::endl;
  hot_jupiter::run_kozai_phase_space("replications/kozai_1962/sim_phase_space.csv");
  hot_jupiter::run_max_eccentricity_sweep("replications/kozai_1962/sim_emax.csv");
  std::cout << "✅ Kozai (1962) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
