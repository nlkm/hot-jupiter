// C++ Standalone Replication Solver for Wakeford et al. (2017) Science 356, 1150
// Calls core library class hot_jupiter::Wakeford2017PrimordialAtmosphere from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Wakeford2017PrimordialAtmosphere model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_ppm\n";

  for (double wave = 0.5; wave <= 5.0; wave += 0.05) {
    double depth_ppm = model.transmission_depth_ppm(wave);
    out << wave << "," << depth_ppm << "\n";
  }
  out.close();
  std::cout << "--> Wrote Wakeford et al. (2017) Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_mass_metallicity_sweep(const std::string& output_csv) {
  Wakeford2017PrimordialAtmosphere model;
  std::ofstream out(output_csv);
  out << "planet_mass_earth,log10_metallicity\n";

  double masses[] = {1.0, 14.5, 17.1, 19.0, 95.2, 317.8};
  for (double mass : masses) {
    double log_z = model.log10_metallicity(mass);
    out << mass << "," << log_z << "\n";
  }
  out.close();
  std::cout << "--> Wrote Wakeford et al. (2017) Mass-Metallicity dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Wakeford et al. (2017) C++ Primordial Atmosphere Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/wakeford_2017/sim_transmission_spectrum.csv");
  hot_jupiter::run_mass_metallicity_sweep("replications/wakeford_2017/sim_mass_metallicity.csv");
  std::cout << "✅ Wakeford et al. (2017) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
