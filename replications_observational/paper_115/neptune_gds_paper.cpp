// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #115: Neptune Great Dark Spot Vortex Dynamics Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #115: NEPTUNE GREAT DARK SPOT VORTEX DYNAMICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::NeptuneGreatDarkSpotModel model;

  const double u_equator = model.zonal_wind_speed_m_s();             // ~ -400.0 m/s (Retrograde)
  const double v_drift = model.vortex_drift_speed_m_s();              // ~ 15.0 m/s (Poleward)
  const double r_vortex = model.vortex_radius_km();                  // ~ 5000.0 km
  const double z_cirrus = model.methane_companion_cloud_alt_km();    // ~ 50.0 km

  std::cout << "Equatorial Zonal Wind Speed: " << u_equator << " m/s (" << (u_equator * 3.6) << " km/h)" << std::endl;
  std::cout << "Great Dark Spot Drift Velocity: " << v_drift << " m/s" << std::endl;
  std::cout << "Great Dark Spot Semi-Major Radius: " << r_vortex << " km" << std::endl;
  std::cout << "Methane Companion Cirrus Altitude: " << z_cirrus << " km" << std::endl;

  // Track Zonal Wind Speed vs Planetographic Latitude phi from -70 deg to +70 deg (linear scale):
  // Intense retrograde equatorial jet peaking near -400 m/s at equator, turning prograde (+200 m/s) at high latitudes
  std::ofstream out("replications_observational/paper_115/neptune_zonal_winds.csv");
  out << "latitude_deg,zonal_wind_speed_m_s,gds_vortex_perturbation_m_s\n";

  for (double lat = -70.0; lat <= 70.0; lat += 2.0) {
    double rad = lat * M_PI / 180.0;

    // Background zonal jet profile
    double u_bg = u_equator * std::cos(rad) * std::cos(rad) + 220.0 * std::sin(2.0 * rad) * std::sin(2.0 * rad);

    // GDS local perturbation centered at -22 deg latitude
    double d_lat = lat - (-22.0);
    double u_gds = 120.0 * (d_lat / 8.0) * std::exp(-std::pow(d_lat / 8.0, 2.0));

    double u_tot = u_bg + u_gds;

    out << lat << "," << u_bg << "," << u_tot << "\n";
  }
  out.close();

  std::cout << "Generated Neptune Great Dark Spot Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
