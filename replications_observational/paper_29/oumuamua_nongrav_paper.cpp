// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #29: 1I/'Oumuamua Interstellar Non-Gravitational Acceleration & Sublimation Analysis

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #29: 1I/'OUMUAMUA NON-GRAVITATIONAL ACCELERATION ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::OumuamuaNonGravitationalModel model;
  double ecc = model.orbital_eccentricity();
  double a1 = model.non_grav_radial_accel_1au_m_s2();
  double mdot = model.volatile_sublimation_mass_loss_kg_s();
  double a2 = model.transverse_acceleration_a2();

  // HST, VLT, and CFHT astrometric orbital tracking (Micheli et al. 2018 Nature; Seligman & Laughlin 2020)
  double obs_ecc = 1.20;       // Hyperbolic excess velocity v_inf = 26.3 km/s
  double obs_a1 = 4.92e-6;     // m/s^2 at 1 AU (4.92 +/- 0.16 x 10^-6 m/s^2)
  double obs_mdot = 1.50;      // kg/s (H2 / N2 ice sublimation rocket thrust)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Orbital Eccentricity e (Model)      = " << ecc << " (Observed: " << obs_ecc << ")" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Non-Grav Radial Accel A1 at 1 AU    = " << a1 << " m/s^2 (Observed: " << obs_a1 << " m/s^2)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Volatile Mass Loss Rate mdot (Model)= " << mdot << " kg/s (Inferred: " << obs_mdot << " kg/s)" << std::endl;
  std::cout << "Transverse Accel Parameter A2       = " << a2 << std::endl;
  std::cout << "Relative Acceleration Discrepancy   = " << std::abs((a1 - obs_a1) / obs_a1) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
