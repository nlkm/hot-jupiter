// C++ Standalone Replication Solver for Line et al. (2014) ApJ 783, 70
// Calls core library class hot_jupiter::Line2014HotJupiterRetrieval from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_wasp43b_tp_sweep(const std::string& output_csv) {
  Line2014HotJupiterRetrieval model;
  std::ofstream out(output_csv);
  out << "pressure_bar,temp_median_k,temp_upper_1sig,temp_lower_1sig\n";

  for (double log_p = -4.0; log_p <= 1.0; log_p += 0.2) {
    double p_bar = std::pow(10.0, log_p);
    double med, up, low;
    model.wasp43b_tp_profile(p_bar, med, up, low);
    out << p_bar << "," << med << "," << up << "," << low << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2014) WASP-43b T-P Retrieval dataset to " << output_csv << std::endl;
}

void run_wasp43b_spectrum_sweep(const std::string& output_csv) {
  Line2014HotJupiterRetrieval model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,flux_ratio_pct\n";

  for (double wave = 3.0; wave <= 9.0; wave += 0.1) {
    double ratio = model.wasp43b_eclipse_flux_ratio_pct(wave);
    out << wave << "," << ratio << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2014) WASP-43b Spectrum Retrieval dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Line et al. (2014) C++ WASP-43b Retrieval Solver ===" << std::endl;
  hot_jupiter::run_wasp43b_tp_sweep("replications/line_2014/sim_wasp43b_tp.csv");
  hot_jupiter::run_wasp43b_spectrum_sweep("replications/line_2014/sim_wasp43b_spectrum.csv");
  std::cout << "✅ Line et al. (2014) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
