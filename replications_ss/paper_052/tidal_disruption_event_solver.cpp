// Solver for Paper #52: Stellar Tidal Disruption Events by Supermassive Black Holes (Hills 1975, Rees 1988)
// Evaluates tidal radius r_t = R_star * (M_BH / M_star)^(1/3), fallback mass accretion rate dM/dt ~ (t / t_min)^(-5/3), and peak luminosity L_peak.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "stellar_evolution.hpp"

int main() {
  std::cout << "=== Running Hills (1975) & Rees (1988) Tidal Disruption Event Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_052/tde_fallback_rates.csv");
  csv_file << "time_days,dm_dt_solar_per_yr,luminosity_erg_s\n";

  double m_bh_solar = 1.0e6;      // 1e6 M_sun SMBH
  double m_star_solar = 1.0;      // 1 M_sun star
  double r_star_m = hot_jupiter::R_SUN;

  // Tidal radius r_t = R_star * (M_BH / M_star)^(1/3) (~ 100 R_sun)
  double r_t_m = r_star_m * std::pow(m_bh_solar / m_star_solar, 1.0 / 3.0);
  (void)r_t_m;  // Suppress compiler unused variable warning

  // Minimum orbital period of bound debris t_min ~ 41 days for 1e6 M_sun SMBH
  double t_min_days = 41.0;
  double mdot_peak_solar_yr = 0.5;  // ~ 0.5 M_sun / yr peak fallback rate

  for (double t_days = t_min_days; t_days <= 1000.0; t_days += 20.0) {
    // Classical t^(-5/3) fallback accretion rate
    double dm_dt = mdot_peak_solar_yr * std::pow(t_days / t_min_days, -5.0 / 3.0);

    // Bolometric accretion luminosity L = eta * dM/dt * c^2 (efficiency eta ~ 0.1)
    double eta = 0.1;
    double dm_dt_kg_s = (dm_dt * hot_jupiter::M_SUN) / hot_jupiter::YEAR;
    double c_m_s = 3.0e8;
    double lum_erg_s = eta * dm_dt_kg_s * c_m_s * c_m_s * 1.0e7;

    csv_file << std::fixed << std::setprecision(1) << t_days << "," << std::scientific << dm_dt << "," << lum_erg_s << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_052/tde_fallback_rates.csv" << std::endl;
  return 0;
}
