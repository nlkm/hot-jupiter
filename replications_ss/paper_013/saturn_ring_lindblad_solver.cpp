// Solver for Paper #13: Saturn Ring Density Waves & Satellite Lindblad Resonances (Goldreich & Tremaine 1978)
// Evaluates resonant Lindblad angular momentum flux and torque density exerted on ring particles.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Goldreich & Tremaine (1978) Lindblad Resonance Solver ===" << std::endl;

  hot_jupiter::PlanetaryRingModel ring_model;

  std::ofstream csv_file("replications_ss/paper_013/saturn_lindblad_torque.csv");
  csv_file << "satellite_mass_kg,torque_nm_prom,torque_nm_pan\n";

  // Prometheus (m = 1.6e17 kg) and Pandora (m = 1.4e17 kg)
  for (double m_ratio = 0.1; m_ratio <= 3.0; m_ratio += 0.1) {
    double m_sat = m_ratio * 1.0e17;
    double t_prom = ring_model.lindblad_resonance_torque_nm(m_sat, 1.3935e8);
    double t_pan = ring_model.lindblad_resonance_torque_nm(m_sat, 1.4172e8);

    csv_file << std::scientific << std::setprecision(6) << m_sat << "," << t_prom << "," << t_pan << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_013/saturn_lindblad_torque.csv" << std::endl;
  return 0;
}
