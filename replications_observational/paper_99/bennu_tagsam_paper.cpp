// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #99: Bennu Surface Granular Mechanics & TAGSAM Penetration Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #99: BENNU SURFACE GRANULAR MECHANICS & TAGSAM" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::BennuTAGSAMGranularModel model;

  const double g_eff = model.surface_gravity_m_s2();         // 6.0e-5 m/s^2 (60 um/s^2)
  const double cohesion = model.surface_cohesion_pa();        // 1.5 Pa (near-zero cohesion)
  const double z_pen = model.tagsam_penetration_depth_m();    // 0.488 m (48.8 cm)
  const double m_sample = model.sample_mass_grams();          // 121.6 g

  std::cout << "Bennu Surface Effective Microgravity: " << g_eff << " m/s^2 (" << (g_eff * 1e6) << " um/s^2)" << std::endl;
  std::cout << "Regolith Surface Cohesion: " << cohesion << " Pa" << std::endl;
  std::cout << "TAGSAM Maximum Penetration Depth: " << z_pen << " m (" << (z_pen * 100.0) << " cm)" << std::endl;
  std::cout << "Curated Sample Mass Acquired: " << m_sample << " grams" << std::endl;

  // Track TAGSAM Contact & Penetration Kinematics over 0.0 to 6.0 seconds (linear time scale):
  // Contact begins at t = 0.0 s with v0 = 0.10 m/s (10 cm/s)
  // Reaches maximum penetration at t ~ 4.88 s before back-away thrusters fire
  std::ofstream out("replications_observational/paper_99/bennu_tagsam_penetration.csv");
  out << "time_seconds,penetration_depth_cm,resistance_force_newtons,penetration_velocity_cm_s\n";

  for (double t_s = 0.0; t_s <= 6.0; t_s += 0.1) {
    double z_m = 0.0;
    double v_ms = 0.0;

    if (t_s <= 4.88) {
      z_m = 0.10 * t_s; // Steady descent at 10 cm/s
      v_ms = 0.10;
    } else {
      // Back-away thruster retraction
      double dt_back = t_s - 4.88;
      z_m = z_pen - 0.35 * dt_back;
      v_ms = -0.35;
      if (z_m < 0.0) z_m = 0.0;
    }

    double force_n = model.resistance_force_newtons(z_m, std::abs(v_ms));
    if (z_m <= 0.0) force_n = 0.0;

    out << t_s << "," << (z_m * 100.0) << "," << force_n << "," << (v_ms * 100.0) << "\n";
  }
  out.close();

  std::cout << "Generated Bennu TAGSAM Penetration Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
