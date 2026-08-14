// Copyright 2026 Antigravity Solar System Dynamics Campaign
// Paper #203 Replication: Greenberg et al. (1980) "Tidal Dissipation in Enceladus"
// First-principles C++ simulation of 2:1 orbital resonance with Dione and tidal heating

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::EnceladusDioneTidalResonanceModel model;

  std::cout << std::fixed << std::setprecision(6);
  std::cout << "================================================================================" << std::endl;
  std::cout << "   PAPER #203 REPLICATION: GREENBERG ET AL. (1980) - ENCELADUS TIDAL HEATING   " << std::endl;
  std::cout << "================================================================================" << std::endl;

  // 1. Orbital & Resonance Mechanics
  double n_E = model.orbital_frequency_enceladus_rad_s();
  double n_D = model.orbital_frequency_dione_rad_s();
  double P_E_hr = model.orbital_period_enceladus_hours();
  double P_D_hr = model.orbital_period_dione_hours();
  double ratio = model.resonance_frequency_ratio();
  double e_forced = model.forced_eccentricity_dione();

  std::cout << "\n[1] ORBITAL RESONANCE MECHANICS (Enceladus - Dione 2:1 MMR)" << std::endl;
  std::cout << "  Enceladus Mean Motion n_E:       " << n_E << " rad/s" << std::endl;
  std::cout << "  Dione Mean Motion n_D:           " << n_D << " rad/s" << std::endl;
  std::cout << "  Enceladus Orbital Period:        " << P_E_hr << " hours (" << P_E_hr / 24.0 << " days)" << std::endl;
  std::cout << "  Dione Orbital Period:            " << P_D_hr << " hours (" << P_D_hr / 24.0 << " days)" << std::endl;
  std::cout << "  Resonant Period Ratio (n_E/n_D): " << ratio << " (Exact 2:1 Resonance)" << std::endl;
  std::cout << "  Resonant Forced Eccentricity e:  " << e_forced << std::endl;

  // 2. Tidal Dissipation Power vs Eccentricity
  std::cout << "\n[2] TIDAL DISSIPATION HEATING POWER (Peale / Greenberg Formulation)" << std::endl;
  double nominal_e = 0.0047;
  double nominal_k2_Q = 0.0107;
  double p_tide_nominal_gw = model.tidal_heating_power_gw(nominal_e, nominal_k2_Q);
  double flux_nominal_mw_m2 = model.tidal_heat_flux_mw_m2(nominal_e, nominal_k2_Q);

  std::cout << "  Nominal Eccentricity e:          " << nominal_e << std::endl;
  std::cout << "  Nominal Dissipation k2/Q:        " << nominal_k2_Q << std::endl;
  std::cout << "  Total Tidal Heating Power:       " << p_tide_nominal_gw << " GW" << std::endl;
  std::cout << "  Surface Average Tidal Flux:      " << flux_nominal_mw_m2 << " mW/m^2" << std::endl;
  std::cout << "  Core Radiogenic Power:           " << hot_jupiter::EnceladusDioneTidalResonanceModel::P_RADIO_NOM_GW << " GW" << std::endl;

  // 3. Sweep over Eccentricity
  std::cout << "\n  Tidal Power Sweep across Eccentricity (k2/Q = 0.0107):" << std::endl;
  std::cout << "  --------------------------------------------------" << std::endl;
  std::cout << "    e         P_tide (GW)    F_tide (mW/m^2)   d_eq (km)" << std::endl;
  std::cout << "  --------------------------------------------------" << std::endl;
  for (double e = 0.001; e <= 0.0101; e += 0.001) {
    double p_gw = model.tidal_heating_power_gw(e, nominal_k2_Q);
    double flux = model.tidal_heat_flux_mw_m2(e, nominal_k2_Q);
    double d_eq = model.equilibrium_shell_thickness_km(e, nominal_k2_Q);
    std::cout << "   " << std::setw(6) << e
              << "   " << std::setw(10) << p_gw
              << "     " << std::setw(10) << flux
              << "    " << std::setw(8) << d_eq << std::endl;
  }

  // 4. Ice Shell Thermodynamics & Basal Melting
  std::cout << "\n[3] ICE SHELL CONDUCTIVE HEAT LOSS & BASAL MELTING" << std::endl;
  std::cout << "  --------------------------------------------------------------" << std::endl;
  std::cout << "    d (km)    P_base (MPa)    T_melt (K)    Q_cond (GW)    F (mW/m^2)" << std::endl;
  std::cout << "  --------------------------------------------------------------" << std::endl;
  for (double d = 5.0; d <= 50.1; d += 5.0) {
    double p_base_mpa = model.basal_pressure_pa(d) / 1.0e6;
    double t_melt = model.basal_melting_temperature_k(d);
    double q_gw = model.conductive_heat_loss_gw(d);
    double f_cond = model.conductive_heat_flux_mw_m2(d);
    std::cout << "   " << std::setw(5) << d
              << "      " << std::setw(8) << p_base_mpa
              << "      " << std::setw(8) << t_melt
              << "     " << std::setw(8) << q_gw
              << "    " << std::setw(8) << f_cond << std::endl;
  }

  // 5. Equilibrium Conditions
  double d_eq_nominal = model.equilibrium_shell_thickness_km(nominal_e, nominal_k2_Q);
  double e_crit_40km = model.critical_eccentricity_for_melting(40.0, nominal_k2_Q);
  double e_crit_25km = model.critical_eccentricity_for_melting(25.0, nominal_k2_Q);

  std::cout << "\n[4] THERMAL EQUILIBRIUM & CRITICAL CRITERIA" << std::endl;
  std::cout << "  Self-Consistent Shell Thickness d_eq:  " << d_eq_nominal << " km" << std::endl;
  std::cout << "  Critical Eccentricity (d = 40 km):     " << e_crit_40km << std::endl;
  std::cout << "  Critical Eccentricity (d = 25 km):     " << e_crit_25km << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
