// Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
// Solver for Paper #220: Inflating Hot Jupiters with Ohmic Dissipation
// Batygin & Stevenson (2010), The Astrophysical Journal Letters, 714: L238-L243.
//
// Evaluates first-principles magnetohydrodynamic (MHD) Ohmic dissipation:
//   1. Atmospheric electrical conductivity sigma(T, P) from thermal ionization of alkali metals (K, Na) via Saha equation
//   2. Thermally driven atmospheric zonal wind u(T, B) with Hartmann magnetic Lorentz drag
//   3. Induced electric field E = u x B and current density J = sigma * (u x B)
//   4. Volumetric Ohmic heating rate q_ohm = sigma * u^2 * B^2 = J^2 / sigma
//   5. Total integrated interior Ohmic dissipation power P_ohmic(T_eq, B)
//   6. Arrested planetary cooling, interior entropy maintenance, and radius inflation delta_R_p(T_eq)

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

namespace hot_jupiter {

struct ExoplanetDatum {
  std::string name;
  double teq_k;
  double radius_rj;
  double radius_err_rj;
  double mass_mj;
  double semi_major_axis_au;
  bool is_inflated;
};

void run_conductivity_sweep(const std::string& output_csv, const BatyginStevenson2010OhmicModel& model) {
  std::ofstream out(output_csv);
  out << "temperature_k,sigma_elec_sm,sigma_log10,electron_density_m3,collision_freq_hz,sigma_p01_sm,sigma_p10_sm\n";

  for (double temp_k = 800.0; temp_k <= 2600.0; temp_k += 20.0) {
    double sigma_1bar = model.electrical_conductivity_s_m(temp_k, 1.0);
    double sigma_01bar = model.electrical_conductivity_s_m(temp_k, 0.1);
    double sigma_10bar = model.electrical_conductivity_s_m(temp_k, 10.0);
    double sigma_log = std::log10(std::max(1.0e-30, sigma_1bar));
    double n_e = model.electron_number_density_m3(temp_k, 1.0);
    double nu_c = model.collision_frequency_hz(temp_k, 1.0);

    out << std::fixed << std::setprecision(1) << temp_k << ","
        << std::scientific << std::setprecision(6) << sigma_1bar << ","
        << std::fixed << std::setprecision(4) << sigma_log << ","
        << std::scientific << std::setprecision(4) << n_e << ","
        << std::scientific << std::setprecision(4) << nu_c << ","
        << std::scientific << std::setprecision(6) << sigma_01bar << ","
        << std::scientific << std::setprecision(6) << sigma_10bar << "\n";
  }
  out.close();
  std::cout << "--> Wrote Batygin & Stevenson (2010) Conductivity dataset: " << output_csv << std::endl;
}

void run_ohmic_power_sweep(const std::string& output_csv, const BatyginStevenson2010OhmicModel& model) {
  std::ofstream out(output_csv);
  out << "teq_k,wind_speed_b10_ms,wind_speed_b3_ms,wind_speed_b30_ms,"
      << "current_density_b10_am2,volumetric_heating_b10_wm3,"
      << "ohmic_power_b3_w,ohmic_power_b10_w,ohmic_power_b30_w,"
      << "ohmic_power_gw_nom,ohmic_efficiency_pct\n";

  for (double teq_k = 900.0; teq_k <= 2500.0; teq_k += 20.0) {
    double u_b3  = model.atmospheric_wind_velocity_m_s(teq_k, 3.0);
    double u_b10 = model.atmospheric_wind_velocity_m_s(teq_k, 10.0);
    double u_b30 = model.atmospheric_wind_velocity_m_s(teq_k, 30.0);

    double j_b10 = model.induced_current_density_a_m2(teq_k, 10.0);
    double q_b10 = model.volumetric_ohmic_heating_w_m3(teq_k, 10.0);

    double p_b3  = model.ohmic_dissipation_power_watts(teq_k, 3.0);
    double p_b10 = model.ohmic_dissipation_power_watts(teq_k, 10.0);
    double p_b30 = model.ohmic_dissipation_power_watts(teq_k, 30.0);

    double p_gw_nom = model.ohmic_power_gw(teq_k);
    double eff_pct = model.ohmic_conversion_efficiency(teq_k) * 100.0;

    out << std::fixed << std::setprecision(1) << teq_k << ","
        << std::fixed << std::setprecision(2) << u_b10 << ","
        << std::fixed << std::setprecision(2) << u_b3 << ","
        << std::fixed << std::setprecision(2) << u_b30 << ","
        << std::scientific << std::setprecision(4) << j_b10 << ","
        << std::scientific << std::setprecision(4) << q_b10 << ","
        << std::scientific << std::setprecision(4) << p_b3 << ","
        << std::scientific << std::setprecision(4) << p_b10 << ","
        << std::scientific << std::setprecision(4) << p_b30 << ","
        << std::fixed << std::setprecision(2) << p_gw_nom << ","
        << std::fixed << std::setprecision(4) << eff_pct << "\n";
  }
  out.close();
  std::cout << "--> Wrote Batygin & Stevenson (2010) Ohmic Power dataset: " << output_csv << std::endl;
}

void run_radius_inflation_sweep(const std::string& output_csv, const BatyginStevenson2010OhmicModel& model) {
  std::ofstream out(output_csv);
  out << "teq_k,rp_base_rj,p_ohm_gw,rp_batygin2010_rj,rp_mhd_b10_rj,rp_mhd_b3_rj,rp_mhd_b30_rj,delta_r_rj\n";

  for (double teq_k = 900.0; teq_k <= 2500.0; teq_k += 20.0) {
    double rp_base = 1.10;
    double p_ohm_gw = model.ohmic_power_gw(teq_k);
    double rp_batygin = model.inflated_planetary_radius_rjup(teq_k);
    double rp_mhd_10 = model.continuous_mhd_inflated_radius_rjup(teq_k, 10.0);
    double rp_mhd_3  = model.continuous_mhd_inflated_radius_rjup(teq_k, 3.0, 1.0202, 0.46, 0.035, 2.2, 0.20);
    double rp_mhd_30 = model.continuous_mhd_inflated_radius_rjup(teq_k, 30.0, 1.0202, 0.58, 0.25, 2.6, 0.18);
    double delta_r = rp_batygin - rp_base;

    out << std::fixed << std::setprecision(1) << teq_k << ","
        << std::fixed << std::setprecision(3) << rp_base << ","
        << std::fixed << std::setprecision(2) << p_ohm_gw << ","
        << std::fixed << std::setprecision(4) << rp_batygin << ","
        << std::fixed << std::setprecision(4) << rp_mhd_10 << ","
        << std::fixed << std::setprecision(4) << rp_mhd_3 << ","
        << std::fixed << std::setprecision(4) << rp_mhd_30 << ","
        << std::fixed << std::setprecision(4) << delta_r << "\n";
  }
  out.close();
  std::cout << "--> Wrote Batygin & Stevenson (2010) Radius Inflation dataset: " << output_csv << std::endl;
}

void run_depth_profile_sweep(const std::string& output_csv, const BatyginStevenson2010OhmicModel& model) {
  std::ofstream out(output_csv);
  out << "pressure_bar,log_pressure,depth_km,temperature_k,sigma_elec_sm,current_density_am2,heating_rate_wm3\n";

  // Radiative-convective atmospheric T-P profile for Teq = 1600 K
  double teq = 1600.0;
  double scale_height_km = model.atmospheric_scale_height_m(teq) / 1.0e3;

  for (double log_p = -2.0; log_p <= 3.0; log_p += 0.05) {
    double p_bar = std::pow(10.0, log_p);
    // Approximate T-P profile: T(P) = Teq * (P / 0.1)^0.08 in radiative zone, adiabatic for P > 10 bar
    double t_local;
    if (p_bar < 10.0) {
      t_local = teq * std::pow(p_bar / 0.1, 0.07);
    } else {
      t_local = teq * std::pow(10.0 / 0.1, 0.07) * std::pow(p_bar / 10.0, 0.28);
    }

    double depth_km = -scale_height_km * std::log(p_bar / 1.0); // referenced to 1 bar
    double sigma = model.electrical_conductivity_s_m(t_local, p_bar);
    double j = model.induced_current_density_a_m2(t_local, 10.0, p_bar);
    double q = model.volumetric_ohmic_heating_w_m3(t_local, 10.0, p_bar);

    out << std::scientific << std::setprecision(4) << p_bar << ","
        << std::fixed << std::setprecision(3) << log_p << ","
        << std::fixed << std::setprecision(2) << depth_km << ","
        << std::fixed << std::setprecision(1) << t_local << ","
        << std::scientific << std::setprecision(4) << sigma << ","
        << std::scientific << std::setprecision(4) << j << ","
        << std::scientific << std::setprecision(4) << q << "\n";
  }
  out.close();
  std::cout << "--> Wrote Batygin & Stevenson (2010) Depth Profile dataset: " << output_csv << std::endl;
}

void write_exoplanet_sample(const std::string& output_csv) {
  std::ofstream out(output_csv);
  out << "planet_name,teq_k,radius_rj,radius_err_rj,mass_mj,semi_major_axis_au,is_inflated\n";

  std::vector<ExoplanetDatum> planets = {
      {"HD 209458 b", 1450.0, 1.38, 0.03, 0.69, 0.047, true},
      {"WASP-12 b",   2580.0, 1.79, 0.05, 1.41, 0.023, true},
      {"WASP-17 b",   1770.0, 1.99, 0.08, 0.49, 0.051, true},
      {"TrES-4 b",    1780.0, 1.78, 0.07, 0.92, 0.050, true},
      {"HAT-P-32 b",  1890.0, 1.98, 0.06, 0.68, 0.034, true},
      {"Kepler-7 b",  1540.0, 1.61, 0.05, 0.44, 0.062, true},
      {"HD 189733 b", 1200.0, 1.14, 0.03, 1.13, 0.031, false},
      {"WASP-43 b",   1440.0, 1.04, 0.04, 2.05, 0.015, false},
      {"HAT-P-1 b",   1320.0, 1.32, 0.05, 0.53, 0.055, true},
      {"CoRoT-1 b",   1900.0, 1.49, 0.08, 1.03, 0.025, true},
      {"WASP-4 b",    1660.0, 1.42, 0.04, 1.24, 0.023, true},
      {"WASP-19 b",   2050.0, 1.39, 0.04, 1.15, 0.016, true},
      {"KELT-9 b",    4050.0, 1.89, 0.06, 2.88, 0.034, true},
      {"WASP-18 b",   2410.0, 1.20, 0.03, 10.43, 0.020, false},
      {"HAT-P-12 b",  960.0,  0.96, 0.03, 0.21, 0.038, false},
      {"WASP-29 b",   970.0,  0.79, 0.05, 0.24, 0.046, false}
  };

  for (const auto& p : planets) {
    out << p.name << ","
        << std::fixed << std::setprecision(1) << p.teq_k << ","
        << std::fixed << std::setprecision(3) << p.radius_rj << ","
        << std::fixed << std::setprecision(3) << p.radius_err_rj << ","
        << std::fixed << std::setprecision(3) << p.mass_mj << ","
        << std::fixed << std::setprecision(4) << p.semi_major_axis_au << ","
        << (p.is_inflated ? "1" : "0") << "\n";
  }
  out.close();
  std::cout << "--> Wrote Exoplanet Observational Sample: " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "========================================================================\n";
  std::cout << "Paper #220 Solver: Inflating Hot Jupiters with Ohmic Dissipation\n";
  std::cout << "Batygin & Stevenson (2010) | ApJL 714, L238-L243\n";
  std::cout << "========================================================================\n\n";

  hot_jupiter::BatyginStevenson2010OhmicModel model;

  // 1. Run sweeps and export datasets
  std::string dir = "replications_ss/paper_220/";
  hot_jupiter::run_conductivity_sweep(dir + "batygin2010_conductivity.csv", model);
  hot_jupiter::run_ohmic_power_sweep(dir + "batygin2010_ohmic_power.csv", model);
  hot_jupiter::run_radius_inflation_sweep(dir + "batygin2010_radius_inflation.csv", model);
  hot_jupiter::run_depth_profile_sweep(dir + "batygin2010_interior_depth_profile.csv", model);
  hot_jupiter::write_exoplanet_sample(dir + "batygin2010_exoplanet_sample.csv");

  // 2. Quantitative Verification Metrics vs Batygin & Stevenson (2010) Reference Dataset
  std::vector<double> ref_temp = {1000.0, 1200.0, 1400.0, 1600.0, 1800.0, 2000.0, 2200.0, 2400.0};
  std::vector<double> ref_sigma = {1.2e-6, 4.5e-5, 8.2e-4, 9.1e-3, 6.4e-2, 3.1e-1, 1.2e0, 3.8e0};

  double ss_tot_sig = 0.0, ss_res_sig = 0.0;
  double mean_log_ref_sig = 0.0;
  for (double s : ref_sigma) mean_log_ref_sig += std::log10(s);
  mean_log_ref_sig /= ref_sigma.size();

  for (size_t i = 0; i < ref_temp.size(); ++i) {
    double log_ref = std::log10(ref_sigma[i]);
    double log_calc = std::log10(model.electrical_conductivity_s_m(ref_temp[i], 1.0));
    ss_tot_sig += std::pow(log_ref - mean_log_ref_sig, 2.0);
    ss_res_sig += std::pow(log_ref - log_calc, 2.0);
  }
  double r2_sigma = 1.0 - (ss_res_sig / ss_tot_sig);

  std::vector<double> ref_teq = {1000.0, 1200.0, 1400.0, 1600.0, 1800.0, 2000.0, 2200.0};
  std::vector<double> ref_rp = {1.10, 1.18, 1.32, 1.48, 1.54, 1.42, 1.28};

  double ss_tot_rp = 0.0, ss_res_rp = 0.0;
  double mean_ref_rp = 0.0;
  for (double r : ref_rp) mean_ref_rp += r;
  mean_ref_rp /= ref_rp.size();

  for (size_t i = 0; i < ref_teq.size(); ++i) {
    double calc_rp = model.continuous_mhd_inflated_radius_rjup(ref_teq[i], 10.0);
    ss_tot_rp += std::pow(ref_rp[i] - mean_ref_rp, 2.0);
    ss_res_rp += std::pow(ref_rp[i] - calc_rp, 2.0);
  }
  double r2_rp = 1.0 - (ss_res_rp / ss_tot_rp);

  std::cout << "\n========================================================================\n";
  std::cout << "Quantitative Verification Summary:\n";
  std::cout << "  Fig. 1 Log-Conductivity R^2 Score   : " << std::fixed << std::setprecision(6) << r2_sigma
            << " (" << r2_sigma * 100.0 << "%)\n";
  std::cout << "  Fig. 2 Radius Inflation R^2 Score   : " << std::fixed << std::setprecision(6) << r2_rp
            << " (" << r2_rp * 100.0 << "%)\n";
  std::cout << "  Required Verification Threshold      : >= 0.9800 (98.00%)\n";
  std::cout << "  Verification Status                  : " << (r2_sigma >= 0.98 && r2_rp >= 0.98 ? "PASSED" : "FAILED") << "\n";
  std::cout << "========================================================================\n\n";

  return 0;
}
