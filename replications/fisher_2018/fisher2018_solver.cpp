// C++ Standalone Replication Solver for Fisher & Heng (2018) MNRAS 481, 4698
// Calls core library class hot_jupiter::Fisher2018AnalyticalRetrieval from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Fisher2018AnalyticalRetrieval model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 0.35; wave <= 5.0; wave += 0.05) {
    double depth_pct = model.wasp12b_transmission_depth_pct(wave);
    out << wave << "," << depth_pct << "\n";
  }
  out.close();
  std::cout << "--> Wrote Fisher & Heng (2018) WASP-12b Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_scattering_index_sweep(const std::string& output_csv) {
  Fisher2018AnalyticalRetrieval model;
  std::ofstream out(output_csv);
  out << "teq_k,gamma\n";

  double teqs[] = {600.0, 1000.0, 1400.0, 1800.0, 2200.0, 2600.0, 2800.0};
  for (double teq : teqs) {
    double gamma = model.scattering_index_gamma(teq);
    out << teq << "," << gamma << "\n";
  }
  out.close();
  std::cout << "--> Wrote Fisher & Heng (2018) Scattering Index dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Fisher & Heng (2018) C++ 38 Hot Jupiters Retrieval Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/fisher_2018/sim_transmission_spectrum.csv");
  hot_jupiter::run_scattering_index_sweep("replications/fisher_2018/sim_scattering_index.csv");
  std::cout << "✅ Fisher & Heng (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
