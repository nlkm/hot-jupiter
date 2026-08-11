// C++ Standalone Replication Solver for Parmentier et al. (2018) A&A 617, A110
// Calls core library class hot_jupiter::Parmentier2018UltraHotJupiterAtmosphere from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_h2o_dissociation_sweep(const std::string& output_csv) {
  Parmentier2018UltraHotJupiterAtmosphere model;
  std::ofstream out(output_csv);
  out << "temp_k,log10_xh2o\n";

  for (double temp = 1500.0; temp <= 4000.0; temp += 50.0) {
    double log_x = model.log10_h2o_abundance(temp);
    out << temp << "," << log_x << "\n";
  }
  out.close();
  std::cout << "--> Wrote Parmentier et al. (2018) H2O Thermal Dissociation dataset to " << output_csv << std::endl;
}

void run_wasp121b_emission_sweep(const std::string& output_csv) {
  Parmentier2018UltraHotJupiterAtmosphere model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,emission_wasp121b_ppm\n";

  for (double wave = 1.10; wave <= 1.70; wave += 0.005) {
    double em = model.emission_spectrum_wasp121b_ppm(wave);
    out << wave << "," << em << "\n";
  }
  out.close();
  std::cout << "--> Wrote Parmentier et al. (2018) WASP-121b Emission Spectrum dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Parmentier et al. (2018) C++ Ultra-Hot Jupiter Solver ===" << std::endl;
  hot_jupiter::run_h2o_dissociation_sweep("replications/parmentier_2018/sim_h2o_dissociation.csv");
  hot_jupiter::run_wasp121b_emission_sweep("replications/parmentier_2018/sim_wasp121b_emission.csv");
  std::cout << "✅ Parmentier et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
