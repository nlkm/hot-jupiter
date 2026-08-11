// C++ Standalone Replication Solver for Carone et al. (2020) A&A 638, A14
// Calls core library class hot_jupiter::Carone2020VerticalJetModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_wasp43b_jet_sweep(const std::string& output_csv) {
  Carone2020VerticalJetModel model;
  std::ofstream out(output_csv);
  out << "pressure_bar,zonal_wind_ms\n";

  for (double logp = -5.0; logp <= 2.0; logp += 0.1) {
    double p = std::pow(10.0, logp);
    double u = model.wasp43b_zonal_wind_ms(p);
    out << p << "," << u << "\n";
  }
  out.close();
  std::cout << "--> Wrote Carone et al. (2020) WASP-43b Zonal Wind dataset to " << output_csv << std::endl;
}

void run_hd209458b_jet_sweep(const std::string& output_csv) {
  Carone2020VerticalJetModel model;
  std::ofstream out(output_csv);
  out << "pressure_bar,zonal_wind_ms\n";

  for (double logp = -5.0; logp <= 2.0; logp += 0.1) {
    double p = std::pow(10.0, logp);
    double u = model.hd209458b_zonal_wind_ms(p);
    out << p << "," << u << "\n";
  }
  out.close();
  std::cout << "--> Wrote Carone et al. (2020) HD 209458b Zonal Wind dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Carone et al. (2020) C++ Vertical Jet Extension Solver ===" << std::endl;
  hot_jupiter::run_wasp43b_jet_sweep("replications/carone_2020/sim_wasp43b_jet.csv");
  hot_jupiter::run_hd209458b_jet_sweep("replications/carone_2020/sim_hd209458b_jet.csv");
  std::cout << "✅ Carone et al. (2020) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
