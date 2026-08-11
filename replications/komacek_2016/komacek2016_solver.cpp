// C++ Standalone Replication Solver for Komacek & Showman (2016) ApJ 821, 16
// Calls core library class hot_jupiter::Komacek2016ThermalContrastModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_teq_contrast_sweep(const std::string& output_csv) {
  Komacek2016ThermalContrastModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,a_weak_drag,a_interm_drag,a_strong_drag\n";

  for (double teq = 1000.0; teq <= 3000.0; teq += 50.0) {
    double a_w = model.thermal_contrast_amplitude(teq, 0.01);
    double a_m = model.thermal_contrast_amplitude(teq, 1.0);
    double a_s = model.thermal_contrast_amplitude(teq, 100.0);
    out << teq << "," << a_w << "," << a_m << "," << a_s << "\n";
  }
  out.close();
  std::cout << "--> Wrote Komacek & Showman (2016) Teq Contrast dataset to " << output_csv << std::endl;
}

void run_gamma_drag_sweep(const std::string& output_csv) {
  Komacek2016ThermalContrastModel model;
  std::ofstream out(output_csv);
  out << "gamma_drag,a_contrast\n";

  for (double log_g = -2.0; log_g <= 2.0; log_g += 0.05) {
    double gamma = std::pow(10.0, log_g);
    double a = model.thermal_contrast_amplitude(2000.0, gamma);
    out << gamma << "," << a << "\n";
  }
  out.close();
  std::cout << "--> Wrote Komacek & Showman (2016) Wave Drag Contrast dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Komacek & Showman (2016) C++ Wave Drag Thermal Contrast Solver ===" << std::endl;
  hot_jupiter::run_teq_contrast_sweep("replications/komacek_2016/sim_teq_contrast.csv");
  hot_jupiter::run_gamma_drag_sweep("replications/komacek_2016/sim_gamma_drag.csv");
  std::cout << "✅ Komacek & Showman (2016) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
