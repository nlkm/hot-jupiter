// Solver for Paper #60: Giant Molecular Cloud Gravitational Collapse & Singular Isothermal Sphere Accretion (Jeans 1902, Shu 1977)
// Evaluates Shu self-similar expansion wave collapse rate dM/dt = 0.975 * c_s^3 / G and opacity-limited minimum fragment mass M_min.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "star_formation.hpp"

int main() {
  std::cout << "=== Running Jeans (1902) & Shu (1977) GMC Collapse & Fragmentation Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_060/gmc_collapse_rates.csv");
  csv_file << "cloud_temp_k,sound_speed_m_s,shu_accretion_solar_kyr,jeans_mass_solar\n";

  double mu_molecular = 2.3;  // H2 + He molecular gas mean molecular weight

  // Gas temperatures T from 10 K to 50 K
  for (double temp_k = 10.0; temp_k <= 50.0; temp_k += 5.0) {
    // Isothermal sound speed c_s = sqrt(k_B * T / (mu * m_p))
    double c_s_m_s = std::sqrt(hot_jupiter::KB * temp_k / (mu_molecular * hot_jupiter::MASS_P));

    // Shu (1977) self-similar expansion wave accretion rate dM/dt = 0.975 * c_s^3 / G
    double dm_dt_kg_s = 0.975 * std::pow(c_s_m_s, 3.0) / hot_jupiter::G;
    double dm_dt_solar_kyr = (dm_dt_kg_s * 1000.0 * hot_jupiter::YEAR) / hot_jupiter::M_SUN;

    // Jeans mass M_J = (pi / 6) * rho * lambda_J^3 ~ 1.0 * (T / 10K)^(3/2) M_sun at n_H2 = 1e4 cm^-3
    double m_jeans_solar = 1.0 * std::pow(temp_k / 10.0, 1.5);

    csv_file << std::fixed << std::setprecision(1) << temp_k << "," << std::setprecision(1) << c_s_m_s << "," << std::scientific << dm_dt_solar_kyr << "," << std::fixed << std::setprecision(2) << m_jeans_solar << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_060/gmc_collapse_rates.csv" << std::endl;
  return 0;
}
