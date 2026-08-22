// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #106: Saturn Mimas Subsurface Ocean & Libration Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #106: SATURN MIMAS SUBSURFACE OCEAN & LIBRATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::MimasSubsurfaceOceanModel model;

  const double r_mimas = model.mimas_radius_km();               // ~ 198.2 km
  const double m_mimas = model.mimas_mass_kg();                 // ~ 3.75e19 kg
  const double ecc = model.orbital_eccentricity();              // ~ 0.0202
  const double amp_lib = model.libration_amplitude_arcsec();    // ~ 49.3 arcsec
  const double z_shell = model.ice_shell_thickness_km();        // ~ 25.0 km
  const double h_ocean = model.ocean_layer_thickness_km();      // ~ 45.0 km
  const double age_myr = model.ocean_age_myr();                 // ~ 15.0 Myr

  std::cout << "Mimas Mean Radius: " << r_mimas << " km" << std::endl;
  std::cout << "Mimas Mass: " << m_mimas << " kg" << std::endl;
  std::cout << "Orbital Eccentricity: " << ecc << std::endl;
  std::cout << "Observed Physical Libration Amplitude: " << amp_lib << " arcsec" << std::endl;
  std::cout << "Outer Ice Shell Thickness: " << z_shell << " km" << std::endl;
  std::cout << "Liquid Ocean Layer Thickness: " << h_ocean << " km" << std::endl;
  std::cout << "Ocean Age (Young Thermal Regime): " << age_myr << " Myr" << std::endl;

  // Track Physical Libration over 1 Orbital Period (P_orb = 22.56 hours) (linear time scale):
  // Libration angle theta_lib(t) = Amp * sin(n * t)
  std::ofstream out("replications_observational/paper_106/mimas_libration_evolution.csv");
  out << "time_hours,libration_ocean_arcsec,libration_solid_interior_arcsec\n";

  const double p_orb_hr = 22.56;
  const double amp_solid = 24.5; // arcsec for a solid uniform ice interior

  for (double t_hr = 0.0; t_hr <= p_orb_hr; t_hr += 0.3) {
    double mean_anom = (2.0 * M_PI * t_hr) / p_orb_hr;

    // Ocean-decoupled shell libration: Amp = 49.3 arcsec
    double lib_ocean = amp_lib * std::sin(mean_anom);

    // Solid coupled interior: Amp = 24.5 arcsec
    double lib_solid = amp_solid * std::sin(mean_anom);

    out << t_hr << "," << lib_ocean << "," << lib_solid << "\n";
  }
  out.close();

  std::cout << "Generated Mimas Libration Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
