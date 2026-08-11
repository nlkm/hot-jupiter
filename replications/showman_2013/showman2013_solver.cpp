// C++ Standalone Replication Solver for Showman & Kaspi (2013) ApJ 776, 85
// Calls core library class hot_jupiter::Showman2013TerrestrialDynamicsModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_zonal_jet_sweep(const std::string& output_csv) {
  Showman2013TerrestrialDynamicsModel model;
  std::ofstream out(output_csv);
  out << "t_eq_k,ujet_ms\n";

  for (double teq = 300.0; teq <= 2000.0; teq += 50.0) {
    double ujet = model.zonal_jet_speed_ms(teq);
    out << teq << "," << ujet << "\n";
  }
  out.close();
  std::cout << "--> Wrote Showman & Kaspi (2013) Zonal Jet Speed dataset to " << output_csv << std::endl;
}

void run_rossby_radius_sweep(const std::string& output_csv) {
  Showman2013TerrestrialDynamicsModel model;
  std::ofstream out(output_csv);
  out << "prot_days,ld_over_a\n";

  for (double prot = 1.0; prot <= 30.0; prot += 0.5) {
    double ld = model.rossby_deformation_ratio(prot);
    out << prot << "," << ld << "\n";
  }
  out.close();
  std::cout << "--> Wrote Showman & Kaspi (2013) Rossby Deformation Radius dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Showman & Kaspi (2013) C++ Terrestrial Dynamics Solver ===" << std::endl;
  hot_jupiter::run_zonal_jet_sweep("replications/showman_2013/sim_zonal_jet.csv");
  hot_jupiter::run_rossby_radius_sweep("replications/showman_2013/sim_rossby_radius.csv");
  std::cout << "✅ Showman & Kaspi (2013) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
