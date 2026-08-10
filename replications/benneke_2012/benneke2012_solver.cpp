// C++ Standalone Replication Solver for Benneke & Seager (2012) ApJ 753, 100
// Calls core library class hot_jupiter::Benneke2012MolecularWeight from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectra_sweep(const std::string& output_csv) {
  Benneke2012MolecularWeight model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_mu4,transit_depth_mu18\n";

  for (double wave = 0.5; wave <= 1.8; wave += 0.05) {
    double depth_mu4 = model.transmission_spectrum_depth(wave, 4.0);
    double depth_mu18 = model.transmission_spectrum_depth(wave, 18.0);
    out << wave << "," << depth_mu4 << "," << depth_mu18 << "\n";
  }
  out.close();
  std::cout << "--> Wrote Benneke & Seager (2012) Transmission Spectra dataset to " << output_csv << std::endl;
}

void run_posterior_sweep(const std::string& output_csv) {
  Benneke2012MolecularWeight model;
  std::ofstream out(output_csv);
  out << "mu_amu,posterior_density\n";

  for (double mu = 2.0; mu <= 20.0; mu += 0.5) {
    double density = model.posterior_density(mu);
    out << mu << "," << density << "\n";
  }
  out.close();
  std::cout << "--> Wrote Benneke & Seager (2012) Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Benneke & Seager (2012) C++ Mean Molecular Weight Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectra_sweep("replications/benneke_2012/sim_transmission_spectra.csv");
  hot_jupiter::run_posterior_sweep("replications/benneke_2012/sim_posterior_density.csv");
  std::cout << "✅ Benneke & Seager (2012) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
