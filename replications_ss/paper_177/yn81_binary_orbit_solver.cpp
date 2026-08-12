// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Grundy et al. (2012, 2019) & Benecchi et al. (2011)
// TNO Binary (80801) 2000 YN81 Mutual Orbit & Density Engine

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::YN81BinaryModel model;

  double a_orb_km = 12800.0;
  double M_sys_kg = 1.12e18;
  double r_primary_km = 72.0;
  double r_sec_km = 59.0;

  double period_days = model.orbital_period_days(a_orb_km, M_sys_kg);
  double density_kg_m3 = model.system_bulk_density_kg_m3(M_sys_kg, r_primary_km, r_sec_km);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "--- Paper #177: (80801) 2000 YN81 Binary Orbit Replication ---" << std::endl;
  std::cout << "Semi-Major Axis: " << a_orb_km << " km" << std::endl;
  std::cout << "System Mass: " << M_sys_kg << " kg" << std::endl;
  std::cout << "Calculated Orbital Period: " << period_days << " days (Observed: 410.0 days)" << std::endl;
  std::cout << "Calculated Bulk Density: " << density_kg_m3 << " kg/m^3 (Observed: 470 kg/m^3)" << std::endl;

  return 0;
}
