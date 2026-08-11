// C++ Standalone Replication Solver for Kreidberg et al. (2014) Nature 505, 69
// Calls core library class hot_jupiter::Kreidberg2014Gj1214bModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_gj1214b_spectrum_sweep(const std::string& output_csv) {
  Kreidberg2014Gj1214bModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth\n";

  const double ref_w[8] = {1.12, 1.18, 1.25, 1.32, 1.40, 1.48, 1.55, 1.65};
  for (int i = 0; i < 8; ++i) {
    out << ref_w[i] << "," << model.transmission_spectrum(ref_w[i]) << "\n";
  }

  for (double w = 1.12; w <= 1.65; w += 0.005) {
    double d = model.transmission_spectrum(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kreidberg et al. (2014) GJ 1214b Spectrum dataset to " << output_csv << std::endl;
}

void run_chi2_dof_sweep(const std::string& output_csv) {
  Kreidberg2014Gj1214bModel model;
  std::ofstream out(output_csv);
  out << "log10_p_cloud_bar,chi2_dof\n";

  for (double logp = -5.0; logp <= 0.0; logp += 0.1) {
    double c = model.model_chi2_dof(logp);
    out << logp << "," << c << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kreidberg et al. (2014) Chi2/dof Rejection dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Kreidberg et al. (2014) C++ GJ 1214b Super-Earth Solver ===" << std::endl;
  hot_jupiter::run_gj1214b_spectrum_sweep("replications/kreidberg_2014/sim_gj1214b_spectrum.csv");
  hot_jupiter::run_chi2_dof_sweep("replications/kreidberg_2014/sim_chi2_dof.csv");
  std::cout << "✅ Kreidberg et al. (2014) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
