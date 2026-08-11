// C++ Standalone Replication Solver for May et al. (2021) AJ 162, 158
// Calls core library class hot_jupiter::May2021UltraHotPhaseCurveModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_wasp76b_sweep(const std::string& output_csv) {
  May2021UltraHotPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "phase,flux_ratio\n";

  for (double phi = -0.5; phi <= 0.5; phi += 0.01) {
    double f = model.wasp76b_flux_ratio(phi);
    out << phi << "," << f << "\n";
  }
  out.close();
  std::cout << "--> Wrote May et al. (2021) WASP-76b Phase Curve dataset to " << output_csv << std::endl;
}

void run_wasp121b_sweep(const std::string& output_csv) {
  May2021UltraHotPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "phase,flux_ratio\n";

  for (double phi = -0.5; phi <= 0.5; phi += 0.01) {
    double f = model.wasp121b_flux_ratio(phi);
    out << phi << "," << f << "\n";
  }
  out.close();
  std::cout << "--> Wrote May et al. (2021) WASP-121b Phase Curve dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== May et al. (2021) C++ Phase Curve Solver ===" << std::endl;
  hot_jupiter::run_wasp76b_sweep("replications/may_2021/sim_wasp76b_phase.csv");
  hot_jupiter::run_wasp121b_sweep("replications/may_2021/sim_wasp121b_phase.csv");
  std::cout << "✅ May et al. (2021) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
