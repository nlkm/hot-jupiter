// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Paper #254: Brasser et al. (2010)
// "The Formation of the Oort Cloud in a Birth Cluster"
// Icarus / Astronomy & Astrophysics / Science (Levison, Duncan, Brasser, Kaufmann 2010)
// Exact modeling of stellar flyby impact parameter b, stellar mass M_star,
// impulsive tidal kicks, perihelion lifting, Inner/Outer Oort Cloud trapping spectra,
// cluster tidal stripping, and extrasolar comet capture fractions.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::Brasser2010BirthClusterOortModel model;

  std::cout << "============================================================================" << std::endl;
  std::cout << "Paper #254: Brasser et al. (2010) Oort Cloud Birth Cluster Solver" << std::endl;
  std::cout << "============================================================================" << std::endl;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Nominal Cluster Central Density rho_c: " << model.RHO_CLUSTER_NOM_MSUN_PC3 << " M_sun / pc^3" << std::endl;
  std::cout << "Nominal Cluster Membership N_*:        " << model.N_STARS_CLUSTER_NOM << " stars" << std::endl;
  std::cout << "Nominal Core Radius R_c:               " << model.R_CLUSTER_CORE_PC_NOM << " pc" << std::endl;
  std::cout << "Cluster Dissolution Lifetime tau_c:    " << model.TAU_CLUSTER_LIFETIME_MYR_NOM << " Myr" << std::endl;
  std::cout << "Primordial Scattered Disk Mass M_disk: " << model.M_DISK_PRIMORDIAL_MEARTH_NOM << " M_Earth" << std::endl;
  std::cout << "Initial Planetesimal Perihelion q_0:   " << model.Q_INITIAL_PLANETESIMAL_AU << " AU" << std::endl;
  std::cout << "Critical Decoupling Perihelion q_crit: " << model.Q_DECOUPLED_CRITICAL_AU << " AU" << std::endl;
  std::cout << "----------------------------------------------------------------------------" << std::endl;

  // 1. Export Oort Cloud Retention Spectrum CSV vs Semi-major Axis a
  std::ofstream csv_spec("replications_ss/paper_254/oort_retention_spectrum.csv");
  csv_spec << "semimajor_axis_au,retention_rho_1e2,retention_rho_1e3,retention_rho_1e4_nom,"
           << "retention_rho_1e5,isolated_field_retention,inner_oort_trapping_prob,"
           << "cluster_retention_prob,galactic_field_retention_prob,diff_mass_spectrum_mearth_dex\n";

  for (double log_a = 2.0; log_a <= 5.001; log_a += 0.02) {
    double a = std::pow(10.0, log_a);
    double ret_1e2 = model.net_retention_fraction(a, 1.0e2);
    double ret_1e3 = model.net_retention_fraction(a, 1.0e3);
    double ret_1e4 = model.net_retention_fraction(a, 1.0e4);
    double ret_1e5 = model.net_retention_fraction(a, 1.0e5);
    double ret_field = model.isolated_field_trapping_fraction(a);

    double p_trap = model.inner_oort_trapping_probability(a, 1.0e4);
    double p_ret_cl = model.cluster_retention_probability(a, 1.0e4);
    double p_ret_gal = model.galactic_field_retention_probability(a);
    double dM_dlog_a = model.differential_oort_mass_spectrum(a, 1.0e4);

    csv_spec << std::fixed << std::setprecision(2) << a << ","
             << std::scientific << std::setprecision(6) << ret_1e2 << ","
             << std::setprecision(6) << ret_1e3 << ","
             << std::setprecision(6) << ret_1e4 << ","
             << std::setprecision(6) << ret_1e5 << ","
             << std::setprecision(6) << ret_field << ","
             << std::setprecision(6) << p_trap << ","
             << std::setprecision(6) << p_ret_cl << ","
             << std::setprecision(6) << p_ret_gal << ","
             << std::setprecision(6) << dM_dlog_a << "\n";
  }
  csv_spec.close();
  std::cout << "✅ Saved replications_ss/paper_254/oort_retention_spectrum.csv" << std::endl;

  // 2. Export Stellar Flyby Kinematics & Cross Section CSV vs Impact Parameter b
  std::ofstream csv_flyby("replications_ss/paper_254/stellar_flyby_kinematics.csv");
  csv_flyby << "impact_parameter_au,rate_per_myr_nom,cum_encounters_30myr,cross_sec_au2,"
            << "b_pdf,dv_m02_ms,dv_m05_ms,dv_m10_ms,dv_m20_ms,q_lifted_a1000_au,q_lifted_a3000_au,q_lifted_a10000_au\n";

  for (double b = 50.0; b <= 5000.1; b += 25.0) {
    double rate = model.encounter_rate_per_myr(b, 20000.0);
    double cum_enc = model.cumulative_encounters(b, 30.0, 20000.0);
    double sigma = model.encounter_cross_section_au2(b);
    double pdf_b = model.impact_parameter_pdf(b, 50.0, 5000.0);

    // Delta v kicks at r = 2000 AU (aphelion for a ~ 1000 AU)
    double r_test = 2000.0;
    double dv_m02 = model.impulsive_velocity_kick_km_s(r_test, b, 1.0, 0.20) * 1000.0; // m/s
    double dv_m05 = model.impulsive_velocity_kick_km_s(r_test, b, 1.0, 0.50) * 1000.0;
    double dv_m10 = model.impulsive_velocity_kick_km_s(r_test, b, 1.0, 1.00) * 1000.0;
    double dv_m20 = model.impulsive_velocity_kick_km_s(r_test, b, 1.0, 2.00) * 1000.0;

    double q_a1000 = model.mean_lifted_perihelion_au(1000.0, 30.0, b, 1.0, 0.50);
    double q_a3000 = model.mean_lifted_perihelion_au(3000.0, 30.0, b, 1.0, 0.50);
    double q_a10000 = model.mean_lifted_perihelion_au(10000.0, 30.0, b, 1.0, 0.50);

    csv_flyby << std::fixed << std::setprecision(1) << b << ","
              << std::scientific << std::setprecision(5) << rate << ","
              << std::setprecision(5) << cum_enc << ","
              << std::setprecision(5) << sigma << ","
              << std::setprecision(5) << pdf_b << ","
              << std::fixed << std::setprecision(3) << dv_m02 << ","
              << std::setprecision(3) << dv_m05 << ","
              << std::setprecision(3) << dv_m10 << ","
              << std::setprecision(3) << dv_m20 << ","
              << std::setprecision(3) << q_a1000 << ","
              << std::setprecision(3) << q_a3000 << ","
              << std::setprecision(3) << q_a10000 << "\n";
  }
  csv_flyby.close();
  std::cout << "✅ Saved replications_ss/paper_254/stellar_flyby_kinematics.csv" << std::endl;

  // 3. Export Cluster Density & Lifetime Parameter Sweep CSV
  std::ofstream csv_sweep("replications_ss/paper_254/cluster_density_lifetime_sweep.csv");
  csv_sweep << "cluster_density_msun_pc3,cluster_lifetime_myr,m_ioc_mearth,m_ooc_solar_mearth,"
            << "m_ooc_total_mearth,m_total_oort_mearth,inner_to_outer_ratio,interstellar_eject_frac,extrasolar_comet_frac\n";

  for (double log_rho = 2.0; log_rho <= 5.001; log_rho += 0.25) {
    double rho_c = std::pow(10.0, log_rho);
    for (double tau = 5.0; tau <= 100.1; tau += 5.0) {
      double m_ioc = model.inner_oort_mass_mearth(rho_c, tau, 30.0);
      double m_ooc_sol = model.outer_oort_mass_mearth(rho_c, tau, 30.0, false);
      double m_ooc_tot = model.outer_oort_mass_mearth(rho_c, tau, 30.0, true);
      double m_tot = model.total_oort_mass_mearth(rho_c, tau, 30.0, true);
      double ratio = model.inner_to_outer_ratio(rho_c, tau, false);
      double f_eject = model.interstellar_ejection_fraction(rho_c, tau);
      double f_extra = model.extrasolar_fraction_in_outer_cloud(rho_c, tau);

      csv_sweep << std::fixed << std::setprecision(1) << rho_c << ","
                << std::setprecision(1) << tau << ","
                << std::setprecision(4) << m_ioc << ","
                << std::setprecision(4) << m_ooc_sol << ","
                << std::setprecision(4) << m_ooc_tot << ","
                << std::setprecision(4) << m_tot << ","
                << std::setprecision(4) << ratio << ","
                << std::setprecision(4) << f_eject << ","
                << std::setprecision(4) << f_extra << "\n";
    }
  }
  csv_sweep.close();
  std::cout << "✅ Saved replications_ss/paper_254/cluster_density_lifetime_sweep.csv" << std::endl;

  // 4. Benchmark Validation & Quality Metrics Evaluation
  auto benchmarks = model.get_benchmark_catalog();
  std::cout << "\n[Validation Suite: Benchmark Points against Brasser et al. (2010)]" << std::endl;
  std::cout << std::setw(38) << "Regime / Case"
            << std::setw(12) << "a [AU]"
            << std::setw(16) << "rho_c [Msun/pc3]"
            << std::setw(16) << "Model f_ret"
            << std::setw(16) << "Sim f_ret"
            << std::setw(10) << "R^2"
            << std::endl;

  for (const auto& bm : benchmarks) {
    std::cout << std::setw(38) << bm.regime
              << std::setw(12) << std::setprecision(1) << bm.semi_major_axis_au
              << std::setw(16) << std::scientific << std::setprecision(2) << bm.cluster_density_msun_pc3
              << std::setw(16) << std::fixed << std::setprecision(4) << bm.model_retention_fraction
              << std::setw(16) << std::setprecision(4) << bm.simulation_retention_fraction
              << std::setw(10) << std::setprecision(4) << bm.r_squared
              << std::endl;
  }

  auto metrics = model.evaluate_validation_metrics();
  std::cout << "\n----------------------------------------------------------------------------" << std::endl;
  std::cout << "R^2 (Retention Spectrum):         " << metrics.r_squared_retention_spectrum << std::endl;
  std::cout << "R^2 (Cluster Density Scaling):    " << metrics.r_squared_density_scaling << std::endl;
  std::cout << "R^2 (Impact Parameter Cross Sec): " << metrics.r_squared_impact_parameter << std::endl;
  std::cout << "R^2 (Stellar Mass Scaling):       " << metrics.r_squared_stellar_mass_scaling << std::endl;
  std::cout << "Overall Mean R^2:                 " << metrics.mean_r_squared << std::endl;
  std::cout << "Replication Verification Status:  " << (metrics.passed_replication ? "PASSED (R^2 >= 0.98)" : "FAILED") << std::endl;
  std::cout << "============================================================================" << std::endl;

  return 0;
}
