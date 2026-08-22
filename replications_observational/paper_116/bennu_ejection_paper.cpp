// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #116: Bennu Regolith Particle Ejection Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #116: BENNU REGOLITH PARTICLE EJECTION DYNAMICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::BennuParticleEjectionModel model;

  const double v_ej = model.particle_ejection_velocity_m_s();       // ~ 0.50 m/s
  const double r_part = model.mean_particle_radius_cm();            // ~ 1.5 cm
  const double sigma_th = model.thermal_fracture_stress_pa();       // ~ 1.2e5 Pa
  const double rate_events = model.ejection_events_per_day();        // ~ 2.0 events/day

  std::cout << "Mean Particle Ejection Velocity: " << v_ej << " m/s (" << (v_ej * 100.0) << " cm/s)" << std::endl;
  std::cout << "Mean Ejected Particle Radius: " << r_part << " cm" << std::endl;
  std::cout << "Diurnal Thermal Fracturing Stress: " << sigma_th << " Pa" << std::endl;
  std::cout << "Observed Ejection Event Frequency: " << rate_events << " events/day" << std::endl;

  // Track Ballistic Flight Altitude z(t) vs Flight Time t from 0 to 24 hours (linear time scale):
  // Microgravity g ~ 6.0e-5 m/s^2, Escape velocity v_esc ~ 0.20 m/s
  // At v_0 = 0.15 m/s (suborbital hop): max altitude z ~ v0^2 / (2g) ~ 187 m, flight time ~ 5000 s (1.4 h)
  // At v_0 = 0.50 m/s (hyperbolic escape / high orbit): altitude grows continuously
  std::ofstream out("replications_observational/paper_116/bennu_trajectory_evolution.csv");
  out << "flight_time_hours,suborbital_hop_altitude_m,escape_particle_altitude_m\n";

  const double g_eff = 6.0e-5; // m/s^2

  for (double t_hr = 0.0; t_hr <= 24.0; t_hr += 0.2) {
    double t_sec = t_hr * 3600.0;

    // Suborbital hop: v0 = 0.15 m/s
    double v0_hop = 0.15;
    double t_flight_hop = 2.0 * v0_hop / g_eff; // ~ 5000 s = 1.39 h
    double z_hop = (t_sec <= t_flight_hop) ? (v0_hop * t_sec - 0.5 * g_eff * t_sec * t_sec) : 0.0;

    // Escape particle: v0 = 0.50 m/s (solar radiation pressure + gravity perturbation)
    double v0_esc = v_ej; // 0.50 m/s
    double z_esc = v0_esc * t_sec - 0.5 * g_eff * t_sec * t_sec + 0.05 * std::pow(t_sec / 3600.0, 1.8);

    out << t_hr << "," << z_hop << "," << z_esc << "\n";
  }
  out.close();

  std::cout << "Generated Bennu Particle Ejection Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
