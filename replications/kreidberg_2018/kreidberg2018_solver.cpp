// C++ Standalone Replication Solver for Kreidberg et al. (2018) AJ 156, 17
// Calls core library class hot_jupiter::Kreidberg2018Wasp103bPhaseCurveModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_phase_curve_sweep(const std::string& output_csv) {
  Kreidberg2018Wasp103bPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "orbital_phase,flux_ppm\n";

  for (double phi = 0.0; phi <= 1.0; phi += 0.01) {
    double flux = model.phase_curve_flux_ppm(phi);
    out << phi << "," << flux << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kreidberg et al. (2018) WASP-103b Phase Curve dataset to " << output_csv << std::endl;
}

void run_temperature_sweep(const std::string& output_csv) {
  Kreidberg2018Wasp103bPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "orbital_phase,temp_k\n";

  for (double phi = 0.0; phi <= 1.0; phi += 0.01) {
    double temp = model.temperature_k(phi);
    out << phi << "," << temp << "\n";
  }
  out.close();
  std::cout << "--> Wrote Kreidberg et al. (2018) WASP-103b Phase Temperature dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Kreidberg et al. (2018) C++ WASP-103b Phase Curve Solver ===" << std::endl;
  hot_jupiter::run_phase_curve_sweep("replications/kreidberg_2018/sim_phase_curve.csv");
  hot_jupiter::run_temperature_sweep("replications/kreidberg_2018/sim_phase_temp.csv");
  std::cout << "✅ Kreidberg et al. (2018) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
