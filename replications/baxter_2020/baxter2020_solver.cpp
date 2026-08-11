// C++ Standalone Replication Solver for Baxter et al. (2020) A&A 639, A36
// Calls core library class hot_jupiter::Baxter2020UltraHotPopulationModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_tbright_sweep(const std::string& output_csv) {
  Baxter2020UltraHotPopulationModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,tbright_36_k,tbright_45_k\n";

  for (double teq = 2200.0; teq <= 4000.0; teq += 50.0) {
    double tb36 = model.t_bright_36_k(teq);
    double tb45 = model.t_bright_45_k(teq);
    out << teq << "," << tb36 << "," << tb45 << "\n";
  }
  out.close();
  std::cout << "--> Wrote Baxter et al. (2020) Brightness Temperature dataset to " << output_csv << std::endl;
}

void run_delta_tbright_sweep(const std::string& output_csv) {
  Baxter2020UltraHotPopulationModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,delta_tbright_k\n";

  for (double teq = 2200.0; teq <= 4000.0; teq += 50.0) {
    double delta_tb = model.delta_t_bright_k(teq);
    out << teq << "," << delta_tb << "\n";
  }
  out.close();
  std::cout << "--> Wrote Baxter et al. (2020) Delta Brightness Temperature dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Baxter et al. (2020) C++ Ultra-Hot Jupiter Population Solver ===" << std::endl;
  hot_jupiter::run_tbright_sweep("replications/baxter_2020/sim_tbright.csv");
  hot_jupiter::run_delta_tbright_sweep("replications/baxter_2020/sim_delta_tbright.csv");
  std::cout << "✅ Baxter et al. (2020) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
