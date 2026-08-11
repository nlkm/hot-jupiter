// C++ Standalone Replication Solver for Tsiaras et al. (2019) Nature Astronomy 3, 1086
// Calls core library class hot_jupiter::Tsiaras2019SuperEarthAtmosphere from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_transmission_spectrum_sweep(const std::string& output_csv) {
  Tsiaras2019SuperEarthAtmosphere model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,transit_depth_pct\n";

  for (double wave = 1.10; wave <= 1.70; wave += 0.005) {
    double depth_pct = model.k2_18b_transmission_depth_pct(wave);
    out << wave << "," << depth_pct << "\n";
  }
  out.close();
  std::cout << "--> Wrote Tsiaras et al. (2019) K2-18b Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_mu_posterior_sweep(const std::string& output_csv) {
  Tsiaras2019SuperEarthAtmosphere model;
  std::ofstream out(output_csv);
  out << "mu_g_mol,posterior_density\n";

  for (double mu = 2.3; mu <= 18.0; mu += 0.2) {
    double prob = model.mu_log_posterior_density(mu);
    out << mu << "," << prob << "\n";
  }
  out.close();
  std::cout << "--> Wrote Tsiaras et al. (2019) Mean Molecular Weight Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Tsiaras et al. (2019) C++ K2-18b Super-Earth Solver ===" << std::endl;
  hot_jupiter::run_transmission_spectrum_sweep("replications/tsiaras_2019/sim_transmission_spectrum.csv");
  hot_jupiter::run_mu_posterior_sweep("replications/tsiaras_2019/sim_mu_posterior.csv");
  std::cout << "✅ Tsiaras et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
