// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #30: Phobos Mars Tidal Orbital Decay & Future Ring Formation Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #30: PHOBOS MARS TIDAL DECAY & RING FORMATION ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::PhobosMarsTidalDecayModel model;
  double a0_km = model.current_semimajor_axis_km();
  double dadt = model.orbital_decay_rate_cm_yr();
  double q_mars = model.mars_tidal_quality_factor_q();
  double a_roche = model.fluid_roche_limit_km();
  double t_disrupt = model.time_to_roche_disruption_myr();
  double m_ring = model.future_ring_mass_kg();
  double tau_ring = model.future_ring_peak_optical_depth();

  // Viking, MGS, and Mars Express astrometric radio science tracking (Bills et al. 2005; Black & Mittal 2015)
  double obs_dadt = -1.82;     // cm/yr (-1.82 +/- 0.04 cm/yr)
  double obs_q_mars = 86.0;    // Mars dissipation factor
  double obs_t_disrupt = 38.5; // Myr to fluid Roche limit

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Current Semi-Major Axis a_0         = " << a0_km << " km (2.76 R_Mars)" << std::endl;
  std::cout << "Present Orbital Decay Rate da/dt    = " << dadt << " cm/yr (Observed: " << obs_dadt << " cm/yr)" << std::endl;
  std::cout << "Mars Tidal Dissipation Factor Q     = " << q_mars << " (Observed: " << obs_q_mars << ")" << std::endl;
  std::cout << "Fluid Roche Disruption Boundary     = " << a_roche << " km" << std::endl;
  std::cout << "Time to Tidal Disruption            = " << t_disrupt << " Myr (Inferred: " << obs_t_disrupt << " Myr)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Future Martian Ring Mass            = " << m_ring << " kg" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Future Ring Peak Optical Depth      = " << tau_ring << std::endl;
  std::cout << "Relative Decay Rate Discrepancy     = " << std::abs((dadt - obs_dadt) / obs_dadt) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
