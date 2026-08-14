// Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
// Solver for Paper #206: Tidal Dissipation in Europa's Ice Shell (Ross & Schubert 1987)
// Ross & Schubert (1987), Nature 325, 133-134; Ross & Schubert (1986, 1989); Tobie et al. (2003)
//
// Evaluates first-principles viscoelastic tidal strain tensor, Maxwell-Andrade rheology,
// depth-dependent volumetric dissipation rate q_tide(r), total tidal power, surface heat flux,
// and conductive/convective equilibrium ice shell thickness.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

struct EuropaModelParameters {
  double M_Jupiter = 1.89813e27;      // Jupiter mass [kg]
  double M_Europa = 4.7998e22;        // Europa mass [kg]
  double R_Europa = 1.5608e6;         // Europa mean radius [m] (1560.8 km)
  double a_Europa = 6.7090e8;         // Semi-major axis [m] (670,900 km)
  double eccentricity = 0.0090;       // Forced orbital eccentricity
  double g_surf = 1.315;              // Surface gravity [m/s^2]
  double rho_ice = 920.0;             // Ice Ih density [kg/m^3]
  double mu_ice = 3.5e9;              // Unrelaxed ice shear modulus [Pa] (3.5 GPa)
  double T_surf = 100.0;              // Surface mean temperature [K]
  double T_melt = 273.15;             // Basal melting temperature [K]
  double E_act = 59400.0;             // Activation energy [J/mol] (59.4 kJ/mol)
  double eta_0 = 1.0e14;              // Reference basal viscosity at melting point [Pa s]
  double d_0_mm = 1.0;                // Reference grain size [mm]
  double k_conduct = 567.0;           // Thermal conductivity constant [W/m]
  double shell_thickness_m = 20000.0; // Nominal shell thickness [m] (20 km)
};

int main() {
  std::cout << "========================================================================\n";
  std::cout << "Paper #206 Solver: Tidal Dissipation in Europa's Ice Shell\n";
  std::cout << "Ross & Schubert (1987) | Nature 325, 133-134\n";
  std::cout << "========================================================================\n\n";

  EuropaModelParameters param;
  hot_jupiter::EuropaViscoelasticTidalModel model;

  double n_mean = model.orbital_frequency_rad_s();
  double period_days = model.orbital_period_days();
  double nominal_power_tw = model.total_tidal_power_tw(param.shell_thickness_m, 1.0, true);
  double nominal_flux_mw_m2 = model.surface_heat_flux_mw_m2(param.shell_thickness_m, 1.0, true);
  double nominal_im_k2 = model.effective_k2_over_q(param.shell_thickness_m, 1.0, true);
  double d_eq_km = model.conductive_equilibrium_thickness_km(1.0, true);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Europa Physical & Orbital Characteristics:\n";
  std::cout << "  Mean Radius R_E            : " << param.R_Europa / 1000.0 << " km\n";
  std::cout << "  Semi-Major Axis a_E        : " << param.a_Europa / 1000.0 << " km\n";
  std::cout << "  Mean Motion Frequency n    : " << std::scientific << n_mean << " rad/s\n" << std::fixed;
  std::cout << "  Orbital Period P           : " << period_days << " days (Laplace 1:2:4 resonance)\n";
  std::cout << "  Forced Eccentricity e      : " << param.eccentricity << "\n";
  std::cout << "  Ice Shear Modulus mu_0     : " << param.mu_ice / 1.0e9 << " GPa\n";
  std::cout << "  Activation Energy E*       : " << param.E_act / 1000.0 << " kJ/mol\n\n";

  std::cout << "Tidal Dissipation & Thermal Balance Outputs (Nominal 20 km Shell, 1 mm Grain Size):\n";
  std::cout << "  Total Tidal Power P_tide   : " << nominal_power_tw << " TW (" << nominal_power_tw * 1e12 << " W)\n";
  std::cout << "  Surface Heat Flux F_surf   : " << nominal_flux_mw_m2 << " mW/m^2\n";
  std::cout << "  Effective Im(k2) = k2/Q    : " << std::setprecision(6) << nominal_im_k2 << "\n" << std::fixed << std::setprecision(4);
  std::cout << "  Equilibrium Shell Thickness: " << d_eq_km << " km\n\n";

  // Determine output directory
  const char* ws_dir = std::getenv("BUILD_WORKSPACE_DIRECTORY");
  std::string base_dir = (ws_dir != nullptr) ? std::string(ws_dir) + "/replications_ss/paper_206/" : "replications_ss/paper_206/";

  // 1. Export CSV: Volumetric Tidal Heating Profile vs Depth
  std::string csv_vol_path = base_dir + "europa_volumetric_heating.csv";
  std::ofstream csv_vol(csv_vol_path);
  if (!csv_vol.is_open()) {
    // Fallback to local
    csv_vol_path = "europa_volumetric_heating.csv";
    csv_vol.open(csv_vol_path);
  }

  csv_vol << "depth_km,radius_km,temp_k_cond,temp_k_conv,viscosity_pa_s,maxwell_time_s,phi_dissipation,q_tide_cond_w_m3,q_tide_conv_w_m3\n";

  std::vector<double> sim_q_profile;
  std::vector<double> expected_q_profile;

  for (double z_km = 0.0; z_km <= 20.0001; z_km += 0.20) {
    double r_m = param.R_Europa - z_km * 1000.0;
    double rad_km = r_m / 1000.0;
    double t_cond = model.temperature_at_radius_k(r_m, 20000.0, false);
    double t_conv = model.temperature_at_radius_k(r_m, 20000.0, true);
    double eta = model.ice_viscosity_pa_s(t_conv, 1.0);
    double tau_m = model.maxwell_relaxation_time_s(eta);
    double phi = model.viscoelastic_dissipation_function(eta);
    double q_cond = model.volumetric_heating_rate_w_m3(r_m, 20000.0, 1.0, false);
    double q_conv = model.volumetric_heating_rate_w_m3(r_m, 20000.0, 1.0, true);

    csv_vol << std::fixed << std::setprecision(2) << z_km << ","
            << std::setprecision(2) << rad_km << ","
            << std::setprecision(2) << t_cond << ","
            << std::setprecision(2) << t_conv << ","
            << std::scientific << std::setprecision(6)
            << eta << "," << tau_m << ","
            << std::fixed << std::setprecision(6) << phi << ","
            << std::scientific << std::setprecision(6)
            << q_cond << "," << q_conv << "\n";

    // Track for validation metrics
    sim_q_profile.push_back(q_conv);
    // Theoretical first-principles formulation from Ross & Schubert (1987)
    double T_exp = (z_km <= 7.0)
        ? param.T_surf * std::pow(260.0 / param.T_surf, z_km / 7.0)
        : 260.0 + (param.T_melt - 260.0) * (z_km - 7.0) / 13.0;
    double eta_exp = param.eta_0 * std::exp((param.E_act / 8.314462) * (1.0 / T_exp - 1.0 / param.T_melt));
    double tau_exp = eta_exp / param.mu_ice;
    double phi_exp = (n_mean * tau_exp) / (1.0 + std::pow(n_mean * tau_exp, 2.0));
    double eps_exp = 0.97e-5 * (r_m / param.R_Europa);
    double expected_q = 2.0 * param.mu_ice * n_mean * (eps_exp * eps_exp) * phi_exp;
    expected_q_profile.push_back(expected_q);
  }
  csv_vol.close();
  std::cout << " Saved " << csv_vol_path << "\n";

  // 2. Export CSV: Total Tidal Power & Equilibrium Shell Thickness vs Grain Size
  std::string csv_grain_path = base_dir + "europa_power_vs_grain_size.csv";
  std::ofstream csv_grain(csv_grain_path);
  if (!csv_grain.is_open()) {
    csv_grain_path = "europa_power_vs_grain_size.csv";
    csv_grain.open(csv_grain_path);
  }
  csv_grain << "grain_size_mm,basal_viscosity_pa_s,power_tw_conv,power_tw_cond,flux_mw_m2_conv,im_k2,d_eq_km\n";

  for (double d_mm = 0.10; d_mm <= 10.0001; d_mm += 0.10) {
    double eta_b = model.ice_viscosity_pa_s(param.T_melt, d_mm);
    double p_conv = model.total_tidal_power_tw(20000.0, d_mm, true);
    double p_cond = model.total_tidal_power_tw(20000.0, d_mm, false);
    double flux_conv = model.surface_heat_flux_mw_m2(20000.0, d_mm, true);
    double im_k2 = model.effective_k2_over_q(20000.0, d_mm, true);
    double d_eq = model.conductive_equilibrium_thickness_km(d_mm, true);

    csv_grain << std::fixed << std::setprecision(2) << d_mm << ","
              << std::scientific << std::setprecision(4) << eta_b << ","
              << std::fixed << std::setprecision(4)
              << p_conv << "," << p_cond << ","
              << flux_conv << "," << std::setprecision(6) << im_k2 << ","
              << std::setprecision(2) << d_eq << "\n";
  }
  csv_grain.close();
  std::cout << " Saved " << csv_grain_path << "\n";

  // 3. Export CSV: Tidal Dissipation vs Shell Thickness
  std::string csv_thick_path = base_dir + "europa_power_vs_thickness.csv";
  std::ofstream csv_thick(csv_thick_path);
  if (!csv_thick.is_open()) {
    csv_thick_path = "europa_power_vs_thickness.csv";
    csv_thick.open(csv_thick_path);
  }
  csv_thick << "shell_thickness_km,power_tw_conv,power_tw_cond,cond_loss_tw,flux_mw_m2,im_k2\n";


  for (double h_km = 5.0; h_km <= 50.0001; h_km += 1.0) {
    double h_m = h_km * 1000.0;
    double p_conv = model.total_tidal_power_tw(h_m, 1.0, true);
    double p_cond = model.total_tidal_power_tw(h_m, 1.0, false);
    double area = 4.0 * hot_jupiter::PI * param.R_Europa * param.R_Europa;
    double q_loss_tw = (param.k_conduct * std::log(param.T_melt / param.T_surf) / h_m * area) / 1e12;
    double flux = model.surface_heat_flux_mw_m2(h_m, 1.0, true);
    double im_k2 = model.effective_k2_over_q(h_m, 1.0, true);

    csv_thick << std::fixed << std::setprecision(1) << h_km << ","
              << std::setprecision(4) << p_conv << "," << p_cond << ","
              << q_loss_tw << "," << std::setprecision(2) << flux << ","
              << std::setprecision(6) << im_k2 << "\n";
  }
  csv_thick.close();
  std::cout << " Saved " << csv_thick_path << "\n";

  // Compute R^2 validation metric
  double mean_expected = std::accumulate(expected_q_profile.begin(), expected_q_profile.end(), 0.0) / expected_q_profile.size();
  double ss_tot = 0.0;
  double ss_res = 0.0;
  for (size_t i = 0; i < sim_q_profile.size(); ++i) {
    ss_tot += std::pow(expected_q_profile[i] - mean_expected, 2.0);
    ss_res += std::pow(expected_q_profile[i] - sim_q_profile[i], 2.0);
  }
  double r_squared = 1.0 - (ss_res / ss_tot);

  std::cout << "\nReplication Agreement & Validation Metrics:\n";
  std::cout << "  Coefficient of Determination R^2 : " << std::fixed << std::setprecision(6) << r_squared << "\n";
  std::cout << "  Minimum Target Threshold       : 0.980000\n";
  if (r_squared >= 0.98) {
    std::cout << "  Status                         :  VERIFIED & PASSING (R^2 >= 0.98)\n";
  } else {
    std::cout << "  Status                         : ❌ FAILING\n";
  }
  std::cout << "========================================================================\n";

  return 0;
}
