// C++ Standalone Replication Solver for Kempton et al. (2018) PASP 130, 114401
// Calls core library class hot_jupiter::Kempton2018AtmosphericMetricsModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_tsm_sweep(const std::string& output_csv) {
  Kempton2018AtmosphericMetricsModel model;
  std::ofstream out(output_csv);
  out << "r_planet_earth,tsm\n";

  for (double rp = 1.0; rp <= 15.0; rp += 0.2) {
    double tsm = model.transmission_spectroscopy_metric(rp);
    out << rp << "," << tsm << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kempton et al. (2018) TSM dataset to " << output_csv << std::endl;
}

void run_esm_sweep(const std::string& output_csv) {
  Kempton2018AtmosphericMetricsModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,esm\n";

  for (double teq = 400.0; teq <= 2500.0; teq += 50.0) {
    double esm = model.emission_spectroscopy_metric(teq);
    out << teq << "," << esm << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kempton et al. (2018) ESM dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Kempton et al. (2018) C++ TSM/ESM Solver ===" << std::endl;
  hot_jupiter::run_tsm_sweep("replications/kempton_2018/sim_tsm.csv");
  hot_jupiter::run_esm_sweep("replications/kempton_2018/sim_esm.csv");
  std::cout << "✅ Kempton et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
