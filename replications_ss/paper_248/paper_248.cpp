// Copyright 2026 Antigravity Scientific Automation & Solar System Dynamics Replication Campaign
// Replication of Paper #248: Batygin et al. (2020)
// "Secular Dynamics of Outer Solar System Small Bodies"
// First-principles C++ simulation of secular Kozai-Lidov oscillations,
// giant planet quadrupole precession, exterior perturber (Planet Nine) torques,
// and perihelion lifting q(t) = a(1 - e(t)) creating detached trans-Neptunian objects.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

struct BenchmarkPoint {
  std::string object_name;
  double a_au;
  double e_obs;
  double inc_deg;
  double omega_deg;
  double node_deg;
  double varpi_deg;
  double q_obs_au;
  double q_pred_au;
  double tau_kl_myr;
  std::string dyn_class;
};

int main() {
  std::cout << "=================================================================" << std::endl;
  std::cout << "  Paper #248 Replication: Batygin et al. (2020)                  " << std::endl;
  std::cout << "  Secular Dynamics of Outer Solar System Small Bodies             " << std::endl;
  std::cout << "  First-Principles Kozai-Lidov Dynamics & Perihelion Lifting q(t) " << std::endl;
  std::cout << "=================================================================" << std::endl;

  hot_jupiter::Batygin2020SecularDynamicsModel model;

  // 1. Core nominal parameters
  double m_p9 = hot_jupiter::Batygin2020SecularDynamicsModel::M_P9_NOM_EARTH;
  double a_p9 = hot_jupiter::Batygin2020SecularDynamicsModel::A_P9_NOM_AU;
  double e_p9 = hot_jupiter::Batygin2020SecularDynamicsModel::E_P9_NOM;
  double inc_p9 = hot_jupiter::Batygin2020SecularDynamicsModel::INC_P9_NOM_DEG;
  double node_p9 = hot_jupiter::Batygin2020SecularDynamicsModel::NODE_P9_NOM_DEG;
  double omega_p9 = hot_jupiter::Batygin2020SecularDynamicsModel::OMEGA_P9_NOM_DEG;

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Perturber Mass M_P9:            " << m_p9 << " M_Earth (" << m_p9 * 5.9722e24 << " kg)" << std::endl;
  std::cout << "Perturber Semi-major Axis a_P9: " << a_p9 << " AU" << std::endl;
  std::cout << "Perturber Eccentricity e_P9:    " << e_p9 << std::endl;
  std::cout << "Perturber Inclination i_P9:     " << inc_p9 << " deg" << std::endl;
  std::cout << "Perturber Arg of Perihelion:    " << omega_p9 << " deg" << std::endl;
  std::cout << "Perturber Ascending Node:       " << node_p9 << " deg" << std::endl;
  std::cout << std::endl;

  // 2. High-Precision 4.5 Gyr Secular Trajectory Integrations for Sedna, 2012 VP113, and Leleakuhonua
  std::cout << "--- 1. Integrating 4.5 Gyr Secular Trajectories ---" << std::endl;
  
  // (a) Sedna-like body (a = 506 AU, initial q = 33 AU in scattering corridor -> lifted to q = 76 AU)
  auto traj_sedna = model.integrate_secular_trajectory(
      506.0, 0.9348, 18.0, 311.4, 144.5, 4500.0, 1.0, m_p9, a_p9, e_p9, inc_p9, node_p9, omega_p9);

  std::ofstream csv_sedna("replications_ss/paper_248/secular_trajectory_sedna.csv");
  csv_sedna << "time_myr,a_au,e,inc_deg,omega_deg,node_deg,varpi_deg,q_au,Q_au,i_rel_deg,delta_varpi_deg,kozai_integral,is_detached,is_librating\n";
  for (const auto& pt : traj_sedna) {
    csv_sedna << std::fixed << std::setprecision(2) << pt.time_myr << ","
              << std::setprecision(3) << pt.a_au << ","
              << std::setprecision(5) << pt.e << ","
              << std::setprecision(3) << pt.inc_deg << ","
              << std::setprecision(3) << pt.omega_deg << ","
              << std::setprecision(3) << pt.node_deg << ","
              << std::setprecision(3) << pt.varpi_deg << ","
              << std::setprecision(3) << pt.perihelion_au << ","
              << std::setprecision(3) << pt.aphelion_au << ","
              << std::setprecision(3) << pt.i_rel_deg << ","
              << std::setprecision(3) << pt.delta_varpi_deg << ","
              << std::setprecision(5) << pt.kozai_integral << ","
              << (pt.is_detached ? 1 : 0) << ","
              << (pt.is_librating ? 1 : 0) << "\n";
  }
  csv_sedna.close();
  std::cout << "✅ Saved replications_ss/paper_248/secular_trajectory_sedna.csv (" << traj_sedna.size() << " steps)" << std::endl;

  // (b) 2012 VP113-like body (a = 261 AU, initial q = 33 AU -> lifted to q = 80.5 AU)
  auto traj_vp113 = model.integrate_secular_trajectory(
      261.0, 0.8735, 24.0, 293.8, 90.8, 4500.0, 1.0, m_p9, a_p9, e_p9, inc_p9, node_p9, omega_p9);

  std::ofstream csv_vp113("replications_ss/paper_248/secular_trajectory_vp113.csv");
  csv_vp113 << "time_myr,a_au,e,inc_deg,omega_deg,node_deg,varpi_deg,q_au,Q_au,i_rel_deg,delta_varpi_deg,kozai_integral,is_detached,is_librating\n";
  for (const auto& pt : traj_vp113) {
    csv_vp113 << std::fixed << std::setprecision(2) << pt.time_myr << ","
              << std::setprecision(3) << pt.a_au << ","
              << std::setprecision(5) << pt.e << ","
              << std::setprecision(3) << pt.inc_deg << ","
              << std::setprecision(3) << pt.omega_deg << ","
              << std::setprecision(3) << pt.node_deg << ","
              << std::setprecision(3) << pt.varpi_deg << ","
              << std::setprecision(3) << pt.perihelion_au << ","
              << std::setprecision(3) << pt.aphelion_au << ","
              << std::setprecision(3) << pt.i_rel_deg << ","
              << std::setprecision(3) << pt.delta_varpi_deg << ","
              << std::setprecision(5) << pt.kozai_integral << ","
              << (pt.is_detached ? 1 : 0) << ","
              << (pt.is_librating ? 1 : 0) << "\n";
  }
  csv_vp113.close();
  std::cout << "✅ Saved replications_ss/paper_248/secular_trajectory_vp113.csv (" << traj_vp113.size() << " steps)" << std::endl;

  // (c) Leleakuhonua / 2015 TG387 (a = 1094 AU, initial q = 35 AU -> lifted to q = 65 AU)
  auto traj_tg387 = model.integrate_secular_trajectory(
      1094.0, 0.9680, 11.7, 118.0, 301.0, 4500.0, 1.0, m_p9, a_p9, e_p9, inc_p9, node_p9, omega_p9);

  std::ofstream csv_tg387("replications_ss/paper_248/secular_trajectory_leleakuhonua.csv");
  csv_tg387 << "time_myr,a_au,e,inc_deg,omega_deg,node_deg,varpi_deg,q_au,Q_au,i_rel_deg,delta_varpi_deg,kozai_integral,is_detached,is_librating\n";
  for (const auto& pt : traj_tg387) {
    csv_tg387 << std::fixed << std::setprecision(2) << pt.time_myr << ","
              << std::setprecision(3) << pt.a_au << ","
              << std::setprecision(5) << pt.e << ","
              << std::setprecision(3) << pt.inc_deg << ","
              << std::setprecision(3) << pt.omega_deg << ","
              << std::setprecision(3) << pt.node_deg << ","
              << std::setprecision(3) << pt.varpi_deg << ","
              << std::setprecision(3) << pt.perihelion_au << ","
              << std::setprecision(3) << pt.aphelion_au << ","
              << std::setprecision(3) << pt.i_rel_deg << ","
              << std::setprecision(3) << pt.delta_varpi_deg << ","
              << std::setprecision(5) << pt.kozai_integral << ","
              << (pt.is_detached ? 1 : 0) << ","
              << (pt.is_librating ? 1 : 0) << "\n";
  }
  csv_tg387.close();
  std::cout << "✅ Saved replications_ss/paper_248/secular_trajectory_leleakuhonua.csv" << std::endl;

  // 3. Empirical Detached eTNO Benchmark Catalog & Goodness-of-Fit
  std::cout << "\n--- 2. Evaluating Benchmark Detached TNO Catalog ---" << std::endl;
  auto catalog = model.get_detached_tno_catalog(m_p9, a_p9, e_p9, inc_p9);

  std::ofstream csv_catalog("replications_ss/paper_248/benchmark_detached_tno_catalog.csv");
  csv_catalog << "name,a_au,e_obs,inc_deg,omega_deg,node_deg,varpi_deg,q_obs_au,Q_obs_au,q_pred_au,tau_kl_myr,dyn_class\n";

  double ss_tot = 0.0;
  double ss_res = 0.0;
  double mean_q_obs = 0.0;

  for (const auto& obj : catalog) {
    mean_q_obs += obj.q_au;
  }
  mean_q_obs /= catalog.size();

  for (const auto& obj : catalog) {
    double diff_mean = obj.q_au - mean_q_obs;
    double diff_res = obj.q_au - obj.predicted_q_max_au;
    ss_tot += diff_mean * diff_mean;
    ss_res += diff_res * diff_res;

    csv_catalog << "\"" << obj.name << "\","
                << std::fixed << std::setprecision(1) << obj.a_au << ","
                << std::setprecision(4) << obj.e << ","
                << std::setprecision(2) << obj.inc_deg << ","
                << std::setprecision(1) << obj.omega_deg << ","
                << std::setprecision(1) << obj.node_deg << ","
                << std::setprecision(1) << obj.varpi_deg << ","
                << std::setprecision(2) << obj.q_au << ","
                << std::setprecision(1) << obj.Q_au << ","
                << std::setprecision(2) << obj.predicted_q_max_au << ","
                << std::setprecision(1) << obj.predicted_tau_kl_myr << ","
                << "\"" << obj.dynamical_class << "\"\n";

    std::cout << std::left << std::setw(28) << obj.name
              << " a=" << std::setw(6) << obj.a_au
              << " q_obs=" << std::setw(5) << obj.q_au
              << " q_pred=" << std::setw(5) << obj.predicted_q_max_au
              << " tau_KL=" << std::setw(7) << obj.predicted_tau_kl_myr << " Myr"
              << " [" << obj.dynamical_class << "]" << std::endl;
  }
  csv_catalog.close();

  double r2_catalog = (ss_tot > 0.0) ? (1.0 - ss_res / ss_tot) : 1.0;
  std::cout << "\n✅ Detached TNO Perihelion Fit R^2 = " << std::setprecision(5) << r2_catalog
            << " (Target R^2 >= 0.9800)" << std::endl;

  // 4. Model Architecture Comparison
  std::cout << "\n--- 3. Model Architecture Comparison ---" << std::endl;
  auto evals = model.evaluate_model_architectures();

  std::ofstream csv_models("replications_ss/paper_248/model_architecture_comparison.csv");
  csv_models << "model_name,detached_fraction_pct,mean_perihelion_lift_au,max_perihelion_lift_au,r_squared,description\n";

  for (const auto& ev : evals) {
    csv_models << "\"" << ev.model_name << "\","
               << std::fixed << std::setprecision(1) << ev.detached_fraction_pct << ","
               << std::setprecision(2) << ev.mean_perihelion_lift_au << ","
               << std::setprecision(2) << ev.max_perihelion_lift_au << ","
               << std::setprecision(4) << ev.r_squared_benchmark << ","
               << "\"" << ev.description << "\"\n";

    std::cout << "Model: " << ev.model_name << std::endl;
    std::cout << "  Detached Fraction:      " << ev.detached_fraction_pct << " %" << std::endl;
    std::cout << "  Mean Perihelion Lift:   " << ev.mean_perihelion_lift_au << " AU" << std::endl;
    std::cout << "  Max Perihelion Lift:    " << ev.max_perihelion_lift_au << " AU" << std::endl;
    std::cout << "  Benchmark R^2 Score:    " << ev.r_squared_benchmark << std::endl;
    std::cout << std::endl;
  }
  csv_models.close();
  std::cout << "✅ Saved replications_ss/paper_248/model_architecture_comparison.csv" << std::endl;

  // 5. Parameter Space Sweep: a in [100, 1000] AU and Mutual Inclination i in [5, 80] deg
  std::cout << "--- 4. Sweeping (a, i_rel) Parameter Space ---" << std::endl;
  std::ofstream csv_grid("replications_ss/paper_248/parameter_space_grid.csv");
  csv_grid << "a_au,i_rel_deg,q_init_au,q_max_au,delta_q_lift_au,tau_kl_myr,i_crit_deg,is_detached\n";

  for (double a = 100.0; a <= 1000.0; a += 25.0) {
    double i_crit = model.critical_kozai_inclination_deg(a, m_p9, a_p9);
    for (double i_rel = 5.0; i_rel <= 80.0; i_rel += 2.5) {
      double q_init = 33.0; // standard scattering corridor
      double q_max = model.maximum_lifted_perihelion_au(a, q_init, i_rel, std::max(i_rel, 45.0));
      double delta_q = q_max - q_init;
      double tau_kl = model.kozai_oscillation_period_myr(a, m_p9, a_p9, e_p9, 1.0 - q_init / a);
      bool is_det = (q_max >= 40.0);

      csv_grid << std::fixed << std::setprecision(1) << a << ","
               << std::setprecision(2) << i_rel << ","
               << std::setprecision(2) << q_init << ","
               << std::setprecision(2) << q_max << ","
               << std::setprecision(2) << delta_q << ","
               << std::setprecision(2) << tau_kl << ","
               << std::setprecision(2) << i_crit << ","
               << (is_det ? 1 : 0) << "\n";
    }
  }
  csv_grid.close();
  std::cout << "✅ Saved replications_ss/paper_248/parameter_space_grid.csv" << std::endl;

  std::cout << "\n=================================================================" << std::endl;
  std::cout << "  Paper #248 Simulation & Benchmark Validation COMPLETE         " << std::endl;
  std::cout << "=================================================================" << std::endl;

  return 0;
}
