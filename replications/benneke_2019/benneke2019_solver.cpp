// C++ Standalone Replication Solver for Benneke et al. (2019) Nature Astronomy 3, 813
// Calls core library class hot_jupiter::Benneke2019SubNeptuneAtmosphere from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Benneke2019SubNeptuneAtmosphere model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 0.40; wave <= 5.00; wave += 0.05) {
    double depth_pct = model.k2_18b_joint_transmission_depth_pct(wave);
    out << wave << "," << depth_pct << "\n";
  }
  out.close();
  std::cout << "--> Wrote Benneke et al. (2019) K2-18b Joint Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_h2o_posterior_sweep(const std::string& output_csv) {
  Benneke2019SubNeptuneAtmosphere model;
  std::ofstream out(output_csv);
  out << "log10_xh2o,posterior_density\n";

  for (double log_h2o = -5.0; log_h2o <= -1.0; log_h2o += 0.1) {
    double prob = model.h2o_log_posterior_density(log_h2o);
    out << log_h2o << "," << prob << "\n";
  }
  out.close();
  std::cout << "--> Wrote Benneke et al. (2019) Water Abundance Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Benneke et al. (2019) C++ K2-18b Sub-Neptune Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/benneke_2019/sim_transmission_spectrum.csv");
  hot_jupiter::run_h2o_posterior_sweep("replications/benneke_2019/sim_h2o_posterior.csv");
  std::cout << "✅ Benneke et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
