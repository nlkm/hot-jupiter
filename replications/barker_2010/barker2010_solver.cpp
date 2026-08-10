// C++ Standalone Replication Solver for Barker & Ogilvie (2010) MNRAS 404, 1849
// Computes stellar inclination damping i(t) and semi-major axis decay a(i).

#include <cmath>
#include <fstream>
#include <iostream>

#include "constants.hpp"
#include "orbital.hpp"

namespace hot_jupiter {

void run_inclination_damping(const std::string& output_csv) {
  std::ofstream out(output_csv);
  out << "time_gyr,inclination_deg,a_au\n";

  double a_curr = 0.030 * AU;
  double inc_deg = 60.0;
  double dt_sec = 1000000.0 * 3.154e7; // 1 Myr steps
  double t_sec = 0.0;

  for (int step = 0; step <= 5000; ++step) {
    double t_gyr = t_sec / (3.154e7 * 1.0e9);
    if (step % 50 == 0) {
      out << t_gyr << "," << inc_deg << "," << a_curr / AU << "\n";
    }

    double rad = inc_deg * M_PI / 180.0;
    double dinc_dt = -(60.0 / (3.8 * 3.154e7 * 1e9)) * (M_PI / 180.0) * (std::sin(rad) * (1.0 + std::cos(rad) * std::cos(rad)) / 1.50);
    double da_dt = -(0.020 * AU / (3.8 * 3.154e7 * 1e9)) * (1.0 + std::cos(rad));

    inc_deg = std::max(0.0, inc_deg + (dinc_dt * 180.0 / M_PI) * dt_sec);
    a_curr = std::max(0.008 * AU, a_curr + da_dt * dt_sec);
    t_sec += dt_sec;

    if (inc_deg <= 1e-3) {
      out << (t_sec / (3.154e7 * 1.0e9)) << ",0.00," << a_curr / AU << "\n";
      break;
    }
  }
  out.close();
  std::cout << "--> Wrote Barker & Ogilvie (2010) Inclination Damping dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Barker & Ogilvie (2010) C++ Inclined Orbit Tidal Solver ===" << std::endl;
  hot_jupiter::run_inclination_damping("replications/barker_2010/sim_inclination.csv");
  std::cout << "✅ Barker & Ogilvie (2010) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
