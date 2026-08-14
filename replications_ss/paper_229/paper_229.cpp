// Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
// Solver for Paper #229: Young Solar System Dynamics: The Fifth Giant Planet Hypothesis
// David Nesvorný (2011) | The Astrophysical Journal Letters, 742:L22 (arXiv:1109.2949)
//
// Evaluates first-principles 5-planet Nice model migration, ice giant gravitational
// scattering cross-sections, jumping-Jupiter impulse dynamics, and secular resonance
// sweeping across the inner Solar System.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "========================================================================\n";
  std::cout << "Paper #229 Solver: Young Solar System's Fifth Giant Planet Hypothesis\n";
  std::cout << "David Nesvorný (2011) | The Astrophysical Journal Letters, 742:L22\n";
  std::cout << "========================================================================\n\n";

  hot_jupiter::Nesvorny2011FifthGiantPlanetModel model;

  // 1. Safronov Numbers & Scattering Regimes
  double theta_jup = model.safronov_number(model.M_JUPITER, model.R_JUPITER, 5.45);
  double theta_sat = model.safronov_number(model.M_SATURN, model.R_SATURN, 8.65);
  double theta_ura = model.safronov_number(model.M_URANUS, model.R_URANUS, 15.80);
  double theta_nep = model.safronov_number(model.M_NEPTUNE, model.R_NEPTUNE, 21.20);
  double theta_p5  = model.safronov_number(model.M_NEPTUNE, model.R_NEPTUNE, 11.80);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Physical & Orbital Dynamics Framework:\n";
  std::cout << "  Solar Mass M_sun          : " << model.M_SUN << " kg\n";
  std::cout << "  Jupiter Mass M_J          : " << model.M_JUPITER << " kg (317.83 M_earth)\n";
  std::cout << "  Saturn Mass M_S           : " << model.M_SATURN << " kg (95.16 M_earth)\n";
  std::cout << "  Ice Giant Mass M_ice      : " << model.M_NEPTUNE << " kg (17.15 M_earth)\n\n";

  std::cout << "Safronov Parameter Theta = (M_p / M_sun) * (a_p / R_p):\n";
  std::cout << "  Jupiter Theta_J           : " << theta_jup << " (Strong Interstellar Ejection Regime, Theta >> 1)\n";
  std::cout << "  Saturn Theta_S            : " << theta_sat << " (Intermediate Ejection Regime)\n";
  std::cout << "  Fifth Planet Theta_5      : " << theta_p5  << " (Mutual Scattering Regime)\n";
  std::cout << "  Uranus Theta_U            : " << theta_ura << " (Mutual Scattering Regime)\n";
  std::cout << "  Neptune Theta_N           : " << theta_nep << " (Mutual Scattering Regime)\n\n";

  // 2. Export CSV 1: 100 Myr Representative Orbital Evolution (4-Planet vs 5-Planet)
  std::string csv_evol_path = "replications_ss/paper_229/orbital_evolution_comparison.csv";
  std::ofstream csv_evol(csv_evol_path);
  if (!csv_evol.is_open()) {
    std::cerr << "Error opening " << csv_evol_path << std::endl;
    return 1;
  }

  csv_evol << "time_myr,"
           << "a_J_4p,a_S_4p,a_U_4p,a_N_4p,e_J_4p,e_S_4p,e_Mars_4p,e_Earth_4p,Pratio_4p,"
           << "a_J_5p,a_S_5p,a_U_5p,a_N_5p,a_5_5p,e_J_5p,e_S_5p,e_Mars_5p,e_Earth_5p,Pratio_5p,is_ejected_5p\n";

  auto traj_4p = model.integrate_representative_trajectory(false, 100.0, 0.1, 0.5);
  auto traj_5p = model.integrate_representative_trajectory(true, 100.0, 0.1, 0.5);

  size_t n_steps = std::min(traj_4p.size(), traj_5p.size());
  for (size_t i = 0; i < n_steps; ++i) {
    const auto& s4 = traj_4p[i];
    const auto& s5 = traj_5p[i];
    csv_evol << std::fixed << std::setprecision(2) << s4.time_myr << ","
             << std::setprecision(4)
             << s4.a_J_au << "," << s4.a_S_au << "," << s4.a_U_au << "," << s4.a_N_au << ","
             << s4.e_J << "," << s4.e_S << "," << s4.e_Mars << "," << s4.e_Earth << "," << s4.P_ratio_SJ << ","
             << s5.a_J_au << "," << s5.a_S_au << "," << s5.a_U_au << "," << s5.a_N_au << "," << s5.a_5_au << ","
             << s5.e_J << "," << s5.e_S << "," << s5.e_Mars << "," << s5.e_Earth << "," << s5.P_ratio_SJ << ","
             << (s5.is_5_ejected ? 1 : 0) << "\n";
  }
  csv_evol.close();
  std::cout << "✅ Exported 100 Myr Trajectories -> " << csv_evol_path << " (" << n_steps << " rows)\n";

  // 3. Export CSV 2: Monte Carlo Ensemble Statistical Success Rates Across Criteria
  std::string csv_ensemble_path = "replications_ss/paper_229/ensemble_criteria_statistics.csv";
  std::ofstream csv_ensemble(csv_ensemble_path);
  if (!csv_ensemble.is_open()) {
    std::cerr << "Error opening " << csv_ensemble_path << std::endl;
    return 1;
  }

  csv_ensemble << "model_architecture,initial_planets,total_runs,crit1_count,crit1_rate,crit2_count,crit2_rate,"
               << "crit3_count,crit3_rate,crit4_count,crit4_rate,all_pass_count,overall_success_rate\n";

  int n_mc_runs = 5000;
  auto ensemble_4p = model.run_ensemble(false, n_mc_runs, 101);
  auto ensemble_5p = model.run_ensemble(true, n_mc_runs, 202);

  auto stats_4p = model.compute_statistics(ensemble_4p);
  auto stats_5p = model.compute_statistics(ensemble_5p);

  csv_ensemble << std::fixed << std::setprecision(4);
  csv_ensemble << "4_planet_canonical," << 4 << "," << stats_4p.count_total << ","
               << stats_4p.count_crit1 << "," << stats_4p.rate_crit1 << ","
               << stats_4p.count_crit2 << "," << stats_4p.rate_crit2 << ","
               << stats_4p.count_crit3 << "," << stats_4p.rate_crit3 << ","
               << stats_4p.count_crit4 << "," << stats_4p.rate_crit4 << ","
               << stats_4p.count_all << "," << stats_4p.rate_all << "\n";

  csv_ensemble << "5_planet_hypothesis," << 5 << "," << stats_5p.count_total << ","
               << stats_5p.count_crit1 << "," << stats_5p.rate_crit1 << ","
               << stats_5p.count_crit2 << "," << stats_5p.rate_crit2 << ","
               << stats_5p.count_crit3 << "," << stats_5p.rate_crit3 << ","
               << stats_5p.count_crit4 << "," << stats_5p.rate_crit4 << ","
               << stats_5p.count_all << "," << stats_5p.rate_all << "\n";

  csv_ensemble.close();
  std::cout << "✅ Exported Monte Carlo Ensemble Stats -> " << csv_ensemble_path << "\n";
  std::cout << "   4-Planet Overall Success: " << stats_4p.rate_all * 100.0 << " %\n";
  std::cout << "   5-Planet Overall Success: " << stats_5p.rate_all * 100.0 << " % (Enhancement Factor: "
            << (stats_5p.rate_all / std::max(1e-5, stats_4p.rate_all)) << "x)\n\n";

  // 4. Export CSV 3: Gravitational Scattering & Ejection Cross Sections vs Relative Velocity
  std::string csv_cross_path = "replications_ss/paper_229/scattering_ejection_cross_sections.csv";
  std::ofstream csv_cross(csv_cross_path);
  if (!csv_cross.is_open()) {
    std::cerr << "Error opening " << csv_cross_path << std::endl;
    return 1;
  }

  csv_cross << "v_rel_vK,v_rel_km_s,sigma_scatt_jup_au2,sigma_ej_jup_au2,b_ej_jup_au,"
            << "delta_v_ice_km_s,v_post_km_s,delta_a_jup_au,is_ejected\n";

  double a_J = 5.45;
  double v_orb_J = model.orbital_velocity_m_s(a_J);
  double r_hill_J_m = model.hill_radius_m(a_J, model.M_JUPITER);

  for (double v_frac = 0.05; v_frac <= 0.60; v_frac += 0.01) {
    double v_rel_m_s = v_frac * v_orb_J;
    double sigma_scatt_m2 = model.gravitational_scattering_cross_section_m2(
        model.M_JUPITER, model.M_NEPTUNE, r_hill_J_m, v_rel_m_s);
    double sigma_ej_m2 = model.ejection_cross_section_m2(model.M_JUPITER, a_J, v_rel_m_s);
    double b_ej_m = model.ejection_impact_parameter_m(model.M_JUPITER, a_J, v_rel_m_s);

    double AU2 = model.AU_M * model.AU_M;
    double sig_scatt_au2 = sigma_scatt_m2 / AU2;
    double sig_ej_au2 = sigma_ej_m2 / AU2;
    double b_ej_au = b_ej_m / model.AU_M;

    auto enc = model.compute_jupiter_encounter(0.5 * b_ej_m, a_J, model.M_NEPTUNE, v_frac);

    csv_cross << std::fixed << std::setprecision(3) << v_frac << ","
              << std::setprecision(2) << (v_rel_m_s / 1e3) << ","
              << std::setprecision(4) << sig_scatt_au2 << "," << sig_ej_au2 << "," << b_ej_au << ","
              << (enc.delta_v_ice_m_s / 1e3) << "," << (enc.v_post_ice_m_s / 1e3) << ","
              << enc.delta_a_jupiter_au << "," << (enc.is_ejected ? 1 : 0) << "\n";
  }
  csv_cross.close();
  std::cout << "✅ Exported Scattering & Ejection Cross Sections -> " << csv_cross_path << "\n";

  // 5. Export CSV 4: Secular Precession Frequencies & Resonance Sweeping Excitation
  std::string csv_sec_path = "replications_ss/paper_229/secular_resonance_sweeping.csv";
  std::ofstream csv_sec(csv_sec_path);
  if (!csv_sec.is_open()) {
    std::cerr << "Error opening " << csv_sec_path << std::endl;
    return 1;
  }

  csv_sec << "a_terr_au,g_freq_arcsec_yr,delta_e_smooth_fast,delta_e_smooth_nominal,delta_e_smooth_slow,delta_e_jumping\n";

  for (double a = 0.35; a <= 2.20; a += 0.02) {
    double g_freq = model.inner_secular_precession_frequency_arcsec_yr(a, 5.20, 9.58);
    double de_smooth_fast = model.secular_resonance_eccentricity_excitation(a, 3.0, false);
    double de_smooth_nom  = model.secular_resonance_eccentricity_excitation(a, 10.0, false);
    double de_smooth_slow = model.secular_resonance_eccentricity_excitation(a, 30.0, false);
    double de_jump        = model.secular_resonance_eccentricity_excitation(a, 0.05, true);

    csv_sec << std::fixed << std::setprecision(3) << a << ","
            << std::setprecision(4) << g_freq << ","
            << de_smooth_fast << "," << de_smooth_nom << "," << de_smooth_slow << ","
            << de_jump << "\n";
  }
  csv_sec.close();
  std::cout << "✅ Exported Secular Resonance Sweeping Data -> " << csv_sec_path << "\n\n";

  std::cout << "========================================================================\n";
  std::cout << "Paper #229 Solver Execution Complete: All CSV Artifacts Generated.\n";
  std::cout << "========================================================================\n";

  return 0;
}
