// C++ Standalone Replication Solver for Barstow et al. (2017) MNRAS 464, 1728
// Calls core library class hot_jupiter::Barstow2017ConsistentRetrieval from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Barstow2017ConsistentRetrieval model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 0.35; wave <= 5.0; wave += 0.05) {
    double depth_pct = model.hd209458b_transmission_depth_pct(wave);
    out << wave << "," << depth_pct << "\n";
  }
  out.close();
  std::cout << "--> Wrote Barstow et al. (2017) HD 209458b Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_cloud_pressure_sweep(const std::string& output_csv) {
  Barstow2017ConsistentRetrieval model;
  std::ofstream out(output_csv);
  out << "teq_k,log10_pcloud_bar\n";

  double teqs[] = {950.0, 1100.0, 1200.0, 1450.0, 1600.0, 1750.0, 2200.0};
  for (double teq : teqs) {
    double p_cloud = model.log10_cloud_pressure_bar(teq);
    out << teq << "," << p_cloud << "\n";
  }
  out.close();
  std::cout << "--> Wrote Barstow et al. (2017) Cloud Pressure dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Barstow et al. (2017) C++ 10 Hot Jupiters Retrieval Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/barstow_2017/sim_transmission_spectrum.csv");
  hot_jupiter::run_cloud_pressure_sweep("replications/barstow_2017/sim_cloud_pressure.csv");
  std::cout << "✅ Barstow et al. (2017) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
