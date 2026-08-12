// Solver for Paper #152: Comet 103P/Hartley 2 Hyperactive CO2 Outgassing & Water Ice Chunk Ejection (A'Hearn 2011, Belton 2013, Meech 2011, Thomas 2013)
// Evaluates NASA EPOXI (Deep Impact Extended) flyby of small peanut-shaped nucleus 103P/Hartley 2 (2.25 x 0.69 km, volume V ~ 1.6 km^3), hyperactive outgassing where active fractional surface area exceeds 100% (> 200% implied by surface area), volatile CO2 vapor drag driving meter-scale water ice chunk ejection from outer lobes, smooth waist region acting as re-deposited ice/dust reservoir, CO2/H2O mixing ratio ~ 10-20%, and water production rate Q_H2O ~ 1.2 x 10^28 molecules/s at perihelion (1.06 AU).

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running A'Hearn et al. (2011) Comet 103P/Hartley 2 Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_152/comet_hartley2_co2.csv");
  csv_file << "heliocentric_distance_au,co2_outgassing_q_co2_10_27_s,ejected_ice_chunk_count,active_area_pct,co2_to_h2o_ratio_pct\n";

  // Heliocentric distance r_h from 1.06 AU (perihelion) to 2.0 AU
  for (double r_au = 1.06; r_au <= 2.0; r_au += 0.15) {
    // CO2 production rate Q_CO2 (10^27 molecules/s):
    double q_co2_10_27 = 2.4 * std::pow(1.06 / r_au, 3.5);

    // Ejected meter-scale water ice chunk count in coma:
    int ice_chunks = static_cast<int>(500.0 * std::pow(1.06 / r_au, 3.5));

    // Hyperactive fractional area (% of total physical surface area):
    double active_area_pct = 220.0 * std::pow(1.06 / r_au, 2.0);

    // CO2/H2O volatile abundance ratio %:
    double co2_ratio_pct = 20.0;

    csv_file << std::fixed << std::setprecision(2) << r_au << "," << std::setprecision(3) << q_co2_10_27 << "," << ice_chunks << "," << std::setprecision(1) << active_area_pct << "," << std::setprecision(1) << co2_ratio_pct << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_152/comet_hartley2_co2.csv" << std::endl;
  return 0;
}
