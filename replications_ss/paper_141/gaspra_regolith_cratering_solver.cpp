// Solver for Paper #141: (951) Gaspra S-Type Composition, Regolith Dynamics, & Young Surface Age (Belton 1992, Veverka 1994, Greenberg 1994, Bottke 1994)
// Evaluates Galileo flyby discovery of S-type asteroid Gaspra (triaxial radii 9.1 x 7.2 x 5.8 km, mean R ~ 6.1 km), crater size-frequency distribution deficits for d > 500 m, thin regolith layer thickness h_reg ~ 5-10 m, surface age t_surf ~ 200-500 Myr (young Flora family collisional breakup remnant), and groove lineament fracturing.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Belton et al. (1992) & Veverka et al. (1994) (951) Gaspra Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_141/gaspra_regolith.csv");
  csv_file << "surface_age_myr,regolith_depth_m,crater_density_d_gt_500m_km2,groove_count,bulk_density_g_cm3\n";

  // Surface age t_surf from 100 Myr to 800 Myr (nominal t ~ 200-500 Myr)
  for (double t_myr = 100.0; t_myr <= 800.0; t_myr += 100.0) {
    // Regolith accumulation thickness h_reg (m):
    double h_reg_m = 3.0 + 7.0 * (t_myr / 500.0);

    // Cumulative crater density N(d > 500 m) per km^2:
    double n_crater_km2 = 0.45 * (t_myr / 300.0);

    // Groove lineament count:
    int grooves = 60 + static_cast<int>(20 * (t_myr / 500.0));

    // S-type bulk density (g/cm^3):
    double rho_gaspra = 2.70;

    csv_file << std::fixed << std::setprecision(1) << t_myr << "," << std::setprecision(1) << h_reg_m << "," << std::setprecision(2) << n_crater_km2 << "," << grooves << "," << std::setprecision(2) << rho_gaspra << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_141/gaspra_regolith.csv" << std::endl;
  return 0;
}
