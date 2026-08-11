// C++ Standalone Replication Solver for Welbanks et al. (2019) ApJL 887, L20
// Calls core library class hot_jupiter::Welbanks2019WaterDepletion from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Welbanks2019WaterDepletion model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 0.35; wave <= 5.0; wave += 0.05) {
    double depth_pct = model.wasp127b_transmission_depth_pct(wave);
    out << wave << "," << depth_pct << "\n";
  }
  out.close();
  std::cout << "--> Wrote Welbanks et al. (2019) WASP-127b Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_mass_metallicity_sweep(const std::string& output_csv) {
  Welbanks2019WaterDepletion model;
  std::ofstream out(output_csv);
  out << "planet_mass_earth,log10_h2o,log10_na\n";

  double masses[] = {10.0, 19.0, 50.0, 95.0, 150.0, 300.0};
  for (double mass : masses) {
    double log_h2o = model.log10_h2o_relative_solar(mass);
    double log_na = model.log10_na_relative_solar(mass);
    out << mass << "," << log_h2o << "," << log_na << "\n";
  }
  out.close();
  std::cout << "--> Wrote Welbanks et al. (2019) Mass-Metallicity dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Welbanks et al. (2019) C++ Colossal Water Depletion Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/welbanks_2019/sim_transmission_spectrum.csv");
  hot_jupiter::run_mass_metallicity_sweep("replications/welbanks_2019/sim_mass_metallicity.csv");
  std::cout << "✅ Welbanks et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
