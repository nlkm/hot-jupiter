// C++ Standalone Replication Solver for Gandhi & Madhusudhan (2019) MNRAS 485, 5817
// Calls core library class hot_jupiter::Gandhi2019RetrievalModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_abundances_sweep(const std::string& output_csv) {
  Gandhi2019RetrievalModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,log10_x_h2o,log10_x_co\n";

  for (double teq = 1000.0; teq <= 3000.0; teq += 50.0) {
    double x_h2o = model.log10_x_h2o(teq);
    double x_co = model.log10_x_co(teq);
    out << teq << "," << x_h2o << "," << x_co << "\n";
  }
  out.close();
  std::cout << "--> Wrote Gandhi & Madhusudhan (2019) Volume Mixing Ratios dataset to " << output_csv << std::endl;
}

void run_co_ratio_sweep(const std::string& output_csv) {
  Gandhi2019RetrievalModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,co_ratio\n";

  for (double teq = 1000.0; teq <= 3000.0; teq += 50.0) {
    double co = model.co_ratio(teq);
    out << teq << "," << co << "\n";
  }
  out.close();
  std::cout << "--> Wrote Gandhi & Madhusudhan (2019) C/O Ratio dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Gandhi & Madhusudhan (2019) C++ Atmospheric Retrieval Solver ===" << std::endl;
  hot_jupiter::run_abundances_sweep("replications/gandhi_2019/sim_abundances.csv");
  hot_jupiter::run_co_ratio_sweep("replications/gandhi_2019/sim_co_ratio.csv");
  std::cout << "✅ Gandhi & Madhusudhan (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
