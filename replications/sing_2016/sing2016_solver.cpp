// C++ Standalone Replication Solver for Sing et al. (2016) Nature 529, 59
// Calls core library class hot_jupiter::Sing2016TransmissionContinuum from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Sing2016TransmissionContinuum model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 0.3; wave <= 5.0; wave += 0.05) {
    double depth_pct = model.transmission_depth_pct(wave);
    out << wave << "," << depth_pct << "\n";
  }
  out.close();
  std::cout << "--> Wrote Sing et al. (2016) Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_water_amplitude_sweep(const std::string& output_csv) {
  Sing2016TransmissionContinuum model;
  std::ofstream out(output_csv);
  out << "planet_index,water_amplitude_h\n";

  for (double p_idx = 1.0; p_idx <= 10.0; p_idx += 0.5) {
    double amp_h = model.water_amplitude_scale_heights(p_idx);
    out << p_idx << "," << amp_h << "\n";
  }
  out.close();
  std::cout << "--> Wrote Sing et al. (2016) Water Amplitude dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Sing et al. (2016) C++ Transmission Continuum Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/sing_2016/sim_transmission_spectrum.csv");
  hot_jupiter::run_water_amplitude_sweep("replications/sing_2016/sim_water_amplitude.csv");
  std::cout << "✅ Sing et al. (2016) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
