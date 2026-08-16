// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #42: Neptune Great Dark Spot Vortex & Companion Cloud Dynamics

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #42: NEPTUNE GREAT DARK SPOT VORTEX DYNAMICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::NeptuneGreatDarkSpotModel model;
  double u_wind = model.zonal_wind_speed_m_s();
  double u_drift = model.vortex_drift_speed_m_s();
  double r_vort = model.vortex_radius_km();
  double z_cloud = model.methane_companion_cloud_alt_km();

  // Voyager 2 ISS & HST WFC3 observations (Smith 1989 Science, Sromovsky 1993, Wong 2022)
  double obs_uwind = -400.0; // m/s retrograde equatorial jet
  double obs_udrift = 15.0;  // m/s northward/retrograde drift relative to ambient wind
  double obs_rvort = 5000.0; // km semi-major radius of dark oval
  double obs_zcloud = 50.0;  // km altitude of bright methane companion clouds above tropopause

  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Zonal Wind Speed (Model)            = " << u_wind << " m/s (Observed: " << obs_uwind << " m/s)" << std::endl;
  std::cout << "Vortex Drift Speed (Model)          = " << u_drift << " m/s (Observed: " << obs_udrift << " m/s)" << std::endl;
  std::cout << "Vortex Semi-Major Radius (Model)    = " << r_vort << " km (Observed: " << obs_rvort << " km)" << std::endl;
  std::cout << "Methane Companion Cloud Altitude    = " << z_cloud << " km (Observed: " << obs_zcloud << " km)" << std::endl;
  std::cout << "Relative Wind Speed Discrepancy     = " << std::abs((u_wind - obs_uwind) / obs_uwind) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
