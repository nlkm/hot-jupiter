// Copyright 2026 Antigravity Scientific Automation & Solar System Dynamics Replication Campaign
// Replication of Paper #247: Gladman, Migliorini, Morbidelli, Zappalà, Michel, Cellino,
// Froeschle, Levison, Bailey, Duncan (1997)
// "Dynamical Lifetimes of Objects Injected into Asteroid Belt Resonances"
// Science, 277(5323), 197-201 (1997)
// First-principles C++ simulation of NEO orbital decay timescales, chaotic eccentricity pumping,
// Öpik close-encounter scattering, terrestrial planet impact probabilities,
// and solar collision vs. Jupiter ejection branching ratios.

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
  std::string parameter_name;
  double observed_value;
  double model_value;
  std::string unit;
  std::string description;
};

int main() {
  std::cout << "=================================================================" << std::endl;
  std::cout << "  Paper #247 Replication: Gladman et al. (1997)                  " << std::endl;
  std::cout << "  Dynamical Lifetimes of Objects Injected into Resonances        " << std::endl;
  std::cout << "  Science 277, 197-201 (1997)                                    " << std::endl;
  std::cout << "=================================================================" << std::endl;

  hot_jupiter::Gladman1997ResonanceLifetimesModel model;

  // 1. Resonance Lifetime Computations
  double med_31 = model.median_lifetime_myr("3:1");
  double mean_31 = model.mean_lifetime_myr("3:1");
  double med_nu6 = model.median_lifetime_myr("nu6");
  double mean_nu6 = model.mean_lifetime_myr("nu6");
  double med_52 = model.median_lifetime_myr("5:2");
  double mean_52 = model.mean_lifetime_myr("5:2");
  double med_21 = model.median_lifetime_myr("2:1");
  double mean_21 = model.mean_lifetime_myr("2:1");

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "--- Resonance Dynamical Lifetimes ---" << std::endl;
  std::cout << "3:1 MMR (a ~ 2.50 AU):     Median = " << med_31 << " Myr, Mean = " << mean_31 << " Myr" << std::endl;
  std::cout << "nu_6 Secular (a ~ 2.15 AU): Median = " << med_nu6 << " Myr, Mean = " << mean_nu6 << " Myr" << std::endl;
  std::cout << "5:2 MMR (a ~ 2.82 AU):     Median = " << med_52 << " Myr, Mean = " << mean_52 << " Myr" << std::endl;
  std::cout << "2:1 MMR (a ~ 3.28 AU):     Median = " << med_21 << " Myr, Mean = " << mean_21 << " Myr" << std::endl;
  std::cout << std::endl;

  // 2. Branching Ratios for 3:1 MMR & nu_6
  std::cout << "--- 3:1 MMR Elimination Fates (t -> inf) ---" << std::endl;
  std::cout << "Solar Photosphere Collision:  " << model.cumulative_sun_collision_fraction("3:1", 50.0) * 100.0 << " %" << std::endl;
  std::cout << "Jupiter Hyperbolic Ejection:  " << model.cumulative_jupiter_ejection_fraction("3:1", 50.0) * 100.0 << " %" << std::endl;
  std::cout << "Terrestrial Planet Impacts:   " << model.cumulative_terrestrial_impact_fraction("3:1", 50.0) * 100.0 << " %" << std::endl;
  std::cout << "  - Earth Impact Fraction:    " << model.cumulative_planet_impact_fraction("3:1", "Earth", 50.0) * 100.0 << " %" << std::endl;
  std::cout << "  - Venus Impact Fraction:    " << model.cumulative_planet_impact_fraction("3:1", "Venus", 50.0) * 100.0 << " %" << std::endl;
  std::cout << "  - Mars Impact Fraction:     " << model.cumulative_planet_impact_fraction("3:1", "Mars", 50.0) * 100.0 << " %" << std::endl;
  std::cout << "  - Mercury Impact Fraction:  " << model.cumulative_planet_impact_fraction("3:1", "Mercury", 50.0) * 100.0 << " %" << std::endl;
  std::cout << std::endl;

  // 3. Steady-State Near-Earth Asteroid Supply Budget
  double n_nea_target = 1000.0;  // D > 1 km NEAs
  double inj_rate_31 = model.steady_state_injection_rate_per_myr(n_nea_target, mean_31);
  std::cout << "--- Steady-State NEA (D > 1 km) Balance ---" << std::endl;
  std::cout << "Steady-state NEA Population: " << n_nea_target << " objects" << std::endl;
  std::cout << "Required Injection Rate:     " << inj_rate_31 << " asteroids / Myr ("
            << inj_rate_31 / 1e6 << " yr^-1)" << std::endl;
  std::cout << std::endl;

  // 4. Export Time-Series CSV: Survival Fraction & Elimination Rates
  std::ofstream csv_surv("replications_ss/paper_247/resonance_survival_timeseries.csv");
  csv_surv << "t_myr,surv_31,surv_nu6,surv_52,surv_21,rate_31,rate_nu6,rate_52,rate_21,"
           << "sun_31,jup_31,terr_31,earth_31,venus_31,mars_31,"
           << "sun_nu6,jup_nu6,terr_nu6,sun_52,jup_52\n";

  for (double t = 0.0; t <= 25.0; t += 0.05) {
    double s_31 = model.survival_fraction("3:1", t);
    double s_nu6 = model.survival_fraction("nu6", t);
    double s_52 = model.survival_fraction("5:2", t);
    double s_21 = model.survival_fraction("2:1", t);

    double r_31 = model.removal_rate_per_myr("3:1", t);
    double r_nu6 = model.removal_rate_per_myr("nu6", t);
    double r_52 = model.removal_rate_per_myr("5:2", t);
    double r_21 = model.removal_rate_per_myr("2:1", t);

    double sun_31 = model.cumulative_sun_collision_fraction("3:1", t);
    double jup_31 = model.cumulative_jupiter_ejection_fraction("3:1", t);
    double terr_31 = model.cumulative_terrestrial_impact_fraction("3:1", t);
    double earth_31 = model.cumulative_planet_impact_fraction("3:1", "Earth", t);
    double venus_31 = model.cumulative_planet_impact_fraction("3:1", "Venus", t);
    double mars_31 = model.cumulative_planet_impact_fraction("3:1", "Mars", t);

    double sun_nu6 = model.cumulative_sun_collision_fraction("nu6", t);
    double jup_nu6 = model.cumulative_jupiter_ejection_fraction("nu6", t);
    double terr_nu6 = model.cumulative_terrestrial_impact_fraction("nu6", t);

    double sun_52 = model.cumulative_sun_collision_fraction("5:2", t);
    double jup_52 = model.cumulative_jupiter_ejection_fraction("5:2", t);

    csv_surv << std::fixed << std::setprecision(4) << t << ","
             << std::setprecision(6)
             << s_31 << "," << s_nu6 << "," << s_52 << "," << s_21 << ","
             << r_31 << "," << r_nu6 << "," << r_52 << "," << r_21 << ","
             << sun_31 << "," << jup_31 << "," << terr_31 << "," << earth_31 << "," << venus_31 << "," << mars_31 << ","
             << sun_nu6 << "," << jup_nu6 << "," << terr_nu6 << "," << sun_52 << "," << jup_52 << "\n";
  }
  csv_surv.close();
  std::cout << "✅ Saved replications_ss/paper_247/resonance_survival_timeseries.csv" << std::endl;

  // 5. Export Öpik Encounter Sweep CSV
  std::ofstream csv_opik("replications_ss/paper_247/opik_encounter_sweep.csv");
  csv_opik << "a_au,e,inc_deg,q_au,Q_au,planet,a_p_au,U_dim,v_enc_kms,sigma_km2,p_coll_per_yr\n";

  struct PlanetData {
    std::string name;
    double a_p;
    double r_p;
    double m_p;
  };

  std::vector<PlanetData> planets = {
    {"Mercury", 0.3871, 2439.7, 3.3011e23},
    {"Venus", 0.7233, 6051.8, 4.8675e24},
    {"Earth", 1.0000, 6371.0, 5.9722e24},
    {"Mars", 1.5237, 3389.5, 6.4171e23},
    {"Jupiter", 5.2044, 69911.0, 1.89813e27}
  };

  for (double a = 1.5; a <= 3.5; a += 0.25) {
    for (double e = 0.1; e <= 0.95; e += 0.05) {
      for (double inc_deg : {5.0, 15.0, 30.0}) {
        double inc_rad = inc_deg * (M_PI / 180.0);
        double q = a * (1.0 - e);
        double Q = a * (1.0 + e);

        for (const auto& pl : planets) {
          if (q <= pl.a_p && Q >= pl.a_p) {
            double u_dim = model.opik_encounter_velocity_dimensionless(a, e, inc_rad, pl.a_p);
            double v_enc = model.opik_encounter_velocity_km_s(a, e, inc_rad, pl.a_p);
            double sigma = model.opik_collision_cross_section_km2(v_enc, pl.r_p, pl.m_p);
            double p_coll = model.opik_intrinsic_collision_prob_per_yr(a, e, inc_rad, pl.a_p, pl.r_p, pl.m_p);

            csv_opik << std::fixed << std::setprecision(3) << a << ","
                     << std::setprecision(3) << e << ","
                     << std::setprecision(1) << inc_deg << ","
                     << std::setprecision(4) << q << ","
                     << std::setprecision(4) << Q << ","
                     << pl.name << ","
                     << std::setprecision(4) << pl.a_p << ","
                     << std::setprecision(4) << u_dim << ","
                     << std::setprecision(3) << v_enc << ","
                     << std::scientific << std::setprecision(4) << sigma << ","
                     << std::scientific << std::setprecision(4) << p_coll << "\n";
          }
        }
      }
    }
  }
  csv_opik.close();
  std::cout << "✅ Saved replications_ss/paper_247/opik_encounter_sweep.csv" << std::endl;

  // 6. Export Simulated Orbit Evolution Trajectory CSV
  std::ofstream csv_traj("replications_ss/paper_247/orbit_evolution_trajectory.csv");
  csv_traj << "trajectory_id,resonance,t_myr,a_au,e,inc_deg,q_au,Q_au,tisserand_jup,regime,fate\n";

  std::vector<std::pair<std::string, double>> test_runs = {
    {"3:1 MMR (Sun-Grazer Mode)", 2.50},
    {"nu6 Secular (Sun-Grazer Mode)", 2.15},
    {"5:2 MMR (Jupiter-Ejection Mode)", 2.82}
  };

  for (size_t id = 0; id < test_runs.size(); ++id) {
    std::string res_key = (id == 0) ? "3:1" : (id == 1) ? "nu6" : "5:2";
    auto traj = model.simulate_trajectory(test_runs[id].second, 0.15, 8.5, res_key, 6.0, 0.02);
    for (const auto& pt : traj) {
      csv_traj << std::fixed << (id + 1) << ","
               << test_runs[id].first << ","
               << std::setprecision(3) << pt.time_myr << ","
               << std::setprecision(4) << pt.a_au << ","
               << std::setprecision(4) << pt.eccentricity << ","
               << std::setprecision(2) << pt.inc_deg << ","
               << std::setprecision(4) << pt.perihelion_au << ","
               << std::setprecision(4) << pt.aphelion_au << ","
               << std::setprecision(4) << pt.tisserand_jup << ","
               << "\"" << pt.regime << "\","
               << "\"" << pt.fate << "\"\n";
    }
  }
  csv_traj.close();
  std::cout << "✅ Saved replications_ss/paper_247/orbit_evolution_trajectory.csv" << std::endl;

  // 7. Benchmark Validation Points & R^2 Calculation
  std::vector<BenchmarkPoint> benchmarks = {
    {"3:1 MMR Median Lifetime", 2.0, med_31, "Myr", "Median lifetime of particles in 3:1 resonance (Gladman et al. 1997 Fig. 1)"},
    {"nu_6 Secular Median Lifetime", 1.8, med_nu6, "Myr", "Median lifetime of particles in nu_6 secular resonance"},
    {"5:2 MMR Median Lifetime", 0.6, med_52, "Myr", "Median lifetime of particles in 5:2 resonance"},
    {"2:1 MMR Median Lifetime", 10.0, med_21, "Myr", "Median lifetime of particles in 2:1 resonance"},
    {"3:1 Solar Collision Fraction", 70.0, model.cumulative_sun_collision_fraction("3:1", 50.0) * 100.0, "%", "Percentage of 3:1 particles falling into Sun"},
    {"3:1 Jupiter Ejection Fraction", 28.0, model.cumulative_jupiter_ejection_fraction("3:1", 50.0) * 100.0, "%", "Percentage of 3:1 particles ejected by Jupiter"},
    {"3:1 Terrestrial Impact Fraction", 2.0, model.cumulative_terrestrial_impact_fraction("3:1", 50.0) * 100.0, "%", "Percentage of 3:1 particles colliding with inner planets"},
    {"3:1 Earth Impact Fraction", 0.8, model.cumulative_planet_impact_fraction("3:1", "Earth", 50.0) * 100.0, "%", "Percentage of 3:1 particles hitting Earth"},
    {"3:1 Venus Impact Fraction", 0.9, model.cumulative_planet_impact_fraction("3:1", "Venus", 50.0) * 100.0, "%", "Percentage of 3:1 particles hitting Venus"},
    {"3:1 Mars Impact Fraction", 0.2, model.cumulative_planet_impact_fraction("3:1", "Mars", 50.0) * 100.0, "%", "Percentage of 3:1 particles hitting Mars"},
    {"nu_6 Solar Collision Fraction", 72.0, model.cumulative_sun_collision_fraction("nu6", 50.0) * 100.0, "%", "Percentage of nu_6 particles falling into Sun"},
    {"nu_6 Jupiter Ejection Fraction", 25.0, model.cumulative_jupiter_ejection_fraction("nu6", 50.0) * 100.0, "%", "Percentage of nu_6 particles ejected by Jupiter"},
    {"nu_6 Terrestrial Impact Fraction", 3.0, model.cumulative_terrestrial_impact_fraction("nu6", 50.0) * 100.0, "%", "Percentage of nu_6 particles colliding with inner planets"},
    {"5:2 Jupiter Ejection Fraction", 88.0, model.cumulative_jupiter_ejection_fraction("5:2", 50.0) * 100.0, "%", "Percentage of 5:2 particles ejected by Jupiter"},
    {"5:2 Solar Collision Fraction", 11.0, model.cumulative_sun_collision_fraction("5:2", 50.0) * 100.0, "%", "Percentage of 5:2 particles falling into Sun"},
    {"Steady-State Supply Rate", 267.0, inj_rate_31, "obj/Myr", "Supply rate needed to sustain 1000 NEA population"}
  };

  std::ofstream csv_bench("replications_ss/paper_247/benchmark_validation.csv");
  csv_bench << "parameter,observed_value,model_value,unit,rel_error_pct,description\n";

  double ss_tot = 0.0;
  double ss_res = 0.0;
  double y_mean = 0.0;
  for (const auto& bp : benchmarks) {
    y_mean += bp.observed_value;
  }
  y_mean /= static_cast<double>(benchmarks.size());

  std::cout << "--- Benchmark Quantitative Validation ---" << std::endl;
  for (const auto& bp : benchmarks) {
    double err = std::abs(bp.model_value - bp.observed_value);
    double rel_err = (bp.observed_value != 0.0) ? (err / bp.observed_value) * 100.0 : 0.0;
    ss_tot += std::pow(bp.observed_value - y_mean, 2.0);
    ss_res += std::pow(bp.observed_value - bp.model_value, 2.0);

    csv_bench << "\"" << bp.parameter_name << "\","
              << std::fixed << std::setprecision(4) << bp.observed_value << ","
              << std::setprecision(4) << bp.model_value << ","
              << "\"" << bp.unit << "\","
              << std::setprecision(3) << rel_err << ","
              << "\"" << bp.description << "\"\n";

    std::cout << "• " << std::left << std::setw(34) << bp.parameter_name
              << ": Lit = " << std::setw(8) << bp.observed_value << " " << std::setw(8) << bp.unit
              << " | Model = " << std::setw(8) << bp.model_value
              << " | RelErr = " << std::setw(5) << rel_err << " %" << std::endl;
  }
  csv_bench.close();
  std::cout << "✅ Saved replications_ss/paper_247/benchmark_validation.csv" << std::endl;

  double r2 = 1.0 - (ss_res / ss_tot);
  std::cout << std::endl;
  std::cout << "=================================================================" << std::endl;
  std::cout << "  Overall Validation R^2 Score: " << std::fixed << std::setprecision(5) << r2 << std::endl;
  std::cout << "  Target Minimum R^2:            0.98000" << std::endl;
  std::cout << "  Status:                       " << (r2 >= 0.98 ? "PASSED (VERIFIED ✅)" : "FAILED ❌") << std::endl;
  std::cout << "=================================================================" << std::endl;

  return (r2 >= 0.98) ? 0 : 1;
}
