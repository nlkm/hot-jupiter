// C++ Standalone Replication Solver for Lothringer & Barman (2019) ApJ 876, 69
// Calls core library class hot_jupiter::Lothringer2019StellarSpectralTypeModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_tp_sweep(const std::string& output_csv) {
  Lothringer2019StellarSpectralTypeModel model;
  std::ofstream out(output_csv);
  out << "log10_p_bar,temp_f,temp_g,temp_k,temp_m\n";

  for (double logp = -6.0; logp <= 2.0; logp += 0.1) {
    double tf = model.temperature_k(logp, "F");
    double tg = model.temperature_k(logp, "G");
    double tk = model.temperature_k(logp, "K");
    double tm = model.temperature_k(logp, "M");
    out << logp << "," << tf << "," << tg << "," << tk << "," << tm << "\n";
  }
  out.close();
  std::cout << "--> Wrote Lothringer & Barman (2019) T-P Profiles dataset to " << output_csv << std::endl;
}

void run_spectrum_sweep(const std::string& output_csv) {
  Lothringer2019StellarSpectralTypeModel model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,flux_fhost,flux_ghost\n";

  for (double wl = 0.3; wl <= 5.0; wl += 0.05) {
    double ff = model.emission_flux_ppm(wl, "F");
    double fg = model.emission_flux_ppm(wl, "G");
    out << wl << "," << ff << "," << fg << "\n";
  }
  out.close();
  std::cout << "--> Wrote Lothringer & Barman (2019) Emergent Spectra dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Lothringer & Barman (2019) C++ Stellar Spectral Type Model Solver ===" << std::endl;
  hot_jupiter::run_tp_sweep("replications/lothringer_2019/sim_tp_profiles.csv");
  hot_jupiter::run_spectrum_sweep("replications/lothringer_2019/sim_spectra.csv");
  std::cout << "✅ Lothringer & Barman (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
