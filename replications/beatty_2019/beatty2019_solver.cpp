// C++ Standalone Replication Solver for Beatty et al. (2019) AJ 158, 166
// Calls core library class hot_jupiter::Beatty2019Kelt1bPhaseCurveModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_phase_curve_sweep(const std::string& output_csv) {
  Beatty2019Kelt1bPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "phase,flux_ppm\n";

  for (double p = 0.0; p <= 1.0; p += 0.01) {
    double f = model.phase_curve_flux_ppm(p);
    out << p << "," << f << "\n";
  }
  out.close();
  std::cout << "--> Wrote Beatty et al. (2019) KELT-1b Phase Curve dataset to " << output_csv << std::endl;
}

void run_recirculation_sweep(const std::string& output_csv) {
  Beatty2019Kelt1bPhaseCurveModel model;
  std::ofstream out(output_csv);
  out << "teq_k,recirculation_eff\n";

  for (double t = 1500.0; t <= 3200.0; t += 25.0) {
    double eps = model.recirculation_efficiency(t);
    out << t << "," << eps << "\n";
  }
  out.close();
  std::cout << "--> Wrote Beatty et al. (2019) Recirculation Efficiency dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Beatty et al. (2019) C++ KELT-1b Phase Curve Solver ===" << std::endl;
  hot_jupiter::run_phase_curve_sweep("replications/beatty_2019/sim_phase_curve.csv");
  hot_jupiter::run_recirculation_sweep("replications/beatty_2019/sim_recirculation.csv");
  std::cout << "✅ Beatty et al. (2019) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
