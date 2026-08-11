// C++ Standalone Replication Solver for Showman et al. (2015) ApJ 801, 95
// Calls core library class hot_jupiter::Showman2015CirculationModel from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_hotspot_phase_shift_sweep(const std::string& output_csv) {
  Showman2015CirculationModel model;
  std::ofstream out(output_csv);
  out << "tau_rad_days,phase_shift_deg\n";

  for (double log_t = -1.0; log_t <= 2.0; log_t += 0.05) {
    double t_rad = std::pow(10.0, log_t);
    double shift = model.hotspot_phase_shift_deg(t_rad);
    out << t_rad << "," << shift << "\n";
  }
  out.close();
  std::cout << "--> Wrote Showman et al. (2015) Hotspot Phase Shift dataset to " << output_csv << std::endl;
}

void run_temp_contrast_sweep(const std::string& output_csv) {
  Showman2015CirculationModel model;
  std::ofstream out(output_csv);
  out << "pressure_bar,delta_t_day_night_k\n";

  for (double log_p = -4.0; log_p <= 1.0; log_p += 0.05) {
    double pressure = std::pow(10.0, log_p);
    double dt = model.day_night_temp_contrast_k(pressure);
    out << pressure << "," << dt << "\n";
  }
  out.close();
  std::cout << "--> Wrote Showman et al. (2015) Day-Night Temp Contrast dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Showman et al. (2015) C++ Atmospheric Circulation Solver ===" << std::endl;
  hot_jupiter::run_hotspot_phase_shift_sweep("replications/showman_2015/sim_phase_shift.csv");
  hot_jupiter::run_temp_contrast_sweep("replications/showman_2015/sim_temp_contrast.csv");
  std::cout << "✅ Showman et al. (2015) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
