// C++ Standalone Replication Solver for Stevenson et al. (2014) Science 346, 838
// Calls core library class hot_jupiter::Stevenson2014ThermalPhaseCurve from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_phase_curve_sweep(const std::string& output_csv) {
  Stevenson2014ThermalPhaseCurve model;
  std::ofstream out(output_csv);
  out << "orbital_phase,flux_ratio_ppm\n";

  for (double phase = 0.0; phase <= 1.0; phase += 0.02) {
    double flux_ppm = model.flux_ratio_ppm(phase);
    out << phase << "," << flux_ppm << "\n";
  }
  out.close();
  std::cout << "--> Wrote Stevenson et al. (2014) Phase Curve dataset to " << output_csv << std::endl;
}

void run_temperature_profile_sweep(const std::string& output_csv) {
  Stevenson2014ThermalPhaseCurve model;
  std::ofstream out(output_csv);
  out << "longitude_deg,temperature_k\n";

  for (double lon = -180.0; lon <= 180.0; lon += 5.0) {
    double temp_k = model.brightness_temperature_k(lon);
    out << lon << "," << temp_k << "\n";
  }
  out.close();
  std::cout << "--> Wrote Stevenson et al. (2014) Longitudinal Temperature dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Stevenson et al. (2014) C++ Thermal Phase Curve Solver ===" << std::endl;
  hot_jupiter::run_phase_curve_sweep("replications/stevenson_2014/sim_phase_curve.csv");
  hot_jupiter::run_temperature_profile_sweep("replications/stevenson_2014/sim_temperature_profile.csv");
  std::cout << "✅ Stevenson et al. (2014) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
