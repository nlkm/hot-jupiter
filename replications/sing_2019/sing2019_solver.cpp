// C++ Standalone Replication Solver for Sing et al. (2019) AJ 158, 91
// Calls core library class hot_jupiter::Sing2019Wasp121bModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_optical_transmission_sweep(const std::string& output_csv) {
  Sing2019Wasp121bModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth\n";

  const double ref_w[9] = {0.35, 0.45, 0.54, 0.589, 0.64, 0.72, 0.767, 0.85, 0.98};
  for (int i = 0; i < 9; ++i) {
    out << ref_w[i] << "," << model.optical_transmission_spectrum(ref_w[i]) << "\n";
  }

  for (double w = 0.35; w <= 0.98; w += 0.005) {
    double d = model.optical_transmission_spectrum(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Sing et al. (2019) WASP-121b Optical Transmission dataset to " << output_csv << std::endl;
}

void run_exospheric_line_sweep(const std::string& output_csv) {
  Sing2019Wasp121bModel model;
  std::ofstream out(output_csv);
  out << "v_km_s,line_excess\n";

  for (double v = -100.0; v <= 100.0; v += 2.0) {
    double e = model.exospheric_na_line_excess(v);
    out << v << "," << e << "\n";
  }
  out.close();
  std::cout << "--> Wrote Sing et al. (2019) Exospheric Na Line Excess dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Sing et al. (2019) C++ WASP-121b Exospheric Solver ===" << std::endl;
  hot_jupiter::run_optical_transmission_sweep("replications/sing_2019/sim_optical_transmission.csv");
  hot_jupiter::run_exospheric_line_sweep("replications/sing_2019/sim_exospheric_line.csv");
  std::cout << "✅ Sing et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
