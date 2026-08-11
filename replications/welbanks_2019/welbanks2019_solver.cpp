// C++ Standalone Replication Solver for Welbanks et al. (2019) ApJL 887, L20
// Calls core library class hot_jupiter::Welbanks2019MassMetallicityModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_water_abundance_sweep(const std::string& output_csv) {
  Welbanks2019MassMetallicityModel model;
  std::ofstream out(output_csv);
  out << "mass_mjup,log10_x_h2o\n";

  for (double logm = -2.0; logm <= 1.0; logm += 0.05) {
    double m = std::pow(10.0, logm);
    double x_h2o = model.log10_x_h2o(m);
    out << m << "," << x_h2o << "\n";
  }
  out.close();
  std::cout << "--> Wrote Welbanks et al. (2019) Water Abundance dataset to " << output_csv << std::endl;
}

void run_mass_metallicity_sweep(const std::string& output_csv) {
  Welbanks2019MassMetallicityModel model;
  std::ofstream out(output_csv);
  out << "mass_mjup,metallicity_solar\n";

  for (double logm = -2.0; logm <= 1.0; logm += 0.05) {
    double m = std::pow(10.0, logm);
    double z = model.metallicity_solar(m);
    out << m << "," << z << "\n";
  }
  out.close();
  std::cout << "--> Wrote Welbanks et al. (2019) Mass-Metallicity dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Welbanks et al. (2019) C++ Mass-Metallicity Solver ===" << std::endl;
  hot_jupiter::run_water_abundance_sweep("replications/welbanks_2019/sim_water_abundance.csv");
  hot_jupiter::run_mass_metallicity_sweep("replications/welbanks_2019/sim_mass_metallicity.csv");
  std::cout << "✅ Welbanks et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
