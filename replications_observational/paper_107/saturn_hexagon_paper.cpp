// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #107: Saturn Hexagon Rossby Wave Dynamics Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #107: SATURN NORTH POLAR HEXAGON ROSSBY WAVE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::SaturnHexagonRossbyModel model;

  const double lat_deg = model.latitude_degrees();             // ~ 78.3 deg N
  const int k_wave = model.azimuthal_wavenumber();              // 6-fold symmetry
  const double u_jet = model.zonal_jet_speed_m_s();            // ~ 100.0 m/s
  const double w_jet = model.jet_width_km();                   // ~ 1500.0 km
  const double drift_deg_yr = model.phase_drift_rate_deg_yr(); // ~ 0.01 deg/yr (Stationary)

  std::cout << "Hexagon Core Planetographic Latitude: " << lat_deg << " deg N" << std::endl;
  std::cout << "Azimuthal Wavenumber (Symmetry): " << k_wave << std::endl;
  std::cout << "Peak Prograde Zonal Jet Wind Speed: " << u_jet << " m/s (" << (u_jet * 3.6) << " km/h)" << std::endl;
  std::cout << "Jet Stream Gaussian Width: " << w_jet << " km" << std::endl;
  std::cout << "Rossby Wave Phase Drift Rate (System III): " << drift_deg_yr << " deg/yr" << std::endl;

  // Track Hexagon Radius R(lambda) and Zonal Velocity u(lambda) across Azimuth 0 to 360 deg (linear scale):
  // R_mean = R_saturn * cos(78.3 deg) ~ 60268 km * cos(78.3 deg) ~ 12220 km from pole
  std::ofstream out("replications_observational/paper_107/saturn_hexagon_azimuth_profile.csv");
  out << "azimuthal_longitude_deg,radial_distance_from_pole_km,zonal_wind_speed_m_s,coriolis_vorticity_1e5_s\n";

  const double r_mean_km = 12220.0;
  const double r_amp_km = 1450.0; // Hexagonal vertex modulation amplitude

  for (double deg = 0.0; deg <= 360.0; deg += 2.0) {
    double rad = deg * M_PI / 180.0;

    // Hexagonal geometry: 6-fold Fourier perturbation
    double r_polar_km = r_mean_km + r_amp_km * std::cos(k_wave * rad)
                                  + 0.12 * r_amp_km * std::cos(2.0 * k_wave * rad);

    // Zonal jet velocity modulated along meandering streamline
    double u_zonal = u_jet * (1.0 + 0.15 * std::cos(k_wave * rad));

    // Absolute vorticity eta = f + xi
    double f_coriolis = 2.0 * (2.0 * M_PI / (10.656 * 3600.0)) * std::sin(lat_deg * M_PI / 180.0);
    double rel_vorticity = -(u_zonal / (w_jet * 1000.0)) * std::cos(k_wave * rad);
    double total_vorticity_1e5 = (f_coriolis + rel_vorticity) * 1.0e5;

    out << deg << "," << r_polar_km << "," << u_zonal << "," << total_vorticity_1e5 << "\n";
  }
  out.close();

  std::cout << "Generated Saturn Hexagon Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
