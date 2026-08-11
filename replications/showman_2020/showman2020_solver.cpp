// C++ Standalone Replication Solver for Showman et al. (2020) ApJ 891, 78
// Calls core library class hot_jupiter::Showman2020UltraHotPhaseCurveModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_amplitude_sweep(const std::string& output_csv) {
  Showman2020UltraHotPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,aphase\n";

  for (double teq = 1500.0; teq <= 3200.0; teq += 50.0) {
    double a = model.phase_amplitude(teq);
    out << teq << "," << a << "\n";
  }
  out.close();
  std::cout << "--> Wrote Showman et al. (2020) Phase Curve Amplitude dataset to " << output_csv << std::endl;
}

void run_offset_sweep(const std::string& output_csv) {
  Showman2020UltraHotPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,hotspot_offset_deg\n";

  for (double teq = 1500.0; teq <= 3200.0; teq += 50.0) {
    double o = model.hotspot_offset_deg(teq);
    out << teq << "," << o << "\n";
  }
  out.close();
  std::cout << "--> Wrote Showman et al. (2020) Hotspot Offset dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Showman et al. (2020) C++ Ultra-Hot Phase Curve Solver ===" << std::endl;
  hot_jupiter::run_amplitude_sweep("replications/showman_2020/sim_phase_amplitude.csv");
  hot_jupiter::run_offset_sweep("replications/showman_2020/sim_hotspot_offset.csv");
  std::cout << "✅ Showman et al. (2020) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
