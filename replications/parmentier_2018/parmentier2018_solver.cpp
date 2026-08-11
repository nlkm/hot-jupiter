// C++ Standalone Replication Solver for Parmentier et al. (2018) A&A 617, A110
// Calls core library class hot_jupiter::Parmentier2018ColdTrapModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_fe_abundance_sweep(const std::string& output_csv) {
  Parmentier2018ColdTrapModel model;
  std::ofstream out(output_csv);
  out << "teq_k,log10_xfe\n";

  for (double t = 1500.0; t <= 3000.0; t += 25.0) {
    double x = model.fe_gas_abundance_log10(t);
    out << t << "," << x << "\n";
  }
  out.close();
  std::cout << "--> Wrote Parmentier et al. (2018) Gas-phase Fe Abundance dataset to " << output_csv << std::endl;
}

void run_phase_amplitude_sweep(const std::string& output_csv) {
  Parmentier2018ColdTrapModel model;
  std::ofstream out(output_csv);
  out << "teq_k,amp_ratio\n";

  for (double t = 1500.0; t <= 3000.0; t += 25.0) {
    double a = model.phase_curve_amplitude_ratio(t);
    out << t << "," << a << "\n";
  }
  out.close();
  std::cout << "--> Wrote Parmentier et al. (2018) Phase Curve Amplitude Ratio dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Parmentier et al. (2018) C++ Ultra-Hot Jupiter Cold Trap Solver ===" << std::endl;
  hot_jupiter::run_fe_abundance_sweep("replications/parmentier_2018/sim_fe_abundance.csv");
  hot_jupiter::run_phase_amplitude_sweep("replications/parmentier_2018/sim_phase_amplitude.csv");
  std::cout << "✅ Parmentier et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
