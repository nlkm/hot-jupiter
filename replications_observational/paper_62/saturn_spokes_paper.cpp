// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #62: Saturn Ring Spokes Electrostatic Levitation Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #62: SATURN RING SPOKES DUST LEVITATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::SaturnRingSpokesModel model;

  const double r_grain_um = model.dust_grain_radius_um(); // 0.60 um
  const double r_b_ring_km = 115000.0;                       // Central B-ring radius


  // Plasma Debye sheath electrostatic levitation force balance:
  // F_E(z) = q * E_0 * exp(-z / lambda_D)
  // F_g(z) = m_grain * Omega_K^2 * z (vertical tidal restoring force)
  const double lambda_debye_km = 25.0; // Debye length in ring ionosphere

  std::ofstream out("replications_observational/paper_62/saturn_spoke_levitation_track.csv");
  out << "height_z_km,electrostatic_force_n,gravitational_restoring_n,optical_depth_tau\n";

  for (double z = 0.0; z <= 120.0; z += 2.0) {
    // Grain mass m = (4/3)*pi*r^3 * rho_ice (920 kg/m^3)
    double r_m = r_grain_um * 1.0e-6;
    double m_grain = (4.0 / 3.0) * M_PI * std::pow(r_m, 3.0) * 920.0;
    
    // Vertical gravity restoring: g_z = Omega_K^2 * z
    double omega_k = std::sqrt(6.674e-11 * 5.683e26 / std::pow(r_b_ring_km * 1.0e3, 3.0));
    double f_grav = m_grain * std::pow(omega_k, 2.0) * (z * 1.0e3);

    // Electrostatic levitation force
    double f_elec = 1.8e-15 * std::exp(-z / lambda_debye_km);

    // Spoke optical depth contrast profile
    double tau_spoke = 0.15 * std::exp(-std::pow(z / 40.0, 2.0));

    out << z << "," << f_elec << "," << f_grav << "," << tau_spoke << "\n";
  }
  out.close();

  std::cout << "Generated Saturn Ring Spoke Levitation Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
