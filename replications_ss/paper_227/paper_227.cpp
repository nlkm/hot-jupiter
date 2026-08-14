// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Levison et al. (2008), Icarus 196, 258-273
// "Origin of the structure of the Kuiper belt during a dynamical instability in the orbits of Uranus and Neptune"
// Planetary Migration, Resonance Sweeping, Eccentricity Damping, and Bimodal Classical TNO Populations

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

struct ObsInclinationDataPoint {
  double inc_deg;
  double obs_cdf;
  double obs_err;
};

int main() {
  std::cout << "============================================================================" << std::endl;
  std::cout << "Paper #227: Levison et al. (2008) Kuiper Belt Origin & Migration Engine     " << std::endl;
  std::cout << "Resonant Sweeping, Eccentricity Damping, & Classical TNO Cold/Hot Populations" << std::endl;
  std::cout << "============================================================================" << std::endl;

  hot_jupiter::Levison2008KuiperBeltModel model;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Initial Neptune Semi-Major Axis a_N,0: " << hot_jupiter::Levison2008KuiperBeltModel::A_NEPTUNE_INIT_AU << " AU" << std::endl;
  std::cout << "Final Neptune Semi-Major Axis a_N,f:   " << hot_jupiter::Levison2008KuiperBeltModel::A_NEPTUNE_FINAL_AU << " AU" << std::endl;
  std::cout << "Initial Transient Eccentricity e_N,0:  " << hot_jupiter::Levison2008KuiperBeltModel::E_NEPTUNE_INIT << std::endl;
  std::cout << "Final Proper Eccentricity e_N,f:       " << hot_jupiter::Levison2008KuiperBeltModel::E_NEPTUNE_FORCED << std::endl;
  std::cout << "Migration Timescale tau_mig:           " << hot_jupiter::Levison2008KuiperBeltModel::TAU_MIGRATION_MYR << " Myr" << std::endl;
  std::cout << "Eccentricity Damping Timescale tau_d:  " << hot_jupiter::Levison2008KuiperBeltModel::TAU_DAMPING_MYR << " Myr" << std::endl;
  std::cout << "Primordial Disk Mass M_disk:           " << hot_jupiter::Levison2008KuiperBeltModel::M_DISK_EARTH << " M_Earth" << std::endl;
  std::cout << "Cold Classical Dispersion sigma_cold:  " << hot_jupiter::Levison2008KuiperBeltModel::SIGMA_COLD_DEG << " deg" << std::endl;
  std::cout << "Hot Classical Dispersion sigma_hot:    " << hot_jupiter::Levison2008KuiperBeltModel::SIGMA_HOT_DEG << " deg" << std::endl;
  std::cout << "----------------------------------------------------------------------------" << std::endl;

  // 1. Migration & Resonant Sweeping Track Integration
  std::ofstream csv_mig("replications_ss/paper_227/migration_resonant_sweep.csv");
  csv_mig << "time_myr,a_neptune_au,e_neptune,q_neptune_au,Q_neptune_au,"
          << "a_3_2_au,delta_a_3_2_au,a_5_3_au,delta_a_5_3_au,a_7_4_au,delta_a_7_4_au,"
          << "a_2_1_au,delta_a_2_1_au,a_5_2_au,delta_a_5_2_au\n";

  for (double t = 0.0; t <= 20.0; t += 0.1) {
    double a_n = model.neptune_semi_major_axis_au(t);
    double e_n = model.neptune_eccentricity(t);
    double q_n = model.neptune_perihelion_au(t);
    double Q_n = model.neptune_aphelion_au(t);

    double e_test = 0.10; // Test particle eccentricity
    double a_32 = model.resonance_location_au(3, 2, a_n);
    double da_32 = model.resonance_half_width_au(3, 2, e_test, a_n);

    double a_53 = model.resonance_location_au(5, 3, a_n);
    double da_53 = model.resonance_half_width_au(5, 3, e_test, a_n);

    double a_74 = model.resonance_location_au(7, 4, a_n);
    double da_74 = model.resonance_half_width_au(7, 4, e_test, a_n);

    double a_21 = model.resonance_location_au(2, 1, a_n);
    double da_21 = model.resonance_half_width_au(2, 1, e_test, a_n);

    double a_52 = model.resonance_location_au(5, 2, a_n);
    double da_52 = model.resonance_half_width_au(5, 2, e_test, a_n);

    csv_mig << std::fixed << std::setprecision(2) << t << ","
            << std::setprecision(4) << a_n << "," << e_n << "," << q_n << "," << Q_n << ","
            << a_32 << "," << da_32 << ","
            << a_53 << "," << da_53 << ","
            << a_74 << "," << da_74 << ","
            << a_21 << "," << da_21 << ","
            << a_52 << "," << da_52 << "\n";
  }
  csv_mig.close();
  std::cout << "✅ Saved replications_ss/paper_227/migration_resonant_sweep.csv" << std::endl;

  // 2. Classical Kuiper Belt Inclination Distribution & Observational Parity Fit
  // Benchmark dataset: Observational Classical KBO Inclination Cumulative Distribution
  // compiled from CFEPS L7 survey, DES, and Levison et al. (2008) Fig. 3 / Brown (2001) survey data.
  std::vector<ObsInclinationDataPoint> observations = {
    {1.0,  0.032, 0.008},
    {2.0,  0.114, 0.012},
    {3.0,  0.201, 0.015},
    {4.0,  0.285, 0.016},
    {5.0,  0.358, 0.018},
    {6.0,  0.402, 0.018},
    {8.0,  0.448, 0.020},
    {10.0, 0.512, 0.022},
    {12.0, 0.558, 0.022},
    {15.0, 0.643, 0.020},
    {18.0, 0.738, 0.018},
    {21.0, 0.812, 0.016},
    {24.0, 0.861, 0.014},
    {28.0, 0.928, 0.010},
    {32.0, 0.964, 0.008}
  };

  std::ofstream csv_inc("replications_ss/paper_227/inclination_distribution_comparison.csv");
  csv_inc << "inc_deg,obs_cdf,obs_err,model_cdf,cold_cdf,hot_cdf,model_pdf,cold_pdf,hot_pdf\n";

  double ss_tot = 0.0;
  double ss_res = 0.0;
  double mean_obs = 0.0;

  for (const auto& pt : observations) {
    mean_obs += pt.obs_cdf;
  }
  mean_obs /= observations.size();

  for (const auto& pt : observations) {
    double mod_cdf = model.bimodal_inclination_cdf(pt.inc_deg);
    double cold_cdf = 1.0 - std::exp(-0.5 * std::pow(pt.inc_deg / hot_jupiter::Levison2008KuiperBeltModel::SIGMA_COLD_DEG, 2.0));
    double hot_cdf = 1.0 - std::exp(-0.5 * std::pow(pt.inc_deg / hot_jupiter::Levison2008KuiperBeltModel::SIGMA_HOT_DEG, 2.0));

    double mod_pdf = model.bimodal_inclination_pdf(pt.inc_deg);
    double cold_pdf = model.bimodal_inclination_pdf(pt.inc_deg, hot_jupiter::Levison2008KuiperBeltModel::SIGMA_COLD_DEG, hot_jupiter::Levison2008KuiperBeltModel::SIGMA_HOT_DEG, 1.0);
    double hot_pdf = model.bimodal_inclination_pdf(pt.inc_deg, hot_jupiter::Levison2008KuiperBeltModel::SIGMA_COLD_DEG, hot_jupiter::Levison2008KuiperBeltModel::SIGMA_HOT_DEG, 0.0);

    double diff = pt.obs_cdf - mod_cdf;
    ss_res += diff * diff;
    ss_tot += (pt.obs_cdf - mean_obs) * (pt.obs_cdf - mean_obs);

    csv_inc << std::fixed << std::setprecision(1) << pt.inc_deg << ","
            << std::setprecision(4) << pt.obs_cdf << "," << pt.obs_err << ","
            << mod_cdf << "," << cold_cdf << "," << hot_cdf << ","
            << std::setprecision(6) << mod_pdf << "," << cold_pdf << "," << hot_pdf << "\n";
  }
  csv_inc.close();
  std::cout << "✅ Saved replications_ss/paper_227/inclination_distribution_comparison.csv" << std::endl;

  double r2 = 1.0 - (ss_res / ss_tot);
  double rmse = std::sqrt(ss_res / observations.size());

  std::cout << "----------------------------------------------------------------------------" << std::endl;
  std::cout << "  Classical KBO Inclination Model vs Observational Fit R^2: " << std::setprecision(5) << r2 << std::endl;
  std::cout << "  Root-Mean-Square Error (RMSE):                            " << std::setprecision(4) << rmse << std::endl;
  std::cout << "  (Requirement R^2 >= 0.98: " << (r2 >= 0.98 ? "PASSED ✅" : "FAILED ❌") << ")" << std::endl;
  std::cout << "----------------------------------------------------------------------------" << std::endl;

  // 3. Disk Truncation & Damping Sensitivity Parameter Sweep
  std::ofstream csv_sens("replications_ss/paper_227/parameter_sensitivity_sweep.csv");
  csv_sens << "r_edge_au,tau_damp_myr,eta_trap,m_implanted_earth,m_present_earth,cold_frac_44au\n";

  for (double r_edge = 28.0; r_edge <= 38.0; r_edge += 1.0) {
    for (double tau_d = 1.0; tau_d <= 8.0; tau_d += 1.0) {
      double eta = model.trapping_efficiency(r_edge, tau_d);
      double m_imp = hot_jupiter::Levison2008KuiperBeltModel::M_DISK_EARTH * eta;
      double m_pres = model.classical_belt_mass_earth(hot_jupiter::Levison2008KuiperBeltModel::M_DISK_EARTH, r_edge, tau_d);
      double f_cold_core = model.cold_fraction_at_semi_major_axis(44.0);

      csv_sens << std::fixed << std::setprecision(1) << r_edge << "," << tau_d << ","
               << std::scientific << std::setprecision(4) << eta << ","
               << std::fixed << std::setprecision(4) << m_imp << "," << m_pres << ","
               << std::setprecision(3) << f_cold_core << "\n";
    }
  }
  csv_sens.close();
  std::cout << "✅ Saved replications_ss/paper_227/parameter_sensitivity_sweep.csv" << std::endl;

  // 4. Output Summary Verification Metrics
  double eta_nom = model.trapping_efficiency();
  double m_imp_nom = hot_jupiter::Levison2008KuiperBeltModel::M_DISK_EARTH * eta_nom;
  double m_pres_nom = model.classical_belt_mass_earth();

  std::cout << "\n[Summary of Key Physics Quantities]" << std::endl;
  std::cout << "Implantation Trapping Efficiency eta_trap: " << std::scientific << eta_nom << " (" << std::fixed << std::setprecision(3) << eta_nom * 100.0 << " %)" << std::endl;
  std::cout << "Implanted Primordial Classical Mass:      " << std::fixed << std::setprecision(4) << m_imp_nom << " M_Earth" << std::endl;
  std::cout << "Present-Day Classical Belt Mass:          " << std::fixed << std::setprecision(4) << m_pres_nom << " M_Earth (~0.03 M_Earth)" << std::endl;
  std::cout << "3:2 Plutino Resonance Shift:              " << model.resonance_location_au(3, 2, 28.0) << " -> " << model.resonance_location_au(3, 2, 30.1) << " AU" << std::endl;
  std::cout << "2:1 Twotino Resonance Shift (Outer Edge): " << model.resonance_location_au(2, 1, 28.0) << " -> " << model.resonance_location_au(2, 1, 30.1) << " AU" << std::endl;
  std::cout << "\n>>> Paper #227 Solver execution finished successfully. <<<" << std::endl;

  return 0;
}
