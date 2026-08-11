// Solver for Paper #79: Protoplanetary Disk Photoevaporative Clearing & Dispersal (Hollenbach 1994, Johnstone 1998, Alexander 2006, Owen 2011)
// Evaluates EUV/X-ray gravitational radius r_g = G M_* / c_s^2, photoevaporative mass loss rate M_dot_wind, gap opening at r_g, and disk clearing timescale.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Hollenbach (1994) & Alexander (2006) Disk Photoevaporation Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_079/disk_photoevaporation_rates.csv");
  csv_file << "euv_photon_luminosity_s1,star_mass_solar,gravitational_radius_au,wind_mass_loss_solar_yr,clearing_timescale_myr\n";

  double m_star_solar = 1.0;
  double m_star_kg = m_star_solar * hot_jupiter::M_SUN;

  // EUV ionized gas sound speed c_s = 10 km/s
  double c_s_m_s = 10.0 * 1000.0;

  // Hollenbach et al. (1994) gravitational radius r_g = G M_* / c_s^2
  double r_g_m = hot_jupiter::G * m_star_kg / (c_s_m_s * c_s_m_s);
  double r_g_au = r_g_m / hot_jupiter::AU;

  // EUV photon luminosity Phi from 1e41 s^-1 to 1e43 s^-1
  for (double log_phi = 41.0; log_phi <= 43.0; log_phi += 0.2) {
    double phi_s1 = std::pow(10.0, log_phi);

    // Hollenbach (1994) EUV photoevaporative mass loss formula:
    // Mdot_wind = 4e-10 * (Phi / 1e41)^0.5 * (M_* / M_sun)^0.5 M_sun/yr
    double mdot_wind_solar_yr = 4.0e-10 * std::pow(phi_s1 / 1.0e41, 0.5) * std::pow(m_star_solar, 0.5);

    // Disk clearing timescale tau_clear ~ (1e-2 M_sun) / Mdot_wind
    double tau_clear_myr = (0.01 / mdot_wind_solar_yr) / 1.0e6;

    csv_file << std::scientific << std::setprecision(2) << phi_s1 << "," << std::fixed << std::setprecision(1) << m_star_solar << "," << std::setprecision(2) << r_g_au << "," << std::scientific << std::setprecision(2) << mdot_wind_solar_yr << "," << std::fixed << std::setprecision(2) << tau_clear_myr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_079/disk_photoevaporation_rates.csv" << std::endl;
  return 0;
}
