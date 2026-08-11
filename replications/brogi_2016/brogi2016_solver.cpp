// C++ Standalone Replication Solver for Brogi et al. (2016) ApJ 817, 106
// Calls core library class hot_jupiter::Brogi2016WindRotationModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_wind_sweep(const std::string& output_csv) {
  Brogi2016WindRotationModel model;
  std::ofstream out(output_csv);
  out << "v_offset_km_s,ccf_sn\n";

  for (double v = -10.0; v <= 10.0; v += 0.2) {
    double sn = model.wind_blueshift_ccf(v);
    out << v << "," << sn << "\n";
  }
  out.close();
  std::cout << "--> Wrote Brogi et al. (2016) Day-to-Night Wind CCF dataset to " << output_csv << std::endl;
}

void run_rot_sweep(const std::string& output_csv) {
  Brogi2016WindRotationModel model;
  std::ofstream out(output_csv);
  out << "v_rot_km_s,ccf_sn\n";

  for (double vr = 0.0; vr <= 10.0; vr += 0.2) {
    double sn = model.rotational_broadening_ccf(vr);
    out << vr << "," << sn << "\n";
  }
  out.close();
  std::cout << "--> Wrote Brogi et al. (2016) Rotational Broadening CCF dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Brogi et al. (2016) C++ Atmospheric Wind & Rotation Solver ===" << std::endl;
  hot_jupiter::run_wind_sweep("replications/brogi_2016/sim_wind_sweep.csv");
  hot_jupiter::run_rot_sweep("replications/brogi_2016/sim_rot_sweep.csv");
  std::cout << "✅ Brogi et al. (2016) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
