// C++ Standalone Replication Solver for Line et al. (2015) ApJ 807, 183
// Calls core library class hot_jupiter::Line2015PopulationRetrieval from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_mass_metallicity_sweep(const std::string& output_csv) {
  Line2015PopulationRetrieval model;
  std::ofstream out(output_csv);
  out << "planet_mass_mjup,metallicity_dex\n";

  for (double m_p = 0.05; m_p <= 5.0; m_p *= 1.15) {
    double z_dex = model.metallicity_dex(m_p);
    out << m_p << "," << z_dex << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2015) Mass-Metallicity dataset to " << output_csv << std::endl;
}

void run_co_distribution_sweep(const std::string& output_csv) {
  Line2015PopulationRetrieval model;
  std::ofstream out(output_csv);
  out << "co_bin_center,co_count\n";

  for (double co = 0.2; co <= 1.2; co += 0.1) {
    double count = model.co_ratio_distribution(co);
    out << co << "," << count << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2015) C/O Distribution dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Line et al. (2015) C++ Population Retrieval Solver ===" << std::endl;
  hot_jupiter::run_mass_metallicity_sweep("replications/line_2015/sim_mass_metallicity.csv");
  hot_jupiter::run_co_distribution_sweep("replications/line_2015/sim_co_distribution.csv");
  std::cout << "✅ Line et al. (2015) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
