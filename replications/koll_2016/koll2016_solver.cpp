// C++ Standalone Replication Solver for Koll & Abbot (2016) ApJ 825, 99
// Calls core library class hot_jupiter::Koll2016InversionModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_day_night_contrast_sweep(const std::string& output_csv) {
  Koll2016InversionModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,delta_t_dn_k\n";

  for (double teq = 500.0; teq <= 2500.0; teq += 50.0) {
    double dt = model.delta_t_dn_k(teq);
    out << teq << "," << dt << "\n";
  }
  out.close();
  std::cout << "--> Wrote Koll & Abbot (2016) Day-Night Contrast dataset to " << output_csv << std::endl;
}

void run_inversion_strength_sweep(const std::string& output_csv) {
  Koll2016InversionModel model;
  std::ofstream out(output_csv);
  out << "gamma_opacity,inversion_strength\n";

  for (double logg = -2.0; logg <= 2.0; logg += 0.05) {
    double gamma = std::pow(10.0, logg);
    double eta = model.inversion_strength(gamma);
    out << gamma << "," << eta << "\n";
  }
  out.close();
  std::cout << "--> Wrote Koll & Abbot (2016) Thermal Inversion Strength dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Koll & Abbot (2016) C++ Inversion Solver ===" << std::endl;
  hot_jupiter::run_day_night_contrast_sweep("replications/koll_2016/sim_day_night_contrast.csv");
  hot_jupiter::run_inversion_strength_sweep("replications/koll_2016/sim_inversion_strength.csv");
  std::cout << "✅ Koll & Abbot (2016) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
