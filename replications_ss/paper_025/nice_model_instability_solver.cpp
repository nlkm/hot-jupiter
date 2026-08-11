// Solver for Paper #25: Nice Model Resonances & Planetary Migration Instability (Tsiganis et al. 2005, Morbidelli et al. 2005)
// Evaluates 2:1 Jupiter-Saturn mean motion resonance crossing and planetesimal belt scattering.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Tsiganis et al. (2005) & Morbidelli et al. (2005) Nice Model Solver ===" << std::endl;

  hot_jupiter::NiceModelResonanceCrossing nice_model;

  std::ofstream csv_file("replications_ss/paper_025/nice_model_eccentricities.csv");
  csv_file << "time_myr,m_belt_earth,e_kick_uranes_neptune\n";

  for (double time_myr = 100.0; time_myr <= 1000.0; time_myr += 50.0) {
    double m_belt = 35.0 * (1.0 - (time_myr / 1500.0));
    double e_kick = nice_model.ice_giant_eccentricity_kick(time_myr, m_belt);

    csv_file << std::fixed << std::setprecision(1) << time_myr << "," << std::setprecision(2) << m_belt << "," << std::setprecision(4) << e_kick << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_025/nice_model_eccentricities.csv" << std::endl;
  return 0;
}
