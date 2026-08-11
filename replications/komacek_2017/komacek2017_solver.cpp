// C++ Standalone Replication Solver for Komacek et al. (2017) ApJ 835, 198
// Calls core library class hot_jupiter::Komacek2017PhaseCurvePopulationModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_phase_amplitude_sweep(const std::string& output_csv) {
  Komacek2017PhaseCurvePopulationModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,a_obs_amplitude\n";

  for (double teq = 1000.0; teq <= 3000.0; teq += 50.0) {
    double amp = model.observed_phase_amplitude(teq);
    out << teq << "," << amp << "\n";
  }
  out.close();
  std::cout << "--> Wrote Komacek et al. (2017) Observed Phase Curve Amplitude dataset to " << output_csv << std::endl;
}

void run_phase_offset_sweep(const std::string& output_csv) {
  Komacek2017PhaseCurvePopulationModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,delta_phi_offset_deg\n";

  for (double teq = 1000.0; teq <= 3000.0; teq += 50.0) {
    double offset = model.phase_offset_deg(teq);
    out << teq << "," << offset << "\n";
  }
  out.close();
  std::cout << "--> Wrote Komacek et al. (2017) Phase Curve Peak Offset dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Komacek et al. (2017) C++ Observational Phase Curve Solver ===" << std::endl;
  hot_jupiter::run_phase_amplitude_sweep("replications/komacek_2017/sim_phase_amplitude.csv");
  hot_jupiter::run_phase_offset_sweep("replications/komacek_2017/sim_phase_offset.csv");
  std::cout << "✅ Komacek et al. (2017) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
