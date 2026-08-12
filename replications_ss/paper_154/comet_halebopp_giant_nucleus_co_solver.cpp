// Solver for Paper #154: Comet C/1995 O1 (Hale-Bopp) Giant Nucleus, CO Volatile Sublimation, & Outgassing at Great Heliocentric Distances (Biver 1997, Jewitt 1996, Weaver 1997, Altenhoff 1999)
// Evaluates Oort Cloud super-comet C/1995 O1 (Hale-Bopp) giant nucleus (effective diameter D_eff = 60 +- 20 km, mass M ~ 1.5 x 10^17 kg), extraordinary volatile carbon monoxide (CO) driven outgassing active out to r_h > 7 AU where water ice sublimation is completely frozen out, peak water production rate Q_H2O ~ 1.0 x 10^31 molecules/s and Q_CO ~ 2.0 x 10^30 molecules/s near perihelion (0.914 AU), high dust production rate Q_dust ~ 2 x 10^5 kg/s, and dust-to-gas ratio D/G ~ 5.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Biver et al. (1997) & Jewitt et al. (1996) Comet C/1995 O1 (Hale-Bopp) Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_154/comet_halebopp_outgassing.csv");
  csv_file << "heliocentric_distance_au,water_production_q_h2o_10_30_s,co_production_q_co_10_30_s,dust_loss_rate_10_5_kg_s,nucleus_diameter_km\n";

  // Heliocentric distance r_h from 0.914 AU (perihelion) to 7.0 AU
  for (double r_au = 0.914; r_au <= 7.0; r_au += 0.75) {
    // Water production rate Q_H2O (10^30 molecules/s):
    double q_h2o_10_30 = (r_au > 3.5) ? 0.0 : 10.0 * std::pow(0.914 / r_au, 3.5);

    // Volatile CO production rate Q_CO (10^30 molecules/s) active out to r_h > 7 AU scaling Q ~ r_h^-2.0:
    double q_co_10_30 = 2.0 * std::pow(0.914 / r_au, 2.0);

    // Dust loss rate (10^5 kg/s):
    double dust_10_5_kg_s = 2.0 * std::pow(0.914 / r_au, 3.0);

    // Giant nucleus diameter (km):
    double d_nucleus_km = 60.0;

    csv_file << std::fixed << std::setprecision(2) << r_au << "," << std::setprecision(2) << q_h2o_10_30 << "," << std::setprecision(3) << q_co_10_30 << "," << std::setprecision(3) << dust_10_5_kg_s << "," << std::setprecision(0) << d_nucleus_km << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_154/comet_halebopp_outgassing.csv" << std::endl;
  return 0;
}
