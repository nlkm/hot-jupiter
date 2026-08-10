// C++ Standalone Replication Solver for Parmentier et al. (2018) A&A 617, A110
// Calls core library class hot_jupiter::Parmentier2018ThermalRegimes from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_thermal_profile_sweep(const std::string& output_csv) {
  Parmentier2018ThermalRegimes model;
  std::ofstream out(output_csv);
  out << "pressure_bar,temperature_k\n";

  for (double log_p = -4.0; log_p <= 1.0; log_p += 0.2) {
    double p_bar = std::pow(10.0, log_p);
    double temp_k = model.temperature_k(p_bar);
    out << p_bar << "," << temp_k << "\n";
  }
  out.close();
  std::cout << "--> Wrote Parmentier et al. (2018) Thermal Profile dataset to " << output_csv << std::endl;
}

void run_contrast_sweep(const std::string& output_csv) {
  Parmentier2018ThermalRegimes model;
  std::ofstream out(output_csv);
  out << "t_eq_k,delta_tb_k\n";

  for (double t_eq = 1000.0; t_eq <= 3000.0; t_eq += 50.0) {
    double delta_tb = model.brightness_temperature_contrast_k(t_eq);
    out << t_eq << "," << delta_tb << "\n";
  }
  out.close();
  std::cout << "--> Wrote Parmentier et al. (2018) Brightness Temperature Contrast dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Parmentier et al. (2018) C++ Thermal Regimes Solver ===" << std::endl;
  hot_jupiter::run_thermal_profile_sweep("replications/parmentier_2018/sim_thermal_profile.csv");
  hot_jupiter::run_contrast_sweep("replications/parmentier_2018/sim_contrast.csv");
  std::cout << "✅ Parmentier et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
