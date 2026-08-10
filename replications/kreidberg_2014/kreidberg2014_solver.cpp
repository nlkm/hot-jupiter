// C++ Standalone Replication Solver for Kreidberg et al. (2014) Nature 505, 69
// Calls core library class hot_jupiter::Kreidberg2014CloudyAtmosphere from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_flat_spectrum_sweep(const std::string& output_csv) {
  Kreidberg2014CloudyAtmosphere model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 1.1; wave <= 1.7; wave += 0.02) {
    double depth = model.flat_cloud_deck_transit_depth_pct(wave);
    out << wave << "," << depth << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kreidberg et al. (2014) Flat Spectrum dataset to " << output_csv << std::endl;
}

void run_water_amplitude_sweep(const std::string& output_csv) {
  Kreidberg2014CloudyAtmosphere model;
  std::ofstream out(output_csv);
  out << "p_cloud_mbar,h2o_amplitude_ppm\n";

  for (double log_p = -2.0; log_p <= 2.0; log_p += 0.2) {
    double p_mbar = std::pow(10.0, log_p);
    double amp = model.water_feature_amplitude_ppm(p_mbar);
    out << p_mbar << "," << amp << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kreidberg et al. (2014) Water Feature Amplitude dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Kreidberg et al. (2014) C++ Cloudy Atmosphere Solver ===" << std::endl;
  hot_jupiter::run_flat_spectrum_sweep("replications/kreidberg_2014/sim_flat_spectrum.csv");
  hot_jupiter::run_water_amplitude_sweep("replications/kreidberg_2014/sim_water_amplitude.csv");
  std::cout << "✅ Kreidberg et al. (2014) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
