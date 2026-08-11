// C++ Standalone Replication Solver for Arcangeli et al. (2018) ApJL 855, L30
// Calls core library class hot_jupiter::Arcangeli2018HMinerOpacityModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_wasp18b_emission_sweep(const std::string& output_csv) {
  Arcangeli2018HMinerOpacityModel model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,emission_wasp18b_ppm\n";

  for (double wave = 1.10; wave <= 1.70; wave += 0.005) {
    double em = model.emission_spectrum_wasp18b_ppm(wave);
    out << wave << "," << em << "\n";
  }
  out.close();
  std::cout << "--> Wrote Arcangeli et al. (2018) WASP-18b Emission Spectrum dataset to " << output_csv << std::endl;
}

void run_tp_profile_sweep(const std::string& output_csv) {
  Arcangeli2018HMinerOpacityModel model;
  std::ofstream out(output_csv);
  out << "log10_p_bar,temp_k\n";

  for (double log_p = -4.0; log_p <= 1.0; log_p += 0.1) {
    double temp = model.temperature_k(log_p);
    out << log_p << "," << temp << "\n";
  }
  out.close();
  std::cout << "--> Wrote Arcangeli et al. (2018) WASP-18b T-P Profile dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Arcangeli et al. (2018) C++ H- Opacity Solver ===" << std::endl;
  hot_jupiter::run_wasp18b_emission_sweep("replications/arcangeli_2018/sim_wasp18b_emission.csv");
  hot_jupiter::run_tp_profile_sweep("replications/arcangeli_2018/sim_tp_profile.csv");
  std::cout << "✅ Arcangeli et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
