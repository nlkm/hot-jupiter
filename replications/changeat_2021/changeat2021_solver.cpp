// C++ Standalone Replication Solver for Changeat et al. (2021) ApJ 913, 73
// Calls core library class hot_jupiter::Changeat2021Hd209458bModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_sweep(const std::string& output_csv) {
  Changeat2021Hd209458bModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth\n";

  for (double w = 0.35; w <= 5.00; w += 0.05) {
    double d = model.transmission_transit_depth(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Changeat et al. (2021) HD 209458b Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_hcn_posterior_sweep(const std::string& output_csv) {
  Changeat2021Hd209458bModel model;
  std::ofstream out(output_csv);
  out << "log10_xhcn,posterior_pdf\n";

  for (double logx = -7.0; logx <= -2.0; logx += 0.05) {
    double p = model.hcn_abundance_posterior(logx);
    out << logx << "," << p << "\n";
  }
  out.close();
  std::cout << "--> Wrote Changeat et al. (2021) HCN Abundance Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Changeat et al. (2021) C++ HD 209458b HCN Opacity Solver ===" << std::endl;
  hot_jupiter::run_transmission_sweep("replications/changeat_2021/sim_transmission_spectrum.csv");
  hot_jupiter::run_hcn_posterior_sweep("replications/changeat_2021/sim_hcn_posterior.csv");
  std::cout << "✅ Changeat et al. (2021) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
