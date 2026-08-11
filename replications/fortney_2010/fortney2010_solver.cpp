// C++ Standalone Replication Solver for Fortney et al. (2010) ApJ 709, 1396
// Calls core library class hot_jupiter::Fortney2010GasGiantGrid from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_metallicity_grid_sweep(const std::string& output_csv) {
  Fortney2010GasGiantGrid model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,depth_1x_pct,depth_10x_pct,depth_30x_pct\n";

  for (double wave = 0.35; wave <= 5.00; wave += 0.05) {
    double d1 = model.transmission_depth_pct(wave, 1.0, 0.0);
    double d10 = model.transmission_depth_pct(wave, 10.0, 0.0);
    double d30 = model.transmission_depth_pct(wave, 30.0, 0.0);
    out << wave << "," << d1 << "," << d10 << "," << d30 << "\n";
  }
  out.close();
  std::cout << "--> Wrote Fortney et al. (2010) Metallicity Grid dataset to " << output_csv << std::endl;
}

void run_cloud_deck_sweep(const std::string& output_csv) {
  Fortney2010GasGiantGrid model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,depth_clear_pct,depth_10mbar_pct,depth_1mbar_pct\n";

  for (double wave = 0.35; wave <= 5.00; wave += 0.05) {
    double d_clear = model.transmission_depth_pct(wave, 1.0, 0.0);
    double d_10mbar = model.transmission_depth_pct(wave, 1.0, 10.0);
    double d_1mbar = model.transmission_depth_pct(wave, 1.0, 1.0);
    out << wave << "," << d_clear << "," << d_10mbar << "," << d_1mbar << "\n";
  }
  out.close();
  std::cout << "--> Wrote Fortney et al. (2010) Cloud Deck Grid dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Fortney et al. (2010) C++ Gas Giant Solver ===" << std::endl;
  hot_jupiter::run_metallicity_grid_sweep("replications/fortney_2010/sim_metallicity_grid.csv");
  hot_jupiter::run_cloud_deck_sweep("replications/fortney_2010/sim_cloud_grid.csv");
  std::cout << "✅ Fortney et al. (2010) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
