// C++ Standalone Replication Solver for Line et al. (2016) AJ 152, 203
// Calls core library class hot_jupiter::Line2016WaterDepletionRetrieval from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_secondary_eclipse_sweep(const std::string& output_csv) {
  Line2016WaterDepletionRetrieval model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,flux_ratio_ppm\n";

  for (double wave = 1.1; wave <= 1.7; wave += 0.02) {
    double flux_ppm = model.secondary_eclipse_flux_ratio_ppm(wave);
    out << wave << "," << flux_ppm << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2016) Secondary Eclipse dataset to " << output_csv << std::endl;
}

void run_water_posterior_sweep(const std::string& output_csv) {
  Line2016WaterDepletionRetrieval model;
  std::ofstream out(output_csv);
  out << "log10_xh2o,posterior_density\n";

  for (double log_x = -7.0; log_x <= -2.0; log_x += 0.2) {
    double density = model.h2o_log_posterior_density(log_x);
    out << log_x << "," << density << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2016) Water Abundance Posterior dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Line et al. (2016) C++ WASP-12b Retrieval Solver ===" << std::endl;
  hot_jupiter::run_secondary_eclipse_sweep("replications/line_2016/sim_secondary_eclipse.csv");
  hot_jupiter::run_water_posterior_sweep("replications/line_2016/sim_water_posterior.csv");
  std::cout << "✅ Line et al. (2016) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
