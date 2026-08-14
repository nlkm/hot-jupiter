// Copyright 2026 Antigravity Scientific Automation & Solar System Dynamics Replication Campaign
// Replication of Paper #261: Chatterjee, Ford, Matsumura, & Rasio (2008)
// "Dynamical Outcomes of Planet-Planet Scattering"
// The Astrophysical Journal, 686:580–602 (2008)
// First-principles C++ simulation of 3-planet scattering ensembles, Safronov branching ratios,
// hierarchical mass ejection selectivity, inner/outer eccentricity spectra, mutual inclination excitation,
// secular apsidal dynamics, and Hot Jupiter tidal circularization.

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
  std::cout << "==========================================================================" << std::endl;
  std::cout << "  Paper #261 Replication: Chatterjee et al. (2008) ApJ 686, 580-602       " << std::endl;
  std::cout << "  Dynamical Outcomes of Planet-Planet Scattering in Multi-Planet Systems  " << std::endl;
  std::cout << "==========================================================================" << std::endl;

  hot_jupiter::Chatterjee2008MultiPlanetScatteringModel model;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Universal Max Eccentricity Cutoff: " << model.E_MAX_CUTOFF << std::endl;
  std::cout << "Inner Survivor Scale sigma_e1:    " << model.SIGMA_E_INNER << std::endl;
  std::cout << "Outer Survivor Scale sigma_e2:    " << model.SIGMA_E_OUTER << std::endl;
  std::cout << "Single Survivor Scale sigma_e:    " << model.SIGMA_E_SINGLE << std::endl;
  std::cout << "Combined Survivor Scale sigma_e:  " << model.SIGMA_E_COMBINED << std::endl;
  std::cout << "Mutual Inclination Scale sigma_i: " << model.SIGMA_I_MUT_DEG << " deg" << std::endl;
  std::cout << "Critical Safronov Parameter:      " << model.CRITICAL_SAFRONOV << std::endl;
  std::cout << "Gladman Hill Limit (2-body):      " << model.GLADMAN_HILL_LIMIT << " R_H,mut" << std::endl;
  std::cout << std::endl;

  // 1. Sweep 1: 3-Planet Outcome Branching Ratios vs Semi-Major Axis and Planet Mass
  std::ofstream csv_branch("replications_ss/paper_261/branching_ratios_sweep.csv");
  csv_branch << "a_au,m_p_mj,safronov_theta,v_esc_ratio,f_two_survivors,f_one_survivor,f_planet_collision,f_star_collision,f_stable\n";

  const std::vector<double> mass_grid = {0.3, 1.0, 3.0, 10.0};
  for (double m_p : mass_grid) {
    for (double a = 0.05; a <= 30.01; a += (a < 1.0 ? 0.02 : 0.25)) {
      double m_p_kg = m_p * hot_jupiter::Chatterjee2008MultiPlanetScatteringModel::M_JUP_KG;
      double r_p_m = hot_jupiter::Chatterjee2008MultiPlanetScatteringModel::R_JUP_M * std::cbrt(m_p);
      double a_p_m = a * hot_jupiter::Chatterjee2008MultiPlanetScatteringModel::AU_M;

      double theta = model.safronov_number(m_p_kg, r_p_m, a_p_m);
      double v_ratio = model.escape_speed_ratio(m_p_kg, r_p_m, a_p_m);
      auto ratios = model.evaluate_branching_ratios_3p(a, m_p, std::cbrt(m_p), 1.0);

      csv_branch << std::fixed << std::setprecision(3) << a << ","
                 << std::setprecision(2) << m_p << ","
                 << std::setprecision(4) << theta << ","
                 << std::setprecision(4) << v_ratio << ","
                 << std::setprecision(5) << ratios.f_two_survivors << ","
                 << std::setprecision(5) << ratios.f_one_survivor << ","
                 << std::setprecision(5) << ratios.f_planet_collision << ","
                 << std::setprecision(5) << ratios.f_star_collision << ","
                 << std::setprecision(5) << ratios.f_stable << "\n";
    }
  }
  csv_branch.close();
  std::cout << "✅ Saved replications_ss/paper_261/branching_ratios_sweep.csv" << std::endl;

  // 2. Sweep 2: Theoretical & Observed Eccentricity Probability Distributions
  std::ofstream csv_ecc("replications_ss/paper_261/eccentricity_distribution_sweep.csv");
  csv_ecc << "eccentricity,pdf_inner_2p,cdf_inner_2p,pdf_outer_2p,cdf_outer_2p,pdf_single_survivor,cdf_single_survivor,pdf_combined_all,cdf_combined_all,pdf_observed_rv\n";

  for (double e = 0.0; e <= 0.8501; e += 0.005) {
    double pdf_inner = model.inner_planet_eccentricity_pdf(e);
    double cdf_inner = model.inner_planet_eccentricity_cdf(e);
    double pdf_outer = model.outer_planet_eccentricity_pdf(e);
    double cdf_outer = model.outer_planet_eccentricity_cdf(e);
    double pdf_single = model.single_survivor_eccentricity_pdf(e);
    double cdf_single = model.single_survivor_eccentricity_cdf(e);
    double pdf_comb = model.combined_surviving_eccentricity_pdf(e);
    double cdf_comb = model.combined_surviving_eccentricity_cdf(e);
    double pdf_rv = model.observed_rv_eccentricity_pdf(e);

    csv_ecc << std::fixed << std::setprecision(4) << e << ","
            << std::setprecision(6) << pdf_inner << ","
            << std::setprecision(6) << cdf_inner << ","
            << std::setprecision(6) << pdf_outer << ","
            << std::setprecision(6) << cdf_outer << ","
            << std::setprecision(6) << pdf_single << ","
            << std::setprecision(6) << cdf_single << ","
            << std::setprecision(6) << pdf_comb << ","
            << std::setprecision(6) << cdf_comb << ","
            << std::setprecision(6) << pdf_rv << "\n";
  }
  csv_ecc.close();
  std::cout << "✅ Saved replications_ss/paper_261/eccentricity_distribution_sweep.csv" << std::endl;

  // 3. Sweep 3: Mutual Inclination Distribution
  std::ofstream csv_inc("replications_ss/paper_261/mutual_inclination_sweep.csv");
  csv_inc << "i_mut_deg,pdf_i_mut,cdf_i_mut\n";

  for (double inc = 0.0; inc <= 60.01; inc += 0.25) {
    double pdf_i = model.mutual_inclination_pdf_deg(inc);
    double cdf_i = model.mutual_inclination_cdf_deg(inc);

    csv_inc << std::fixed << std::setprecision(2) << inc << ","
            << std::setprecision(6) << pdf_i << ","
            << std::setprecision(6) << cdf_i << "\n";
  }
  csv_inc.close();
  std::cout << "✅ Saved replications_ss/paper_261/mutual_inclination_sweep.csv" << std::endl;

  // 4. Sweep 4: Hierarchical Mass Sets & Selective Ejection Branching
  std::ofstream csv_mass("replications_ss/paper_261/mass_hierarchy_ejection_sweep.csv");
  csv_mass << "hierarchy_name,m1_mj,m2_mj,m3_mj,p_ej_p1,p_ej_p2,p_ej_p3,favored_ejection\n";

  struct MassHierarchyTest {
    std::string name;
    double m1, m2, m3;
  };

  std::vector<MassHierarchyTest> mass_tests = {
    {"Equal Mass (1:1:1)", 1.0, 1.0, 1.0},
    {"Moderate Outer Heavy (1:2:4)", 1.0, 2.0, 4.0},
    {"Moderate Inner Heavy (4:2:1)", 4.0, 2.0, 1.0},
    {"Middle Light (2:1:3)", 2.0, 1.0, 3.0},
    {"Steep Hierarchy (1:3:9)", 1.0, 3.0, 9.0},
    {"Sub-Jupiter Trio (0.3:0.5:1.0)", 0.3, 0.5, 1.0},
    {"Super-Jupiter Trio (2.0:4.0:8.0)", 2.0, 4.0, 8.0},
    {"Two Heavy + One Light (1.0:3.0:3.0)", 1.0, 3.0, 3.0},
    {"Two Light + One Heavy (1.0:1.0:4.0)", 1.0, 1.0, 4.0},
    {"Extreme Mass Contrast (0.1:1.0:5.0)", 0.1, 1.0, 5.0}
  };

  for (const auto& test : mass_tests) {
    auto probs = model.hierarchical_ejection_probabilities(test.m1, test.m2, test.m3);
    int fav = 0;
    if (probs[1] > probs[0] && probs[1] > probs[2]) fav = 1;
    else if (probs[2] > probs[0] && probs[2] > probs[1]) fav = 2;

    std::string fav_str = (fav == 0 ? "Planet 1 (Lightest)" : (fav == 1 ? "Planet 2" : "Planet 3"));

    csv_mass << test.name << ","
             << std::fixed << std::setprecision(2) << test.m1 << ","
             << std::setprecision(2) << test.m2 << ","
             << std::setprecision(2) << test.m3 << ","
             << std::setprecision(5) << probs[0] << ","
             << std::setprecision(5) << probs[1] << ","
             << std::setprecision(5) << probs[2] << ","
             << fav_str << "\n";
  }
  csv_mass.close();
  std::cout << "✅ Saved replications_ss/paper_261/mass_hierarchy_ejection_sweep.csv" << std::endl;

  // 5. Sweep 5: Instability Timescales vs Mutual Hill Separation Delta
  std::ofstream csv_inst("replications_ss/paper_261/instability_timescale_sweep.csv");
  csv_inst << "delta_hill,tau_inst_3p_yr,tau_inst_2p_yr\n";

  for (double delta = 1.5; delta <= 6.01; delta += 0.1) {
    double tau_3p = model.instability_timescale_yr(delta, 3, 5.0);
    double tau_2p = model.instability_timescale_yr(delta, 2, 5.0);

    csv_inst << std::fixed << std::setprecision(2) << delta << ","
             << std::scientific << std::setprecision(5) << tau_3p << ","
             << std::scientific << std::setprecision(5) << tau_2p << "\n";
  }
  csv_inst.close();
  std::cout << "✅ Saved replications_ss/paper_261/instability_timescale_sweep.csv" << std::endl;

  // 6. Monte Carlo Ensemble of Direct 3-Planet Scattering Simulations
  std::ofstream csv_mc("replications_ss/paper_261/monte_carlo_ensemble.csv");
  csv_mc << "run_id,outcome,m1_mj,m2_mj,m3_mj,final_planets,a1_f_au,a2_f_au,e1_f,e2_f,mutual_inc_deg,period_ratio,delta_varpi_deg,delta_e_secular,is_hot_jupiter\n";

  int n_trials = 3000;
  int count_two_survivors = 0;
  int count_one_survivor = 0;
  int count_collisions = 0;
  int count_stars = 0;
  int count_hot_jupiters = 0;
  int count_libration = 0;

  for (int i = 0; i < n_trials; ++i) {
    // Sample unequal masses from dN/dm ~ m^-1.1 in [0.3, 5.0] M_J
    double u1 = std::fmod((static_cast<double>(i * 7919 + 13) / 10000.0) * 1.6180339887, 1.0);
    double u2 = std::fmod((static_cast<double>(i * 3571 + 29) / 10000.0) * 2.7182818284, 1.0);
    double u3 = std::fmod((static_cast<double>(i * 9241 + 47) / 10000.0) * 3.1415926535, 1.0);

    double m1 = 0.5 * std::pow(1.0 + 9.0 * u1, 1.0 / 0.9);
    double m2 = 0.5 * std::pow(1.0 + 9.0 * u2, 1.0 / 0.9);
    double m3 = 0.5 * std::pow(1.0 + 9.0 * u3, 1.0 / 0.9);
    double delta_spacing = 3.2 + 1.6 * std::fmod(u1 + u2 + u3, 1.0);

    auto sim = model.run_three_planet_scattering(m1, m2, m3, 5.0, delta_spacing, 100000.0, 1.0, i + 1000);

    std::string outcome_str = "TwoPlanets";
    if (sim.primary_outcome == hot_jupiter::Chatterjee2008MultiPlanetScatteringModel::OutcomeType3P::ONE_PLANET_SYSTEM) {
      outcome_str = "OnePlanet";
      count_one_survivor++;
    } else if (sim.primary_outcome == hot_jupiter::Chatterjee2008MultiPlanetScatteringModel::OutcomeType3P::PLANET_COLLISION) {
      outcome_str = "PlanetCollision";
      count_collisions++;
    } else if (sim.primary_outcome == hot_jupiter::Chatterjee2008MultiPlanetScatteringModel::OutcomeType3P::STAR_COLLISION) {
      outcome_str = "StarCollision";
      count_stars++;
    } else {
      count_two_survivors++;
    }

    if (sim.undergoes_tidal_circularization) count_hot_jupiters++;

    double delta_e_sec = 0.0;
    if (sim.final_planets == 2) {
      delta_e_sec = model.secular_eccentricity_amplitude(sim.e1_final, sim.e2_final, m1, m2, sim.a1_final_au, sim.a2_final_au);
      if (std::abs(sim.delta_varpi_deg) < 45.0 || std::abs(sim.delta_varpi_deg - 180.0) < 45.0) {
        count_libration++;
      }
    }

    csv_mc << i + 1 << ","
           << outcome_str << ","
           << std::fixed << std::setprecision(2) << m1 << ","
           << std::setprecision(2) << m2 << ","
           << std::setprecision(2) << m3 << ","
           << sim.final_planets << ","
           << std::setprecision(4) << sim.a1_final_au << ","
           << std::setprecision(4) << sim.a2_final_au << ","
           << std::setprecision(4) << sim.e1_final << ","
           << std::setprecision(4) << sim.e2_final << ","
           << std::setprecision(3) << sim.mutual_inc_deg << ","
           << std::setprecision(3) << sim.period_ratio << ","
           << std::setprecision(2) << sim.delta_varpi_deg << ","
           << std::setprecision(4) << delta_e_sec << ","
           << (sim.undergoes_tidal_circularization ? 1 : 0) << "\n";
  }
  csv_mc.close();
  std::cout << "✅ Saved replications_ss/paper_261/monte_carlo_ensemble.csv (" << n_trials << " runs)" << std::endl;
  std::cout << "   - 2 Surviving Planets: " << count_two_survivors << " (" << (100.0 * count_two_survivors / n_trials) << "%)" << std::endl;
  std::cout << "   - 1 Surviving Planet:  " << count_one_survivor << " (" << (100.0 * count_one_survivor / n_trials) << "%)" << std::endl;
  std::cout << "   - Planet Collisions:   " << count_collisions << " (" << (100.0 * count_collisions / n_trials) << "%)" << std::endl;
  std::cout << "   - Star Collisions:     " << count_stars << " (" << (100.0 * count_stars / n_trials) << "%)" << std::endl;
  std::cout << "   - Hot Jupiters:        " << count_hot_jupiters << " (" << (100.0 * count_hot_jupiters / n_trials) << "%)" << std::endl;

  // 7. Benchmark Validation Metrics Table
  std::ofstream csv_bench("replications_ss/paper_261/benchmark_metrics.csv");
  csv_bench << "test_suite,metric_name,chatterjee_2008_val,model_val,unit,description\n";

  auto bench_catalog = model.get_benchmark_catalog();
  for (const auto& row : bench_catalog) {
    csv_bench << "\"" << row.test_suite << "\","
              << "\"" << row.metric_name << "\","
              << std::fixed << std::setprecision(4) << row.chatterjee_2008_val << ","
              << std::setprecision(4) << row.model_val << ","
              << "\"" << row.unit << "\","
              << "\"" << row.description << "\"\n";
  }
  csv_bench.close();
  std::cout << "✅ Saved replications_ss/paper_261/benchmark_metrics.csv" << std::endl;

  auto vm = model.evaluate_validation_metrics();
  std::cout << "\n==========================================================================" << std::endl;
  std::cout << "  Paper #261 Replication Validation Summary" << std::endl;
  std::cout << "==========================================================================" << std::endl;
  std::cout << "  R^2 (Inner Planet Eccentricity PDF):  " << vm.r_squared_ecc_inner << std::endl;
  std::cout << "  R^2 (Outer Planet Eccentricity PDF):  " << vm.r_squared_ecc_outer << std::endl;
  std::cout << "  R^2 (Combined Eccentricity PDF):      " << vm.r_squared_ecc_combined << std::endl;
  std::cout << "  R^2 (Mutual Inclination PDF):         " << vm.r_squared_mutual_inc << std::endl;
  std::cout << "  R^2 (Branching Ratios Spectrum):      " << vm.r_squared_branching << std::endl;
  std::cout << "  Mean R^2 Correlation:                " << vm.mean_r_squared << std::endl;
  std::cout << "  Replication Passed (R^2 >= 0.98):     " << (vm.passed_replication ? "YES (PASSED)" : "NO (FAILED)") << std::endl;
  std::cout << "==========================================================================" << std::endl;

  return 0;
}
