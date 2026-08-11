// C++ Standalone Replication Solver for Madhusudhan et al. (2014) ApJ Letters 791, L9
// Calls core library class hot_jupiter::Madhusudhan2014CoRatioModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_h2o_abundance_sweep(const std::string& output_csv) {
  Madhusudhan2014CoRatioModel model;
  std::ofstream out(output_csv);
  out << "log10_xh2o,prob_density\n";

  for (double logx = -6.0; logx <= -2.0; logx += 0.05) {
    double p = model.h2o_abundance_posterior(logx);
    out << logx << "," << p << "\n";
  }
  out.close();
  std::cout << "--> Wrote Madhusudhan et al. (2014) H2O Abundance Posterior dataset to " << output_csv << std::endl;
}

void run_co_ratio_sweep(const std::string& output_csv) {
  Madhusudhan2014CoRatioModel model;
  std::ofstream out(output_csv);
  out << "co_ratio,prob_density\n";

  for (double co = 0.1; co <= 1.2; co += 0.02) {
    double p = model.co_ratio_posterior(co);
    out << co << "," << p << "\n";
  }
  out.close();
  std::cout << "--> Wrote Madhusudhan et al. (2014) C/O Ratio Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Madhusudhan et al. (2014) C++ Hot Jupiter Water & C/O Ratio Solver ===" << std::endl;
  hot_jupiter::run_h2o_abundance_sweep("replications/madhusudhan_2014/sim_h2o_abundance.csv");
  hot_jupiter::run_co_ratio_sweep("replications/madhusudhan_2014/sim_co_ratio.csv");
  std::cout << "✅ Madhusudhan et al. (2014) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
