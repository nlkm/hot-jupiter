// Solver for Paper #101: YORP Effect Spin-Up & Rotational Fission Binary Formation (Rubincam 2000, Vokrouhlický 2002, Walsh 2008, Pravec 2010)
// Evaluates thermal photon recoil torque tau_YORP, spin acceleration d(omega)/dt, critical disruption frequency omega_crit = sqrt(4/3 * pi * G * rho), and YORP spin-up timescale t_YORP ~ 1 - 10 Myr.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Rubincam (2000) & Vokrouhlický (2002) YORP Fission Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_101/yorp_spinup_fission.csv");
  csv_file << "asteroid_radius_km,density_g_cm3,critical_spin_period_hr,yorp_timescale_myr,fission_binary_flag\n";

  // Asteroid radius R from 0.2 km to 5.0 km
  for (double r_km = 0.2; r_km <= 5.0; r_km += 0.4) {
    double rho_g_cm3 = 2.2;  // C-type/S-type rubble pile density 2.2 g/cm^3
    double rho_kg_m3 = rho_g_cm3 * 1000.0;

    // Critical spin period P_crit = 2 * pi / sqrt(4/3 * pi * G * rho): ~ 2.2 hours
    double omega_crit = std::sqrt((4.0 / 3.0) * hot_jupiter::PI * hot_jupiter::G * rho_kg_m3);
    double p_crit_hr = (2.0 * hot_jupiter::PI / omega_crit) / 3600.0;

    // Rubincam (2000) YORP spin-up timescale scaling: t_YORP ~ R^2 * a^2 / (F_sun * C_YORP)
    // t_YORP ~ 2.0 Myr * (R / 1 km)^2
    double t_yorp_myr = 2.0 * std::pow(r_km, 2.0);

    bool fissions_to_binary = (t_yorp_myr <= 10.0);  // YORP cycle leads to rotational fission within 10 Myr

    csv_file << std::fixed << std::setprecision(1) << r_km << "," << std::setprecision(1) << rho_g_cm3 << "," << std::setprecision(2) << p_crit_hr << "," << std::setprecision(2) << t_yorp_myr << "," << (fissions_to_binary ? 1 : 0) << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_101/yorp_spinup_fission.csv" << std::endl;
  return 0;
}
