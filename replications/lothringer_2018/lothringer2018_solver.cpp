// C++ Standalone Replication Solver for Lothringer et al. (2018) ApJ 866, 27
// Calls core library class hot_jupiter::Lothringer2018UltraHotInversionModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_tp_profile_sweep(const std::string& output_csv) {
  Lothringer2018UltraHotInversionModel model;
  std::ofstream out(output_csv);
  out << "log10_p_bar,temp_k\n";

  for (double log_p = -5.0; log_p <= 1.0; log_p += 0.1) {
    double temp = model.temperature_k(log_p);
    out << log_p << "," << temp << "\n";
  }
  out.close();
  std::cout << "--> Wrote Lothringer et al. (2018) T-P Profile dataset to " << output_csv << std::endl;
}

void run_emission_spectrum_sweep(const std::string& output_csv) {
  Lothringer2018UltraHotInversionModel model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,emission_3000k_ppm\n";

  for (double wave = 0.20; wave <= 2.00; wave += 0.01) {
    double em = model.emission_spectrum_ppm(wave);
    out << wave << "," << em << "\n";
  }
  out.close();
  std::cout << "--> Wrote Lothringer et al. (2018) Emission Spectrum dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Lothringer et al. (2018) C++ NUV Inversion Solver ===" << std::endl;
  hot_jupiter::run_tp_profile_sweep("replications/lothringer_2018/sim_tp_profile.csv");
  hot_jupiter::run_emission_spectrum_sweep("replications/lothringer_2018/sim_emission_spectrum.csv");
  std::cout << "✅ Lothringer et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
