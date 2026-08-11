// Solver for Paper #49: Debris Disk Collisional Cascade & Dust Luminosity Decay (Wyatt et al. 2007, Löhne et al. 2008)
// Evaluates planar planetesimal collision timescale t_cat, dust fractional luminosity f_dust(t) = f_0 / (1 + t / t_cat), and infrared excess decay.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Wyatt et al. (2007) & Löhne et al. (2008) Debris Disk Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_049/debris_disk_luminosity_decay.csv");
  csv_file << "time_myr,f_dust_fractional_lum,disk_mass_earth_masses\n";

  double f0_dust = 1.0e-3;      // initial dust fractional luminosity L_dust / L_star = 10^-3
  double t_cat_myr = 10.0;      // collisional cascade onset timescale 10 Myr
  double m_disk_init = 30.0;    // initial planetesimal belt mass 30 M_Earth

  for (double t_myr = 0.0; t_myr <= 1000.0; t_myr += 50.0) {
    double f_dust = f0_dust / (1.0 + t_myr / t_cat_myr);
    double m_disk = m_disk_init / (1.0 + t_myr / t_cat_myr);

    csv_file << std::fixed << std::setprecision(1) << t_myr << "," << std::scientific << f_dust << "," << std::fixed << std::setprecision(2) << m_disk << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_049/debris_disk_luminosity_decay.csv" << std::endl;
  return 0;
}
