// C++ Standalone Replication Solver for Zhang & Showman (2018a) ApJ 866, 1
// Calls core library class hot_jupiter::Zhang2018aCirculationModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_superrotation_sweep(const std::string& output_csv) {
  Zhang2018aCirculationModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,ueq_ms\n";

  for (double teq = 400.0; teq <= 2400.0; teq += 50.0) {
    double ueq = model.equatorial_superrotation_ms(teq);
    out << teq << "," << ueq << "\n";
  }
  out.close();
  std::cout << "--> Wrote Zhang & Showman (2018a) Superrotation Speed dataset to " << output_csv << std::endl;
}

void run_contrast_amplitude_sweep(const std::string& output_csv) {
  Zhang2018aCirculationModel model;
  std::ofstream out(output_csv);
  out << "tau_drag_s,adn_flux\n";

  for (double logt = 3.0; logt <= 7.0; logt += 0.05) {
    double td = std::pow(10.0, logt);
    double adn = model.day_night_contrast_amplitude(td);
    out << td << "," << adn << "\n";
  }
  out.close();
  std::cout << "--> Wrote Zhang & Showman (2018a) Day-Night Contrast Amplitude dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Zhang & Showman (2018a) C++ Circulation Solver ===" << std::endl;
  hot_jupiter::run_superrotation_sweep("replications/zhang_2018a/sim_superrotation.csv");
  hot_jupiter::run_contrast_amplitude_sweep("replications/zhang_2018a/sim_contrast_amplitude.csv");
  std::cout << "✅ Zhang & Showman (2018a) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
