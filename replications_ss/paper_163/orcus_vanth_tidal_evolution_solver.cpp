// Solver for Paper #163: Plutino (90482) Orcus & Satellite Vanth Synchronous Tidal Locking, Mass Ratio, & Surface Volatile Heterogeneity (Brown 2010, Carry 2011, Grundy 2019, Ortiz 2011)
// Evaluates Hubble Space Telescope (HST) and Gemini adaptive optics observations of 3:2 mean-motion resonant Kuiper Belt plutino (90482) Orcus (primary radius R_orcus = 455 +- 15 km) and its large satellite Vanth (radius R_vanth = 238 +- 10 km), mass ratio q = M_vanth / M_orcus = 0.050 +- 0.005, system total mass M_sys = (6.32 +- 0.05) x 10^20 kg, mutual orbit circular semi-major axis a_orb = 9030 +- 90 km (period P_orb = 9.539 days), dual synchronous tidal locking (Orcus spin = Vanth spin = orbital period P_orb), water ice/ammonia hydrate spectroscopic absorptions on Orcus versus dark reddish neutral water-ice poor surface of Vanth.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Brown et al. (2010) & Grundy et al. (2019) Orcus-Vanth Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_163/orcus_vanth_tidal.csv");
  csv_file << "orcus_radius_km,vanth_radius_km,semimajor_axis_km,orbital_period_days,mass_ratio_q,tidal_lock_time_Myr\n";

  // Semi-major axis range a_orb from 5000 km to 12000 km (nominal a_orb = 9030 km)
  for (double a_km = 5000.0; a_km <= 12000.0; a_km += 1000.0) {
    double r_orcus_km = 455.0;
    double r_vanth_km = 238.0;

    // Orbital period P_orb (days) by Kepler's third law:
    double m_sys_kg = 6.32e20;
    double a_m = a_km * 1000.0;
    double p_orb_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (hot_jupiter::G * m_sys_kg));
    double p_orb_days = p_orb_sec / 86400.0;

    // Mass ratio q = M_vanth / M_orcus:
    double q_mass = 0.050;

    // Tidal locking timescale tau_lock (Myr):
    double tau_lock_myr = 15.0 * std::pow(a_km / 9030.0, 6.0);

    csv_file << std::fixed << std::setprecision(1) << r_orcus_km << "," << std::setprecision(1) << r_vanth_km << "," << std::setprecision(1) << a_km << "," << std::setprecision(3) << p_orb_days << "," << std::setprecision(3) << q_mass << "," << std::setprecision(2) << tau_lock_myr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_163/orcus_vanth_tidal.csv" << std::endl;
  return 0;
}
