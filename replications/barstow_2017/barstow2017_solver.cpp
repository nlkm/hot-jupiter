// C++ Standalone Replication Solver for Barstow et al. (2017) MNRAS 464, 1727
// Calls core library class hot_jupiter::Barstow2017RayleighRetrieval from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Barstow2017RayleighRetrieval model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 0.3; wave <= 1.8; wave += 0.02) {
    double depth = model.transmission_spectrum_depth_pct(wave);
    out << wave << "," << depth << "\n";
  }
  out.close();
  std::cout << "--> Wrote Barstow et al. (2017) Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_rayleigh_slope_sweep(const std::string& output_csv) {
  Barstow2017RayleighRetrieval model;
  std::ofstream out(output_csv);
  out << "p_cloud_mbar,rayleigh_gamma\n";

  for (double log_p = -2.0; log_p <= 2.0; log_p += 0.2) {
    double p_mbar = std::pow(10.0, log_p);
    double gamma = model.rayleigh_slope_index(p_mbar);
    out << p_mbar << "," << gamma << "\n";
  }
  out.close();
  std::cout << "--> Wrote Barstow et al. (2017) Rayleigh Slope dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Barstow et al. (2017) C++ Rayleigh Slope Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/barstow_2017/sim_transmission_spectrum.csv");
  hot_jupiter::run_rayleigh_slope_sweep("replications/barstow_2017/sim_rayleigh_slope.csv");
  std::cout << "✅ Barstow et al. (2017) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
