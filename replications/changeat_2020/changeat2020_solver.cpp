// C++ Standalone Replication Solver for Changeat et al. (2020) AJ 160, 80
// Calls core library class hot_jupiter::Changeat2020Kelt11bModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Changeat2020Kelt11bModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth\n";

  for (double w = 1.10; w <= 1.70; w += 0.01) {
    double d = model.transmission_transit_depth(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Changeat et al. (2020) Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_water_posterior_sweep(const std::string& output_csv) {
  Changeat2020Kelt11bModel model;
  std::ofstream out(output_csv);
  out << "log10_x_h2o,posterior_density\n";

  for (double x = -6.0; x <= -1.0; x += 0.05) {
    double p = model.water_posterior_density(x);
    out << x << "," << p << "\n";
  }
  out.close();
  std::cout << "--> Wrote Changeat et al. (2020) Water Abundance Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Changeat et al. (2020) C++ Retrieval Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/changeat_2020/sim_transmission_spectrum.csv");
  hot_jupiter::run_water_posterior_sweep("replications/changeat_2020/sim_water_posterior.csv");
  std::cout << "✅ Changeat et al. (2020) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
