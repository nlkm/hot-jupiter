// C++ Standalone Replication Solver for Batalha et al. (2019) ApJ 878, 70
// Calls core library class hot_jupiter::Batalha2019PandExoNoiseModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_noise_precision_sweep(const std::string& output_csv) {
  Batalha2019PandExoNoiseModel model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,noise_precision_ppm\n";

  for (double wave = 2.8; wave <= 5.2; wave += 0.05) {
    double precision_ppm = model.noise_precision_ppm(wave);
    out << wave << "," << precision_ppm << "\n";
  }
  out.close();
  std::cout << "--> Wrote Batalha et al. (2019) Noise Precision dataset to " << output_csv << std::endl;
}

void run_snr_sweep(const std::string& output_csv) {
  Batalha2019PandExoNoiseModel model;
  std::ofstream out(output_csv);
  out << "magnitude_j,snr\n";

  for (double mag = 6.0; mag <= 12.0; mag += 0.2) {
    double snr = model.snr_per_bin(mag);
    out << mag << "," << snr << "\n";
  }
  out.close();
  std::cout << "--> Wrote Batalha et al. (2019) SNR dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Batalha et al. (2019) C++ PandExo JWST Noise Model Solver ===" << std::endl;
  hot_jupiter::run_noise_precision_sweep("replications/batalha_2019/sim_noise_precision.csv");
  hot_jupiter::run_snr_sweep("replications/batalha_2019/sim_snr.csv");
  std::cout << "✅ Batalha et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
