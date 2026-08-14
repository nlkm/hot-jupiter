// Solver for Paper #205: Ojakangas & Stevenson (1989)
// "Thermal State of Enceladus' Ice Shell" / Viscoelastic Maxwell Rheology & Tidal Dissipation
// Evaluates temperature-dependent Maxwell viscoelastic rheology, tidal dissipation Love number Im(k2),
// and conductive thermal equilibrium in Enceladus' ice shell.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "=================================================================" << std::endl;
  std::cout << "=== Paper #205: Ojakangas & Stevenson (1989) Enceladus Solver ===" << std::endl;
  std::cout << "=================================================================" << std::endl;

  hot_jupiter::OjakangasStevenson1989EnceladusModel model;

  const double forcing_freq = model.orbital_mean_motion();
  std::cout << "--> Orbital mean motion n / forcing frequency: " << forcing_freq << " rad/s" << std::endl;
  std::cout << "--> Ice shear modulus mu: " << hot_jupiter::OjakangasStevenson1989EnceladusModel::MU_ICE / 1.0e9 << " GPa" << std::endl;
  std::cout << "--> Basal ice viscosity eta_0: " << hot_jupiter::OjakangasStevenson1989EnceladusModel::ETA_0_NOM << " Pa s" << std::endl;
  std::cout << "--> Peak Love number Im(k2): " << hot_jupiter::OjakangasStevenson1989EnceladusModel::K2_PEAK_NOM << std::endl;

  // --------------------------------------------------------------------------
  // 1. Output Im(k2) vs Maxwell Relaxation Frequency omega_M
  // --------------------------------------------------------------------------
  std::ofstream file_imk2("replications_ss/paper_205/ojakangas1989_imk2_vs_omegam.csv");
  file_imk2 << "log10_omega_M,omega_M_rad_s,viscosity_pa_s,im_k2_model,im_k2_asymptotic_low,im_k2_asymptotic_high\n";

  for (double log_omega = -10.0; log_omega <= 0.0; log_omega += 0.05) {
    double omega_M = std::pow(10.0, log_omega);
    double eta = hot_jupiter::OjakangasStevenson1989EnceladusModel::MU_ICE / omega_M;
    double im_k2 = model.dissipation_love_number_im_k2(omega_M, 0.0107, forcing_freq);

    // Asymptotes:
    // Low omega_M (viscous regime / high eta): Im(k2) ~ 2 * k2_peak * (omega_M / omega_forcing)
    double asymp_low = 2.0 * 0.0107 * (omega_M / forcing_freq);
    // High omega_M (elastic regime / low eta): Im(k2) ~ 2 * k2_peak * (omega_forcing / omega_M)
    double asymp_high = 2.0 * 0.0107 * (forcing_freq / omega_M);

    file_imk2 << std::fixed << std::setprecision(6)
              << log_omega << "," << omega_M << "," << eta << ","
              << im_k2 << "," << asymp_low << "," << asymp_high << "\n";
  }
  file_imk2.close();
  std::cout << "✅ Generated replications_ss/paper_205/ojakangas1989_imk2_vs_omegam.csv" << std::endl;

  // --------------------------------------------------------------------------
  // 2. Output Tidal Dissipation Power vs Viscosity for Different Eccentricities
  // --------------------------------------------------------------------------
  std::ofstream file_power("replications_ss/paper_205/ojakangas1989_power_vs_viscosity.csv");
  file_power << "log10_viscosity,viscosity_pa_s,power_gw_e_nom,power_gw_e_low,power_gw_e_high\n";

  for (double log_eta = 10.0; log_eta <= 18.0; log_eta += 0.05) {
    double eta = std::pow(10.0, log_eta);
    double p_nom = model.tidal_power_from_viscosity_gw(eta, 0.0047);
    double p_low = model.tidal_power_from_viscosity_gw(eta, 0.0025);
    double p_high = model.tidal_power_from_viscosity_gw(eta, 0.0075);

    file_power << std::fixed << std::setprecision(6)
               << log_eta << "," << eta << ","
               << p_nom << "," << p_low << "," << p_high << "\n";
  }
  file_power.close();
  std::cout << "✅ Generated replications_ss/paper_205/ojakangas1989_power_vs_viscosity.csv" << std::endl;

  // --------------------------------------------------------------------------
  // 3. Output Ice Shell Thermal Profile & Volumetric Heating vs Depth
  // --------------------------------------------------------------------------
  std::ofstream file_shell("replications_ss/paper_205/ojakangas1989_shell_profile.csv");
  file_shell << "depth_km,norm_depth,temp_k_20km,viscosity_pa_s_20km,heating_w_m3_20km,temp_k_5km,viscosity_pa_s_5km,heating_w_m3_5km\n";

  const double d_global = 20.0;  // Global ice shell thickness [km]
  const double d_south = 5.0;    // South polar terrain ice shell thickness [km]

  for (double frac = 0.0; frac <= 1.0; frac += 0.01) {
    double z_20 = frac * d_global;
    double T_20 = model.ice_shell_temperature_k(z_20, d_global);
    double eta_20 = model.viscosity_at_temperature_pa_s(T_20);
    double q_20 = model.volumetric_tidal_heating_w_m3(z_20, d_global);

    double z_5 = frac * d_south;
    double T_5 = model.ice_shell_temperature_k(z_5, d_south);
    double eta_5 = model.viscosity_at_temperature_pa_s(T_5);
    double q_5 = model.volumetric_tidal_heating_w_m3(z_5, d_south);

    file_shell << std::fixed << std::setprecision(6)
               << z_20 << "," << frac << ","
               << T_20 << "," << eta_20 << "," << q_20 << ","
               << T_5 << "," << eta_5 << "," << q_5 << "\n";
  }
  file_shell.close();
  std::cout << "✅ Generated replications_ss/paper_205/ojakangas1989_shell_profile.csv" << std::endl;

  // --------------------------------------------------------------------------
  // 4. Output Conductive Heat Flux vs Shell Thickness & Equilibrium Balance
  // --------------------------------------------------------------------------
  std::ofstream file_cond("replications_ss/paper_205/ojakangas1989_conduction_balance.csv");
  file_cond << "thickness_km,q_cond_gw,q_cond_mw_m2,p_tide_gw,net_budget_gw\n";

  double p_tide_peak = model.tidal_dissipation_power_gw(0.0107, 0.0047);
  for (double d = 2.0; d <= 50.0; d += 0.5) {
    double q_cond_gw = model.conductive_heat_loss_gw(d);
    double q_cond_mw = model.conductive_heat_flux_mw_m2(d);
    double net = p_tide_peak + 0.4 - q_cond_gw;  // P_tide + P_radio - Q_cond

    file_cond << std::fixed << std::setprecision(6)
              << d << "," << q_cond_gw << "," << q_cond_mw << ","
              << p_tide_peak << "," << net << "\n";
  }
  file_cond.close();
  std::cout << "✅ Generated replications_ss/paper_205/ojakangas1989_conduction_balance.csv" << std::endl;

  // --------------------------------------------------------------------------
  // Summary Metrics & Statistical Verification
  // --------------------------------------------------------------------------
  double d_eq = model.equilibrium_shell_thickness_km(0.0107, 0.0047, 0.4);
  double p_tide_nominal = model.tidal_dissipation_power_gw(0.0107);
  double q_cond_20km = model.conductive_heat_loss_gw(20.0);
  double q_cond_5km_spt = model.conductive_heat_loss_gw(5.0) * 0.10;  // 10% area for SPT

  std::cout << "\n=== Summary Results ===" << std::endl;
  std::cout << "Peak Tidal Dissipation Power: " << p_tide_nominal << " GW" << std::endl;
  std::cout << "Conductive Loss (20 km shell): " << q_cond_20km << " GW" << std::endl;
  std::cout << "South Polar Conductive Loss (5 km shell, 10% area): " << q_cond_5km_spt << " GW" << std::endl;
  std::cout << "Equilibrium Shell Thickness: " << d_eq << " km" << std::endl;
  std::cout << "=================================================================\n" << std::endl;

  return 0;
}
