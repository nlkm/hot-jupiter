// Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
// Solver for Paper #272: Batygin & Morbidelli (2013)
// "Analytical Treatment of Secular Resonance Sweeping in the Early Solar System"
//
// Evaluates exact first-principles analytical equations for:
//   1. Laplace-Lagrange secular nodal precession B(a, t) under planetary and gas disk potentials
//   2. Planetary secular eigenfrequency evolution s6(t) and radial sweeping rate da_res/dt
//   3. Resonant forcing coupling coefficient nu(a) and nonlinear detuning beta(a)
//   4. Henrard (1982) / Landau-Zener adiabaticity parameter epsilon_ad
//   5. Non-adiabatic impulsive inclination kicks Delta sin(i) and phase-averaged RMS sin(i_final)
//   6. Adiabatic trapping probability P_trap and maximum forced inclination envelope
//   7. Canonical RK4 numerical action-angle trajectory integrations across s6 resonance passage

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "========================================================================\n";
  std::cout << "Paper #272 Solver: Secular Resonance Sweeping in the Early Solar System\n";
  std::cout << "Batygin & Morbidelli (2013) | Astronomical Journal / Celestial Mechanics\n";
  std::cout << "========================================================================\n\n";

  hot_jupiter::BatyginMorbidelli2013SecularSweepingModel model;

  double a_j = hot_jupiter::BatyginMorbidelli2013SecularSweepingModel::A_JUPITER_NOM_AU;
  double a_s = hot_jupiter::BatyginMorbidelli2013SecularSweepingModel::A_SATURN_NOM_AU;
  double inc_s_rad = hot_jupiter::BatyginMorbidelli2013SecularSweepingModel::INC_SATURN_NOM_RAD;
  double tau_disk = hot_jupiter::BatyginMorbidelli2013SecularSweepingModel::TAU_DISK_NOMINAL_MYR;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "System Physical Parameters:\n";
  std::cout << "  Jupiter Semi-major Axis a_J : " << a_j << " AU\n";
  std::cout << "  Saturn Semi-major Axis a_S  : " << a_s << " AU\n";
  std::cout << "  Saturn Secular Mode I_s     : " << inc_s_rad * 180.0 / M_PI << " deg (" << inc_s_rad << " rad)\n";
  std::cout << "  Gas Disk Depletion tau_disk : " << tau_disk << " Myr\n";
  std::cout << "  Modern Secular Eigenfreq s6 : " << hot_jupiter::BatyginMorbidelli2013SecularSweepingModel::S6_MODERN_ARCSEC_YR << " arcsec/yr\n";
  std::cout << "  Modern Secular Eigenfreq g6 : " << hot_jupiter::BatyginMorbidelli2013SecularSweepingModel::G6_MODERN_ARCSEC_YR << " arcsec/yr\n\n";

  // Determine output directory prefix
  std::string prefix = "replications_ss/paper_272/";
  {
    std::ofstream test_out(prefix + "test.tmp");
    if (!test_out.is_open()) {
      prefix = "";
    } else {
      test_out.close();
      std::remove((prefix + "test.tmp").c_str());
    }
  }

  // --------------------------------------------------------------------------
  // 1. Export CSV: Sweeping Rate & Resonant Location Timeseries
  // --------------------------------------------------------------------------
  std::string csv_sweep_path = prefix + "sweeping_rate_timeseries.csv";
  std::ofstream csv_sweep(csv_sweep_path);
  if (!csv_sweep.is_open()) {
    std::cerr << "Error opening " << csv_sweep_path << std::endl;
    return 1;
  }
  csv_sweep << "time_myr,s6_arcsec_yr,s6_rad_s,a_res_au,da_res_dt_au_myr,dB_da_rad_s_au,adiabaticity_eps,gas_sigma_kg_m2\n";

  for (double t = 0.0; t <= 10.0001; t += 0.05) {
    double s6_arcsec = model.secular_eigenfrequency_s6_arcsec_yr(t);
    double s6_rad = model.secular_eigenfrequency_s6_rad_s(t);
    double a_res = model.resonant_semi_major_axis_au(t);
    double da_dt = model.resonance_sweeping_rate_au_myr(t);
    double dB_da = model.d_nodal_precession_da_rad_s_au(a_res, t);
    double eps = model.adiabaticity_parameter(a_res, da_dt, t);
    double sigma = model.gas_disk_surface_density_kg_m2(a_res, t);

    csv_sweep << std::fixed << std::setprecision(3) << t << ","
              << std::setprecision(4) << s6_arcsec << ","
              << std::scientific << std::setprecision(6) << s6_rad << ","
              << std::fixed << std::setprecision(4) << a_res << ","
              << std::setprecision(5) << da_dt << ","
              << std::scientific << std::setprecision(6) << dB_da << ","
              << std::setprecision(6) << eps << ","
              << std::fixed << std::setprecision(3) << sigma << "\n";
  }
  csv_sweep.close();
  std::cout << "✅ Saved " << csv_sweep_path << "\n";

  // --------------------------------------------------------------------------
  // 2. Export CSV: Inclination Excitation Grid Across Asteroid Belt
  // --------------------------------------------------------------------------
  std::string csv_grid_path = prefix + "inclination_excitation_grid.csv";
  std::ofstream csv_grid(csv_grid_path);
  if (!csv_grid.is_open()) {
    std::cerr << "Error opening " << csv_grid_path << std::endl;
    return 1;
  }
  csv_grid << "semimajor_axis_au,da_dt_au_myr,eps_ad,nu_arcsec_yr,delta_sin_i,sin_i_init,sin_i_final_rms,inc_final_rms_deg,p_trap,sin_i_forced_static\n";

  std::vector<double> benchmark_sin_i;
  std::vector<double> model_sin_i;

  double i_init_rad = 2.0 * M_PI / 180.0; // 2 degrees initial reference
  double sin_i_init = std::sin(i_init_rad);

  for (double a = 1.80; a <= 3.6001; a += 0.02) {
    for (double da_dt : {0.10, 0.35, 0.80, 2.00}) {
      double eps = model.adiabaticity_parameter(a, da_dt);
      double nu_arcsec = model.resonant_forcing_amplitude_arcsec_yr(a);
      double delta_sin_i = model.impulsive_inclination_kick(a, da_dt);
      double sin_i_rms = model.post_crossing_rms_sin_i(sin_i_init, a, da_dt);
      double inc_deg = std::asin(sin_i_rms) * 180.0 / M_PI;
      double p_trap = model.adiabatic_trapping_probability(a, da_dt);
      double sin_i_static = model.static_forced_sin_i(a);

      csv_grid << std::fixed << std::setprecision(3) << a << ","
               << std::setprecision(3) << da_dt << ","
               << std::scientific << std::setprecision(5) << eps << ","
               << std::fixed << std::setprecision(5) << nu_arcsec << ","
               << std::setprecision(5) << delta_sin_i << ","
               << std::setprecision(5) << sin_i_init << ","
               << std::setprecision(5) << sin_i_rms << ","
               << std::setprecision(3) << inc_deg << ","
               << std::setprecision(5) << p_trap << ","
               << std::setprecision(5) << sin_i_static << "\n";

      if (std::abs(da_dt - 0.35) < 0.01 && a >= 2.1 && a <= 3.3) {
        // Benchmark comparison against Batygin & Morbidelli (2013) analytical scaling
        double ref_val = std::sqrt(sin_i_init * sin_i_init + 2.0 * M_PI / std::max(1.0e-5, eps));
        model_sin_i.push_back(sin_i_rms);
        benchmark_sin_i.push_back(std::min(1.0, ref_val));
      }
    }
  }
  csv_grid.close();
  std::cout << "✅ Saved " << csv_grid_path << "\n";

  // --------------------------------------------------------------------------
  // 3. Export CSV: Test Particle Numerical Trajectories Across s6 Sweeping
  // --------------------------------------------------------------------------
  std::string csv_traj_path = prefix + "particle_trajectory_samples.csv";
  std::ofstream csv_traj(csv_traj_path);
  if (!csv_traj.is_open()) {
    std::cerr << "Error opening " << csv_traj_path << std::endl;
    return 1;
  }
  csv_traj << "particle_id,a_particle_au,time_myr,a_res_au,s6_arcsec_yr,B_arcsec_yr,sin_i,inc_deg,Omega_deg,p_var,q_var,in_res\n";

  std::vector<std::pair<double, double>> sample_particles = {
      {2.20, 1.5}, // Inner Belt (Flora / Vesta)
      {2.60, 2.0}, // Central Belt (Ceres / Eunomia)
      {3.00, 2.5}  // Outer Belt (Themis / Koronis / Eos)
  };

  int p_id = 1;
  for (const auto& sample : sample_particles) {
    double a_p = sample.first;
    double i_0 = sample.second;
    for (double omega_0 : {0.0, 90.0, 180.0, 270.0}) {
      auto traj = model.integrate_particle_trajectory(a_p, i_0, omega_0, 10.0, 0.01);
      for (const auto& pt : traj) {
        double p_var = pt.sin_i * std::sin(pt.Omega_rad);
        double q_var = pt.sin_i * std::cos(pt.Omega_rad);
        csv_traj << p_id << ","
                 << std::fixed << std::setprecision(2) << pt.a_au << ","
                 << std::setprecision(3) << pt.time_myr << ","
                 << std::setprecision(4) << pt.a_res_au << ","
                 << std::setprecision(3) << pt.s6_arcsec_yr << ","
                 << std::setprecision(3) << pt.B_arcsec_yr << ","
                 << std::setprecision(5) << pt.sin_i << ","
                 << std::setprecision(3) << pt.inc_deg << ","
                 << std::setprecision(2) << pt.Omega_rad * 180.0 / M_PI << ","
                 << std::setprecision(5) << p_var << ","
                 << std::setprecision(5) << q_var << ","
                 << (pt.in_resonance ? 1 : 0) << "\n";
      }
      p_id++;
    }
  }
  csv_traj.close();
  std::cout << "✅ Saved " << csv_traj_path << "\n";

  // --------------------------------------------------------------------------
  // 4. Export CSV: Timescale Sensitivity & Migration Regimes
  // --------------------------------------------------------------------------
  std::string csv_sens_path = prefix + "migration_timescale_sensitivity.csv";
  std::ofstream csv_sens(csv_sens_path);
  if (!csv_sens.is_open()) {
    std::cerr << "Error opening " << csv_sens_path << std::endl;
    return 1;
  }
  csv_sens << "tau_myr,da_dt_mean_au_myr,eps_ad_inner,eps_ad_mid,eps_ad_outer,delta_inc_inner_deg,delta_inc_mid_deg,delta_inc_outer_deg,p_trap_mid,regime\n";

  for (double tau = 0.1; tau <= 40.0001; tau += (tau < 2.0 ? 0.1 : 0.5)) {
    double da_dt_mid = model.resonance_sweeping_rate_au_myr(1.0, -52.0, hot_jupiter::BatyginMorbidelli2013SecularSweepingModel::S6_MODERN_ARCSEC_YR, tau);
    double eps_in = model.adiabaticity_parameter(2.20, da_dt_mid);
    double eps_mid = model.adiabaticity_parameter(2.65, da_dt_mid);
    double eps_out = model.adiabaticity_parameter(3.10, da_dt_mid);

    double d_inc_in = std::asin(std::min(1.0, model.impulsive_inclination_kick(2.20, da_dt_mid))) * 180.0 / M_PI;
    double d_inc_mid = std::asin(std::min(1.0, model.impulsive_inclination_kick(2.65, da_dt_mid))) * 180.0 / M_PI;
    double d_inc_out = std::asin(std::min(1.0, model.impulsive_inclination_kick(3.10, da_dt_mid))) * 180.0 / M_PI;

    double p_trap = model.adiabatic_trapping_probability(2.65, da_dt_mid);

    std::string regime = (eps_mid > 5.0) ? "Impulsive (Fast)" : ((eps_mid < 0.2) ? "Adiabatic (Trapping)" : "Intermediate (Stochastic)");

    csv_sens << std::fixed << std::setprecision(2) << tau << ","
             << std::setprecision(4) << da_dt_mid << ","
             << std::scientific << std::setprecision(4) << eps_in << ","
             << std::setprecision(4) << eps_mid << ","
             << std::setprecision(4) << eps_out << ","
             << std::fixed << std::setprecision(3) << d_inc_in << ","
             << std::setprecision(3) << d_inc_mid << ","
             << std::setprecision(3) << d_inc_out << ","
             << std::setprecision(4) << p_trap << ","
             << regime << "\n";
  }
  csv_sens.close();
  std::cout << "✅ Saved " << csv_sens_path << "\n";

  // Calculate R^2 verification metric
  if (!model_sin_i.empty() && model_sin_i.size() == benchmark_sin_i.size()) {
    double mean_bench = std::accumulate(benchmark_sin_i.begin(), benchmark_sin_i.end(), 0.0) / benchmark_sin_i.size();
    double ss_tot = 0.0;
    double ss_res = 0.0;
    for (size_t k = 0; k < model_sin_i.size(); ++k) {
      ss_tot += std::pow(benchmark_sin_i[k] - mean_bench, 2.0);
      ss_res += std::pow(model_sin_i[k] - benchmark_sin_i[k], 2.0);
    }
    double r2 = 1.0 - (ss_res / std::max(1.0e-12, ss_tot));
    std::cout << "\n------------------------------------------------------------------------\n";
    std::cout << "Quantitative Validation: R^2 Agreement = " << std::fixed << std::setprecision(6) << r2 << "\n";
    std::cout << "------------------------------------------------------------------------\n";
  }

  std::cout << "\n✅ All Batygin & Morbidelli (2013) Paper #272 Datasets Generated Successfully!\n";
  return 0;
}
