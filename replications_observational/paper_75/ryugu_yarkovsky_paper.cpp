// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #75: Asteroid (162173) Ryugu Yarkovsky Drift Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #75: RYUGU THERMAL INERTIA & YARKOVSKY DRIFT" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::RyuguYarkovskyModel model;

  const double da_dt_m_yr = model.yarkovsky_drift_m_yr(); // -215.0 m/yr
  const double a0_au = 1.1896;


  // Along-track orbital timing and semi-major axis evolution over 100 years (linear scale 1950-2050):
  // Delta a(t) = (da/dt) * t
  // Delta along_track_km(t) = 0.5 * (3 * n_0 / a_0) * (da/dt) * t^2 * (a0 * AU)
  const double au_m = 1.495978707e11;
  const double n0_rad_yr = 2.0 * M_PI / std::pow(a0_au, 1.5);

  std::ofstream out("replications_observational/paper_75/ryugu_orbital_drift_evolution.csv");
  out << "time_years,semimajor_axis_offset_km,along_track_displacement_km\n";

  for (double t_yr = 0.0; t_yr <= 100.0; t_yr += 2.0) {
    double delta_a_km = (da_dt_m_yr * t_yr) / 1000.0;
    
    // Quadratic along-track advance: Delta L = 0.5 * (3 * n / a) * (da/dt) * t^2
    double delta_l_km = 0.5 * (3.0 * n0_rad_yr / (a0_au * au_m)) * (da_dt_m_yr) * (t_yr * t_yr) * (a0_au * au_m) / 1000.0;

    out << t_yr << "," << delta_a_km << "," << delta_l_km << "\n";
  }
  out.close();

  std::cout << "Generated Asteroid Ryugu Yarkovsky Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
