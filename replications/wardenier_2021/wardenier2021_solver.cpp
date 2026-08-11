// C++ Standalone Replication Solver for Wardenier et al. (2021) MNRAS 506, 1258
// Calls core library class hot_jupiter::Wardenier2021LimbAsymmetryModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_limb_transmission_sweep(const std::string& output_csv) {
  Wardenier2021LimbAsymmetryModel model;
  std::ofstream out(output_csv);
  out << "wavelength_um,transit_depth_evening\n";

  for (double w = 0.35; w <= 5.00; w += 0.05) {
    double d = model.evening_limb_transit_depth(w);
    out << w << "," << d << "\n";
  }
  out.close();
  std::cout << "--> Wrote Wardenier et al. (2021) Limb Transmission Spectrum dataset to " << output_csv << std::endl;
}

void run_limb_tp_sweep(const std::string& output_csv) {
  Wardenier2021LimbAsymmetryModel model;
  std::ofstream out(output_csv);
  out << "pressure_bar,t_k_evening\n";

  for (double logp = -5.0; logp <= 0.0; logp += 0.05) {
    double p = std::pow(10.0, logp);
    double t = model.evening_limb_temperature_k(p);
    out << p << "," << t << "\n";
  }
  out.close();
  std::cout << "--> Wrote Wardenier et al. (2021) Limb Thermal Profile dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Wardenier et al. (2021) C++ 3D Limb Asymmetry Solver ===" << std::endl;
  hot_jupiter::run_limb_transmission_sweep("replications/wardenier_2021/sim_limb_transmission.csv");
  hot_jupiter::run_limb_tp_sweep("replications/wardenier_2021/sim_limb_tp.csv");
  std::cout << "✅ Wardenier et al. (2021) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
