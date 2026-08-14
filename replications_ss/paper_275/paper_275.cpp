// Copyright 2026 Antigravity Scientific Automation & Solar System Dynamics Replication Campaign
// Replication of Paper #275: Bottke, Vokrouhlický, Walsh, Delbo, Michel, Jedicke,
// Pravec, Morbidelli (2015) / Bottke et al. (2000, 2002), Granvik et al. (2018)
// "The In-orbit Distribution of Near-Earth Objects"
// Asteroids IV (P. Michel, F. E. DeMeo, W. F. Bottke, Eds.), Univ. of Arizona Press, pp. 701-724.
//
// First-principles C++ evaluation of debiased NEO steady-state orbital distributions (a, e, i, q),
// 5 dynamical source regions (nu6 secular, 3:1 MMR, Intermediate Mars-Crossers, Outer Main Belt, JFCs),
// transport fractions, absolute magnitude H and size frequency distributions N(>D),
// low-perihelion catastrophic thermal disruption (q < 0.06 AU),
// and terrestrial planet collision rates / impact frequencies.

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
  std::cout << "=================================================================" << std::endl;
  std::cout << "  Paper #275 Replication: Bottke et al. (2015 / 2002 / 2000)     " << std::endl;
  std::cout << "  The In-orbit Distribution of Near-Earth Objects                " << std::endl;
  std::cout << "  Asteroids IV (Univ. of Arizona Press), pp. 701-724             " << std::endl;
  std::cout << "=================================================================" << std::endl;

  hot_jupiter::Bottke2015NEOModel model;

  // 1. Five Main Source Regions & Transport Fractions
  std::cout << "\n--- 1. Dynamical Source Regions & Transfer Fractions ---" << std::endl;
  std::cout << std::fixed << std::setprecision(4);
  auto sources = model.get_source_regions();
  double total_alpha = 0.0;
  for (const auto& s : sources) {
    total_alpha += s.transport_fraction;
    std::cout << std::setw(6) << s.code
              << " | " << std::setw(28) << s.name
              << " | alpha = " << std::setw(6) << s.transport_fraction * 100.0 << " %"
              << " | tau = " << std::setw(5) << s.mean_lifetime_myr << " Myr"
              << " | Sun = " << std::setw(5) << s.frac_sun * 100.0 << " %"
              << " | Eject = " << std::setw(5) << s.frac_ejection * 100.0 << " %"
              << " | Earth = " << std::setw(6) << s.frac_earth * 100.0 << " %"
              << std::endl;
  }
  std::cout << "Total Transfer Fraction Sum: " << total_alpha * 100.0 << " %\n";

  // 2. Global Steady-State Residence Time & Replenishment Rates
  double tau_mean = model.weighted_mean_lifetime_myr();
  double repl_rate = model.steady_state_replenishment_rate_per_myr(960.0);
  double f_sun_tot = model.composite_sun_collision_fraction();
  double f_ej_tot = model.composite_ejection_fraction();
  double f_terr_tot = model.composite_terrestrial_impact_fraction();
  double f_earth_tot = model.composite_earth_impact_fraction();
  double r_earth_myr = model.earth_impact_rate_per_myr(960.0);
  double dt_earth_yr = model.earth_impact_mean_interval_yr(960.0);

  std::cout << "\n--- 2. Composite Steady-State Properties (D > 1 km, N = 960) ---" << std::endl;
  std::cout << "Weighted Mean NEO Residence Lifetime <tau>: " << tau_mean << " Myr" << std::endl;
  std::cout << "Required Steady-State Supply Rate dN/dt:    " << repl_rate << " asteroids / Myr ("
            << repl_rate / 1.0e6 << " yr^-1)" << std::endl;
  std::cout << "Composite Solar Collision / Disruption:      " << f_sun_tot * 100.0 << " %" << std::endl;
  std::cout << "Composite Hyperbolic Ejection (Jupiter):    " << f_ej_tot * 100.0 << " %" << std::endl;
  std::cout << "Composite Terrestrial Planet Impacts:       " << f_terr_tot * 100.0 << " %" << std::endl;
  std::cout << "  - Earth Impact Fraction:                  " << f_earth_tot * 100.0 << " %" << std::endl;
  std::cout << "  - Earth Impact Rate (D > 1 km):           " << r_earth_myr << " impacts / Myr" << std::endl;
  std::cout << "  - Mean Earth Impact Interval:             " << std::setprecision(0) << dt_earth_yr << " years" << std::endl;

  // 3. Subpopulation Distribution Breakdown (Atira, Aten, Apollo, Amor)
  std::cout << "\n--- 3. NEO Subpopulations (D > 1 km) ---" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  auto subpops = model.get_subpopulation_distribution(960.0);
  for (const auto& sp : subpops) {
    std::cout << std::setw(12) << sp.name
              << " | " << std::setw(38) << sp.definition
              << " | " << std::setw(5) << sp.fraction * 100.0 << " %"
              << " | N = " << std::setw(6) << sp.count_d_gt_1km
              << " | <a> = " << sp.mean_a_au << " AU"
              << " | <e> = " << sp.mean_e
              << " | <i> = " << sp.mean_i_deg << " deg"
              << std::endl;
  }

  // 4. Export CSV 1: Source Transport and Fate Branching Ratios
  std::ofstream csv_sources("replications_ss/paper_275/source_transport_fates.csv");
  csv_sources << "code,name,transport_fraction,mean_lifetime_myr,frac_sun,frac_ejection,frac_earth,frac_venus,frac_mars,frac_mercury,mean_a_au,mean_e,mean_i_deg\n";
  for (const auto& s : sources) {
    csv_sources << s.code << ",\""
                << s.name << "\","
                << std::fixed << std::setprecision(4)
                << s.transport_fraction << ","
                << s.mean_lifetime_myr << ","
                << s.frac_sun << ","
                << s.frac_ejection << ","
                << s.frac_earth << ","
                << s.frac_venus << ","
                << s.frac_mars << ","
                << s.frac_mercury << ","
                << s.mean_a_au << ","
                << s.mean_e << ","
                << s.mean_i_deg << "\n";
  }
  csv_sources.close();
  std::cout << "\n✅ Saved replications_ss/paper_275/source_transport_fates.csv" << std::endl;

  // 5. Export CSV 2: Orbital Phase Space Marginal PDFs (a, e, i, q)
  std::ofstream csv_orbits("replications_ss/paper_275/orbital_distributions.csv");
  csv_orbits << "grid_index,a_au,pdf_a,e,pdf_e,inc_deg,pdf_inc,q_au,pdf_q,p_disrupt_q\n";
  int n_grid = 200;
  for (int idx = 0; idx < n_grid; ++idx) {
    double a = 0.5 + (4.0 - 0.5) * (idx / double(n_grid - 1));
    double e = 0.0 + (0.99 - 0.0) * (idx / double(n_grid - 1));
    double inc = 0.0 + (60.0 - 0.0) * (idx / double(n_grid - 1));
    double q = 0.001 + (1.30 - 0.001) * (idx / double(n_grid - 1));

    double pdf_a = model.orbit_pdf_semimajor(a);
    double pdf_e = model.orbit_pdf_eccentricity(e);
    double pdf_inc = model.orbit_pdf_inclination(inc);
    double pdf_q = model.orbit_pdf_perihelion(q);
    double p_dis = model.disruption_probability_low_perihelion(q);

    csv_orbits << idx << ","
               << std::fixed << std::setprecision(5)
               << a << "," << pdf_a << ","
               << e << "," << pdf_e << ","
               << inc << "," << pdf_inc << ","
               << q << "," << pdf_q << ","
               << p_dis << "\n";
  }
  csv_orbits.close();
  std::cout << "✅ Saved replications_ss/paper_275/orbital_distributions.csv" << std::endl;

  // 6. Export CSV 3: Size Frequency Distribution N(>D) & Magnitude Distribution N(<H)
  std::ofstream csv_sfd("replications_ss/paper_275/size_magnitude_sfd.csv");
  csv_sfd << "D_km,H_mag,N_gt_D,N_lt_H,d_from_H_km\n";
  for (double d = 0.01; d <= 25.0; d *= 1.10) {
    double h = model.magnitude_from_size(d, 0.14);
    double n_gt_d = model.cumulative_size_distribution(d, 960.0);
    double n_lt_h = model.cumulative_magnitude_distribution(h, 960.0);
    double d_recon = model.size_from_magnitude(h, 0.14);

    csv_sfd << std::scientific << std::setprecision(5)
            << d << ","
            << std::fixed << std::setprecision(2)
            << h << ","
            << std::scientific << n_gt_d << ","
            << n_lt_h << ","
            << d_recon << "\n";
  }
  csv_sfd.close();
  std::cout << "✅ Saved replications_ss/paper_275/size_magnitude_sfd.csv" << std::endl;

  // 7. Export CSV 4: Subpopulation Breakdown
  std::ofstream csv_subpop("replications_ss/paper_275/subpopulations.csv");
  csv_subpop << "name,definition,fraction,count_d_gt_1km,mean_a_au,mean_e,mean_i_deg\n";
  for (const auto& sp : subpops) {
    csv_subpop << "\"" << sp.name << "\",\""
               << sp.definition << "\","
               << std::fixed << std::setprecision(4)
               << sp.fraction << ","
               << sp.count_d_gt_1km << ","
               << sp.mean_a_au << ","
               << sp.mean_e << ","
               << sp.mean_i_deg << "\n";
  }
  csv_subpop.close();
  std::cout << "✅ Saved replications_ss/paper_275/subpopulations.csv" << std::endl;

  // 8. Export CSV 5: Planetary Impact Rates & Öpik Collision Probabilities
  std::ofstream csv_impacts("replications_ss/paper_275/planetary_impact_rates.csv");
  csv_impacts << "target,impact_rate_per_myr,mean_interval_yr,mean_opik_prob_per_yr\n";
  
  double r_earth = model.earth_impact_rate_per_myr(960.0);
  double dt_earth = model.earth_impact_mean_interval_yr(960.0);
  double opik_earth = model.opik_collision_probability(1.85, 0.55, 14.0, "Earth");

  double r_venus = model.venus_impact_rate_per_myr(960.0);
  double dt_venus = 1.0e6 / std::max(1.0e-5, r_venus);
  double opik_venus = model.opik_collision_probability(1.65, 0.58, 14.0, "Venus");

  double r_mars = model.mars_impact_rate_per_myr(960.0);
  double dt_mars = 1.0e6 / std::max(1.0e-5, r_mars);
  double opik_mars = model.opik_collision_probability(2.10, 0.45, 12.0, "Mars");

  double r_mercury = repl_rate * model.composite_mercury_impact_fraction();
  double dt_mercury = 1.0e6 / std::max(1.0e-5, r_mercury);
  double opik_mercury = model.opik_collision_probability(1.20, 0.70, 16.0, "Mercury");

  double r_moon = model.moon_impact_rate_per_myr(960.0);
  double dt_moon = 1.0e6 / std::max(1.0e-5, r_moon);

  csv_impacts << "Earth," << r_earth << "," << dt_earth << "," << opik_earth << "\n";
  csv_impacts << "Venus," << r_venus << "," << dt_venus << "," << opik_venus << "\n";
  csv_impacts << "Mars," << r_mars << "," << dt_mars << "," << opik_mars << "\n";
  csv_impacts << "Mercury," << r_mercury << "," << dt_mercury << "," << opik_mercury << "\n";
  csv_impacts << "Moon," << r_moon << "," << dt_moon << "," << opik_earth * 0.038 << "\n";
  csv_impacts.close();
  std::cout << "✅ Saved replications_ss/paper_275/planetary_impact_rates.csv" << std::endl;

  // 9. Export CSV 6: Benchmark Validation Suite & R^2 Computation
  std::ofstream csv_bench("replications_ss/paper_275/benchmark_validation.csv");
  csv_bench << "parameter,observed_bottke,model_value,rel_diff_pct,source_ref\n";

  auto benchmarks = model.get_benchmark_validation_suite();
  std::vector<double> obs_vec, mod_vec;
  for (const auto& b : benchmarks) {
    double rel_diff = std::abs(b.model_value - b.observed_bottke) / std::max(1.0e-5, std::abs(b.observed_bottke)) * 100.0;
    csv_bench << "\"" << b.parameter << "\","
              << std::scientific << std::setprecision(4)
              << b.observed_bottke << ","
              << b.model_value << ","
              << std::fixed << std::setprecision(2)
              << rel_diff << ",\""
              << b.source_ref << "\"\n";
    obs_vec.push_back(b.observed_bottke);
    mod_vec.push_back(b.model_value);
  }
  csv_bench.close();
  std::cout << "✅ Saved replications_ss/paper_275/benchmark_validation.csv" << std::endl;

  // Compute R^2 across log-scaled benchmarks
  double mean_obs = 0.0, mean_mod = 0.0;
  int n_b = obs_vec.size();
  std::vector<double> log_obs(n_b), log_mod(n_b);
  for (int i = 0; i < n_b; ++i) {
    log_obs[i] = std::log10(std::max(1.0e-5, obs_vec[i]));
    log_mod[i] = std::log10(std::max(1.0e-5, mod_vec[i]));
    mean_obs += log_obs[i];
    mean_mod += log_mod[i];
  }
  mean_obs /= n_b;
  mean_mod /= n_b;

  double ss_tot = 0.0, ss_res = 0.0;
  for (int i = 0; i < n_b; ++i) {
    ss_tot += (log_obs[i] - mean_obs) * (log_obs[i] - mean_obs);
    ss_res += (log_obs[i] - log_mod[i]) * (log_obs[i] - log_mod[i]);
  }
  double r_squared = 1.0 - (ss_res / std::max(1.0e-12, ss_tot));

  std::cout << "\n=================================================================" << std::endl;
  std::cout << "  Replication Fidelity Assessment: " << std::endl;
  std::cout << "  Logarithmic Benchmark Correlation R^2 = " << std::fixed << std::setprecision(6) << r_squared << std::endl;
  std::cout << "  Passed Campaign Target (R^2 >= 0.98): " << (r_squared >= 0.98 ? "YES [PASSED]" : "NO [FAILED]") << std::endl;
  std::cout << "=================================================================" << std::endl;

  return 0;
}
