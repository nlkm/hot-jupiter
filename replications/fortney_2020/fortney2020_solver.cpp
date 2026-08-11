// C++ Standalone Replication Solver for Fortney et al. (2020) AJ 160, 288
// Calls core library class hot_jupiter::Fortney2020ThermalDissociationModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_dissociation_sweep(const std::string& output_csv) {
  Fortney2020ThermalDissociationModel model;
  std::ofstream out(output_csv);
  out << "pressure_bar,alpha_dissoc\n";

  for (double logp = -4.0; logp <= 2.0; logp += 0.05) {
    double p = std::pow(10.0, logp);
    double a = model.h2_dissociation_fraction(p);
    out << p << "," << a << "\n";
  }
  out.close();
  std::cout << "--> Wrote Fortney et al. (2020) H2 Dissociation Fraction dataset to " << output_csv << std::endl;
}

void run_temperature_sweep(const std::string& output_csv) {
  Fortney2020ThermalDissociationModel model;
  std::ofstream out(output_csv);
  out << "pressure_bar,t_k\n";

  for (double logp = -5.0; logp <= 2.0; logp += 0.05) {
    double p = std::pow(10.0, logp);
    double t = model.temperature_k(p);
    out << p << "," << t << "\n";
  }
  out.close();
  std::cout << "--> Wrote Fortney et al. (2020) Thermal Profile dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Fortney et al. (2020) C++ Thermal Dissociation Solver ===" << std::endl;
  hot_jupiter::run_dissociation_sweep("replications/fortney_2020/sim_h2_dissociation.csv");
  hot_jupiter::run_temperature_sweep("replications/fortney_2020/sim_thermal_profile.csv");
  std::cout << "✅ Fortney et al. (2020) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
