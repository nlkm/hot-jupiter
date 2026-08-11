// Solver for Paper #44: Photo-Evaporative Dispersal of Protoplanetary Disks (Hollenbach et al. 1994, Alexander et al. 2006)
// Evaluates EUV/FUV photo-evaporative mass loss rate Mdot_wind and gravitational radius r_g = G * M_sun / c_s^2.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Hollenbach (1994) & Alexander (2006) Disk Photoevaporation Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_044/disk_photoevaporation_rates.csv");
  csv_file << "phi_euv_s1,r_g_au,mdot_wind_solar_yr,t_dispersal_myr\n";

  double m_sun = hot_jupiter::M_SUN;
  double c_s_wind = 10000.0;  // ionized gas sound speed ~ 10 km/s (10,000 K)

  // Gravitational radius r_g = G * M_sun / c_s^2 (~ 8.9 AU for 10,000 K gas around 1 M_sun)
  double r_g_m = hot_jupiter::G * m_sun / (c_s_wind * c_s_wind);
  double r_g_au = r_g_m / hot_jupiter::AU;

  // EUV photon emission rates Phi_EUV from 1e41 s^-1 to 1e45 s^-1
  for (double phi_euv = 1.0e41; phi_euv <= 1.0e45; phi_euv *= 10.0) {
    // Hollenbach et al. (1994) photoevaporative wind mass loss rate Mdot_wind ~ 4e-10 * (Phi_EUV / 1e41)^0.5 M_sun/yr
    double mdot_wind_solar_yr = 4.0e-10 * std::sqrt(phi_euv / 1.0e41);
    double m_disk_init_solar = 0.01;  // typical MMSN gas disk mass 0.01 M_sun
    double t_dispersal_myr = (m_disk_init_solar / mdot_wind_solar_yr) / 1.0e6;

    csv_file << std::scientific << phi_euv << "," << std::fixed << std::setprecision(2) << r_g_au << "," << std::scientific << mdot_wind_solar_yr << "," << std::fixed << std::setprecision(2) << t_dispersal_myr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_044/disk_photoevaporation_rates.csv" << std::endl;
  return 0;
}
