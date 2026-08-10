// C++ Standalone Replication Solver for Arcangeli et al. (2018) ApJL 855, L30
// Calls core library class hot_jupiter::Arcangeli2018HMinerOpacity from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_secondary_eclipse_sweep(const std::string& output_csv) {
  Arcangeli2018HMinerOpacity model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,flux_ratio_ppm\n";

  for (double wave = 1.1; wave <= 1.7; wave += 0.02) {
    double flux_ppm = model.secondary_eclipse_flux_ratio_ppm(wave);
    out << wave << "," << flux_ppm << "\n";
  }
  out.close();
  std::cout << "--> Wrote Arcangeli et al. (2018) Secondary Eclipse dataset to " << output_csv << std::endl;
}

void run_dissociation_sweep(const std::string& output_csv) {
  Arcangeli2018HMinerOpacity model;
  std::ofstream out(output_csv);
  out << "temperature_k,dissociation_fraction\n";

  for (double temp = 1500.0; temp <= 3500.0; temp += 50.0) {
    double alpha = model.hydrogen_dissociation_fraction(temp);
    out << temp << "," << alpha << "\n";
  }
  out.close();
  std::cout << "--> Wrote Arcangeli et al. (2018) Hydrogen Dissociation dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Arcangeli et al. (2018) C++ WASP-18b H- Opacity Solver ===" << std::endl;
  hot_jupiter::run_secondary_eclipse_sweep("replications/arcangeli_2018/sim_secondary_eclipse.csv");
  hot_jupiter::run_dissociation_sweep("replications/arcangeli_2018/sim_hydrogen_dissociation.csv");
  std::cout << "✅ Arcangeli et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
