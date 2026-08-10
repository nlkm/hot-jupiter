// C++ Standalone Replication Solver for Lothringer et al. (2018) ApJ 866, 27
// Calls core library class hot_jupiter::Lothringer2018UltraHotJupiter from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_thermal_profile_sweep(const std::string& output_csv) {
  Lothringer2018UltraHotJupiter model;
  std::ofstream out(output_csv);
  out << "pressure_bar,temperature_k\n";

  for (double log_p = -4.0; log_p <= 1.0; log_p += 0.2) {
    double p_bar = std::pow(10.0, log_p);
    double temp_k = model.temperature_k(p_bar);
    out << p_bar << "," << temp_k << "\n";
  }
  out.close();
  std::cout << "--> Wrote Lothringer et al. (2018) Thermal Profile dataset to " << output_csv << std::endl;
}

void run_emergent_spectrum_sweep(const std::string& output_csv) {
  Lothringer2018UltraHotJupiter model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,flux_lambda\n";

  for (double wave = 0.3; wave <= 1.8; wave += 0.02) {
    double flux = model.emergent_flux_lambda(wave);
    out << wave << "," << flux << "\n";
  }
  out.close();
  std::cout << "--> Wrote Lothringer et al. (2018) Emergent Spectrum dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Lothringer et al. (2018) C++ Ultra-Hot Jupiter Solver ===" << std::endl;
  hot_jupiter::run_thermal_profile_sweep("replications/lothringer_2018/sim_thermal_profile.csv");
  hot_jupiter::run_emergent_spectrum_sweep("replications/lothringer_2018/sim_emergent_spectrum.csv");
  std::cout << "✅ Lothringer et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
