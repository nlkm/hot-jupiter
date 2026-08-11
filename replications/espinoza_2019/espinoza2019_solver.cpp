// C++ Standalone Replication Solver for Espinoza et al. (2019) MNRAS 482, 2065
// Calls core library class hot_jupiter::Espinoza2019ClearAtmosphere from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Espinoza2019ClearAtmosphere model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 0.40; wave <= 0.90; wave += 0.005) {
    double depth_pct = model.transmission_depth_pct(wave);
    out << wave << "," << depth_pct << "\n";
  }
  out.close();
  std::cout << "--> Wrote Espinoza et al. (2019) Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_sodium_posterior_sweep(const std::string& output_csv) {
  Espinoza2019ClearAtmosphere model;
  std::ofstream out(output_csv);
  out << "log10_xna,posterior_density\n";

  for (double log_xna = -7.0; log_xna <= -2.0; log_xna += 0.1) {
    double prob = model.na_log_posterior_density(log_xna);
    out << log_xna << "," << prob << "\n";
  }
  out.close();
  std::cout << "--> Wrote Espinoza et al. (2019) Sodium Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Espinoza et al. (2019) C++ WASP-19b Clear Atmosphere Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/espinoza_2019/sim_transmission_spectrum.csv");
  hot_jupiter::run_sodium_posterior_sweep("replications/espinoza_2019/sim_sodium_posterior.csv");
  std::cout << "✅ Espinoza et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
