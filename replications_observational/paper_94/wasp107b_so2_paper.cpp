// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #94: WASP-107b Puffy Atmosphere & SO2 Photochemistry Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #94: WASP-107b TIDAL INFLATION & SO2 PHOTOCHEMISTRY" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::WASP107bPuffyNeptuneModel model;

  const double m_p = model.planet_mass_mearth();         // ~ 30.5 M_earth
  const double r_p = model.planet_radius_rjup();          // ~ 0.94 R_jup
  const double rho_p = model.bulk_density_g_cm3();        // ~ 0.13 g/cm^3 (cotton-candy density)
  const double x_so2 = model.so2_mixing_ratio();          // ~ 2.0e-5 (20 ppm)
  const double f_tide = model.interior_tidal_flux_w_m2(); // ~ 1.2 W/m^2

  std::cout << "WASP-107b Mass: " << m_p << " M_earth" << std::endl;
  std::cout << "WASP-107b Radius: " << r_p << " R_jup" << std::endl;
  std::cout << "Bulk Density: " << rho_p << " g/cm^3" << std::endl;
  std::cout << "SO2 Photochemical Mixing Ratio: " << x_so2 << " (" << (x_so2 * 1e6) << " ppm)" << std::endl;
  std::cout << "Interior Tidal Dissipation Heat Flux: " << f_tide << " W/m^2" << std::endl;

  // Track JWST Infrared Transmission Spectrum from 3.0 um to 12.0 um (linear wavelength scale):
  // Baseline transit depth ~ 2.05%
  // SO2 absorption peaks at 4.05 um (nu_1 + nu_3) and 8.6 um (nu_3 fundamental)
  // H2O absorption complex at 5.5 - 7.5 um
  // Silicate cloud deck continuum slope
  std::ofstream out("replications_observational/paper_94/wasp107b_jwst_transmission.csv");
  out << "wavelength_um,transit_depth_percent,continuum_transit_depth_percent\n";

  for (double lam = 3.0; lam <= 12.0; lam += 0.05) {
    double depth = 2.05; // Baseline transit depth

    // 4.05 um SO2 feature
    double so2_4um = 0.095 * std::exp(-std::pow((lam - 4.05) / 0.15, 2.0));

    // 8.6 um SO2 feature
    double so2_8um = 0.115 * std::exp(-std::pow((lam - 8.65) / 0.35, 2.0));

    // 5.5 - 7.5 um H2O band
    double h2o_6um = 0.080 * std::exp(-std::pow((lam - 6.20) / 0.70, 2.0));

    // Silicate cloud deck continuum
    double silicate_slope = 0.02 * (1.0 - (lam - 3.0) / 9.0);

    depth += (so2_4um + so2_8um + h2o_6um + silicate_slope);

    out << lam << "," << depth << "," << 2.05 << "\n";
  }
  out.close();

  std::cout << "Generated WASP-107b JWST Transmission Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
