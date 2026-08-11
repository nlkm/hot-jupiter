// Solver for Paper #36: YORP Effect & Asteroid Spin Vector Evolution (Rubincam 2000, Vokrouhlický et al. 2015)
// Evaluates YORP spin acceleration domega/dt ~ Y_0 * (F_sun / (rho * R^2 * c)) and spin-up/spin-down timescales.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Rubincam (2000) & Vokrouhlický (2015) YORP Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_036/yorp_spin_timescales.csv");
  csv_file << "radius_m,radius_km,t_yorp_myr,domega_dt_rad_s_yr\n";

  double f_sun_1au = hot_jupiter::L_SUN / (4.0 * hot_jupiter::PI * hot_jupiter::AU * hot_jupiter::AU);
  double c = 299792458.0;
  double rho = 2500.0;     // rocky asteroid density [kg/m^3]
  double y0_shape = 0.01;  // dimensionless YORP shape asymmetry factor

  // Asteroid radii from 50 m to 10 km at 2.5 AU
  for (double radius_m = 50.0; radius_m <= 10000.0; radius_m *= 2.0) {
    double a_au = 2.5;
    double f_sun = f_sun_1au / (a_au * a_au);

    // YORP torque scaling T_YORP ~ Y_0 * F_sun * R^3 / c
    // Moment of inertia C ~ (2/5) * M * R^2 ~ (8/15) * pi * rho * R^5
    // domega/dt = T_YORP / C ~ (15 / (8 * pi)) * (Y_0 * F_sun) / (rho * R^2 * c)
    double domega_dt_rad_s2 = (15.0 / (8.0 * hot_jupiter::PI)) * (y0_shape * f_sun) / (rho * radius_m * radius_m * c);
    double domega_dt_yr = domega_dt_rad_s2 * hot_jupiter::YEAR;

    // YORP timescale to alter spin rate by 2*pi / (5 hours)
    double omega_ref = (2.0 * hot_jupiter::PI) / (5.0 * 3600.0);
    double t_yorp_sec = omega_ref / domega_dt_rad_s2;
    double t_yorp_myr = t_yorp_sec / (hot_jupiter::YEAR * 1.0e6);

    csv_file << std::fixed << std::setprecision(1) << radius_m << "," << std::setprecision(3) << (radius_m / 1000.0) << "," << std::fixed << std::setprecision(3) << t_yorp_myr << "," << std::scientific << domega_dt_yr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_036/yorp_spin_timescales.csv" << std::endl;
  return 0;
}
