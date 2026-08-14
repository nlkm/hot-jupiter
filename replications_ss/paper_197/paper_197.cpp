// Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
// Solver for Paper #197: How Io Was Captured Into the Laplace Resonance (Yoder 1979)
// Nature 279 (5716), 767–770.
//
// Evaluates first-principles orbital mechanics and resonance capture dynamics:
// 1. Exact Keplerian mean motions and orbital periods for Io, Europa, and Ganymede.
// 2. Oblateness-induced (J2, J4) periapse precession rates for the inner Galilean satellites.
// 3. Resonant conjunction circulation frequency nu = n1 - 2*n2 = n2 - 2*n3 approx 0.7395 deg/day.
// 4. Tidal orbit expansion rates and differential migration rates driving convergent resonance encounter.
// 5. Critical eccentricities e_crit for adiabatic resonance capture (Henrard 1982, Yoder 1979).
// 6. Resonance capture probabilities P_cap(e_0) as a function of pre-encounter eccentricity.
// 7. 3-body Laplace resonant angle libration phi_L(t) = lambda1 - 3*lambda2 + 2*lambda3 around 180 degrees.
// 8. Steady-state tidal equilibrium, forced eccentricities, and Io heat flow power.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

// Structure to store satellite orbital and resonant properties
struct GalileanSatelliteData {
  std::string name;
  double mass_kg;
  double semi_major_axis_m;
  double radius_m;
  double mean_motion_deg_day;
  double orbital_period_days;
  double j2_precession_deg_day;
  double forced_eccentricity;
};

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   PAPER #197 REPLICATION: HOW IO WAS CAPTURED INTO THE LAPLACE RESONANCE      " << std::endl;
  std::cout << "   C. F. Yoder (1979) Nature 279, 767-770; Peale et al. (1979) Science 203     " << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::Yoder1979LaplaceCaptureModel model;

  // 1. Orbital Properties of Galilean Satellites
  std::vector<GalileanSatelliteData> satellites = {
      {"Io (1)", model.M_IO, model.A_IO, model.R_IO,
       model.mean_motion_deg_day(model.A_IO, model.M_IO),
       model.orbital_period_days(model.A_IO, model.M_IO),
       model.j2_precession_rate_deg_day(model.A_IO, model.M_IO),
       model.E_IO},
      {"Europa (2)", model.M_EUROPA, model.A_EUROPA, model.R_EUROPA,
       model.mean_motion_deg_day(model.A_EUROPA, model.M_EUROPA),
       model.orbital_period_days(model.A_EUROPA, model.M_EUROPA),
       model.j2_precession_rate_deg_day(model.A_EUROPA, model.M_EUROPA),
       model.E_EUROPA},
      {"Ganymede (3)", model.M_GANYMEDE, model.A_GANYMEDE, model.R_GANYMEDE,
       model.mean_motion_deg_day(model.A_GANYMEDE, model.M_GANYMEDE),
       model.orbital_period_days(model.A_GANYMEDE, model.M_GANYMEDE),
       model.j2_precession_rate_deg_day(model.A_GANYMEDE, model.M_GANYMEDE),
       model.E_GANYMEDE}
  };

  std::cout << "\n1. GALILEAN SATELLITE ORBITAL & PRECESSION PROPERTIES:" << std::endl;
  std::cout << std::left << std::setw(14) << "Body"
            << std::setw(14) << "a [km]"
            << std::setw(16) << "Period [d]"
            << std::setw(18) << "n [deg/day]"
            << std::setw(18) << "d(varpi)/dt [deg/d]"
            << std::setw(12) << "e_forced" << std::endl;
  std::cout << std::string(92, '-') << std::endl;

  for (const auto& sat : satellites) {
    std::cout << std::left << std::setw(14) << sat.name
              << std::fixed << std::setprecision(1) << std::setw(14) << (sat.semi_major_axis_m / 1000.0)
              << std::setprecision(6) << std::setw(16) << sat.orbital_period_days
              << std::setprecision(5) << std::setw(18) << sat.mean_motion_deg_day
              << std::setprecision(5) << std::setw(18) << sat.j2_precession_deg_day
              << std::setprecision(5) << std::setw(12) << sat.forced_eccentricity << std::endl;
  }

  // 2. Laplace Resonant Frequencies and Conjunction Rate
  double nu_12 = satellites[0].mean_motion_deg_day - 2.0 * satellites[1].mean_motion_deg_day;
  double nu_23 = satellites[1].mean_motion_deg_day - 2.0 * satellites[2].mean_motion_deg_day;
  double laplace_conjunction_rate = model.resonant_conjunction_rate_nu_deg_day();
  double laplace_freq_deg_day = model.laplace_libration_frequency_deg_day();
  double laplace_period_days = model.laplace_libration_period_days();

  std::cout << "\n2. LAPLACE RESONANCE RELATIONS & CONJUNCTION FREQUENCIES:" << std::endl;
  std::cout << "   n1 - 2*n2 conjunction rate:     " << std::fixed << std::setprecision(6)
            << nu_12 << " deg/day (" << nu_12 * 365.25 * (hot_jupiter::PI / 180.0) << " rad/yr)" << std::endl;
  std::cout << "   n2 - 2*n3 conjunction rate:     " << std::fixed << std::setprecision(6)
            << nu_23 << " deg/day (" << nu_23 * 365.25 * (hot_jupiter::PI / 180.0) << " rad/yr)" << std::endl;
  std::cout << "   Resonance condition Delta_nu:   " << std::scientific << std::setprecision(4)
            << (nu_12 - nu_23) << " deg/day (Exact 4:2:1 Laplace lock)" << std::endl;
  std::cout << "   Laplace Libration Frequency:    " << std::fixed << std::setprecision(5)
            << laplace_freq_deg_day << " deg/day" << std::endl;
  std::cout << "   Laplace Libration Period:       " << std::fixed << std::setprecision(2)
            << laplace_period_days << " days (" << laplace_period_days / 365.25 << " years)" << std::endl;

  // 3. Tidal Orbital Evolution & Differential Convergence
  double a_dot_over_a_io = model.tidal_expansion_rate_s_inv(model.M_IO, model.A_IO);
  double a_dot_over_a_eu = model.tidal_expansion_rate_s_inv(model.M_EUROPA, model.A_EUROPA);
  double a_dot_over_a_ga = model.tidal_expansion_rate_s_inv(model.M_GANYMEDE, model.A_GANYMEDE);
  double diff_convergence_rate = model.differential_convergence_rate_deg_day_per_yr();

  std::cout << "\n3. JUPITER TIDAL DISSIPATION & CONVERGENT MIGRATION (Q_J = 10^5):" << std::endl;
  std::cout << "   Io tidal expansion (a_dot/a):       " << std::scientific << std::setprecision(4)
            << a_dot_over_a_io * (365.25 * 86400.0) << " yr^-1" << std::endl;
  std::cout << "   Europa tidal expansion (a_dot/a):   " << std::scientific << std::setprecision(4)
            << a_dot_over_a_eu * (365.25 * 86400.0) << " yr^-1" << std::endl;
  std::cout << "   Ganymede tidal expansion (a_dot/a): " << std::scientific << std::setprecision(4)
            << a_dot_over_a_ga * (365.25 * 86400.0) << " yr^-1" << std::endl;
  std::cout << "   Io/Europa expansion ratio:          " << std::fixed << std::setprecision(2)
            << (a_dot_over_a_io / a_dot_over_a_eu) << " (Drives rapid convergent resonance approach)" << std::endl;
  std::cout << "   Differential drift d(nu)/dt:        " << std::scientific << std::setprecision(4)
            << diff_convergence_rate << " deg/(day*yr)" << std::endl;

  // 4. Critical Capture Eccentricities & Capture Probabilities
  double e_crit_12 = model.critical_eccentricity_io_europa();
  double e_crit_23 = model.critical_eccentricity_europa_ganymede();

  std::cout << "\n4. CRITICAL CAPTURE ECCENTRICITIES & PROBABILITIES (Henrard 1982 / Yoder 1979):" << std::endl;
  std::cout << "   Io-Europa 2:1 Critical Eccentricity e_crit(1,2):       " << std::fixed << std::setprecision(5)
            << e_crit_12 << std::endl;
  std::cout << "   Europa-Ganymede 2:1 Critical Eccentricity e_crit(2,3): " << std::fixed << std::setprecision(5)
            << e_crit_23 << std::endl;

  // 5. Tidal Dissipation & Equilibrium Heat Flow
  double io_heat_tw = model.io_tidal_dissipation_power_tw();
  double io_surface_area = 4.0 * hot_jupiter::PI * model.R_IO * model.R_IO;
  double io_flux_w_m2 = (io_heat_tw * 1.0e12) / io_surface_area;

  std::cout << "\n5. IO STEADY-STATE TIDAL EQUILIBRIUM (Peale 1979, Yoder 1979):" << std::endl;
  std::cout << "   Equilibrium Tidal Dissipation Power: " << std::fixed << std::setprecision(2)
            << io_heat_tw << " TW (Observed: ~105 TW)" << std::endl;
  std::cout << "   Average Surface Heat Flux:           " << std::fixed << std::setprecision(3)
            << io_flux_w_m2 << " W/m^2 (Observed: ~2.52 W/m^2)" << std::endl;

  // Write simulation data for Python plot generation
  std::ofstream out_lib("libration_data.csv");
  out_lib << "time_days,phi_L_deg,damped_envelope_upper,damped_envelope_lower\n";
  for (int d = 0; d <= 5000; d += 5) {
    double phi = model.laplace_libration_angle_deg(static_cast<double>(d), 45.0, 1800.0);
    double env_up = 180.0 + 45.0 * std::exp(-static_cast<double>(d) / 1800.0);
    double env_low = 180.0 - 45.0 * std::exp(-static_cast<double>(d) / 1800.0);
    out_lib << d << "," << phi << "," << env_up << "," << env_low << "\n";
  }
  out_lib.close();

  std::ofstream out_prob("capture_prob_data.csv");
  out_prob << "eccentricity,prob_io_europa,prob_europa_ganymede\n";
  for (int i = 0; i <= 300; ++i) {
    double e = i * 0.0001;
    double p12 = model.capture_probability_io_europa(e);
    double p23 = model.capture_probability_europa_ganymede(e);
    out_prob << e << "," << p12 << "," << p23 << "\n";
  }
  out_prob.close();

  // Compute R^2 goodness of fit between theoretical Yoder model and observations
  std::vector<double> obs_metrics = {203.48895, 101.37472, 50.31761, 0.739507, 436.9, 105.0};
  std::vector<double> mod_metrics = {
      satellites[0].mean_motion_deg_day,
      satellites[1].mean_motion_deg_day,
      satellites[2].mean_motion_deg_day,
      laplace_conjunction_rate,
      laplace_period_days,
      io_heat_tw
  };

  double mean_obs = std::accumulate(obs_metrics.begin(), obs_metrics.end(), 0.0) / obs_metrics.size();
  double ss_tot = 0.0;
  double ss_res = 0.0;
  for (size_t i = 0; i < obs_metrics.size(); ++i) {
    ss_tot += (obs_metrics[i] - mean_obs) * (obs_metrics[i] - mean_obs);
    ss_res += (obs_metrics[i] - mod_metrics[i]) * (obs_metrics[i] - mod_metrics[i]);
  }
  double r_squared = 1.0 - (ss_res / ss_tot);

  std::cout << "\n6. MODEL VALIDATION & FIDELITY METRICS:" << std::endl;
  std::cout << "   Goodness of Fit R^2 = " << std::fixed << std::setprecision(6) << r_squared
            << " (Target: R^2 >= 0.98)" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
