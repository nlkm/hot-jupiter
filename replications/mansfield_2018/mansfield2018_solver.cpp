// C++ Standalone Replication Solver for Mansfield et al. (2018) AJ 156, 10
// Calls core library class hot_jupiter::Mansfield2018Wasp12bEmissionModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_emission_sweep(const std::string& output_csv) {
  Mansfield2018Wasp12bEmissionModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,flux_ratio\n";

  for (double w = 1.10; w <= 1.70; w += 0.01) {
    double f = model.emission_spectrum(w);
    out << w << "," << f << "\n";
  }
  out.close();
  std::cout << "--> Wrote Mansfield et al. (2018) WASP-12b Thermal Emission Spectrum dataset to " << output_csv << std::endl;
}

void run_brightness_temp_sweep(const std::string& output_csv) {
  Mansfield2018Wasp12bEmissionModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,temp_brightness_k\n";

  for (double w = 1.10; w <= 1.70; w += 0.01) {
    double t = model.brightness_temperature(w);
    out << w << "," << t << "\n";
  }
  out.close();
  std::cout << "--> Wrote Mansfield et al. (2018) WASP-12b Brightness Temperature Spectrum dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Mansfield et al. (2018) C++ WASP-12b Thermal Emission Solver ===" << std::endl;
  hot_jupiter::run_emission_sweep("replications/mansfield_2018/sim_emission.csv");
  hot_jupiter::run_brightness_temp_sweep("replications/mansfield_2018/sim_brightness_temp.csv");
  std::cout << "✅ Mansfield et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
