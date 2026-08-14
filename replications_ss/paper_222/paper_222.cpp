// Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
// Solver for Paper #222: Thermal Evolution and State of Europa's Ice Shell
// Mitri & Showman (2005) | Icarus 177 (2), 447-460
//
// Evaluates first-principles ice shell thermal convection, temperature-dependent
// Arrhenius rheology, stagnant-lid vs. mobile-lid regimes, equilibrium shell thickness,
// convective-conductive transitions, bistability/hysteresis, and sensitivity to basal flux
// and tidal heating perturbations.

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "========================================================================\n";
  std::cout << "Paper #222 Solver: Thermal Evolution and State of Europa's Ice Shell\n";
  std::cout << "Mitri & Showman (2005) | Icarus 177 (2), 447-460\n";
  std::cout << "========================================================================\n\n";

  hot_jupiter::MitriShowman2005IceConvectionModel model;

  // Basic thermodynamic & rheological properties
  double delta_T = model.delta_temperature_k();
  double theta = model.frank_kamenetskii_param();
  double delta_T_rh = model.rheological_temperature_scale_k();
  double visc_contrast = model.viscosity_contrast();
  double Ra_cr = model.critical_rayleigh_number();
  double D_cr_nom = model.critical_thickness_convection_onset_km();
  double T_conv_nom = model.convective_core_temperature_k();

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Physical & Rheological Parameters:\n";
  std::cout << "  Europa Radius R_E          : " << model.R_EUROPA / 1.0e3 << " km\n";
  std::cout << "  Surface Gravity g          : " << model.G_SURF << " m/s^2\n";
  std::cout << "  Ice Density rho_ice        : " << model.RHO_ICE << " kg/m^3\n";
  std::cout << "  Thermal Conductivity k     : " << model.K_COND << " W/(m K)\n";
  std::cout << "  Specific Heat Capacity Cp  : " << model.CP_ICE << " J/(kg K)\n";
  std::cout << "  Thermal Diffusivity kappa  : " << std::scientific << model.KAPPA_DIFF << " m^2/s\n" << std::fixed;
  std::cout << "  Surface Temperature T_surf : " << model.T_SURF_NOM << " K\n";
  std::cout << "  Basal Temperature T_base   : " << model.T_BASE_NOM << " K\n";
  std::cout << "  Delta T across shell       : " << delta_T << " K\n";
  std::cout << "  Activation Energy E*       : " << model.ACTIVATION_E / 1.0e3 << " kJ/mol\n";
  std::cout << "  Frank-Kamenetskii theta    : " << theta << "\n";
  std::cout << "  Rheological Delta T_rh     : " << delta_T_rh << " K\n";
  std::cout << "  Convective Core Temp T_conv: " << T_conv_nom << " K\n";
  std::cout << "  Viscosity Contrast         : " << std::scientific << visc_contrast << "\n" << std::fixed;
  std::cout << "  Critical Rayleigh Number   : " << std::scientific << Ra_cr << "\n" << std::fixed;
  std::cout << "  Critical Convection D_cr   : " << D_cr_nom << " km (eta_base = 1e14 Pa s)\n\n";

  // 1. Export CSV: Convective Heat Transfer & Nusselt Number Scaling vs Rayleigh Number
  std::string csv_nu_path = "replications_ss/paper_222/nu_vs_ra_regimes.csv";
  std::ofstream csv_nu(csv_nu_path);
  if (!csv_nu.is_open()) {
    std::cerr << "Error opening " << csv_nu_path << std::endl;
    return 1;
  }

  csv_nu << "log10_Ra_b,Ra_b,Ra_rh,Nu_stagnant,Nu_mobile,Nu_isoviscous,Nu_conduction,"
         << "F_stag_mW_m2,F_mob_mW_m2,F_iso_mW_m2,F_cond_mW_m2,d_lid_stag_km,u_conv_stag_m_yr\n";

  double D_eval_km = 25.0; // Nominal 25 km shell
  double F_cond_base = model.conductive_heat_flux_mw_m2(D_eval_km);

  for (double log_ra = 3.0; log_ra <= 9.0; log_ra += 0.05) {
    double ra_b = std::pow(10.0, log_ra);
    // Corresponding eta_base for D = 25 km
    double D_m = D_eval_km * 1.0e3;
    double eta_b = (model.RHO_ICE * model.G_SURF * model.ALPHA_EXP * delta_T * std::pow(D_m, 3.0)) /
                   (model.KAPPA_DIFF * ra_b);

    double ra_rh = model.rheological_rayleigh_number(D_eval_km, eta_b);
    double nu_stag = model.nusselt_stagnant_lid(D_eval_km, eta_b);
    double nu_mob = model.nusselt_mobile_lid(D_eval_km, eta_b);
    double nu_iso = model.nusselt_isoviscous(D_eval_km, eta_b);
    double nu_cond = 1.0;

    double f_stag = F_cond_base * nu_stag;
    double f_mob = F_cond_base * nu_mob;
    double f_iso = F_cond_base * nu_iso;
    double f_cond = F_cond_base * nu_cond;

    double d_lid_stag = model.stagnant_lid_thickness_km(D_eval_km, eta_b);
    double u_conv_stag = model.convective_velocity_m_yr(D_eval_km, eta_b);

    csv_nu << std::fixed << std::setprecision(3) << log_ra << ","
           << std::scientific << std::setprecision(5) << ra_b << ","
           << ra_rh << ","
           << std::fixed << std::setprecision(4)
           << nu_stag << "," << nu_mob << "," << nu_iso << "," << nu_cond << ","
           << std::setprecision(2)
           << f_stag << "," << f_mob << "," << f_iso << "," << f_cond << ","
           << std::setprecision(3) << d_lid_stag << ","
           << std::scientific << std::setprecision(4) << u_conv_stag << "\n";
  }
  csv_nu.close();
  std::cout << "✅ Exported Nusselt Number Regimes -> " << csv_nu_path << "\n";

  // 2. Export CSV: Equilibrium Ice Shell Thickness Branches & Bistability / Hysteresis
  std::string csv_eq_path = "replications_ss/paper_222/equilibrium_branches_vs_flux.csv";
  std::ofstream csv_eq(csv_eq_path);
  if (!csv_eq.is_open()) {
    std::cerr << "Error opening " << csv_eq_path << std::endl;
    return 1;
  }

  csv_eq << "F_basal_mW_m2,D_conductive_km,D_convective_eta13_km,D_convective_eta14_km,D_convective_eta15_km,"
         << "D_crit_eta13_km,D_crit_eta14_km,D_crit_eta15_km,d_lid_eta14_km,d_conv_eta14_km,"
         << "F_tide_eta14_mW_m2,F_surf_eta14_mW_m2,bistable_eta14_flag,favored_regime_eta14\n";

  double eta13 = 1.0e13;
  double eta14 = 1.0e14;
  double eta15 = 1.0e15;

  double D_cr_13 = model.critical_thickness_convection_onset_km(eta13);
  double D_cr_14 = model.critical_thickness_convection_onset_km(eta14);
  double D_cr_15 = model.critical_thickness_convection_onset_km(eta15);

  for (double F_b = 5.0; F_b <= 90.0; F_b += 0.5) {
    auto b13 = model.evaluate_equilibrium_branches(F_b, eta13);
    auto b14 = model.evaluate_equilibrium_branches(F_b, eta14);
    auto b15 = model.evaluate_equilibrium_branches(F_b, eta15);

    double D_cond = model.equilibrium_conductive_thickness_km(F_b);
    double D_conv_14 = b14.D_convective_km;
    double d_lid_14 = model.stagnant_lid_thickness_km(D_conv_14, eta14);
    double d_conv_14 = model.convective_sublayer_thickness_km(D_conv_14, eta14);
    double F_tide_14 = model.tidal_heat_flux_mw_m2(D_conv_14, eta14);
    double F_surf_14 = model.stagnant_lid_heat_flux_mw_m2(D_conv_14, eta14);

    csv_eq << std::fixed << std::setprecision(2) << F_b << ","
           << std::setprecision(3) << D_cond << ","
           << b13.D_convective_km << "," << b14.D_convective_km << "," << b15.D_convective_km << ","
           << D_cr_13 << "," << D_cr_14 << "," << D_cr_15 << ","
           << d_lid_14 << "," << d_conv_14 << ","
           << std::setprecision(2) << F_tide_14 << "," << F_surf_14 << ","
           << (b14.has_bistability ? 1 : 0) << ","
           << b14.favored_state << "\n";
  }
  csv_eq.close();
  std::cout << "✅ Exported Equilibrium Branches & Hysteresis -> " << csv_eq_path << "\n";

  // 3. Export CSV: Time-Dependent Thermal Evolution with Step Basal Flux Perturbations
  std::string csv_evol_path = "replications_ss/paper_222/thermal_evolution_perturbation.csv";
  std::ofstream csv_evol(csv_evol_path);
  if (!csv_evol.is_open()) {
    std::cerr << "Error opening " << csv_evol_path << std::endl;
    return 1;
  }

  csv_evol << "time_kyr,D_km,delta_lid_km,delta_conv_km,F_surf_mW_m2,F_basal_mW_m2,F_tide_mW_m2,Nu,u_conv_m_yr,is_convective\n";

  // Run 3000 kyr evolution: Initial D = 28.5 km, F_basal = 30 mW/m^2; at t=500 kyr, F_basal jumps to 50 mW/m^2 until t=1500 kyr
  auto trajectory = model.integrate_thermal_evolution(28.5, 3000.0, 1.0, 30.0, 50.0, 500.0, 1500.0, eta14);

  for (const auto& pt : trajectory) {
    csv_evol << std::fixed << std::setprecision(1) << pt.time_kyr << ","
             << std::setprecision(4) << pt.D_km << ","
             << pt.delta_lid_km << "," << pt.delta_conv_km << ","
             << std::setprecision(2) << pt.F_surf_mw_m2 << "," << pt.F_basal_mw_m2 << "," << pt.F_tide_mw_m2 << ","
             << std::setprecision(4) << pt.Nu << ","
             << std::scientific << std::setprecision(3) << pt.u_conv_m_yr << ","
             << (pt.is_convective ? 1 : 0) << "\n";
  }
  csv_evol.close();
  std::cout << "✅ Exported Thermal Evolution Trajectory -> " << csv_evol_path << " (" << trajectory.size() << " steps)\n";

  // 4. Export CSV: Temperature, Viscosity & Volumetric Tidal Dissipation Depth Profiles
  std::string csv_prof_path = "replications_ss/paper_222/temperature_viscosity_profiles.csv";
  std::ofstream csv_prof(csv_prof_path);
  if (!csv_prof.is_open()) {
    std::cerr << "Error opening " << csv_prof_path << std::endl;
    return 1;
  }

  csv_prof << "z_norm,z_km,T_cond_K,T_conv_K,visc_cond_Pa_s,visc_conv_Pa_s,q_tide_cond_W_m3,q_tide_conv_W_m3\n";

  double D_prof_km = 25.0;
  double d_lid_nom = model.stagnant_lid_thickness_km(D_prof_km, eta14);
  int n_prof_pts = 100;

  for (int i = 0; i <= n_prof_pts; ++i) {
    double z_norm = static_cast<double>(i) / n_prof_pts;
    double z_km = z_norm * D_prof_km;

    // Conductive temperature profile (linear)
    double T_cond = model.T_SURF_NOM + z_norm * delta_T;

    // Stagnant-lid convective temperature profile:
    // Linear conductive gradient in lid [0, d_lid], adiabatic/isothermal in convective sublayer [d_lid, D]
    double T_conv;
    if (z_km <= d_lid_nom) {
      T_conv = model.T_SURF_NOM + (z_km / d_lid_nom) * (T_conv_nom - model.T_SURF_NOM);
    } else {
      // Transition layer near base
      double frac_sublayer = (z_km - d_lid_nom) / (D_prof_km - d_lid_nom);
      if (frac_sublayer < 0.85) {
        T_conv = T_conv_nom;
      } else {
        double f_b = (frac_sublayer - 0.85) / 0.15;
        T_conv = T_conv_nom + f_b * (model.T_BASE_NOM - T_conv_nom);
      }
    }

    double visc_cond = model.viscosity_at_temperature(T_cond, eta14);
    double visc_conv = model.viscosity_at_temperature(T_conv, eta14);

    double q_tide_cond = model.volumetric_tidal_heating_w_m3(T_cond, eta14);
    double q_tide_conv = model.volumetric_tidal_heating_w_m3(T_conv, eta14);

    csv_prof << std::fixed << std::setprecision(4) << z_norm << ","
             << std::setprecision(3) << z_km << ","
             << std::setprecision(2) << T_cond << "," << T_conv << ","
             << std::scientific << std::setprecision(4)
             << visc_cond << "," << visc_conv << ","
             << q_tide_cond << "," << q_tide_conv << "\n";
  }
  csv_prof.close();
  std::cout << "✅ Exported Depth Profiles -> " << csv_prof_path << "\n\n";

  std::cout << "========================================================================\n";
  std::cout << "Solver Paper #222 Completed Successfully.\n";
  std::cout << "========================================================================\n";

  return 0;
}
