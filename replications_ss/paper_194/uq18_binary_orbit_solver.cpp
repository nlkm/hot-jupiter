// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Grundy et al. (2012, 2019) & Benecchi et al. (2011)
// TNO Binary (148780) 2001 UQ18 Mutual Orbit & Density Engine

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::UQ18BinaryModel model;

  double a_orb_km = 8700.0;
  double M_sys_kg = 1.92e18;
  double r_primary_km = 92.0;
  double r_sec_km = 72.0;

  double period_days = model.orbital_period_days(a_orb_km, M_sys_kg);
  double density_kg_m3 = model.system_bulk_density_kg_m3(M_sys_kg, r_primary_km, r_sec_km);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "--- Paper #194: (148780) 2001 UQ18 Binary Orbit Replication ---" << std::endl;
  std::cout << "Semi-Major Axis: " << a_orb_km << " km" << std::endl;
  std::cout << "System Mass: " << M_sys_kg << " kg" << std::endl;
  std::cout << "Calculated Orbital Period: " << period_days << " days (Observed: 165.0 days)" << std::endl;
  std::cout << "Calculated Bulk Density: " << density_kg_m3 << " kg/m^3 (Observed: 395 kg/m^3)" << std::endl;

  return 0;
}
