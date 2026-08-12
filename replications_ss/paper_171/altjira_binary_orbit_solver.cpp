// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Grundy et al. (2012) & Thirouin et al. (2014)
// Classical TNO (148780) Altjira Mutual Orbit & Density Engine

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::AltjiraBinaryModel model;

  double a_orb_km = 9900.0;
  double M_sys_kg = 3.99e18;
  double r_eq_km = 123.0;

  double period_days = model.orbital_period_days(a_orb_km, M_sys_kg);
  double density_kg_m3 = model.system_bulk_density_kg_m3(M_sys_kg, r_eq_km);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "--- Paper #171: (148780) Altjira Binary Orbit Replication ---" << std::endl;
  std::cout << "Semi-Major Axis: " << a_orb_km << " km" << std::endl;
  std::cout << "System Mass: " << M_sys_kg << " kg" << std::endl;
  std::cout << "Calculated Orbital Period: " << period_days << " days (Observed: 139.6 days)" << std::endl;
  std::cout << "Calculated Bulk Density: " << density_kg_m3 << " kg/m^3 (Observed: 510 kg/m^3)" << std::endl;

  return 0;
}
