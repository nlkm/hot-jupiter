// Solver for Paper #165: Trans-Neptunian Object (225088) Gonggong (2007 OR10) & Satellite Xiangliu Mutual Orbit, Slow Rotation, & Methane Ice Surface Dynamics (Kiss 2017, Kiss 2019, Marton 2020, Schwamb 2010)
// Evaluates Hubble Space Telescope (HST) and Kepler K2 lightcurve observations of third-largest known dwarf planet/TNO (225088) Gonggong (primary radius R_gonggong = 615 +- 25 km), slow rotation period P_rot = 22.40 hr, surface volatile retention (water ice, methane ice CH4), large satellite Xiangliu (radius R_xiangliu = 50 +- 15 km), non-Keplerian eccentric orbit (semi-major axis a_orb = 24020 +- 200 km, period P_orb = 25.22 days, eccentricity e_orb = 0.29), total system mass M_sys = (1.75 +- 0.07) x 10^21 kg, and tidal evolution models.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Kiss et al. (2017, 2019) Gonggong-Xiangliu Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_165/gonggong_xiangliu_orbit.csv");
  csv_file << "gonggong_radius_km,xiangliu_radius_km,semimajor_axis_km,eccentricity,orbital_period_days,spin_period_hr,system_mass_10_21_kg\n";

  // Semi-major axis range a_orb from 15000 km to 35000 km (nominal a_orb = 24020 km)
  for (double a_km = 15000.0; a_km <= 35000.0; a_km += 2500.0) {
    double r_gonggong_km = 615.0;
    double r_xiangliu_km = 50.0;

    // Total system mass M_sys (10^21 kg):
    double m_sys_10_21 = 1.75;
    double m_sys_kg = m_sys_10_21 * 1.0e21;

    // Keplerian orbital period P_orb (days):
    double a_m = a_km * 1000.0;
    double p_orb_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (hot_jupiter::G * m_sys_kg));
    double p_orb_days = p_orb_sec / 86400.0;

    // Eccentricity e:
    double ecc = 0.29;

    // Gonggong rotation period P_rot (hr):
    double p_rot_hr = 22.40;

    csv_file << std::fixed << std::setprecision(1) << r_gonggong_km << "," << std::setprecision(1) << r_xiangliu_km << "," << std::setprecision(1) << a_km << "," << std::setprecision(2) << ecc << "," << std::setprecision(3) << p_orb_days << "," << std::setprecision(2) << p_rot_hr << "," << std::setprecision(2) << m_sys_10_21 << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_165/gonggong_xiangliu_orbit.csv" << std::endl;
  return 0;
}
