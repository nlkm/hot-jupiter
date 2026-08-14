// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Paper #221: Spohn & Schubert (2003)
// "Oceans in the Icy Moons of Saturn and Jupiter" (Icarus 161, 456-467)
// Liquid Ocean Maintenance Under Convective Ice Shells in Outer Planet Satellites

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::SpohnSchubert2003IcyMoonOceanModel model;

  std::cout << "============================================================================" << std::endl;
  std::cout << "Paper #221 Replication: Spohn & Schubert (2003)" << std::endl;
  std::cout << "Oceans in the Icy Moons of Saturn and Jupiter (Europa, Ganymede, Callisto, Titan, Enceladus)" << std::endl;
  std::cout << "============================================================================" << std::endl;

  // 1. Multi-Satellite Equilibrium Analysis (Europa, Ganymede, Callisto, Titan, Enceladus)
  std::vector<hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::SatelliteOceanResult> sat_results;
  sat_results.push_back(model.evaluate_europa(17.0, 1.0e14, 0.0)); // Europa nominal F_tide = 17 mW/m2 (F_tot = 23 mW/m2)
  sat_results.push_back(model.evaluate_ganymede(1.0, 1.0e14, 0.0));
  sat_results.push_back(model.evaluate_callisto(0.0, 1.0e14, 0.0));
  sat_results.push_back(model.evaluate_callisto(0.0, 1.0e14, 5.0)); // Callisto with 5% NH3
  sat_results.push_back(model.evaluate_titan(0.0, 1.0e14, 5.0));    // Titan with 5% NH3
  sat_results.push_back(model.evaluate_enceladus(80.0, 1.0e14, 1.0)); // Enceladus south polar active

  std::cout << "\n[1] Multi-Satellite Ocean Maintenance & Ice Shell Equilibrium Summary:\n" << std::endl;
  std::cout << std::left << std::setw(18) << "Satellite"
            << std::setw(10) << "g [m/s2]"
            << std::setw(10) << "Ts [K]"
            << std::setw(10) << "Tb [K]"
            << std::setw(14) << "F_sup[mW/m2]"
            << std::setw(14) << "D_eq [km]"
            << std::setw(14) << "D_lid [km]"
            << std::setw(14) << "D_ocean [km]"
            << std::setw(10) << "Nu"
            << std::setw(12) << "Convective"
            << std::setw(10) << "Ocean?"
            << std::endl;
  std::cout << std::string(126, '-') << std::endl;

  std::ofstream csv_summary("replications_ss/paper_221/ocean_equilibrium_summary.csv");
  csv_summary << "satellite,g_m_s2,T_surf_k,T_base_k,P_base_mpa,F_supply_mw_m2,F_crit_mw_m2,"
              << "D_eq_km,D_lid_km,D_conv_km,D_ocean_km,Nu,Ra_b,Ra_cr,is_convective,ocean_survives\n";

  for (const auto& res : sat_results) {
    std::cout << std::left << std::setw(18) << res.name
              << std::fixed << std::setprecision(3)
              << std::setw(10) << res.g
              << std::setprecision(1)
              << std::setw(10) << res.T_surf_k
              << std::setw(10) << res.T_base_k
              << std::setprecision(2)
              << std::setw(14) << res.F_supply_mw_m2
              << std::setw(14) << res.D_shell_km
              << std::setw(14) << res.D_lid_km
              << std::setw(14) << res.D_ocean_km
              << std::setw(10) << res.Nu
              << std::setw(12) << (res.is_convective ? "YES" : "NO")
              << std::setw(10) << (res.ocean_survives ? "YES" : "NO")
              << std::endl;

    csv_summary << res.name << "," << res.g << "," << res.T_surf_k << "," << res.T_base_k << ","
                << res.P_base_mpa << "," << res.F_supply_mw_m2 << "," << res.F_crit_mw_m2 << ","
                << res.D_shell_km << "," << res.D_lid_km << "," << res.D_conv_km << ","
                << res.D_ocean_km << "," << res.Nu << "," << res.Ra_b << "," << res.Ra_cr << ","
                << (res.is_convective ? 1 : 0) << "," << (res.ocean_survives ? 1 : 0) << "\n";
  }
  csv_summary.close();
  std::cout << ">>> Saved ocean_equilibrium_summary.csv" << std::endl;

  // 2. Ice Shell Thickness Sweep for Europa, Ganymede, Callisto (D = 5 to 150 km)
  std::ofstream csv_sweep("replications_ss/paper_221/shell_thickness_sweep.csv");
  csv_sweep << "D_km,europa_F_total_mw_m2,europa_F_cond_mw_m2,europa_Nu,europa_Ra_b,"
            << "ganymede_F_total_mw_m2,ganymede_F_cond_mw_m2,ganymede_Nu,ganymede_Ra_b,"
            << "callisto_F_total_mw_m2,callisto_F_cond_mw_m2,callisto_Nu,callisto_Ra_b\n";

  for (double d = 5.0; d <= 150.0; d += 2.5) {
    // Europa
    double p_b_eu = model.basal_pressure_pa(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_EUROPA);
    double t_b_eu = model.melting_temperature_k(p_b_eu, 0.0);
    double f_tot_eu = model.total_heat_flux_mw_m2(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_EUROPA,
                                                  hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_EUROPA_K, t_b_eu);
    double f_cond_eu = model.conductive_heat_flux_mw_m2(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_EUROPA_K, t_b_eu);
    double nu_eu = model.nusselt_number(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_EUROPA,
                                        hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_EUROPA_K, t_b_eu);
    double ra_eu = model.basal_rayleigh_number(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_EUROPA,
                                              hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_EUROPA_K, t_b_eu);

    // Ganymede
    double p_b_ga = model.basal_pressure_pa(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_GANYMEDE);
    double t_b_ga = model.melting_temperature_k(p_b_ga, 0.0);
    double f_tot_ga = model.total_heat_flux_mw_m2(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_GANYMEDE,
                                                  hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_GANYMEDE_K, t_b_ga);
    double f_cond_ga = model.conductive_heat_flux_mw_m2(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_GANYMEDE_K, t_b_ga);
    double nu_ga = model.nusselt_number(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_GANYMEDE,
                                        hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_GANYMEDE_K, t_b_ga);
    double ra_ga = model.basal_rayleigh_number(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_GANYMEDE,
                                              hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_GANYMEDE_K, t_b_ga);

    // Callisto
    double p_b_ca = model.basal_pressure_pa(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_CALLISTO);
    double t_b_ca = model.melting_temperature_k(p_b_ca, 0.0);
    double f_tot_ca = model.total_heat_flux_mw_m2(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_CALLISTO,
                                                  hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_CALLISTO_K, t_b_ca);
    double f_cond_ca = model.conductive_heat_flux_mw_m2(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_CALLISTO_K, t_b_ca);
    double nu_ca = model.nusselt_number(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_CALLISTO,
                                        hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_CALLISTO_K, t_b_ca);
    double ra_ca = model.basal_rayleigh_number(d, hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::G_CALLISTO,
                                              hot_jupiter::SpohnSchubert2003IcyMoonOceanModel::T_SURF_CALLISTO_K, t_b_ca);

    csv_sweep << std::fixed << std::setprecision(2)
              << d << "," << f_tot_eu << "," << f_cond_eu << "," << nu_eu << "," << std::scientific << ra_eu << std::fixed << ","
              << f_tot_ga << "," << f_cond_ga << "," << nu_ga << "," << std::scientific << ra_ga << std::fixed << ","
              << f_tot_ca << "," << f_cond_ca << "," << nu_ca << "," << std::scientific << ra_ca << "\n";
  }
  csv_sweep.close();
  std::cout << ">>> Saved shell_thickness_sweep.csv" << std::endl;

  // 3. Ammonia Antifreeze Sensitivity Sweep for Callisto & Titan (0 to 15 wt% NH3)
  std::ofstream csv_nh3("replications_ss/paper_221/ammonia_sensitivity.csv");
  csv_nh3 << "nh3_wt_pct,callisto_T_base_k,callisto_D_eq_km,callisto_D_ocean_km,callisto_Nu,"
          << "titan_T_base_k,titan_D_eq_km,titan_D_ocean_km,titan_Nu\n";

  for (double nh3 = 0.0; nh3 <= 15.0; nh3 += 0.5) {
    auto res_callisto = model.evaluate_callisto(0.0, 1.0e14, nh3);
    auto res_titan = model.evaluate_titan(0.0, 1.0e14, nh3);

    csv_nh3 << std::fixed << std::setprecision(2)
            << nh3 << "," << res_callisto.T_base_k << "," << res_callisto.D_shell_km << ","
            << res_callisto.D_ocean_km << "," << res_callisto.Nu << ","
            << res_titan.T_base_k << "," << res_titan.D_shell_km << ","
            << res_titan.D_ocean_km << "," << res_titan.Nu << "\n";
  }
  csv_nh3.close();
  std::cout << ">>> Saved ammonia_sensitivity.csv" << std::endl;

  // 4. Benchmark Quantitative Fit Comparison vs. Published Values in Spohn & Schubert (2003)
  struct BenchmarkPoint {
    std::string metric_name;
    double published_value;
    double model_value;
    std::string unit;
  };

  auto eu = model.evaluate_europa(17.0, 1.0e14, 0.0);
  auto ga = model.evaluate_ganymede(1.0, 1.0e14, 0.0);
  auto ca_pure = model.evaluate_callisto(0.0, 1.0e14, 0.0);
  auto ca_nh3 = model.evaluate_callisto(0.0, 1.0e14, 5.0);
  auto ti_nh3 = model.evaluate_titan(0.0, 1.0e14, 5.0);

  std::vector<BenchmarkPoint> benchmarks = {
    {"Europa Equilibrium Shell D_eq", 28.0, eu.D_shell_km, "km"},
    {"Europa Ocean Thickness D_oc", 92.0, eu.D_ocean_km, "km"},
    {"Europa Nusselt Number Nu", 1.65, eu.Nu, ""},
    {"Ganymede Equilibrium Shell D_eq", 88.0, ga.D_shell_km, "km"},
    {"Ganymede Nusselt Number Nu", 4.05, ga.Nu, ""},
    {"Callisto (Pure) Shell D_eq", 180.0, ca_pure.D_shell_km, "km"},
    {"Callisto (Pure) Nusselt Nu", 6.25, ca_pure.Nu, ""},
    {"Callisto (5% NH3) Shell D_eq", 125.0, ca_nh3.D_shell_km, "km"},
    {"Callisto (5% NH3) Ocean D_oc", 175.0, ca_nh3.D_ocean_km, "km"},
    {"Titan (5% NH3) Shell D_eq", 105.0, ti_nh3.D_shell_km, "km"},
    {"Titan (5% NH3) Ocean D_oc", 295.0, ti_nh3.D_ocean_km, "km"}
  };

  std::ofstream csv_bench("replications_ss/paper_221/benchmark_comparison.csv");
  csv_bench << "metric_name,published_val,model_val,unit,rel_error_pct\n";

  double ss_res = 0.0;
  double ss_tot = 0.0;
  double mean_pub = 0.0;
  for (const auto& b : benchmarks) {
    mean_pub += b.published_value;
  }
  mean_pub /= benchmarks.size();

  std::cout << "\n[4] Quantitative Verification vs. Spohn & Schubert (2003) Benchmarks:\n" << std::endl;
  std::cout << std::left << std::setw(34) << "Physical Metric"
            << std::setw(16) << "Published"
            << std::setw(16) << "Model Engine"
            << std::setw(14) << "Rel Error [%]"
            << std::setw(10) << "Status"
            << std::endl;
  std::cout << std::string(90, '-') << std::endl;

  for (const auto& b : benchmarks) {
    double rel_err = std::abs(b.model_value - b.published_value) / b.published_value * 100.0;
    double diff = b.model_value - b.published_value;
    ss_res += diff * diff;
    ss_tot += (b.published_value - mean_pub) * (b.published_value - mean_pub);

    std::cout << std::left << std::setw(34) << b.metric_name
              << std::fixed << std::setprecision(2)
              << std::setw(16) << b.published_value
              << std::setw(16) << b.model_value
              << std::setprecision(3)
              << std::setw(14) << rel_err
              << std::setw(10) << (rel_err < 5.0 ? "PASSED" : "REVIEW")
              << std::endl;

    csv_bench << "\"" << b.metric_name << "\"," << b.published_value << ","
              << b.model_value << ",\"" << b.unit << "\"," << rel_err << "\n";
  }
  csv_bench.close();
  std::cout << ">>> Saved benchmark_comparison.csv" << std::endl;

  double r2 = 1.0 - (ss_res / ss_tot);
  std::cout << "\n----------------------------------------------------------------------------" << std::endl;
  std::cout << "Coefficient of Determination R^2: " << std::fixed << std::setprecision(5) << r2 << std::endl;
  std::cout << "Evaluation Standard (R^2 >= 0.98): " << (r2 >= 0.98 ? "PASSED [HIGH FIDELITY]" : "FAILED") << std::endl;
  std::cout << "----------------------------------------------------------------------------\n" << std::endl;

  return 0;
}
