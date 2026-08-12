// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Grundy et al. (2019) & Thirouin et al. (2014)
// Resonant TNO (88611) Teharonhiawako-Sawiskera Mutual Orbit & Density Engine

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::TeharonhiawakoBinaryModel model;

  double a_orb_km = 27600.0;
  double M_sys_kg = 2.44e18;
  double r_teh_km = 89.0;
  double r_saw_km = 61.0;

  double period_days = model.orbital_period_days(a_orb_km, M_sys_kg);
  double density_kg_m3 = model.system_bulk_density_kg_m3(M_sys_kg, r_teh_km, r_saw_km);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "--- Paper #173: (88611) Teharonhiawako & Sawiskera Binary Orbit Replication ---" << std::endl;
  std::cout << "Semi-Major Axis: " << a_orb_km << " km" << std::endl;
  std::cout << "System Mass: " << M_sys_kg << " kg" << std::endl;
  std::cout << "Calculated Orbital Period: " << period_days << " days (Observed: 828.7 days)" << std::endl;
  std::cout << "Calculated Bulk Density: " << density_kg_m3 << " kg/m^3 (Observed: 620 kg/m^3)" << std::endl;

  return 0;
}
