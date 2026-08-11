// Solver for Paper #71: Magnetized Stellar Wind & Weber-Davis Angular Momentum Loss (Weber & Davis 1967, Mestel 1968)
// Evaluates Alfvén radius r_A = (B_*^2 R_*^4 / (M_dot v_wind))^(1/4), angular momentum loss rate dJ/dt = (2/3) M_dot Omega_* r_A^2, and rotational velocity braking.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Weber & Davis (1967) & Mestel (1968) Stellar Wind Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_071/weber_davis_spindown_rates.csv");
  csv_file << "magnetic_field_gauss,mass_loss_rate_solar_yr,alfven_radius_rstar,spindown_timescale_gyr\n";

  double m_star_solar = 1.0;     // Solar mass 1.0 M_sun
  double r_star_solar = 1.0;     // Solar radius 1.0 R_sun
  double p_spin_days = 25.0;     // Solar rotation period 25 days
  double v_wind_km_s = 400.0;    // Solar wind speed 400 km/s

  double m_star_kg = m_star_solar * hot_jupiter::M_SUN;
  double r_star_m = r_star_solar * hot_jupiter::R_SUN;
  double omega_star = 2.0 * hot_jupiter::PI / (p_spin_days * hot_jupiter::DAY);

  // Stellar magnetic field B_* from 1 Gauss (Sun) to 50 Gauss (young Sun)
  for (double b_gauss = 1.0; b_gauss <= 50.0; b_gauss += 5.0) {
    double b_tesla = b_gauss * 1.0e-4;
    double mdot_solar_yr = 2.0e-14 * (b_gauss / 1.0);  // Mass loss rate scaling with magnetic activity
    double mdot_kg_s = mdot_solar_yr * hot_jupiter::M_SUN / hot_jupiter::YEAR;
    double v_wind_m_s = v_wind_km_s * 1000.0;

    // Weber & Davis (1967) Alfvén radius r_A = (B_*^2 R_*^4 / (M_dot v_wind))^(1/4)
    double r_a_m = std::pow(b_tesla * b_tesla * std::pow(r_star_m, 4.0) / (mdot_kg_s * v_wind_m_s), 0.25);
    double r_a_rstar = r_a_m / r_star_m;

    // Angular momentum torque dJ/dt = (2/3) M_dot Omega_* r_A^2
    double dj_dt = (2.0 / 3.0) * mdot_kg_s * omega_star * r_a_m * r_a_m;
    double j_star = 0.1 * m_star_kg * r_star_m * r_star_m * omega_star;
    double tau_spindown_gyr = (j_star / dj_dt) / hot_jupiter::GYR;

    csv_file << std::fixed << std::setprecision(1) << b_gauss << "," << std::scientific << std::setprecision(2) << mdot_solar_yr << "," << std::fixed << std::setprecision(2) << r_a_rstar << "," << std::setprecision(2) << tau_spindown_gyr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_071/weber_davis_spindown_rates.csv" << std::endl;
  return 0;
}
