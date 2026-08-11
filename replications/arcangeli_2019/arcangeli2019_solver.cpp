// C++ Standalone Replication Solver for Arcangeli et al. (2019) A&A 625, A136
// Calls core library class hot_jupiter::Arcangeli2019Wasp18bClimateModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_dayside_spectrum_sweep(const std::string& output_csv) {
  Arcangeli2019Wasp18bClimateModel model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,flux_ppm\n";

  for (double wl = 1.10; wl <= 1.70; wl += 0.01) {
    double flux = model.dayside_emission_flux_ppm(wl);
    out << wl << "," << flux << "\n";
  }
  out.close();
  std::cout << "--> Wrote Arcangeli et al. (2019) Dayside Emission Spectrum dataset to " << output_csv << std::endl;
}

void run_daynight_spectrum_sweep(const std::string& output_csv) {
  Arcangeli2019Wasp18bClimateModel model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,flux_day_ppm,flux_night_ppm\n";

  for (double wl = 1.10; wl <= 1.70; wl += 0.01) {
    double fday = model.dayside_emission_flux_ppm(wl);
    double fnight = model.nightside_emission_flux_ppm(wl);
    out << wl << "," << fday << "," << fnight << "\n";
  }
  out.close();
  std::cout << "--> Wrote Arcangeli et al. (2019) Day-Night Spectrum Comparison dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Arcangeli et al. (2019) C++ WASP-18b Climate Solver ===" << std::endl;
  hot_jupiter::run_dayside_spectrum_sweep("replications/arcangeli_2019/sim_dayside_spectrum.csv");
  hot_jupiter::run_daynight_spectrum_sweep("replications/arcangeli_2019/sim_daynight_spectrum.csv");
  std::cout << "✅ Arcangeli et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
