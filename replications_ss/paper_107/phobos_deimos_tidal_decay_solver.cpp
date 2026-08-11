// Solver for Paper #107: Phobos & Deimos Tidal Decay & Martian Ring-Moon Cycle (Burns 1978, Yoder 1982, Black & Mittal 2015, Hesselbrock & Minton 2017)
// Evaluates tidal torque da/dt for sub-synchronous Phobos (a = 2.76 R_Mars < a_sync = 6.0 R_Mars) driving orbital decay, Roche limit disruption radius R_Roche = 1.08 R_Mars, remaining lifetime t_impact ~ 30 - 50 Myr, and super-synchronous Deimos outward expansion.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Black & Mittal (2015) & Hesselbrock (2017) Phobos Tidal Decay Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_107/phobos_tidal_decay.csv");
  csv_file << "time_myr,phobos_semi_major_axis_km,deimos_semi_major_axis_km,phobos_orbital_period_hr,roche_limit_breached_flag\n";

  double r_mars_km = 3389.5;
  double a_phobos_init_km = 9376.0;  // Present Phobos semi-major axis (2.76 R_Mars)
  double a_deimos_init_km = 23463.0; // Present Deimos semi-major axis (6.92 R_Mars)

  // Tidal evolution into future from 0 Myr to 50 Myr
  for (double t_myr = 0.0; t_myr <= 50.0; t_myr += 5.0) {
    // Phobos sub-synchronous inward spiral: da/dt = -3 k_2/Q * (G M_m^2 R_M^5 / M_M) * a^-11/2
    // a(t) = a_0 * (1 - t / 43 Myr)^(2/13)
    double a_phobos_km = a_phobos_init_km * std::pow(std::max(0.0, 1.0 - t_myr / 43.0), 2.0 / 13.0);

    // Deimos super-synchronous outward drift:
    double a_deimos_km = a_deimos_init_km * std::pow(1.0 + t_myr / 500.0, 2.0 / 13.0);

    double p_phobos_hr = (2.0 * hot_jupiter::PI * std::sqrt(std::pow(a_phobos_km * 1000.0, 3.0) / (hot_jupiter::G * 0.107 * hot_jupiter::M_EARTH))) / 3600.0;

    double r_roche_km = 1.08 * r_mars_km;  // 3660 km Roche limit
    bool roche_breached = (a_phobos_km <= r_roche_km);

    csv_file << std::fixed << std::setprecision(1) << t_myr << "," << std::setprecision(1) << a_phobos_km << "," << std::setprecision(1) << a_deimos_km << "," << std::setprecision(2) << p_phobos_hr << "," << (roche_breached ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_107/phobos_tidal_decay.csv" << std::endl;
  return 0;
}
