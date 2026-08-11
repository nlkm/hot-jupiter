// C++ Standalone Replication Solver for Spake et al. (2018) Nature 557, 68
// Calls core library class hot_jupiter::Spake2018MetastableHeliumModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_helium_spectrum_sweep(const std::string& output_csv) {
  Spake2018MetastableHeliumModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth\n";

  const double ref_w[7] = {1.00, 1.04, 1.07, 1.0833, 1.095, 1.12, 1.15};
  for (int i = 0; i < 7; ++i) {
    out << ref_w[i] << "," << model.helium_transmission_spectrum(ref_w[i]) << "\n";
  }

  for (double w = 1.00; w <= 1.15; w += 0.002) {
    double d = model.helium_transmission_spectrum(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Spake et al. (2018) WASP-107b Helium Transmission dataset to " << output_csv << std::endl;
}

void run_mass_loss_sweep(const std::string& output_csv) {
  Spake2018MetastableHeliumModel model;
  std::ofstream out(output_csv);
  out << "he_fraction,log10_mdot_g_s\n";

  for (double y = 0.05; y <= 0.20; y += 0.005) {
    double m = model.log10_mass_loss_rate_g_s(y);
    out << y << "," << m << "\n";
  }
  out.close();
  std::cout << "--> Wrote Spake et al. (2018) Helium Mass Loss Rate dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Spake et al. (2018) C++ Metastable Helium Solver ===" << std::endl;
  hot_jupiter::run_helium_spectrum_sweep("replications/spake_2018/sim_helium_spectrum.csv");
  hot_jupiter::run_mass_loss_sweep("replications/spake_2018/sim_mass_loss.csv");
  std::cout << "✅ Spake et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
