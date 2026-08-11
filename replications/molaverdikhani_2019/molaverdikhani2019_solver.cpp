// C++ Standalone Replication Solver for Molaverdikhani et al. (2019) A&A 630, A131
// Calls core library class hot_jupiter::Molaverdikhani2019CloudModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_cloud_transmission_sweep(const std::string& output_csv) {
  Molaverdikhani2019CloudModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth\n";

  for (double w = 0.30; w <= 5.00; w += 0.05) {
    double d = model.transmission_transit_depth(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Molaverdikhani et al. (2019) Cloud Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_rayleigh_slope_sweep(const std::string& output_csv) {
  Molaverdikhani2019CloudModel model;
  std::ofstream out(output_csv);
  out << "p_cloud_bar,rayleigh_slope\n";

  for (double logp = -4.0; logp <= 0.0; logp += 0.05) {
    double p = std::pow(10.0, logp);
    double s = model.rayleigh_slope(p);
    out << p << "," << s << "\n";
  }
  out.close();
  std::cout << "--> Wrote Molaverdikhani et al. (2019) Spectral Rayleigh Slope dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Molaverdikhani et al. (2019) C++ Cloud Extinction Solver ===" << std::endl;
  hot_jupiter::run_cloud_transmission_sweep("replications/molaverdikhani_2019/sim_cloud_transmission.csv");
  hot_jupiter::run_rayleigh_slope_sweep("replications/molaverdikhani_2019/sim_rayleigh_slope.csv");
  std::cout << "✅ Molaverdikhani et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
