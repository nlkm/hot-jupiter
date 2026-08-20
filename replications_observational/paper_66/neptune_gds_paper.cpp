// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #66: Neptune Great Dark Spot Atmospheric Vorticity & Drift Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #66: NEPTUNE GREAT DARK SPOT VORTEX DYNAMICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::NeptuneGreatDarkSpotModel model;

  const double u_zonal = model.zonal_wind_speed_m_s();   // -400 m/s
  const double v_drift = model.vortex_drift_speed_m_s(); // 15 m/s equatorward drift


  // Track vortex latitude migration from -30 deg south toward equator (0 deg) over 300 days
  // Beta-drift velocity: c_beta = - beta * L_D^2
  std::ofstream out("replications_observational/paper_66/neptune_gds_drift_track.csv");
  out << "time_days,latitude_deg,zonal_wind_m_s,drift_speed_m_s\n";

  for (double t_days = 0.0; t_days <= 300.0; t_days += 5.0) {
    // Equatorward drift: d(lat)/dt = v_drift / (R_N * pi / 180)

    // 1 deg latitude = ~ 430 km
    double dlat_per_day = (v_drift * 86400.0 / 1000.0) / 430.0; // ~ 3.0 deg per month (~0.1 deg/day)
    double lat = -30.0 + dlat_per_day * t_days;
    if (lat > 0.0) lat = 0.0; // Vortex disrupts/oscillates near equator

    // Neptune zonal wind profile: u(lat) = -400 + 450 * sin^2(lat * pi / 180)
    double u_lat = u_zonal + 450.0 * std::pow(std::sin(lat * M_PI / 180.0), 2.0);

    out << t_days << "," << lat << "," << u_lat << "," << v_drift << "\n";
  }
  out.close();

  std::cout << "Generated Neptune Great Dark Spot Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
