// C++ Standalone Replication Solver for Mansfield et al. (2021) Nature Astronomy 5, 1224
// Calls core library class hot_jupiter::Mansfield2021Wasp33bModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_emission_spectrum_sweep(const std::string& output_csv) {
  Mansfield2021Wasp33bModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,fp_fstar_ppm\n";

  for (double w = 1.12; w <= 1.68; w += 0.01) {
    double f = model.emission_flux_ratio_ppm(w);
    out << w << "," << f << "\n";
  }
  out.close();
  std::cout << "--> Wrote Mansfield et al. (2021) Emission Spectrum dataset to " << output_csv << std::endl;
}

void run_thermal_inversion_sweep(const std::string& output_csv) {
  Mansfield2021Wasp33bModel model;
  std::ofstream out(output_csv);
  out << "pressure_bar,t_k\n";

  for (double logp = -4.0; logp <= 0.0; logp += 0.05) {
    double p = std::pow(10.0, logp);
    double t = model.thermal_inversion_temperature_k(p);
    out << p << "," << t << "\n";
  }
  out.close();
  std::cout << "--> Wrote Mansfield et al. (2021) Thermal Inversion dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Mansfield et al. (2021) C++ WASP-33b Emission Solver ===" << std::endl;
  hot_jupiter::run_emission_spectrum_sweep("replications/mansfield_2021/sim_emission_spectrum.csv");
  hot_jupiter::run_thermal_inversion_sweep("replications/mansfield_2021/sim_thermal_inversion.csv");
  std::cout << "✅ Mansfield et al. (2021) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
