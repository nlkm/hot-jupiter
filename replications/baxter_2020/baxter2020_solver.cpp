// C++ Standalone Replication Solver for Baxter et al. (2020) A&A 639, A36
// Calls core library class hot_jupiter::Baxter2020UltraHotPopulationModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_inversion_sweep(const std::string& output_csv) {
  Baxter2020UltraHotPopulationModel model;
  std::ofstream out(output_csv);
  out << "teq_k,delta_t_inv_k\n";

  for (double t = 1500.0; t <= 3400.0; t += 25.0) {
    double dt = model.delta_t_inversion(t);
    out << t << "," << dt << "\n";
  }
  out.close();
  std::cout << "--> Wrote Baxter et al. (2020) Population Thermal Inversion dataset to " << output_csv << std::endl;
}

void run_tbright_36_sweep(const std::string& output_csv) {
  Baxter2020UltraHotPopulationModel model;
  std::ofstream out(output_csv);
  out << "teq_k,t_bright_36_k\n";

  for (double t = 1500.0; t <= 3400.0; t += 25.0) {
    double tb = model.t_bright_36_k(t);
    out << t << "," << tb << "\n";
  }
  out.close();
  std::cout << "--> Wrote Baxter et al. (2020) Dayside 3.6 um Brightness Temperature dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Baxter et al. (2020) C++ Ultra-Hot Population Solver ===" << std::endl;
  hot_jupiter::run_inversion_sweep("replications/baxter_2020/sim_inversion.csv");
  hot_jupiter::run_tbright_36_sweep("replications/baxter_2020/sim_tbright_36.csv");
  std::cout << "✅ Baxter et al. (2020) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
