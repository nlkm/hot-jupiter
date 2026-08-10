// C++ Standalone Replication Solver for Madhusudhan et al. (2014) Space Sci Rev 186, 269
// Calls core library class hot_jupiter::Madhusudhan2014Chemistry from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_temp_abundance_sweep(const std::string& output_csv) {
  Madhusudhan2014Chemistry model;
  std::ofstream out(output_csv);
  out << "temp_k,log10_x_h2o,log10_x_co,log10_x_ch4,log10_x_co2\n";

  for (double t_k = 500.0; t_k <= 2500.0; t_k += 50.0) {
    double h2o, co, ch4, co2;
    model.equilibrium_abundances_solar(t_k, h2o, co, ch4, co2);
    out << t_k << "," << h2o << "," << co << "," << ch4 << "," << co2 << "\n";
  }
  out.close();
  std::cout << "--> Wrote Madhusudhan et al. (2014) Abundance vs Temp dataset to " << output_csv << std::endl;
}

void run_water_vs_co_sweep(const std::string& output_csv) {
  Madhusudhan2014Chemistry model;
  std::ofstream out(output_csv);
  out << "co_ratio,log10_x_h2o\n";

  for (double co = 0.2; co <= 1.5; co += 0.05) {
    double log_h2o = model.water_abundance_vs_co(co);
    out << co << "," << log_h2o << "\n";
  }
  out.close();
  std::cout << "--> Wrote Madhusudhan et al. (2014) Water vs C/O dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Madhusudhan et al. (2014) C++ C/O Chemistry Solver ===" << std::endl;
  hot_jupiter::run_temp_abundance_sweep("replications/madhusudhan_2014/sim_temp_abundance.csv");
  hot_jupiter::run_water_vs_co_sweep("replications/madhusudhan_2014/sim_water_vs_co.csv");
  std::cout << "✅ Madhusudhan et al. (2014) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
