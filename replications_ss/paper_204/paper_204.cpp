// Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
// First-principles replication of Squyres et al. (1983), Cassen et al. (1979, 1980), Reynolds et al. (1983)
// "Tidal Dissipation and Ice Shell Dynamics of Europa"
// Calculates ice shell thermal conduction, volumetric tidal heating, Clapeyron basal melting depression,
// equilibrium shell thickness vs internal heat flux, and diurnal tidal flexing stresses.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << " Paper #204: Squyres et al. (1983) - Europa Ice Shell Dynamics & Tidal Heating  " << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::EuropaIceShellDynamicsModel model;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Jupiter Mass:                 " << hot_jupiter::EuropaIceShellDynamicsModel::M_JUPITER << " kg" << std::endl;
  std::cout << "Europa Mass:                  " << hot_jupiter::EuropaIceShellDynamicsModel::M_EUROPA << " kg" << std::endl;
  std::cout << "Europa Radius:                " << hot_jupiter::EuropaIceShellDynamicsModel::R_EUROPA / 1000.0 << " km" << std::endl;
  std::cout << "Semi-Major Axis:              " << hot_jupiter::EuropaIceShellDynamicsModel::A_EUROPA / 1000.0 << " km" << std::endl;
  std::cout << "Orbital Period:               " << model.orbital_period_days() << " days" << std::endl;
  std::cout << "Surface Mean Temperature:     " << hot_jupiter::EuropaIceShellDynamicsModel::T_SURF << " K" << std::endl;

  double nominal_e = hot_jupiter::EuropaIceShellDynamicsModel::E_EUROPA_NOM;
  double nominal_k2_q = 0.015;
  double nominal_p_radio_gw = hot_jupiter::EuropaIceShellDynamicsModel::P_RADIO_NOM_GW;

  double p_tide_gw = model.tidal_heating_power_gw(nominal_e, nominal_k2_q);
  double f_tide_mw_m2 = model.tidal_heat_flux_mw_m2(nominal_e, nominal_k2_q);
  double f_radio_mw_m2 = model.radiogenic_heat_flux_mw_m2(nominal_p_radio_gw);
  double f_total_mw_m2 = f_tide_mw_m2 + f_radio_mw_m2;

  std::cout << "\n--- Nominal Tidal & Radiogenic Heating ---" << std::endl;
  std::cout << "Forced Eccentricity e:        " << nominal_e << std::endl;
  std::cout << "Tidal Dissipation (k2/Q):     " << nominal_k2_q << std::endl;
  std::cout << "Tidal Heating Power:          " << p_tide_gw << " GW (" << p_tide_gw / 1000.0 << " TW)" << std::endl;
  std::cout << "Tidal Heat Flux:              " << f_tide_mw_m2 << " mW/m^2" << std::endl;
  std::cout << "Radiogenic Heat Flux:         " << f_radio_mw_m2 << " mW/m^2" << std::endl;
  std::cout << "Total Internal Heat Flux:     " << f_total_mw_m2 << " mW/m^2" << std::endl;

  double h_eq_km = model.equilibrium_shell_thickness_km(nominal_e, nominal_k2_q, nominal_p_radio_gw);
  double p_base_mpa = model.basal_pressure_pa(h_eq_km) / 1.0e6;
  double t_base_k = model.basal_melting_temperature_k(h_eq_km);
  double sigma_diurnal_kpa = model.peak_diurnal_tidal_stress_kpa(h_eq_km, nominal_e);

  std::cout << "\n--- Equilibrium Ice Shell State ---" << std::endl;
  std::cout << "Equilibrium Thickness H_eq:   " << h_eq_km << " km" << std::endl;
  std::cout << "Basal Pressure:               " << p_base_mpa << " MPa" << std::endl;
  std::cout << "Basal Melting Temperature:   " << t_base_k << " K" << std::endl;
  std::cout << "Peak Diurnal Tidal Stress:    " << sigma_diurnal_kpa << " kPa (Tensile strength ~ 40 kPa)" << std::endl;

  // 1. Export Temperature Profile Data across 20 km ice shell
  std::ofstream out_temp("replications_ss/paper_204/temperature_profiles.csv");
  out_temp << "depth_km,T_log_K,T_linear_K,T_volumetric_low_K,T_volumetric_high_K,viscosity_Pa_s\n";

  double nominal_H_km = 20.0;
  int num_depth_steps = 100;
  for (int i = 0; i <= num_depth_steps; ++i) {
    double z_km = (nominal_H_km * i) / num_depth_steps;
    double t_log = model.temperature_at_depth_k(z_km, nominal_H_km);
    double t_base = model.basal_melting_temperature_k(nominal_H_km);
    double t_lin = hot_jupiter::EuropaIceShellDynamicsModel::T_SURF +
                   (t_base - hot_jupiter::EuropaIceShellDynamicsModel::T_SURF) * (z_km / nominal_H_km);
    double t_vol_low = model.temperature_with_volumetric_heating_k(z_km, nominal_H_km, 1.0e-5);
    double t_vol_high = model.temperature_with_volumetric_heating_k(z_km, nominal_H_km, 2.5e-5);
    double eta = model.ice_viscosity_pa_s(t_log);

    out_temp << std::fixed << std::setprecision(3)
             << z_km << "," << t_log << "," << t_lin << ","
             << t_vol_low << "," << t_vol_high << ","
             << std::scientific << std::setprecision(4) << eta << "\n";
  }
  out_temp.close();
  std::cout << "\n✅ Wrote temperature_profiles.csv" << std::endl;

  // 2. Export Equilibrium Shell Thickness vs Internal Heat Flux
  std::ofstream out_flux("replications_ss/paper_204/shell_thickness_vs_flux.csv");
  out_flux << "heat_flux_mW_m2,H_eq_km,P_total_GW,T_base_K,sigma_diurnal_kPa\n";

  for (double flux = 5.0; flux <= 100.0; flux += 1.0) {
    double h_eq = model.equilibrium_shell_thickness_from_flux_km(flux);
    double p_tot_gw = (flux * 1.0e-3 * model.surface_area_m2()) / 1.0e9;
    double t_b = model.basal_melting_temperature_k(h_eq);
    double sig_tide = model.peak_diurnal_tidal_stress_kpa(h_eq, nominal_e);

    out_flux << std::fixed << std::setprecision(2)
             << flux << "," << h_eq << "," << p_tot_gw << ","
             << t_b << "," << sig_tide << "\n";
  }
  out_flux.close();
  std::cout << "✅ Wrote shell_thickness_vs_flux.csv" << std::endl;

  return 0;
}
