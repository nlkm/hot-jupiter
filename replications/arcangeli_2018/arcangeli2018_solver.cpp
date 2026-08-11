// C++ Standalone Replication Solver for Arcangeli et al. (2018) ApJ 855, L30
// Calls core library class hot_jupiter::Arcangeli2018HMinusOpacityModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_emission_sweep(const std::string& output_csv) {
  Arcangeli2018HMinusOpacityModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,flux_ratio\n";

  for (double w = 1.10; w <= 1.70; w += 0.01) {
    double f = model.emission_spectrum(w);
    out << w << "," << f << "\n";
  }
  out.close();
  std::cout << "--> Wrote Arcangeli et al. (2018) WASP-18b H- Emission Spectrum dataset to " << output_csv << std::endl;
}

void run_dissociation_sweep(const std::string& output_csv) {
  Arcangeli2018HMinusOpacityModel model;
  std::ofstream out(output_csv);
  out << "temp_k,dissociation_fraction\n";

  for (double t = 2000.0; t <= 3800.0; t += 25.0) {
    double a = model.h2_dissociation_fraction(t);
    out << t << "," << a << "\n";
  }
  out.close();
  std::cout << "--> Wrote Arcangeli et al. (2018) Thermal Dissociation Fraction dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Arcangeli et al. (2018) C++ WASP-18b H- Opacity & Dissociation Solver ===" << std::endl;
  hot_jupiter::run_emission_sweep("replications/arcangeli_2018/sim_emission.csv");
  hot_jupiter::run_dissociation_sweep("replications/arcangeli_2018/sim_dissociation.csv");
  std::cout << "✅ Arcangeli et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
