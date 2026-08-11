// C++ Standalone Replication Solver for Barman et al. (2015) ApJ 804, 61
// Calls core library class hot_jupiter::Barman2015HighResCorrelatorModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_vk_sweep(const std::string& output_csv) {
  Barman2015HighResCorrelatorModel model;
  std::ofstream out(output_csv);
  out << "vk_km_s,ccf_sn\n";

  for (double vk = 100.0; vk <= 180.0; vk += 2.0) {
    double sn = model.ccf_sn_vs_vk(vk);
    out << vk << "," << sn << "\n";
  }
  out.close();
  std::cout << "--> Wrote Barman et al. (2015) CCF S/N vs V_K dataset to " << output_csv << std::endl;
}

void run_vsys_sweep(const std::string& output_csv) {
  Barman2015HighResCorrelatorModel model;
  std::ofstream out(output_csv);
  out << "vsys_km_s,ccf_sn\n";

  for (double vsys = -100.0; vsys <= 100.0; vsys += 2.0) {
    double sn = model.ccf_sn_vs_vsys(vsys);
    out << vsys << "," << sn << "\n";
  }
  out.close();
  std::cout << "--> Wrote Barman et al. (2015) CCF S/N vs V_sys dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Barman et al. (2015) C++ High-Resolution Doppler Correlator Solver ===" << std::endl;
  hot_jupiter::run_vk_sweep("replications/barman_2015/sim_vk_sweep.csv");
  hot_jupiter::run_vsys_sweep("replications/barman_2015/sim_vsys_sweep.csv");
  std::cout << "✅ Barman et al. (2015) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
