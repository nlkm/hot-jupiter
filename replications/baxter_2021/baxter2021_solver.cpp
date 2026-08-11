// C++ Standalone Replication Solver for Baxter et al. (2021) A&A 648, A127
// Calls core library class hot_jupiter::Baxter2021EclipseTransitionModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_36um_eclipse_sweep(const std::string& output_csv) {
  Baxter2021EclipseTransitionModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,decl_36um_ppm\n";

  for (double teq = 1500.0; teq <= 3000.0; teq += 50.0) {
    double d = model.eclipse_depth_36um_ppm(teq);
    out << teq << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Baxter et al. (2021) 3.6um Eclipse Depth dataset to " << output_csv << std::endl;
}

void run_45um_eclipse_sweep(const std::string& output_csv) {
  Baxter2021EclipseTransitionModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,decl_45um_ppm\n";

  for (double teq = 1500.0; teq <= 3000.0; teq += 50.0) {
    double d = model.eclipse_depth_45um_ppm(teq);
    out << teq << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Baxter et al. (2021) 4.5um Eclipse Depth dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Baxter et al. (2021) C++ Eclipse Transition Solver ===" << std::endl;
  hot_jupiter::run_36um_eclipse_sweep("replications/baxter_2021/sim_36um_eclipse.csv");
  hot_jupiter::run_45um_eclipse_sweep("replications/baxter_2021/sim_45um_eclipse.csv");
  std::cout << "✅ Baxter et al. (2021) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
