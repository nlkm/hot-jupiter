// Copyright 2026 Antigravity Scientific Automation & Solar System Dynamics Replication Campaign
// Replication of Paper #219: Chen, Nimmo, & Glatzmaier (2012, 2014), Chen & Nimmo (2011)
// "Tidal Heating in Saturn's Moon Enceladus"
// Libration-driven shear strain, viscoelastic dissipation, obliquity tides, and ice shell thickness variations.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "=================================================================" << std::endl;
  std::cout << "  Paper #219 Replication: Chen, Nimmo, & Glatzmaier (2012, 2014) " << std::endl;
  std::cout << "  Enceladus Libration Shear Strain & Viscoelastic Tidal Heating   " << std::endl;
  std::cout << "=================================================================" << std::endl;

  hot_jupiter::Chen2012EnceladusTidalModel model;

  const double n_rad_s = model.orbital_frequency_rad_s();
  const double P_orb_hours = model.orbital_period_hours();
  const double P_orb_s = model.orbital_period_s();

  std::cout << std::fixed << std::setprecision(6);
  std::cout << "Enceladus Mean Radius:        " << hot_jupiter::Chen2012EnceladusTidalModel::R_ENCELADUS / 1.0e3 << " km" << std::endl;
  std::cout << "Saturn Semi-Major Axis:       " << hot_jupiter::Chen2012EnceladusTidalModel::A_ENCELADUS / 1.0e3 << " km" << std::endl;
  std::cout << "Orbital Eccentricity:         " << hot_jupiter::Chen2012EnceladusTidalModel::ECCENTRICITY << std::endl;
  std::cout << "Orbital Frequency n:          " << n_rad_s << " rad/s" << std::endl;
  std::cout << "Orbital Period:               " << P_orb_hours << " hours (" << P_orb_s << " s)" << std::endl;
  std::cout << "Ice Shear Modulus mu:         " << hot_jupiter::Chen2012EnceladusTidalModel::MU_ICE / 1.0e9 << " GPa" << std::endl;
  std::cout << "Basal Viscosity eta_0:        " << hot_jupiter::Chen2012EnceladusTidalModel::ETA_0_NOM << " Pa s" << std::endl;
  std::cout << "Nominal Libration Amplitude:  " << hot_jupiter::Chen2012EnceladusTidalModel::LIB_AMP_RAD_NOM * (180.0 / M_PI) << " deg ("
            << hot_jupiter::Chen2012EnceladusTidalModel::LIB_AMP_RAD_NOM << " rad)" << std::endl;
  std::cout << std::endl;

  // --------------------------------------------------------------------------
  // 1. Latitudinal Profiles: Shell Thickness, Strain, and Heat Flux
  // --------------------------------------------------------------------------
  std::ofstream csv_lat("replications_ss/paper_219/latitudinal_profile.csv");
  csv_lat << "colatitude_deg,latitude_deg,colatitude_rad,shell_thickness_km,"
          << "shear_strain_r_phi,shear_strain_theta_phi,total_shear_strain,strain_rate_s_inv,"
          << "conductive_flux_mw_m2,libration_flux_mw_m2,eccentricity_flux_mw_m2,total_heat_flux_mw_m2\n";

  for (int i = 0; i <= 180; ++i) {
    double colat_deg = static_cast<double>(i);
    double lat_deg = 90.0 - colat_deg;
    double colat_rad = colat_deg * (M_PI / 180.0);

    double d_m = model.shell_thickness_m(colat_rad);
    double d_km = d_m / 1.0e3;

    double eps_rp = model.libration_shear_strain_r_phi(colat_rad, d_m);
    double eps_tp = model.libration_shear_strain_theta_phi(colat_rad);
    double eps_tot = model.libration_total_strain(colat_rad, d_m);
    double strain_rate = model.libration_strain_rate(colat_rad, d_m);

    double f_cond = model.conductive_heat_flux_mw_m2(d_m);
    double f_lib = model.libration_heat_flux_mw_m2(colat_rad, d_m);
    
    // Background eccentricity tidal flux
    double area = 4.0 * M_PI * std::pow(hot_jupiter::Chen2012EnceladusTidalModel::R_ENCELADUS, 2.0);
    double p_ecc_w = model.eccentricity_tidal_power_gw() * 1.0e9;
    double f_ecc_bg = (p_ecc_w / area) * 1.0e3;
    double f_ecc_local = f_ecc_bg * (hot_jupiter::Chen2012EnceladusTidalModel::NOMINAL_SHELL_KM / d_km);
    double f_tot = f_lib + f_ecc_local;

    csv_lat << std::fixed << std::setprecision(2) << colat_deg << "," << lat_deg << ","
            << std::setprecision(5) << colat_rad << "," << std::setprecision(3) << d_km << ","
            << std::setprecision(6) << eps_rp << "," << eps_tp << "," << eps_tot << ","
            << std::scientific << std::setprecision(4) << strain_rate << ","
            << std::fixed << std::setprecision(3) << f_cond << "," << f_lib << "," << f_ecc_local << "," << f_tot << "\n";
  }
  csv_lat.close();
  std::cout << "✅ Saved replications_ss/paper_219/latitudinal_profile.csv" << std::endl;

  // --------------------------------------------------------------------------
  // 2. Depth Profiles for SPT (5 km) and Equatorial (25 km) Ice Shells
  // --------------------------------------------------------------------------
  std::ofstream csv_depth("replications_ss/paper_219/depth_profiles.csv");
  csv_depth << "depth_fraction,depth_spt_km,temp_spt_k,viscosity_spt_pa_s,tau_m_spt_s,maxwell_d_spt,q_lib_spt_w_m3,"
            << "depth_eq_km,temp_eq_k,viscosity_eq_pa_s,tau_m_eq_s,maxwell_d_eq,q_lib_eq_w_m3\n";

  const double d_spt_m = 5.0 * 1.0e3;
  const double d_eq_m = 25.0 * 1.0e3;
  const double theta_spt = M_PI;          // South pole
  const double theta_eq = 0.5 * M_PI;     // Equator

  for (int i = 0; i <= 100; ++i) {
    double frac = i / 100.0;
    
    // South polar terrain
    double z_spt = frac * d_spt_m;
    double T_spt = model.ice_temperature_at_depth(z_spt, d_spt_m);
    double eta_spt = model.viscosity_at_temperature(T_spt);
    double tau_spt = eta_spt / hot_jupiter::Chen2012EnceladusTidalModel::MU_ICE;
    double d_spt = model.maxwell_dissipation_factor(n_rad_s, tau_spt);
    double q_spt = model.volumetric_libration_heating_w_m3(z_spt, theta_spt, d_spt_m);

    // Equator
    double z_eq = frac * d_eq_m;
    double T_eq = model.ice_temperature_at_depth(z_eq, d_eq_m);
    double eta_eq = model.viscosity_at_temperature(T_eq);
    double tau_eq = eta_eq / hot_jupiter::Chen2012EnceladusTidalModel::MU_ICE;
    double d_eq = model.maxwell_dissipation_factor(n_rad_s, tau_eq);
    double q_eq = model.volumetric_libration_heating_w_m3(z_eq, theta_eq, d_eq_m);

    csv_depth << std::fixed << std::setprecision(4) << frac << ","
              << std::setprecision(3) << (z_spt / 1.0e3) << "," << std::setprecision(2) << T_spt << ","
              << std::scientific << std::setprecision(4) << eta_spt << "," << tau_spt << ","
              << std::fixed << std::setprecision(6) << d_spt << ","
              << std::scientific << std::setprecision(4) << q_spt << ","
              << std::fixed << std::setprecision(3) << (z_eq / 1.0e3) << "," << std::setprecision(2) << T_eq << ","
              << std::scientific << std::setprecision(4) << eta_eq << "," << tau_eq << ","
              << std::fixed << std::setprecision(6) << d_eq << ","
              << std::scientific << std::setprecision(4) << q_eq << "\n";
  }
  csv_depth.close();
  std::cout << "✅ Saved replications_ss/paper_219/depth_profiles.csv" << std::endl;

  // --------------------------------------------------------------------------
  // 3. Shell Thickness & Libration Amplitude Sensitivity Sweep
  // --------------------------------------------------------------------------
  std::ofstream csv_sens("replications_ss/paper_219/libration_sensitivity.csv");
  csv_sens << "d_mean_km,gamma_0_deg,gamma_0_rad,p_lib_decoupled_gw,f_spt_mw_m2,p_total_decoupled_gw,"
           << "p_lib_coupled_gw,p_total_coupled_gw\n";

  for (double d = 5.0; d <= 45.0; d += 2.0) {
    double gamma_decoupled = model.libration_amplitude_rad(d, true);
    double gamma_coupled = model.libration_amplitude_rad(d, false);

    double p_lib_dec = model.global_libration_power_gw(d, gamma_decoupled);
    double p_lib_coup = model.global_libration_power_gw(d, gamma_coupled);

    double d_spt_local = std::max(2.0, d * 0.25);
    double f_spt = model.spt_heat_flux_mw_m2(d_spt_local, gamma_decoupled);

    double p_tot_dec = model.total_dissipation_power_gw(d, gamma_decoupled);
    double p_tot_coup = model.total_dissipation_power_gw(d, gamma_coupled);

    csv_sens << std::fixed << std::setprecision(1) << d << ","
             << std::setprecision(4) << (gamma_decoupled * 180.0 / M_PI) << ","
             << std::setprecision(6) << gamma_decoupled << ","
             << std::setprecision(3) << p_lib_dec << "," << f_spt << "," << p_tot_dec << ","
             << p_lib_coup << "," << p_tot_coup << "\n";
  }
  csv_sens.close();
  std::cout << "✅ Saved replications_ss/paper_219/libration_sensitivity.csv" << std::endl;

  // --------------------------------------------------------------------------
  // 4. Comparison of Tidal Dissipation Mechanisms
  // --------------------------------------------------------------------------
  std::ofstream csv_mech("replications_ss/paper_219/tidal_mechanism_comparison.csv");
  csv_mech << "mechanism,power_gw,power_watts,fraction_percent,polar_concentration\n";

  double p_lib_nom = model.global_libration_power_gw();
  double p_ecc_nom = model.eccentricity_tidal_power_gw();
  double p_obl_nom = model.obliquity_tidal_power_gw(hot_jupiter::Chen2012EnceladusTidalModel::OBLIQUITY_RAD_NOM);
  double p_obl_high = model.obliquity_tidal_power_gw(0.1 * (M_PI / 180.0));
  double p_ocean_nom = model.ocean_bottom_drag_power_gw();
  double p_tot = p_lib_nom + p_ecc_nom + p_obl_nom + p_ocean_nom;

  csv_mech << std::fixed << std::setprecision(4);
  csv_mech << "Libration Shear Strain (Decoupled Shell)," << p_lib_nom << "," << (p_lib_nom * 1e9) << ","
           << (p_lib_nom / p_tot * 100.0) << ",Extreme (SPT focused)\n";
  csv_mech << "Eccentricity Radial Tidal Flexure," << p_ecc_nom << "," << (p_ecc_nom * 1e9) << ","
           << (p_ecc_nom / p_tot * 100.0) << ",Moderate (global quad)\n";
  csv_mech << "Obliquity Tides (Nominal theta=0.001 deg)," << std::scientific << std::setprecision(6)
           << p_obl_nom << "," << (p_obl_nom * 1e9) << "," << (p_obl_nom / p_tot * 100.0) << ",Negligible\n";
  csv_mech << "Obliquity Tides (Exaggerated theta=0.1 deg)," << std::fixed << std::setprecision(4)
           << p_obl_high << "," << (p_obl_high * 1e9) << "," << (p_obl_high / (p_tot - p_obl_nom + p_obl_high) * 100.0) << ",High\n";
  csv_mech << "Ocean Bottom Turbulent Drag," << std::scientific << std::setprecision(6)
           << p_ocean_nom << "," << (p_ocean_nom * 1e9) << "," << (p_ocean_nom / p_tot * 100.0) << ",Uniform\n";
  csv_mech.close();
  std::cout << "✅ Saved replications_ss/paper_219/tidal_mechanism_comparison.csv" << std::endl;

  // --------------------------------------------------------------------------
  // 5. Benchmark Comparison and Statistical Metric (R^2 Evaluation)
  // --------------------------------------------------------------------------
  struct BenchmarkPoint {
    std::string parameter;
    double observed_or_published;
    double model_predicted;
    std::string units;
    std::string citation;
  };

  std::vector<BenchmarkPoint> benchmarks = {
    {"SPT Thermal Emission Power", 15.8, model.total_dissipation_power_gw(), "GW", "Spencer 2006, Howett 2011"},
    {"Peak SPT Heat Flux", 185.0, model.spt_heat_flux_mw_m2(), "mW/m^2", "Howett et al. 2011"},
    {"Equatorial Libration Amplitude", 0.120, model.libration_amplitude_rad() * 180.0 / M_PI, "deg", "Thomas et al. 2016"},
    {"Obliquity Tidal Power (Nominal)", 0.00015, model.obliquity_tidal_power_gw(), "GW", "Chen & Nimmo 2011"},
    {"Ocean Turbulent Drag Dissipation", 0.000023, model.ocean_bottom_drag_power_gw(), "GW", "Chen et al. 2014"},
    {"Nominal Shell Libration Dissipation", 11.2, model.global_libration_power_gw(), "GW", "Chen et al. 2012"},
    {"Forced Orbital Eccentricity Power", 4.3, model.eccentricity_tidal_power_gw(), "GW", "Peale 1979, Tobie 2008"}
  };

  std::ofstream csv_bench("replications_ss/paper_219/model_comparison.csv");
  csv_bench << "parameter,observed_published,model_predicted,units,citation\n";

  double ss_tot = 0.0;
  double ss_res = 0.0;
  double mean_obs = 0.0;
  for (const auto& b : benchmarks) {
    mean_obs += b.observed_or_published;
  }
  mean_obs /= benchmarks.size();

  for (const auto& b : benchmarks) {
    csv_bench << "\"" << b.parameter << "\"," << std::scientific << std::setprecision(5)
              << b.observed_or_published << "," << b.model_predicted << ",\""
              << b.units << "\",\"" << b.citation << "\"\n";

    double d_obs = b.observed_or_published - mean_obs;
    double d_res = b.observed_or_published - b.model_predicted;
    ss_tot += d_obs * d_obs;
    ss_res += d_res * d_res;
  }
  csv_bench.close();
  std::cout << "✅ Saved replications_ss/paper_219/model_comparison.csv" << std::endl;

  double r_squared = 1.0 - (ss_res / std::max(1.0e-30, ss_tot));
  std::cout << "=================================================================" << std::endl;
  std::cout << "  Model Verification Summary:                                    " << std::endl;
  std::cout << "  Total Tidal Dissipation Power:  " << std::fixed << std::setprecision(2) << model.total_dissipation_power_gw() << " GW" << std::endl;
  std::cout << "  South Polar Terrain Heat Flux:  " << model.spt_heat_flux_mw_m2() << " mW/m^2" << std::endl;
  std::cout << "  Global Libration Power:         " << model.global_libration_power_gw() << " GW" << std::endl;
  std::cout << "  Benchmark R^2 Determination:    " << std::setprecision(4) << r_squared << std::endl;
  std::cout << "=================================================================" << std::endl;

  return 0;
}
