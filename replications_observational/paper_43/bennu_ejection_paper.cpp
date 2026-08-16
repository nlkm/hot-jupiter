// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #43: Asteroid 101955 Bennu Surface Regolith Particle Ejection

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #43: BENNU REGOLITH PARTICLE EJECTION ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::BennuParticleEjectionModel model;
  double v_ej = model.particle_ejection_velocity_m_s();
  double r_p = model.mean_particle_radius_cm();
  double sigma_stress = model.thermal_fracture_stress_pa();
  double rate_day = model.ejection_events_per_day();

  // OSIRIS-REx optical navigation camera tracking (Lauretta 2019 Science, Hergenrother 2019)
  double obs_vej = 0.50;      // m/s mean ejection speed (0.15 - 3.3 m/s)
  double obs_rp = 1.5;        // cm pebble size (0.5 - 6.0 cm)
  double obs_stress = 1.2e5;  // Pa diurnal thermal fatigue stress
  double obs_rate = 2.0;      // events per day during perihelion

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Particle Ejection Velocity (Model)  = " << v_ej << " m/s (Observed: " << obs_vej << " m/s)" << std::endl;
  std::cout << "Mean Particle Radius (Model)        = " << r_p << " cm (Observed: " << obs_rp << " cm)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Thermal Fracture Fatigue Stress     = " << sigma_stress << " Pa (Observed: " << obs_stress << " Pa)" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Daily Ejection Frequency            = " << rate_day << " events/day (Observed: " << obs_rate << " events/day)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Relative Velocity Discrepancy       = " << std::abs((v_ej - obs_vej) / obs_vej) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
