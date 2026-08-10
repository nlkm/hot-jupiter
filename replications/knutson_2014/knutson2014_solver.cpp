// C++ Standalone Replication Solver for Knutson et al. (2014) ApJ 785, 126
// Calls core library class hot_jupiter::Knutson2014HighMetallicityAtmosphere from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Knutson2014HighMetallicityAtmosphere model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 1.1; wave <= 1.7; wave += 0.02) {
    double depth = model.transmission_spectrum_depth_pct(wave);
    out << wave << "," << depth << "\n";
  }
  out.close();
  std::cout << "--> Wrote Knutson et al. (2014) Flat Spectrum dataset to " << output_csv << std::endl;
}

void run_metallicity_dampening_sweep(const std::string& output_csv) {
  Knutson2014HighMetallicityAtmosphere model;
  std::ofstream out(output_csv);
  out << "metallicity_dex,h2o_amplitude_ppm\n";

  for (double dex = 0.0; dex <= 3.0; dex += 0.1) {
    double amp = model.water_feature_amplitude_ppm(dex);
    out << dex << "," << amp << "\n";
  }
  out.close();
  std::cout << "--> Wrote Knutson et al. (2014) Metallicity Dampening dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Knutson et al. (2014) C++ HD 97658b Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/knutson_2014/sim_transmission_spectrum.csv");
  hot_jupiter::run_metallicity_dampening_sweep("replications/knutson_2014/sim_metallicity_dampening.csv");
  std::cout << "✅ Knutson et al. (2014) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
