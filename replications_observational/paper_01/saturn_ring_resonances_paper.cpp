// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #1: Cassini RSS/VIMS Saturn Ring Resonance Analysis & Shepherd Dynamics
// First-principles replication of Goldreich & Tremaine (1978, 1979) and Cassini Occultation Datasets

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::SaturnRingResonanceAnalysisModel model;

  // Cassini Observational Dataset Constants
  double mimas_a_km = 185539.0;
  double janus_a_km = 151460.0;
  double prometheus_a_km = 139380.0;
  double prometheus_M_kg = 1.595e17;
  double pandora_a_km = 141720.0;
  double pandora_M_kg = 1.371e17;

  // 1. Mimas 2:1 ILR (Cassini Division Inner Edge)
  double r_mimas_21_calc = model.inner_lindblad_resonance_km(mimas_a_km, 2, 1);
  double r_mimas_21_obs = 117580.0;

  // 2. Janus 7:6 ILR (A-Ring Outer Boundary Edge)
  double r_janus_76_calc = model.inner_lindblad_resonance_km(janus_a_km, 7, 6);
  double r_janus_76_obs = 136770.0;

  // 3. F-Ring Shepherd Torque Equilibrium Core
  double r_f_ring_calc = model.shepherd_torque_balance_km(prometheus_a_km, prometheus_M_kg, pandora_a_km, pandora_M_kg);
  double r_f_ring_obs = 140220.0;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #1: CASSINI RSS/VIMS SATURN RING RESONANCES & DYNAMICS" << std::endl;
  std::cout << "================================================================================" << std::endl;
  std::cout << "Mimas 2:1 ILR (Cassini Division): Calc = " << r_mimas_21_calc << " km | Obs = " << r_mimas_21_obs << " km" << std::endl;
  std::cout << "Janus 7:6 ILR (A-Ring Edge):      Calc = " << r_janus_76_calc << " km | Obs = " << r_janus_76_obs << " km" << std::endl;
  std::cout << "F-Ring Shepherd Balance Core:     Calc = " << r_f_ring_calc << " km | Obs = " << r_f_ring_obs << " km" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
