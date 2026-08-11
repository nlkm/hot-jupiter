// C++ Standalone Replication Solver for Colón et al. (2020) AJ 160, 243
// Calls core library class hot_jupiter::Colon2020Wasp52bModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_optical_transmission_sweep(const std::string& output_csv) {
  Colon2020Wasp52bModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth\n";

  const double ref_w[9] = {0.42, 0.48, 0.54, 0.589, 0.64, 0.72, 0.767, 0.82, 0.88};
  for (int i = 0; i < 9; ++i) {
    out << ref_w[i] << "," << model.optical_transit_depth(ref_w[i]) << "\n";
  }

  for (double w = 0.42; w <= 0.88; w += 0.005) {
    double d = model.optical_transit_depth(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Colón et al. (2020) WASP-52b Optical Transmission dataset to " << output_csv << std::endl;
}

void run_na_posterior_sweep(const std::string& output_csv) {
  Colon2020Wasp52bModel model;
  std::ofstream out(output_csv);
  out << "log10_xna,prob_density\n";

  for (double logx = -6.0; logx <= -2.0; logx += 0.05) {
    double p = model.na_abundance_posterior(logx);
    out << logx << "," << p << "\n";
  }
  out.close();
  std::cout << "--> Wrote Colón et al. (2020) Na Abundance Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Colón et al. (2020) C++ WASP-52b Transmission Solver ===" << std::endl;
  hot_jupiter::run_optical_transmission_sweep("replications/colon_2020/sim_optical_transmission.csv");
  hot_jupiter::run_na_posterior_sweep("replications/colon_2020/sim_na_posterior.csv");
  std::cout << "✅ Colón et al. (2020) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
