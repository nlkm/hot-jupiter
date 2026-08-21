// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #76: Comet 67P Asymmetric Outgassing & Non-Gravitational Acceleration Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #76: COMET 67P ASYMMETRIC OUTGASSING ACCELERATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::Comet67POutgassingModel model;

  const double q_au = 1.243;      // Perihelion distance in AU
  const double ecc = 0.641;       // Orbital eccentricity
  const double period_days = 2355.0; // 6.45 years


  // Calculate heliocentric distance r_h(t) and non-gravitational acceleration:
  // F_rocket(t) = sqrt(A1^2 + A2^2) * g(r_h(t))
  std::ofstream out("replications_observational/paper_76/comet_67p_outgassing_orbit.csv");
  out << "days_from_perihelion,heliocentric_distance_au,water_production_rate_molecules_s,nongrav_accel_1e8_au_day2\n";

  for (double dt_days = -300.0; dt_days <= +300.0; dt_days += 10.0) {
    // True anomaly and Keplerian orbit distance approximation near perihelion
    double mean_motion = 2.0 * M_PI / period_days;
    double M_anom = mean_motion * dt_days;
    // Low-order Kepler expansion for distance: r(t) ~ q * (1 + 0.5 * e * (M / (1-e))^2)
    double r_h = q_au * (1.0 + 0.5 * ecc * std::pow(M_anom / (1.0 - ecc), 2.0));
    if (r_h > 4.0) r_h = 4.0;

    double a1 = model.radial_acceleration_AU_day2(r_h);
    double a2 = model.transverse_acceleration_AU_day2(r_h);
    double a_mag_1e8 = std::sqrt(a1 * a1 + a2 * a2) * 1.0e8;


    // Peak water outgassing rate ~ 3.5e28 molecules/s at perihelion (Hansen et al. 2016)
    // Asymmetric seasonal lag: peak occurs ~ 20 days post-perihelion
    double lag_days = dt_days - 20.0;
    double r_h_lag = q_au * (1.0 + 0.5 * ecc * std::pow(mean_motion * lag_days / (1.0 - ecc), 2.0));
    double q_h2o = 3.5e28 * model.marsden_g_function(r_h_lag);

    out << dt_days << "," << r_h << "," << q_h2o << "," << a_mag_1e8 << "\n";
  }
  out.close();

  std::cout << "Generated Comet 67P Sublimation & Outgassing Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
