// C++ Standalone Replication Solver for Line et al. (2021) Nature 598, 580
// Calls core library class hot_jupiter::Line2021Wasp77abModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_ccf_sweep(const std::string& output_csv) {
  Line2021Wasp77abModel model;
  std::ofstream out(output_csv);
  out << "vsys_kms,snr\n";

  for (double vsys = -40.0; vsys <= 0.0; vsys += 0.5) {
    double s = model.cross_correlation_snr(vsys);
    out << vsys << "," << s << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2021) Cross-Correlation SNR dataset to " << output_csv << std::endl;
}

void run_abundance_sweep(const std::string& output_csv) {
  Line2021Wasp77abModel model;
  std::ofstream out(output_csv);
  out << "log10_xh2o,posterior_pdf\n";

  for (double logx = -5.0; logx <= -2.0; logx += 0.05) {
    double p = model.water_abundance_posterior(logx);
    out << logx << "," << p << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2021) H2O Abundance Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Line et al. (2021) C++ High-Res Cross-Correlation Solver ===" << std::endl;
  hot_jupiter::run_ccf_sweep("replications/line_2021/sim_ccf_snr.csv");
  hot_jupiter::run_abundance_sweep("replications/line_2021/sim_h2o_posterior.csv");
  std::cout << "✅ Line et al. (2021) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
