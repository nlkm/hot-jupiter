// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #13: Haumea Triaxial Ellipsoid Shape, Ring Dynamics, & Satellite Orbits

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #13: HAUMEA TRIAXIAL ELLIPSOID & RING DYNAMICS ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::HaumeaEllipsoidRingModel h_model;
  double r_ring_model = h_model.ring_3to1_resonance_radius_km();
  double P_hiiaka_model = h_model.hiiaka_period_days();
  double rho_model = h_model.haumea_bulk_density_kg_m3();

  double occultation_r_ring_obs = 2287.3; // km (Ortiz et al. 2017)
  double hst_P_hiiaka_obs = 49.462; // days (Ragozzine & Brown 2009)
  double occultation_rho_obs = 1885.0; // kg/m^3

  std::cout << std::fixed << std::setprecision(5);
  std::cout << "Ring 3:1 Resonance Radius (Model)   = " << r_ring_model << " km" << std::endl;
  std::cout << "Occultation Observed Ring Radius   = " << occultation_r_ring_obs << " km" << std::endl;
  std::cout << "Hi'iaka Orbital Period (Model)      = " << P_hiiaka_model << " days (Observed: " << hst_P_hiiaka_obs << " days)" << std::endl;
  std::cout << "Haumea Bulk Density (Model)         = " << rho_model << " kg/m^3 (Observed: " << occultation_rho_obs << " kg/m^3)" << std::endl;
  std::cout << "Relative Ring Radius Model Error    = " << std::abs((r_ring_model - occultation_r_ring_obs) / occultation_r_ring_obs) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
