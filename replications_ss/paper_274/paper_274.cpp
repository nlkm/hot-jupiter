// Copyright 2026 Antigravity Scientific Automation & Solar System Dynamics Replication Campaign
// First-principles replication of Vokrouhlický et al. (2015), Asteroids IV, pp. 509-531 (arXiv:1502.01249)
// "The Yarkovsky and YORP Effects in Small Body Dynamics"
// High-Precision C++ Solver Engine for Diurnal & Seasonal Thermal Photon Recoil Drift Rates da/dt,
// YORP Torques, Asteroid Family V-Shape Envelopes & Coupled Spin-Orbit State Evolution.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::Vokrouhlicky2015YarkovskyYORPModel model;

  std::cout << "============================================================================" << std::endl;
  std::cout << "Paper #274: Vokrouhlický et al. (2015) The Yarkovsky and YORP Effects      " << std::endl;
  std::cout << "First-Principles C++ Solver: Thermal Photon Recoil & Spin-Orbit Dynamics    " << std::endl;
  std::cout << "============================================================================" << std::endl;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Solar Flux at 1 AU:               " << model.solar_flux_w_m2(1.0) << " W/m^2" << std::endl;
  std::cout << "Nominal Asteroid Bond Albedo:     " << model.ALBEDO_NOM << std::endl;
  std::cout << "Nominal Asteroid Emissivity:      " << model.EMISSIVITY_NOM << std::endl;
  std::cout << "Surface Roughness Enhancement xi: " << model.XI_ROUGHNESS_NOM << std::endl;
  std::cout << "Moment of Inertia Factor alpha:   " << model.ALPHA_INERTIA_NOM << std::endl;
  std::cout << "Nominal YORP Torque Coeff Y_0:    " << model.YORP_COEFF_NOM << std::endl;
  std::cout << "----------------------------------------------------------------------------" << std::endl;

  // ===========================================================================
  // 1. Yarkovsky Semi-Major Axis Drift da/dt Benchmark Comparison (10 Asteroids)
  // ===========================================================================
  std::cout << "\n[1] Directly Measured Yarkovsky Drift Rates vs C++ Thermal Model:" << std::endl;
  std::cout << std::setw(24) << "Asteroid"
            << std::setw(10) << "D [m]"
            << std::setw(10) << "a [AU]"
            << std::setw(8)  << "e"
            << std::setw(8)  << "obl[°]"
            << std::setw(16) << "Obs da/dt[AU/Myr]"
            << std::setw(16) << "Mod da/dt[AU/Myr]"
            << std::setw(14) << "Obs [m/yr]"
            << std::setw(14) << "Mod [m/yr]"
            << std::endl;

  auto yark_bench = model.get_yarkovsky_benchmark_asteroids();
  std::ofstream csv_yark("replications_ss/paper_274/yarkovsky_drift_comparison.csv");
  csv_yark << "designation,diameter_m,density_kg_m3,a_au,eccentricity,obliquity_deg,rot_period_hr,thermal_inertia,"
           << "obs_dadt_au_myr,obs_err_au_myr,mod_dadt_diurnal_au_myr,mod_dadt_seasonal_au_myr,mod_dadt_total_au_myr,"
           << "obs_dadt_m_yr,mod_dadt_total_m_yr,residual_au_myr,residual_m_yr\n";

  double ss_tot_yark = 0.0;
  double ss_res_yark = 0.0;
  double mean_obs_yark = 0.0;

  for (const auto& pt : yark_bench) {
    mean_obs_yark += pt.obs_dadt_au_myr;
  }
  mean_obs_yark /= yark_bench.size();

  for (const auto& pt : yark_bench) {
    double mod_diurnal_au = model.diurnal_drift_au_myr(pt.diameter_m, pt.density_kg_m3, pt.a_au,
                                                      pt.eccentricity, pt.obliquity_deg, pt.rot_period_hr,
                                                      pt.thermal_inertia);
    double mod_seasonal_au = model.seasonal_drift_au_myr(pt.diameter_m, pt.density_kg_m3, pt.a_au,
                                                        pt.eccentricity, pt.obliquity_deg, pt.thermal_inertia);
    double mod_total_au = mod_diurnal_au + mod_seasonal_au;
    double mod_total_m_yr = (mod_total_au * hot_jupiter::AU) / 1.0e6;

    double diff_au = pt.obs_dadt_au_myr - mod_total_au;
    double diff_m_yr = pt.obs_dadt_m_yr - mod_total_m_yr;

    ss_res_yark += diff_au * diff_au;
    ss_tot_yark += (pt.obs_dadt_au_myr - mean_obs_yark) * (pt.obs_dadt_au_myr - mean_obs_yark);

    std::cout << std::setw(24) << pt.designation
              << std::setw(10) << std::setprecision(0) << pt.diameter_m
              << std::setw(10) << std::setprecision(4) << pt.a_au
              << std::setw(8)  << std::setprecision(3) << pt.eccentricity
              << std::setw(8)  << std::setprecision(1) << pt.obliquity_deg
              << std::setw(16) << std::setprecision(2) << pt.obs_dadt_au_myr
              << std::setw(16) << std::setprecision(2) << mod_total_au
              << std::setw(14) << std::setprecision(1) << pt.obs_dadt_m_yr
              << std::setw(14) << std::setprecision(1) << mod_total_m_yr
              << std::endl;

    csv_yark << "\"" << pt.designation << "\","
             << std::fixed << std::setprecision(1) << pt.diameter_m << ","
             << std::setprecision(1) << pt.density_kg_m3 << ","
             << std::setprecision(4) << pt.a_au << ","
             << std::setprecision(4) << pt.eccentricity << ","
             << std::setprecision(1) << pt.obliquity_deg << ","
             << std::setprecision(3) << pt.rot_period_hr << ","
             << std::setprecision(1) << pt.thermal_inertia << ","
             << std::setprecision(4) << pt.obs_dadt_au_myr << ","
             << std::setprecision(4) << pt.obs_err_au_myr << ","
             << std::setprecision(4) << mod_diurnal_au << ","
             << std::setprecision(4) << mod_seasonal_au << ","
             << std::setprecision(4) << mod_total_au << ","
             << std::setprecision(2) << pt.obs_dadt_m_yr << ","
             << std::setprecision(2) << mod_total_m_yr << ","
             << std::setprecision(4) << diff_au << ","
             << std::setprecision(2) << diff_m_yr << "\n";
  }
  csv_yark.close();
  std::cout << "✅ Saved replications_ss/paper_274/yarkovsky_drift_comparison.csv" << std::endl;

  double r2_yark = 1.0 - (ss_res_yark / ss_tot_yark);
  double rmse_yark = std::sqrt(ss_res_yark / yark_bench.size());

  std::cout << "----------------------------------------------------------------------------" << std::endl;
  std::cout << "  Yarkovsky da/dt Model vs Observed Parity R^2: " << std::setprecision(5) << r2_yark << std::endl;
  std::cout << "  Root-Mean-Square Error (RMSE) [AU/Myr]:       " << std::setprecision(4) << rmse_yark << " AU/Myr" << std::endl;
  std::cout << "  (Requirement R^2 >= 0.98: " << (r2_yark >= 0.98 ? "PASSED ✅" : "FAILED ❌") << ")" << std::endl;
  std::cout << "----------------------------------------------------------------------------" << std::endl;

  // ===========================================================================
  // 2. YORP Spin Acceleration domega/dt Benchmark Comparison (6 Asteroids)
  // ===========================================================================
  std::cout << "\n[2] Directly Measured YORP Rotational Accelerations vs C++ Model:" << std::endl;
  std::cout << std::setw(26) << "Asteroid"
            << std::setw(8)  << "D [m]"
            << std::setw(10) << "a [AU]"
            << std::setw(10) << "P_rot [h]"
            << std::setw(22) << "Obs dω/dt [rad/d^2]"
            << std::setw(22) << "Mod dω/dt [rad/d^2]"
            << std::endl;

  auto yorp_bench = model.get_yorp_benchmark_asteroids();
  std::ofstream csv_yorp("replications_ss/paper_274/yorp_spin_rate_comparison.csv");
  csv_yorp << "designation,diameter_m,density_kg_m3,a_au,eccentricity,obliquity_deg,rot_period_hr,yorp_coeff_c,"
           << "obs_domegadt_rad_day2,obs_err_rad_day2,mod_domegadt_rad_day2,residual_rad_day2,tau_yorp_myr\n";

  double ss_tot_yorp = 0.0;
  double ss_res_yorp = 0.0;
  double mean_obs_yorp = 0.0;

  for (const auto& pt : yorp_bench) {
    mean_obs_yorp += pt.obs_domegadt_rad_day2;
  }
  mean_obs_yorp /= yorp_bench.size();

  for (const auto& pt : yorp_bench) {
    double mod_domegadt = model.yorp_spin_rate_derivative_rad_day2(pt.diameter_m, pt.density_kg_m3, pt.a_au,
                                                                   pt.obliquity_deg, pt.yorp_coeff_c);
    double tau_yorp = model.yorp_timescale_myr(pt.diameter_m, pt.density_kg_m3, pt.a_au, pt.rot_period_hr, pt.yorp_coeff_c);
    double diff = pt.obs_domegadt_rad_day2 - mod_domegadt;

    ss_res_yorp += diff * diff;
    ss_tot_yorp += (pt.obs_domegadt_rad_day2 - mean_obs_yorp) * (pt.obs_domegadt_rad_day2 - mean_obs_yorp);

    std::cout << std::setw(26) << pt.designation
              << std::setw(8)  << std::setprecision(0) << pt.diameter_m
              << std::setw(10) << std::setprecision(3) << pt.a_au
              << std::setw(10) << std::setprecision(3) << pt.rot_period_hr
              << std::setw(22) << std::scientific << std::setprecision(3) << pt.obs_domegadt_rad_day2
              << std::setw(22) << std::scientific << std::setprecision(3) << mod_domegadt
              << std::fixed
              << std::endl;

    csv_yorp << "\"" << pt.designation << "\","
             << std::fixed << std::setprecision(1) << pt.diameter_m << ","
             << std::setprecision(1) << pt.density_kg_m3 << ","
             << std::setprecision(4) << pt.a_au << ","
             << std::setprecision(4) << pt.eccentricity << ","
             << std::setprecision(1) << pt.obliquity_deg << ","
             << std::setprecision(4) << pt.rot_period_hr << ","
             << std::setprecision(4) << pt.yorp_coeff_c << ","
             << std::scientific << std::setprecision(5) << pt.obs_domegadt_rad_day2 << ","
             << std::setprecision(5) << pt.obs_err_rad_day2 << ","
             << std::setprecision(5) << mod_domegadt << ","
             << std::setprecision(5) << diff << ","
             << std::fixed << std::setprecision(3) << tau_yorp << "\n";
  }
  csv_yorp.close();
  std::cout << "✅ Saved replications_ss/paper_274/yorp_spin_rate_comparison.csv" << std::endl;

  double r2_yorp = 1.0 - (ss_res_yorp / ss_tot_yorp);
  std::cout << "----------------------------------------------------------------------------" << std::endl;
  std::cout << "  YORP dω/dt Model vs Observed Parity R^2: " << std::setprecision(5) << r2_yorp << std::endl;
  std::cout << "----------------------------------------------------------------------------" << std::endl;

  // ===========================================================================
  // 3. Asteroid Family V-Shape Slopes & Collisional Ages (8 Families)
  // ===========================================================================
  std::cout << "\n[3] Asteroid Family Yarkovsky V-Shape Slopes & Chronology:" << std::endl;
  std::cout << std::setw(18) << "Family"
            << std::setw(10) << "a_c [AU]"
            << std::setw(16) << "Obs Age [Myr]"
            << std::setw(16) << "Mod Age [Myr]"
            << std::setw(16) << "Slope C [AU km]"
            << std::setw(30) << "Classification"
            << std::endl;

  auto fam_bench = model.get_asteroid_family_benchmarks();
  std::ofstream csv_fam("replications_ss/paper_274/asteroid_family_vshapes.csv");
  csv_fam << "family_name,center_a_au,obs_age_myr,obs_err_myr,mod_age_myr,v_slope_c_au_km,residual_myr,classification\n";

  double ss_tot_fam = 0.0;
  double ss_res_fam = 0.0;
  double mean_obs_fam = 0.0;

  for (const auto& pt : fam_bench) {
    mean_obs_fam += pt.obs_age_myr;
  }
  mean_obs_fam /= fam_bench.size();

  for (const auto& pt : fam_bench) {
    double mod_age = model.family_age_from_slope_myr(pt.v_slope_c_au_km, pt.center_a_au);
    double diff = pt.obs_age_myr - mod_age;

    ss_res_fam += diff * diff;
    ss_tot_fam += (pt.obs_age_myr - mean_obs_fam) * (pt.obs_age_myr - mean_obs_fam);

    std::cout << std::setw(18) << pt.family_name
              << std::setw(10) << std::fixed << std::setprecision(3) << pt.center_a_au
              << std::setw(16) << std::setprecision(2) << pt.obs_age_myr
              << std::setw(16) << std::setprecision(2) << mod_age
              << std::setw(16) << std::scientific << std::setprecision(2) << pt.v_slope_c_au_km
              << std::setw(30) << pt.classification
              << std::endl;

    csv_fam << "\"" << pt.family_name << "\","
            << std::fixed << std::setprecision(4) << pt.center_a_au << ","
            << std::setprecision(2) << pt.obs_age_myr << ","
            << std::setprecision(2) << pt.obs_err_myr << ","
            << std::setprecision(2) << mod_age << ","
            << std::scientific << std::setprecision(5) << pt.v_slope_c_au_km << ","
            << std::fixed << std::setprecision(2) << diff << ","
            << "\"" << pt.classification << "\"\n";
  }
  csv_fam.close();
  std::cout << "✅ Saved replications_ss/paper_274/asteroid_family_vshapes.csv" << std::endl;

  double r2_fam = 1.0 - (ss_res_fam / ss_tot_fam);
  std::cout << "----------------------------------------------------------------------------" << std::endl;
  std::cout << "  Asteroid Family Age Model vs Observed R^2: " << std::setprecision(5) << r2_fam << std::endl;
  std::cout << "----------------------------------------------------------------------------" << std::endl;

  // ===========================================================================
  // 4. Coupled Yarkovsky-YORP Dynamical Evolution Track (50 Myr Integration)
  // ===========================================================================
  std::ofstream csv_track("replications_ss/paper_274/coupled_yarkovsky_yorp_evolution.csv");
  csv_track << "time_myr,a_au,omega_rad_s,period_hr,obliquity_deg,da_dt_au_myr,domega_dt_rad_day2,dgamma_dt_deg_myr\n";

  // Simulate test asteroid: D = 500 m, initial a = 2.45 AU (near 3:1 resonance boundary),
  // initial P = 6.0 hr, initial obliquity = 45 deg, density = 2000 kg/m^3
  double cur_a = 2.450;
  double cur_d = 500.0;
  double cur_rho = 2000.0;
  double cur_gamma = 45.0; // deg
  double cur_period = 6.0; // hr
  double cur_omega = (2.0 * hot_jupiter::PI) / (cur_period * 3600.0);
  double dt_myr = 0.10;

  for (double t = 0.0; t <= 50.001; t += dt_myr) {
    double da_dt = model.total_drift_au_myr(cur_d, cur_rho, cur_a, 0.15, cur_gamma, cur_period, 200.0);
    double domegadt_rad_day2 = model.yorp_spin_rate_derivative_rad_day2(cur_d, cur_rho, cur_a, cur_gamma, 0.025);
    double dgamma_dt = model.yorp_obliquity_derivative_deg_myr(cur_d, cur_rho, cur_a, cur_period, cur_gamma, 0.020);

    csv_track << std::fixed << std::setprecision(2) << t << ","
              << std::setprecision(6) << cur_a << ","
              << std::scientific << std::setprecision(6) << cur_omega << ","
              << std::fixed << std::setprecision(4) << cur_period << ","
              << std::setprecision(3) << cur_gamma << ","
              << std::setprecision(5) << da_dt << ","
              << std::scientific << std::setprecision(5) << domegadt_rad_day2 << ","
              << std::fixed << std::setprecision(4) << dgamma_dt << "\n";

    // Advance state
    cur_a += da_dt * dt_myr;
    double domega_dt_s2 = domegadt_rad_day2 / (86400.0 * 86400.0);
    cur_omega += domega_dt_s2 * (dt_myr * 1.0e6 * model.SECONDS_PER_YEAR);

    // Rubble-pile spin barrier / mass shedding regulation (P_crit ~ 2.2 hr)
    double omega_crit = (2.0 * hot_jupiter::PI) / (2.20 * 3600.0);
    double omega_slow = (2.0 * hot_jupiter::PI) / (100.0 * 3600.0);
    if (cur_omega > omega_crit) {
      cur_omega = omega_crit; // Shedding limits spin
    } else if (cur_omega < omega_slow) {
      cur_omega = omega_slow; // Tumbling / NPA regime
    }
    cur_period = (2.0 * hot_jupiter::PI) / (cur_omega * 3600.0);

    cur_gamma += dgamma_dt * dt_myr;
    if (cur_gamma < 0.0) cur_gamma = 0.0;
    if (cur_gamma > 180.0) cur_gamma = 180.0;
  }
  csv_track.close();
  std::cout << "✅ Saved replications_ss/paper_274/coupled_yarkovsky_yorp_evolution.csv" << std::endl;

  // ===========================================================================
  // 5. Parameter Sensitivity & Scaling Grid Sweeps
  // ===========================================================================
  std::ofstream csv_sens("replications_ss/paper_274/parameter_sensitivity_sweep.csv");
  csv_sens << "diameter_m,a_au,obliquity_deg,thermal_inertia,dadt_diurnal_au_myr,dadt_seasonal_au_myr,dadt_total_au_myr,theta_diurnal,theta_seasonal\n";

  for (double d = 100.0; d <= 10000.0; d *= 1.77828) { // 100m to 10km
    for (double a = 1.0; a <= 3.2; a += 0.5) {
      for (double obl = 0.0; obl <= 180.0; obl += 30.0) {
        for (double gamma_th = 50.0; gamma_th <= 500.0; gamma_th += 150.0) {
          double d_diurn = model.diurnal_drift_au_myr(d, 2000.0, a, 0.10, obl, 6.0, gamma_th);
          double d_seas = model.seasonal_drift_au_myr(d, 2000.0, a, 0.10, obl, gamma_th);
          double d_tot = d_diurn + d_seas;
          double th_d = model.diurnal_thermal_parameter(a, 6.0, gamma_th);
          double th_s = model.seasonal_thermal_parameter(a, gamma_th);

          csv_sens << std::fixed << std::setprecision(1) << d << ","
                   << std::setprecision(2) << a << ","
                   << std::setprecision(1) << obl << ","
                   << std::setprecision(1) << gamma_th << ","
                   << std::setprecision(5) << d_diurn << ","
                   << std::setprecision(5) << d_seas << ","
                   << std::setprecision(5) << d_tot << ","
                   << std::setprecision(4) << th_d << ","
                   << std::setprecision(4) << th_s << "\n";
        }
      }
    }
  }
  csv_sens.close();
  std::cout << "✅ Saved replications_ss/paper_274/parameter_sensitivity_sweep.csv" << std::endl;

  std::cout << "============================================================================" << std::endl;
  std::cout << "All C++ Engine Simulations & Parity Checks Completed Successfully." << std::endl;
  std::cout << "============================================================================" << std::endl;

  return 0;
}
