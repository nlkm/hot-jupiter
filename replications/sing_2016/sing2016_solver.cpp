// C++ Standalone Replication Solver for Sing et al. (2016) Nature 529, 59
// Calls core library class hot_jupiter::Sing2016CloudContinuumModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_wasp12b_spectrum_sweep(const std::string& output_csv) {
  Sing2016CloudContinuumModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth\n";

  const double ref_w[9] = {0.35, 0.55, 0.85, 1.15, 1.40, 1.80, 2.50, 3.50, 5.00};
  for (int i = 0; i < 9; ++i) {
    out << ref_w[i] << "," << model.transmission_spectrum(ref_w[i]) << "\n";
  }

  for (double w = 0.35; w <= 5.00; w += 0.05) {
    double d = model.transmission_spectrum(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Sing et al. (2016) WASP-12b Transmission dataset to " << output_csv << std::endl;
}

void run_water_amplitude_sweep(const std::string& output_csv) {
  Sing2016CloudContinuumModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,delta_depth_14um\n";

  for (double teq = 1000.0; teq <= 2500.0; teq += 50.0) {
    double a = model.water_amplitude_14um(teq);
    out << teq << "," << a << "\n";
  }
  out.close();
  std::cout << "--> Wrote Sing et al. (2016) Water Amplitude dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Sing et al. (2016) C++ Transmission Continuum Solver ===" << std::endl;
  hot_jupiter::run_wasp12b_spectrum_sweep("replications/sing_2016/sim_wasp12b_spectrum.csv");
  hot_jupiter::run_water_amplitude_sweep("replications/sing_2016/sim_water_amplitude.csv");
  std::cout << "✅ Sing et al. (2016) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
