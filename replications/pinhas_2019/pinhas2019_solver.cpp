// C++ Standalone Replication Solver for Pinhas et al. (2019) MNRAS 482, 1485
// Calls core library class hot_jupiter::Pinhas2019WaterRetrieval from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Pinhas2019WaterRetrieval model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 0.35; wave <= 5.0; wave += 0.05) {
    double depth_pct = model.wasp31b_transmission_depth_pct(wave);
    out << wave << "," << depth_pct << "\n";
  }
  out.close();
  std::cout << "--> Wrote Pinhas et al. (2019) WASP-31b Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_water_abundance_sweep(const std::string& output_csv) {
  Pinhas2019WaterRetrieval model;
  std::ofstream out(output_csv);
  out << "teq_k,log10_h2o\n";

  double teqs[] = {800.0, 1100.0, 1300.0, 1550.0, 1800.0, 2000.0, 2200.0};
  for (double teq : teqs) {
    double log_h2o = model.log10_h2o_abundance(teq);
    out << teq << "," << log_h2o << "\n";
  }
  out.close();
  std::cout << "--> Wrote Pinhas et al. (2019) Water Abundance dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Pinhas et al. (2019) C++ 10 Hot Jupiters Water Retrieval Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/pinhas_2019/sim_transmission_spectrum.csv");
  hot_jupiter::run_water_abundance_sweep("replications/pinhas_2019/sim_water_abundance.csv");
  std::cout << "✅ Pinhas et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
