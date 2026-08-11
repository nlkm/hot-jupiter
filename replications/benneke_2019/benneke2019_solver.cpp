// C++ Standalone Replication Solver for Benneke et al. (2019) Nature Astronomy 3, 813
// Calls core library class hot_jupiter::Benneke2019K218bModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_k218b_spectrum_sweep(const std::string& output_csv) {
  Benneke2019K218bModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth\n";

  const double ref_w[8] = {1.15, 1.22, 1.30, 1.38, 1.44, 1.52, 1.60, 1.68};
  for (int i = 0; i < 8; ++i) {
    out << ref_w[i] << "," << model.transmission_spectrum(ref_w[i]) << "\n";
  }

  for (double w = 1.15; w <= 1.68; w += 0.005) {
    double d = model.transmission_spectrum(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Benneke et al. (2019) K2-18b Water Spectrum dataset to " << output_csv << std::endl;
}

void run_h2o_posterior_sweep(const std::string& output_csv) {
  Benneke2019K218bModel model;
  std::ofstream out(output_csv);
  out << "log10_xh2o,prob_density\n";

  for (double logx = -4.0; logx <= -1.0; logx += 0.05) {
    double p = model.h2o_abundance_posterior(logx);
    out << logx << "," << p << "\n";
  }
  out.close();
  std::cout << "--> Wrote Benneke et al. (2019) H2O Abundance Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Benneke et al. (2019) C++ K2-18b Water Spectrum Solver ===" << std::endl;
  hot_jupiter::run_k218b_spectrum_sweep("replications/benneke_2019/sim_k218b_spectrum.csv");
  hot_jupiter::run_h2o_posterior_sweep("replications/benneke_2019/sim_h2o_posterior.csv");
  std::cout << "✅ Benneke et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
