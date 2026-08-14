// Replication of Tobie, Mocquet, & Sotin (2005)
// "Tidal dissipation in Titan's interior: Implications for Cassini observations"
// Icarus 177 (2005) 534-549.
//
// Computes degree-2 tidal Love numbers (k2, h2, l2), viscoelastic tidal phase lag delta,
// dissipation factor k2/Q, and total interior tidal heating power P_tide for Titan.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "=================================================================" << std::endl;
  std::cout << "  Paper #207: Tobie, Mocquet, & Sotin (2005) Titan Tidal Solver   " << std::endl;
  std::cout << "=================================================================" << std::endl;

  hot_jupiter::TitanTidalDissipationModel model;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Titan Mean Radius R:           " << hot_jupiter::TitanTidalDissipationModel::R_TITAN / 1.0e3 << " km" << std::endl;
  std::cout << "Titan Mass M:                  " << hot_jupiter::TitanTidalDissipationModel::M_TITAN << " kg" << std::endl;
  std::cout << "Saturn Mass M_S:               " << hot_jupiter::TitanTidalDissipationModel::M_SATURN << " kg" << std::endl;
  std::cout << "Orbital Semi-Major Axis a:     " << hot_jupiter::TitanTidalDissipationModel::A_TITAN / 1.0e6 << " x10^3 km" << std::endl;
  std::cout << "Orbital Eccentricity e:        " << hot_jupiter::TitanTidalDissipationModel::ECCENTRICITY << std::endl;
  std::cout << "Orbital Frequency n:           " << model.orbital_frequency_rad_s() << " rad/s" << std::endl;
  std::cout << "Orbital Period:                " << model.orbital_period_days() << " days" << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;

  // 1. Output Love Numbers vs Ocean Thickness (for crust thicknesses 50, 100, 150 km)
  std::string love_csv_path = "replications_ss/paper_207/titan_love_numbers.csv";
  std::ofstream love_csv(love_csv_path);
  if (!love_csv.is_open()) {
    std::cerr << "Error opening " << love_csv_path << std::endl;
    return 1;
  }
  love_csv << "d_ocean_km,k2_crust50,k2_crust100,k2_crust150,h2_crust100,l2_crust100,dr_amp_crust100_m\n";

  for (double d_oc = 0.0; d_oc <= 400.0; d_oc += 2.0) {
    double k2_50 = model.love_number_k2(50.0, d_oc);
    double k2_100 = model.love_number_k2(100.0, d_oc);
    double k2_150 = model.love_number_k2(150.0, d_oc);
    double h2_100 = model.love_number_h2(100.0, d_oc);
    double l2_100 = model.love_number_l2(100.0, d_oc);
    double dr_100 = model.diurnal_radial_tide_amplitude_m(100.0, d_oc);

    love_csv << std::fixed << std::setprecision(4)
             << d_oc << ","
             << k2_50 << ","
             << k2_100 << ","
             << k2_150 << ","
             << h2_100 << ","
             << l2_100 << ","
             << dr_100 << "\n";
  }
  love_csv.close();
  std::cout << " Saved: " << love_csv_path << std::endl;

  // 2. Output Tidal Phase Lag and Heating Power vs Viscosity
  std::string visc_csv_path = "replications_ss/paper_207/titan_viscous_dissipation.csv";
  std::ofstream visc_csv(visc_csv_path);
  if (!visc_csv.is_open()) {
    std::cerr << "Error opening " << visc_csv_path << std::endl;
    return 1;
  }
  visc_csv << "log10_eta,eta_pa_s,phase_lag_deg,phase_lag_rad,k2_over_Q_ocean200,power_gw_ocean200,heat_flux_mw_m2\n";

  for (double log_eta = 10.0; log_eta <= 18.0; log_eta += 0.05) {
    double eta = std::pow(10.0, log_eta);
    double lag_deg = model.tidal_phase_lag_deg(eta);
    double lag_rad = model.tidal_phase_lag_rad(eta);
    double k2_q = model.dissipation_factor_k2_over_Q(100.0, 200.0, eta);
    double p_gw = model.tidal_heating_power_gw(100.0, 200.0, eta);
    double flux_mw = model.surface_tidal_heat_flux_mw_m2(100.0, 200.0, eta);

    visc_csv << std::fixed << std::setprecision(5)
             << log_eta << ","
             << std::scientific << eta << ","
             << std::fixed << lag_deg << ","
             << lag_rad << ","
             << std::scientific << k2_q << ","
             << std::fixed << p_gw << ","
             << flux_mw << "\n";
  }
  visc_csv.close();
  std::cout << " Saved: " << visc_csv_path << std::endl;

  // Key benchmark checks
  double k2_solid = model.love_number_k2(100.0, 0.0);
  double k2_ocean = model.love_number_k2(100.0, 200.0);
  double h2_ocean = model.love_number_h2(100.0, 200.0);
  double p_tide_nominal = model.tidal_heating_power_gw(100.0, 200.0, 1.0e15);

  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "RESULTS & VALIDATION:" << std::endl;
  std::cout << "  k2 (No Ocean, Solid):        " << k2_solid << " (Tobie 2005: ~0.038)" << std::endl;
  std::cout << "  k2 (With Decoupled Ocean):   " << k2_ocean << " (Cassini Iess 2012: 0.589 +/- 0.075)" << std::endl;
  std::cout << "  h2 (With Decoupled Ocean):   " << h2_ocean << " (Tobie 2005: ~1.28)" << std::endl;
  std::cout << "  Diurnal Tide Amplitude:      " << model.diurnal_radial_tide_amplitude_m(100.0, 200.0) << " m (Peak-to-peak: "
            << 2.0 * model.diurnal_radial_tide_amplitude_m(100.0, 200.0) << " m)" << std::endl;
  std::cout << "  Nominal Tidal Power (eta=10^15 Pa s): " << p_tide_nominal << " GW" << std::endl;
  std::cout << "=================================================================" << std::endl;

  return 0;
}
