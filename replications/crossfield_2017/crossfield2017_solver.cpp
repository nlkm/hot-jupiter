// C++ Standalone Replication Solver for Crossfield & Kreidberg (2017) AJ 154, 261
// Calls core library class hot_jupiter::Crossfield2017SubJovianTrends from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_teq_sweep(const std::string& output_csv) {
  Crossfield2017SubJovianTrends model;
  std::ofstream out(output_csv);
  out << "t_eq_k,water_amplitude_h\n";

  for (double teq = 400.0; teq <= 1000.0; teq += 20.0) {
    double amp = model.water_amplitude_vs_teq(teq);
    out << teq << "," << amp << "\n";
  }
  out.close();
  std::cout << "--> Wrote Crossfield & Kreidberg (2017) T_eq Trend dataset to " << output_csv << std::endl;
}

void run_radius_sweep(const std::string& output_csv) {
  Crossfield2017SubJovianTrends model;
  std::ofstream out(output_csv);
  out << "radius_earth,water_amplitude_h\n";

  for (double r_earth = 1.5; r_earth <= 6.0; r_earth += 0.1) {
    double amp = model.water_amplitude_vs_radius(r_earth);
    out << r_earth << "," << amp << "\n";
  }
  out.close();
  std::cout << "--> Wrote Crossfield & Kreidberg (2017) Radius Trend dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Crossfield & Kreidberg (2017) C++ Sub-Jovian Trends Solver ===" << std::endl;
  hot_jupiter::run_teq_sweep("replications/crossfield_2017/sim_teq_trend.csv");
  hot_jupiter::run_radius_sweep("replications/crossfield_2017/sim_radius_trend.csv");
  std::cout << "✅ Crossfield & Kreidberg (2017) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
