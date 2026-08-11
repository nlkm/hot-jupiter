// Solver for Paper #35: Yarkovsky Thermal Photon Recoil & Asteroid Semi-Major Axis Drift (Vokrouhlický 1999, Bottke et al. 2006)
// Evaluates diurnal Yarkovsky drift rate da/dt ~ (4 / (9 * n * c)) * (alpha / (rho * R)) * F_sun * cos(obliquity).

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Vokrouhlický (1999) & Bottke (2006) Yarkovsky Drift Solver ===" << std::endl;

  hot_jupiter::YarkovskyThermalPhotonRecoilModel yarkovsky_model;

  std::ofstream csv_file("replications_ss/paper_035/yarkovsky_drift_rates.csv");
  csv_file << "radius_m,radius_km,da_dt_au_myr,da_100myr_au\n";

  // Asteroid radii from 10 m to 10 km at 2.5 AU
  for (double radius_m = 10.0; radius_m <= 10000.0; radius_m *= 2.0) {
    double semimajor_axis_au = 2.5;
    double density_kg_m3 = 2500.0;  // rocky S-type asteroid
    double obliquity_deg = 0.0;     // prograde rotation (maximum outward drift)

    double da_dt_au_myr = yarkovsky_model.diurnal_drift_rate_au_myr(radius_m, density_kg_m3, semimajor_axis_au, obliquity_deg);
    double da_100myr = da_dt_au_myr * 100.0;

    csv_file << std::fixed << std::setprecision(1) << radius_m << "," << std::setprecision(3) << (radius_m / 1000.0) << "," << std::scientific << da_dt_au_myr << "," << std::fixed << std::setprecision(4) << da_100myr << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_035/yarkovsky_drift_rates.csv" << std::endl;
  return 0;
}
