// Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
// Solver for Paper #212: Thermal-Orbital Evolution of Io and Europa
// Hussmann & Spohn (2004) | Icarus 171 (2), 391-410.
//
// Evaluates first-principles coupled thermal-orbital evolution in the Laplace resonance:
//   de_1/dt = e_1 * [ A_J - B_1 * (k_2 / Q)_1(T_1) ]
//   M_1 * C_p * dT_1/dt = P_tide(e_1, T_1) + Q_radio - Q_conv(T_1)
// where viscoelastic tidal dissipation Im(k_2)(T) depends on mantle viscosity eta(T),
// convective heat loss Q_conv(T) follows parameterized boundary layer scaling,
// and secular orbital pumping A_J is driven by Jupiter's tidal torque.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "========================================================================\n";
  std::cout << "Paper #212 Solver: Thermal-Orbital Evolution of Io and Europa\n";
  std::cout << "Hussmann & Spohn (2004) | Icarus 171 (2), 391-410\n";
  std::cout << "========================================================================\n\n";

  hot_jupiter::HussmannSpohn2004ThermalOrbitalModel model;

  double n_io = model.io_mean_motion();
  double p_io_days = model.io_orbital_period_days();
  double n_eu = model.europa_mean_motion();
  double p_eu_days = model.europa_orbital_period_days();

  double T_eq_k = model.T_REF_IO;
  double eta_eq = model.io_viscosity_pa_s(T_eq_k);
  double k2_q_eq = model.io_k2_over_q(T_eq_k);
  double e_eq = model.equilibrium_eccentricity(T_eq_k);
  double p_tide_eq_tw = model.io_tidal_power_tw(e_eq, T_eq_k);
  double q_loss_eq_tw = model.io_convective_heat_loss_tw(T_eq_k);
  double flux_eq_wm2 = model.io_surface_heat_flux_w_m2(p_tide_eq_tw * 1.0e12);

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Physical & Orbital Dynamics Framework:\n";
  std::cout << "  Jupiter Mass M_J          : " << model.M_JUPITER << " kg\n";
  std::cout << "  Io Semi-Major Axis a_1    : " << model.A_IO / 1.0e3 << " km\n";
  std::cout << "  Io Mean Motion n_1        : " << std::scientific << n_io << " rad/s\n" << std::fixed;
  std::cout << "  Io Orbital Period P_1     : " << p_io_days << " days\n";
  std::cout << "  Europa Semi-Major Axis a_2: " << model.A_EUROPA / 1.0e3 << " km\n";
  std::cout << "  Europa Mean Motion n_2    : " << std::scientific << n_eu << " rad/s\n" << std::fixed;
  std::cout << "  Europa Orbital Period P_2 : " << p_eu_days << " days\n";
  std::cout << "  Mean Motion Ratio n1 / n2 : " << (n_io / n_eu) << " (Near 2:1 Laplace Resonance)\n\n";

  std::cout << "Nominal Thermal-Orbital Equilibrium State:\n";
  std::cout << "  Equilibrium Temperature   : " << T_eq_k << " K (" << T_eq_k - 273.15 << " deg C)\n";
  std::cout << "  Mantle Viscosity eta      : " << std::scientific << eta_eq << " Pa s\n" << std::fixed;
  std::cout << "  Viscoelastic Im(k2)       : " << k2_q_eq << "\n";
  std::cout << "  Equilibrium Eccentricity  : " << e_eq << "\n";
  std::cout << "  Tidal Dissipation Power   : " << p_tide_eq_tw << " TW\n";
  std::cout << "  Convective Heat Loss      : " << q_loss_eq_tw << " TW\n";
  std::cout << "  Surface Heat Flux         : " << flux_eq_wm2 << " W/m^2\n\n";

  // 1. Export CSV: 1 Gyr Coupled Thermal-Orbital Evolution (Limit Cycle Trajectory)
  // Initial condition perturbed from equilibrium to exhibit the classic Hussmann & Spohn (2004) limit cycle
  std::string csv_evol_path = "replications_ss/paper_212/io_europa_evolution_1gyr.csv";
  std::ofstream csv_evol(csv_evol_path);
  if (!csv_evol.is_open()) {
    std::cerr << "Error opening " << csv_evol_path << std::endl;
    return 1;
  }

  csv_evol << "time_myr,ecc_limit_cycle,temp_limit_cycle_k,power_limit_cycle_tw,loss_limit_cycle_tw,"
           << "k2q_limit_cycle,visc_limit_cycle_pas,ecc_steady,temp_steady_k,power_steady_tw,loss_steady_tw\n";

  // Integrate limit cycle trajectory (starts cold T=1360 K, e=0.0075)
  auto traj_osc = model.integrate_coupled_evolution(0.0075, 1360.0, 1000.0, 0.2);
  // Integrate steady-state equilibrium trajectory (starts at equilibrium)
  auto traj_steady = model.integrate_coupled_evolution(e_eq, T_eq_k, 1000.0, 0.2);

  size_t n_steps = std::min(traj_osc.size(), traj_steady.size());
  for (size_t i = 0; i < n_steps; ++i) {
    const auto& s1 = traj_osc[i];
    const auto& s2 = traj_steady[i];
    csv_evol << std::fixed << std::setprecision(2) << s1.time_myr << ","
             << std::setprecision(6) << s1.eccentricity << ","
             << std::setprecision(2) << s1.temperature_k << ","
             << std::setprecision(4) << s1.tidal_power_tw << ","
             << std::setprecision(4) << s1.heat_loss_tw << ","
             << std::setprecision(6) << s1.k2_over_q << ","
             << std::scientific << std::setprecision(4) << s1.viscosity_pa_s << ","
             << std::fixed << std::setprecision(6) << s2.eccentricity << ","
             << std::setprecision(2) << s2.temperature_k << ","
             << std::setprecision(4) << s2.tidal_power_tw << ","
             << std::setprecision(4) << s2.heat_loss_tw << "\n";
  }
  csv_evol.close();
  std::cout << "✅ Exported 1 Gyr Coupled Trajectories -> " << csv_evol_path << " (" << n_steps << " rows)\n";

  // 2. Export CSV: Equilibrium Eccentricity vs Satellite Dissipation Factor Q and Jupiter k2/Q
  std::string csv_q_path = "replications_ss/paper_212/eccentricity_vs_Q_dissipation.csv";
  std::ofstream csv_q(csv_q_path);
  if (!csv_q.is_open()) {
    std::cerr << "Error opening " << csv_q_path << std::endl;
    return 1;
  }

  csv_q << "Q_io,k2_over_Q_io,ecc_eq_viscoelastic,ecc_eq_fixed_power,ecc_eq_jup_low,ecc_eq_jup_high\n";

  for (double Q_val = 1.0; Q_val <= 500.0; Q_val += 2.0) {
    double k2_io = 0.025;
    double k2_q_val = k2_io / Q_val;
    double e_fixed_power = model.equilibrium_eccentricity_for_Q(Q_val, k2_io, 105.0);

    // Viscoelastic equilibrium eccentricity with varying Jupiter pumping factors
    double e_jup_low = model.equilibrium_eccentricity(T_eq_k, 0.8e-5);
    double e_jup_high = model.equilibrium_eccentricity(T_eq_k, 3.0e-5);
    double e_visco = model.equilibrium_eccentricity_for_Q(Q_val, k2_io, q_loss_eq_tw);

    csv_q << std::fixed << std::setprecision(1) << Q_val << ","
          << std::setprecision(6) << k2_q_val << ","
          << std::setprecision(6) << e_visco << ","
          << std::setprecision(6) << e_fixed_power << ","
          << std::setprecision(6) << e_jup_low << ","
          << std::setprecision(6) << e_jup_high << "\n";
  }
  csv_q.close();
  std::cout << "✅ Exported Equilibrium Eccentricity vs Q -> " << csv_q_path << "\n";

  // 3. Export CSV: Mantle Rheology & Thermal Balance Curves
  std::string csv_rheo_path = "replications_ss/paper_212/io_thermal_viscosity_dissipation.csv";
  std::ofstream csv_rheo(csv_rheo_path);
  if (!csv_rheo.is_open()) {
    std::cerr << "Error opening " << csv_rheo_path << std::endl;
    return 1;
  }

  csv_rheo << "temp_k,viscosity_pas,k2_over_q,tidal_power_tw_nominal_e,tidal_power_tw_high_e,convective_loss_tw,net_heating_rate_k_myr\n";

  for (double T = 1100.0; T <= 1750.0; T += 5.0) {
    double visc = model.io_viscosity_pa_s(T);
    double k2_q = model.io_k2_over_q(T);
    double p_nom = model.io_tidal_power_tw(0.0041, T);
    double p_high = model.io_tidal_power_tw(0.0080, T);
    double q_loss = model.io_convective_heat_loss_tw(T);
    double dT_dt_s = model.temperature_derivative_k_s(0.0041, T);
    double dT_dt_myr = dT_dt_s * (1.0e6 * 365.25 * 86400.0);

    csv_rheo << std::fixed << std::setprecision(1) << T << ","
             << std::scientific << std::setprecision(4) << visc << ","
             << std::fixed << std::setprecision(6) << k2_q << ","
             << std::setprecision(4) << p_nom << ","
             << std::setprecision(4) << p_high << ","
             << std::setprecision(4) << q_loss << ","
             << std::setprecision(4) << dT_dt_myr << "\n";
  }
  csv_rheo.close();
  std::cout << "✅ Exported Mantle Rheology Curves -> " << csv_rheo_path << "\n\n";

  std::cout << "========================================================================\n";
  std::cout << "Hussmann & Spohn (2004) Simulation Completed Successfully!\n";
  std::cout << "========================================================================\n";
  return 0;
}
