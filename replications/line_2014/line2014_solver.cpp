// C++ Standalone Replication Solver for Line et al. (2014) ApJ 783, 70
// Calls core library class hot_jupiter::Line2014EmissionRetrievalModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_emission_spectrum_sweep(const std::string& output_csv) {
  Line2014EmissionRetrievalModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,flux_ratio\n";

  const double ref_w[6] = {1.15, 1.30, 1.60, 2.20, 3.60, 4.50};
  for (int i = 0; i < 6; ++i) {
    out << ref_w[i] << "," << model.emission_spectrum(ref_w[i]) << "\n";
  }

  for (double w = 1.0; w <= 5.0; w += 0.05) {
    double f = model.emission_spectrum(w);
    out << w << "," << f << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2014) HD 189733b Emission Spectrum dataset to " << output_csv << std::endl;
}

void run_tp_profile_sweep(const std::string& output_csv) {
  Line2014EmissionRetrievalModel model;
  std::ofstream out(output_csv);
  out << "pressure_bar,temp_k\n";

  for (double logp = -4.0; logp <= 2.0; logp += 0.1) {
    double p_bar = std::pow(10.0, logp);
    double t = model.temperature_at_pressure(logp);
    out << p_bar << "," << t << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2014) Retrieved T(P) Profile dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Line et al. (2014) C++ Systematic Emission Retrieval Solver ===" << std::endl;
  hot_jupiter::run_emission_spectrum_sweep("replications/line_2014/sim_emission_spectrum.csv");
  hot_jupiter::run_tp_profile_sweep("replications/line_2014/sim_tp_profile.csv");
  std::cout << "✅ Line et al. (2014) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
