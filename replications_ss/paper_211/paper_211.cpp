// Paper #211 Replication: Showman & Han (2004) "Numerical Simulations of Convection in Europa's Ice Shell"
// Physics: Rayleigh number, Arrhenius temperature-dependent viscosity, stagnant-lid thermal convection,
// Nusselt number scaling, lid thickness, and convective diapir dynamics in Europa's icy crust.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "======================================================================\n";
  std::cout << " Paper #211: Showman & Han (2004) Europa Ice Convection Solver\n";
  std::cout << " Numerical Simulations of Convection in Europa's Ice Shell\n";
  std::cout << "======================================================================\n\n";

  hot_jupiter::ShowmanHan2004IceConvectionModel model;

  // Basic thermodynamic & rheological properties
  double delta_T = model.delta_temperature_k();
  double theta = model.frank_kamenetskii_param();
  double delta_T_rh = model.rheological_temperature_scale_k();
  double visc_contrast = model.viscosity_contrast();
  double Ra_cr = model.critical_rayleigh_number();

  std::cout << std::scientific << std::setprecision(3);
  std::cout << "Total Shell Temperature Drop (Delta T) : " << delta_T << " K\n";
  std::cout << "Frank-Kamenetskii Parameter (theta)    : " << theta << "\n";
  std::cout << "Rheological Temperature Scale (DT_rh)  : " << delta_T_rh << " K\n";
  std::cout << "Total Viscosity Contrast (Delta eta)   : " << visc_contrast << "\n";
  std::cout << "Critical Rayleigh Number (Ra_cr)       : " << Ra_cr << "\n\n";

  // 1. Convective heat transport (Nu vs Ra) scan
  std::ofstream nu_ra_file("replications_ss/paper_211/nu_vs_ra.csv");
  if (!nu_ra_file.is_open()) {
    nu_ra_file.open("nu_vs_ra.csv");
  }
  nu_ra_file << "log10_Ra_b,Ra_b,Ra_rh,Nu_stagnant_lid,F_total_mW_m2,F_cond_mW_m2\n";

  std::cout << "--- 1. Nusselt Number vs Basal Rayleigh Number (D = 20 km) ---\n";
  std::cout << " log10(Ra_b) |  Basal Ra_b   |  Rheol Ra_rh  |   Nu   | F_tot (mW/m^2)\n";
  std::cout << "-------------------------------------------------------------------\n";

  for (double log_ra = 5.0; log_ra <= 9.0; log_ra += 0.25) {
    double ra_b = std::pow(10.0, log_ra);
    // Find effective eta_base corresponding to this Ra_b for D = 20 km
    double D_m = 20.0e3;
    double eta_base = (hot_jupiter::ShowmanHan2004IceConvectionModel::RHO_ICE *
                       hot_jupiter::ShowmanHan2004IceConvectionModel::G_SURF *
                       hot_jupiter::ShowmanHan2004IceConvectionModel::ALPHA_EXP *
                       delta_T * std::pow(D_m, 3.0)) /
                      (hot_jupiter::ShowmanHan2004IceConvectionModel::KAPPA_DIFF * ra_b);

    double ra_rh = model.rheological_rayleigh_number(20.0, eta_base);
    double nu = model.nusselt_number(20.0, eta_base);
    double f_cond = model.conductive_heat_flux_mw_m2(20.0);
    double f_tot = model.total_heat_flux_mw_m2(20.0, eta_base);

    nu_ra_file << std::fixed << std::setprecision(2) << log_ra << ","
               << std::scientific << std::setprecision(4) << ra_b << ","
               << ra_rh << ","
               << std::fixed << std::setprecision(3) << nu << ","
               << f_tot << "," << f_cond << "\n";

    if (std::abs(std::round(log_ra) - log_ra) < 1e-4) {
      std::cout << "   " << std::fixed << std::setprecision(1) << log_ra << "     | "
                << std::scientific << std::setprecision(2) << ra_b << " | "
                << ra_rh << " | "
                << std::fixed << std::setprecision(3) << nu << "  |   "
                << std::setprecision(2) << f_tot << "\n";
    }
  }
  nu_ra_file.close();
  std::cout << "\n✅ Generated replications_ss/paper_211/nu_vs_ra.csv\n\n";

  // 2. Stagnant lid thickness vs Shell thickness & Viscosity contrast
  std::ofstream lid_file("replications_ss/paper_211/lid_thickness.csv");
  if (!lid_file.is_open()) {
    lid_file.open("lid_thickness.csv");
  }
  lid_file << "D_shell_km,eta_base_Pa_s,Ra_b,Nu,delta_lid_km,delta_conv_km,u_conv_m_yr,tau_diapir_yr\n";

  std::cout << "--- 2. Stagnant Lid Structure & Convective Dynamics ---\n";
  std::cout << " D (km) | eta_b (Pa s) |   Ra_b   |  Nu  | d_lid (km) | d_conv (km) | u (m/yr) | tau_diapir (yr)\n";
  std::cout << "---------------------------------------------------------------------------------------------\n";

  std::vector<double> thicknesses = {10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0};
  std::vector<double> viscosities = {1.0e13, 1.0e14, 1.0e15};

  for (double d_km : thicknesses) {
    for (double eta_b : viscosities) {
      double ra_b = model.basal_rayleigh_number(d_km, eta_b);
      double nu = model.nusselt_number(d_km, eta_b);
      double d_lid = model.stagnant_lid_thickness_km(d_km, eta_b);
      double d_conv = model.convective_sublayer_thickness_km(d_km, eta_b);
      double u_conv = model.convective_velocity_m_yr(d_km, eta_b);
      double tau_diapir = model.diapir_ascent_timescale_yr(d_km, eta_b);

      lid_file << std::fixed << std::setprecision(1) << d_km << ","
               << std::scientific << std::setprecision(2) << eta_b << ","
               << ra_b << ","
               << std::fixed << std::setprecision(3) << nu << ","
               << std::setprecision(2) << d_lid << ","
               << d_conv << ","
               << std::setprecision(4) << u_conv << ","
               << std::scientific << std::setprecision(2) << tau_diapir << "\n";

      if (eta_b == 1.0e14) {
        std::cout << "  " << std::fixed << std::setprecision(1) << d_km << "  |   1.00e+14   | "
                  << std::scientific << std::setprecision(2) << ra_b << " | "
                  << std::fixed << std::setprecision(2) << nu << " |   "
                  << d_lid << "   |    "
                  << d_conv << "    |  "
                  << std::setprecision(3) << u_conv << "  |   "
                  << std::scientific << std::setprecision(2) << tau_diapir << "\n";
      }
    }
  }
  lid_file.close();
  std::cout << "\n✅ Generated replications_ss/paper_211/lid_thickness.csv\n\n";

  // 3. Temperature and viscosity profiles through the ice shell
  std::ofstream profile_file("replications_ss/paper_211/temperature_profile.csv");
  if (!profile_file.is_open()) {
    profile_file.open("temperature_profile.csv");
  }
  profile_file << "z_km,z_norm,T_k,viscosity_Pa_s\n";

  double D_nom = 20.0;
  for (double z = 0.0; z <= D_nom; z += 0.5) {
    // Model temperature profile: linear in stagnant lid, adiabatic/well-mixed in convective sublayer
    double d_lid = model.stagnant_lid_thickness_km(D_nom, 1.0e14);
    double T_z;
    if (z <= d_lid) {
      // Conductive lid gradient: T_surf to T_lid
      double T_lid = model.T_BASE_NOM - delta_T_rh;
      T_z = model.T_SURF_NOM + (T_lid - model.T_SURF_NOM) * (z / d_lid);
    } else {
      // Convective sublayer: nearly isothermal at T_base with small boundary layer
      double z_sub = z - d_lid;
      double d_sub = D_nom - d_lid;
      double T_lid = model.T_BASE_NOM - delta_T_rh;
      T_z = T_lid + delta_T_rh * (z_sub / d_sub);
    }
    double eta_z = model.viscosity_at_temperature(T_z, 1.0e14);

    profile_file << std::fixed << std::setprecision(2) << z << ","
                 << (z / D_nom) << ","
                 << std::setprecision(2) << T_z << ","
                 << std::scientific << std::setprecision(4) << eta_z << "\n";
  }
  profile_file.close();
  std::cout << "✅ Generated replications_ss/paper_211/temperature_profile.csv\n";

  std::cout << "\n=== Showman & Han (2004) Replication Solver Completed Successfully ===\n";
  return 0;
}
