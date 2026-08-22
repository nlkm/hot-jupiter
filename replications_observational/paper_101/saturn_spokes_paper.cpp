// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #101: Saturn Ring Spokes Electrostatic Levitation Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #101: SATURN RING SPOKES LEVITATION & COROTATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::SaturnRingSpokesModel model;

  const double r_grain_um = model.dust_grain_radius_um();           // ~ 0.60 um
  const double phi_charge_v = model.electrostatic_potential_volts();// ~ -15.0 V
  const double z_lev_km = model.levitation_height_km();             // ~ 80.0 km
  const double p_mag_hr = model.magnetic_corotation_period_hours();  // ~ 10.656 hr

  std::cout << "Dust Grain Radius: " << r_grain_um << " um" << std::endl;
  std::cout << "Grain Electrostatic Surface Potential: " << phi_charge_v << " Volts" << std::endl;
  std::cout << "Maximum Dust Levitation Height: " << z_lev_km << " km" << std::endl;
  std::cout << "Saturn Magnetic Corotation Period: " << p_mag_hr << " hours" << std::endl;

  // Track Spoke Dust Levitation and Keplerian Shearing over 0.0 to 10.656 hours (linear time scale):
  // Synchronous corotation radius r_syn = 112,500 km in mid B-ring
  std::ofstream out("replications_observational/paper_101/saturn_spokes_evolution.csv");
  out << "time_hours,levitation_height_km,angular_shear_deg,optical_contrast_delta_i\n";

  const double r_inner_km = 108000.0;
  const double r_outer_km = 120000.0;

  for (double t_hr = 0.0; t_hr <= 10.656; t_hr += 0.2) {
    // Electrostatic levitation rise
    double z_km = z_lev_km * (1.0 - std::exp(-t_hr / 1.5));

    // Differential Keplerian angular shear between inner and outer spoke edge [degrees]
    // n(r) = sqrt(G*M_sat / r^3)
    double n_inner = std::sqrt(hot_jupiter::G * 5.6834e26 / std::pow(r_inner_km * 1.0e3, 3.0));
    double n_outer = std::sqrt(hot_jupiter::G * 5.6834e26 / std::pow(r_outer_km * 1.0e3, 3.0));
    double delta_omega = (n_inner - n_outer) * (t_hr * 3600.0) * (180.0 / M_PI);

    // Forward scattering optical contrast Delta I/I_0
    double contrast = 0.25 * std::exp(-t_hr / 4.5);

    out << t_hr << "," << z_km << "," << delta_omega << "," << contrast << "\n";
  }
  out.close();

  std::cout << "Generated Saturn Ring Spokes Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
