// Solver for Paper #148: Comet 1P/Halley Nucleus Mass, Porosity, & Jet Outgassing Dynamics (Keller 1986, Sagdeev 1986, Whipple 1986, Rickman 1986)
// Evaluates Giotto & Vega flyby discovery of 1P/Halley dark peanut-shaped nucleus (15.3 x 7.2 x 7.2 km, volume V ~ 365 km^3), mass M = (2.2 +- 1.2) x 10^14 kg, low bulk density rho_bulk = 600 +- 200 kg/m^3 (porosity P ~ 70%), dark geometric albedo A_v ~ 0.04, discrete localized dust/gas active jets covering ~ 10-15% of sunlit surface, and peak water outgassing Q_H2O ~ 3 x 10^29 molecules/s near perihelion (0.59 AU).

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Keller et al. (1986) & Sagdeev et al. (1986) Comet 1P/Halley Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_148/comet_halley_outgassing.csv");
  csv_file << "heliocentric_distance_au,water_production_q_h2o_10_29_s,active_surface_fraction_pct,nucleus_bulk_density_kg_m3,non_grav_acceleration_10_8_au_d2\n";

  // Heliocentric distance r_h from 0.59 AU (perihelion) to 2.5 AU
  for (double r_au = 0.59; r_au <= 2.5; r_au += 0.2) {
    // Water production rate Q_H2O (10^29 molecules/s) scaling Q ~ r_h^-3.2:
    double q_h2o_10_29 = 3.0 * std::pow(0.59 / r_au, 3.2);

    // Active jet surface area fraction % (~ 12% near perihelion):
    double active_area_pct = 12.5 * std::pow(0.59 / r_au, 1.5);
    if (active_area_pct > 15.0) active_area_pct = 15.0;

    // Nucleus bulk density (kg/m^3):
    double rho_bulk = 600.0;

    // Non-gravitational rocket acceleration A2 (10^-8 AU/day^2):
    double a2_nongrav = 0.15 * std::pow(0.59 / r_au, 3.2);

    csv_file << std::fixed << std::setprecision(2) << r_au << "," << std::setprecision(3) << q_h2o_10_29 << "," << std::setprecision(1) << active_area_pct << "," << std::setprecision(0) << rho_bulk << "," << std::setprecision(3) << a2_nongrav << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_148/comet_halley_outgassing.csv" << std::endl;
  return 0;
}
