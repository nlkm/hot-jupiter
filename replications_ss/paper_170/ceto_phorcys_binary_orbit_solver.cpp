// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Grundy et al. (2007) & Santos-Sanz et al. (2012)
// Scattered Disc TNO (65489) Ceto & Satellite Phorcys Mutual Orbit & Density Engine

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::CetoPhorcysBinaryModel model;

  double a_orb_km = 1840.0;
  double M_sys_kg = 5.41e18;
  double r_ceto_km = 87.0;
  double r_phorcys_km = 66.0;

  double period_days = model.orbital_period_days(a_orb_km, M_sys_kg);
  double density_kg_m3 = model.system_bulk_density_kg_m3(M_sys_kg, r_ceto_km, r_phorcys_km);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "--- Paper #170: (65489) Ceto & Phorcys Binary Orbit Replication ---" << std::endl;
  std::cout << "Semi-Major Axis: " << a_orb_km << " km" << std::endl;
  std::cout << "System Mass: " << M_sys_kg << " kg" << std::endl;
  std::cout << "Calculated Orbital Period: " << period_days << " days (Observed: 9.554 days)" << std::endl;
  std::cout << "Calculated Bulk Density: " << density_kg_m3 << " kg/m^3 (Observed: 1370 kg/m^3)" << std::endl;

  return 0;
}
