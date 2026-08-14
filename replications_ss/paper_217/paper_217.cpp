// Copyright 2026 Antigravity Scientific Automation & Solar System Dynamics Replication Campaign
// Replication of Paper #217: Showman et al. (2006) "Atmosphere-Ocean Dynamics of Titan"
// First-principles modeling of Titan's radiative-convective equilibrium, anti-greenhouse haze radiative transfer,
// global Hadley circulation, stratospheric zonal superrotation, and methane hydrologic cycle energetics.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "======================================================================\n";
  std::cout << " Paper #217: Showman et al. (2006) Atmosphere-Ocean Dynamics of Titan\n";
  std::cout << " Radiative-Convective Equilibrium & Methane Hydrologic Energetics\n";
  std::cout << "======================================================================\n\n";

  hot_jupiter::TitanAtmosphereHydrologyModel model;

  // Basic Planetary & Radiative Properties
  double f_toa = model.solar_flux_toa();
  double f_abs = model.absorbed_solar_flux();
  double t_eff = model.effective_temperature();
  double f_surf_sol = model.surface_solar_flux();
  double t_s_rce = model.rce_surface_temperature();
  double dt_gh = model.greenhouse_warming_k();
  double dt_anti_gh = model.antigreenhouse_cooling_k();
  double h_scale = model.scale_height_m() / 1.0e3; // km
  double gamma_dry = model.dry_adiabatic_lapse_rate_k_m() * 1.0e3; // K/km

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Solar Insolation at Titan TOA (F_TOA)    : " << f_toa << " W/m^2\n";
  std::cout << "Absorbed Solar Flux (F_abs)              : " << f_abs << " W/m^2\n";
  std::cout << "Effective Radiating Temperature (T_eff)  : " << t_eff << " K\n";
  std::cout << "Solar Flux Reaching Surface (F_surf)     : " << f_surf_sol << " W/m^2\n";
  std::cout << "RCE Equilibrium Surface Temp (T_surf)    : " << t_s_rce << " K\n";
  std::cout << "Longwave Greenhouse Warming (Delta T_GH) : +" << dt_gh << " K\n";
  std::cout << "Stratospheric Anti-GH Cooling (DT_anti)  : -" << dt_anti_gh << " K\n";
  std::cout << "Atmospheric Scale Height (H)             : " << h_scale << " km\n";
  std::cout << "Dry Adiabatic Lapse Rate (Gamma_d)       : " << gamma_dry << " K/km\n\n";

  // 1. Vertical Atmospheric Structure (RCE Profile, Pressure, Lapse Rate, Methane Humidity, Wind)
  std::ofstream rce_file("replications_ss/paper_217/titan_rce_profile.csv");
  if (!rce_file.is_open()) {
    rce_file.open("titan_rce_profile.csv");
  }
  rce_file << "z_km,p_pa,p_bar,T_k,gamma_moist_k_km,p_sat_ch4_pa,q_sat_ch4,u_zonal_m_s,tau_rad_yr\n";

  std::cout << "--- 1. Atmospheric Vertical Structure (Surface to Stratopause) ---\n";
  std::cout << " z (km) |  P (bar)  |  T (K)  | Gamma_m (K/km) | p_sat (Pa) | q_sat (g/kg) | u_zonal (m/s) | tau_rad (yr)\n";
  std::cout << "------------------------------------------------------------------------------------------------------\n";

  for (double z = 0.0; z <= 320.0; z += 2.0) {
    double p_pa = model.pressure_at_altitude_pa(z);
    double p_bar = p_pa / 1.0e5;
    double t_k = model.temperature_at_altitude_k(z);
    double gamma_m = model.moist_adiabatic_lapse_rate_k_m(t_k, p_pa) * 1.0e3;
    double p_sat = model.methane_sat_vapor_pressure_pa(t_k);
    double q_sat = model.methane_sat_specific_humidity(t_k, p_pa);
    double u_z = model.zonal_superrotation_wind_speed_m_s(z, 30.0);
    double tau_rad = model.radiative_relaxation_timescale_yr(p_pa, t_k);

    rce_file << std::fixed << std::setprecision(2) << z << ","
             << std::scientific << std::setprecision(4) << p_pa << ","
             << std::fixed << std::setprecision(4) << p_bar << ","
             << std::setprecision(2) << t_k << ","
             << std::setprecision(3) << gamma_m << ","
             << std::scientific << std::setprecision(3) << p_sat << ","
             << std::setprecision(4) << q_sat << ","
             << std::fixed << std::setprecision(2) << u_z << ","
             << std::scientific << std::setprecision(3) << tau_rad << "\n";

    if (std::abs(std::round(z / 40.0) * 40.0 - z) < 1e-4) {
      std::cout << "  " << std::fixed << std::setprecision(1) << std::setw(5) << z << " | "
                << std::setprecision(3) << std::setw(9) << p_bar << " | "
                << std::setprecision(1) << std::setw(7) << t_k << " | "
                << std::setprecision(2) << std::setw(14) << gamma_m << " | "
                << std::scientific << std::setprecision(2) << std::setw(10) << p_sat << " | "
                << std::fixed << std::setprecision(2) << std::setw(12) << (q_sat * 1000.0) << " | "
                << std::setprecision(1) << std::setw(13) << u_z << " | "
                << std::scientific << std::setprecision(2) << std::setw(11) << tau_rad << "\n";
    }
  }
  rce_file.close();
  std::cout << "\n✅ Generated replications_ss/paper_217/titan_rce_profile.csv\n\n";

  // 2. Optical Depth Sweep: Greenhouse (tau_lw) vs Anti-Greenhouse (tau_sw)
  std::ofstream gh_file("replications_ss/paper_217/titan_haze_greenhouse_sweep.csv");
  if (!gh_file.is_open()) {
    gh_file.open("titan_haze_greenhouse_sweep.csv");
  }
  gh_file << "tau_lw,tau_sw,T_surf_k,delta_T_gh_k,delta_T_antigh_k,f_surf_solar_w_m2\n";

  std::cout << "--- 2. Greenhouse vs Anti-Greenhouse Haze Optical Depth Sensitivity ---\n";
  std::cout << " tau_lw | tau_sw | T_surf (K) | Delta T_GH (K) | Delta T_antiGH (K) | F_surf_solar (W/m^2)\n";
  std::cout << "-----------------------------------------------------------------------------------\n";

  for (double tau_lw = 0.5; tau_lw <= 5.0; tau_lw += 0.25) {
    for (double tau_sw = 0.0; tau_sw <= 4.0; tau_sw += 0.25) {
      double t_s = model.rce_surface_temperature(tau_lw, tau_sw);
      double dt_g = model.greenhouse_warming_k(tau_lw);
      double dt_ag = model.antigreenhouse_cooling_k(tau_sw);
      double f_surf = model.surface_solar_flux(tau_sw);

      gh_file << std::fixed << std::setprecision(2) << tau_lw << ","
              << tau_sw << ","
              << std::setprecision(2) << t_s << ","
              << dt_g << ","
              << dt_ag << ","
              << std::setprecision(4) << f_surf << "\n";

      if ((std::abs(tau_lw - 2.5) < 1e-4) && (std::abs(std::round(tau_sw) - tau_sw) < 1e-4)) {
        std::cout << "  " << std::setprecision(2) << tau_lw << "  |  "
                  << tau_sw << "  |   "
                  << t_s << "   |     +"
                  << dt_g << "    |       -"
                  << dt_ag << "      |       "
                  << f_surf << "\n";
      }
    }
  }
  gh_file.close();
  std::cout << "\n✅ Generated replications_ss/paper_217/titan_haze_greenhouse_sweep.csv\n\n";

  // 3. Methane Hydrologic Cycle Energetics & Convective Storm Parameters
  std::ofstream hydro_file("replications_ss/paper_217/titan_methane_hydrology.csv");
  if (!hydro_file.is_open()) {
    hydro_file.open("titan_methane_hydrology.csv");
  }
  hydro_file << "RH_surf,F_latent_w_m2,evap_rate_cm_yr,W_ch4_kg_m2,depth_liq_cm,tau_turnover_days,cape_j_kg,w_updraft_m_s,storm_rain_mm_day\n";

  std::cout << "--- 3. Methane Hydrologic Energetics & Convective Plume Dynamics ---\n";
  std::cout << " RH_surf | F_lat (W/m^2) | Evap (cm/yr) | W_CH4 (kg/m^2) | tau_res (days) | CAPE (J/kg) | w_max (m/s) | Rain (mm/day)\n";
  std::cout << "------------------------------------------------------------------------------------------------------------\n";

  for (double rh = 0.20; rh <= 0.95; rh += 0.05) {
    double f_lat = 0.15 * (rh / 0.50); // scales with surface relative humidity
    double evap = model.global_evaporation_rate_cm_yr(f_lat);
    double w_ch4 = model.precipitable_methane_column_kg_m2(rh);
    double d_liq = model.precipitable_methane_depth_cm(rh);
    double tau_res = model.methane_hydrologic_turnover_days(rh, f_lat);
    double cape = model.convective_cape_j_kg(rh, 2.5);
    double w_up = model.max_convective_updraft_m_s(cape);
    double rain = model.storm_precipitation_rate_mm_day(cape);

    hydro_file << std::fixed << std::setprecision(2) << rh << ","
               << std::setprecision(4) << f_lat << ","
               << std::setprecision(3) << evap << ","
               << std::setprecision(3) << w_ch4 << ","
               << std::setprecision(3) << d_liq << ","
               << std::setprecision(1) << tau_res << ","
               << std::setprecision(1) << cape << ","
               << std::setprecision(2) << w_up << ","
               << std::setprecision(1) << rain << "\n";

    if (std::abs(std::round(rh * 10.0) - rh * 10.0) < 1e-4 && (rh >= 0.3 && rh <= 0.9)) {
      std::cout << "  " << std::fixed << std::setprecision(2) << rh << "   |     "
                << std::setprecision(3) << f_lat << "     |    "
                << evap << "     |     "
                << w_ch4 << "      |     "
                << std::setprecision(1) << std::setw(6) << tau_res << "     |    "
                << std::setw(6) << cape << "   |    "
                << std::setprecision(1) << std::setw(5) << w_up << "    |    "
                << std::setw(6) << rain << "\n";
    }
  }
  hydro_file.close();
  std::cout << "\n✅ Generated replications_ss/paper_217/titan_methane_hydrology.csv\n\n";

  // 4. Atmospheric Circulation Regimes & Zonal Superrotation
  std::ofstream circ_file("replications_ss/paper_217/titan_circulation_superrotation.csv");
  if (!circ_file.is_open()) {
    circ_file.open("titan_circulation_superrotation.csv");
  }
  circ_file << "delta_T_pole_eq,Ro_T,L_R_km,theta_H_deg,hadley_regime,superrotation_index,north_lake_frac\n";

  std::cout << "--- 4. Atmospheric Circulation Regimes & Polar Lake Asymmetry ---\n";
  std::cout << " DT_pole_eq (K) |   Ro_T   |  L_R (km)  | theta_H (deg) | Superrot Index | North Lake Frac\n";
  std::cout << "-----------------------------------------------------------------------------------------\n";

  for (double dt_pe = 0.5; dt_pe <= 10.0; dt_pe += 0.5) {
    double ro_t = model.thermal_rossby_number(dt_pe);
    double l_r = model.equatorial_rossby_radius_km();
    double th_h = model.hadley_cell_boundary_lat_deg(dt_pe);
    double s_idx = model.superrotation_index();
    double n_lake = model.northern_lake_fraction();

    circ_file << std::fixed << std::setprecision(2) << dt_pe << ","
              << std::setprecision(3) << ro_t << ","
              << std::setprecision(1) << l_r << ","
              << std::setprecision(1) << th_h << ","
              << (th_h >= 89.9 ? "Global_Hadley" : "Subtropical_Hadley") << ","
              << std::setprecision(2) << s_idx << ","
              << std::setprecision(3) << n_lake << "\n";

    if (std::abs(std::round(dt_pe) - dt_pe) < 1e-4 && dt_pe <= 6.0) {
      std::cout << "     " << std::fixed << std::setprecision(1) << dt_pe << "       |  "
                << std::setprecision(3) << std::setw(6) << ro_t << "  |   "
                << std::setprecision(0) << std::setw(5) << l_r << "    |     "
                << std::setprecision(1) << std::setw(5) << th_h << "     |      "
                << std::setprecision(1) << std::setw(5) << s_idx << "     |      "
                << std::setprecision(3) << n_lake << "\n";
    }
  }
  circ_file.close();
  std::cout << "\n✅ Generated replications_ss/paper_217/titan_circulation_superrotation.csv\n\n";

  std::cout << "======================================================================\n";
  std::cout << " Paper #217 Replication C++ Engine Run Successfully Completed!\n";
  std::cout << "======================================================================\n";

  return 0;
}
