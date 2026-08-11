// C++ Standalone Replication Solver for Kreidberg et al. (2018) AJ 156, 17
// Calls core library class hot_jupiter::Kreidberg2018Wasp103bPhaseCurveModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_phase_curve_sweep(const std::string& output_csv) {
  Kreidberg2018Wasp103bPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "phase,flux_ratio\n";

  for (double p = 0.0; p <= 1.0; p += 0.01) {
    double f = model.phase_curve_flux(p);
    out << p << "," << f << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kreidberg et al. (2018) WASP-103b Phase Curve dataset to " << output_csv << std::endl;
}

void run_lon_temp_sweep(const std::string& output_csv) {
  Kreidberg2018Wasp103bPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "lon_deg,temp_bright_k\n";

  for (double l = -180.0; l <= 180.0; l += 5.0) {
    double t = model.longitudinal_temperature(l);
    out << l << "," << t << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kreidberg et al. (2018) Longitudinal Temperature Profile dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Kreidberg et al. (2018) C++ WASP-103b Phase Curve & Climate Solver ===" << std::endl;
  hot_jupiter::run_phase_curve_sweep("replications/kreidberg_2018/sim_phase_curve.csv");
  hot_jupiter::run_lon_temp_sweep("replications/kreidberg_2018/sim_lon_temp.csv");
  std::cout << "✅ Kreidberg et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
