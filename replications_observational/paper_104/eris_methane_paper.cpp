// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #104: (136199) Eris Surface Methane Frost Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #104: (136199) ERIS SURFACE METHANE FROST VOLATILITY" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::ErisSurfaceMethaneModel model;

  const double r_eris = model.eris_radius_km();              // ~ 1163.0 km
  const double m_eris = model.eris_mass_kg();                // ~ 1.66e22 kg
  const double albedo_pv = model.geometric_albedo();         // ~ 0.96 (Brilliant mirror-like ice)
  const double t_surf = model.surface_temp_aphelion_k();     // ~ 30.0 K
  const double f_ch4 = model.methane_ice_fraction();         // ~ 0.15 (15%)
  const double f_n2 = model.nitrogen_ice_fraction();          // ~ 0.85 (85%)

  std::cout << "Eris Radius: " << r_eris << " km" << std::endl;
  std::cout << "Eris Mass: " << m_eris << " kg" << std::endl;
  std::cout << "Geometric Albedo (V-band): " << albedo_pv << std::endl;
  std::cout << "Aphelion Surface Temperature: " << t_surf << " K" << std::endl;
  std::cout << "Methane Frost (CH4) Fraction: " << (f_ch4 * 100.0) << " %" << std::endl;
  std::cout << "Nitrogen Matrix (N2) Fraction: " << (f_n2 * 100.0) << " %" << std::endl;

  // Track NIR Reflectance Spectrum from 1.4 um to 2.5 um (linear wavelength scale):
  // Hapke scattering model with deep CH4 bands at 1.66, 1.72, 2.20, 2.32 um
  std::ofstream out("replications_observational/paper_104/eris_nir_spectrum.csv");
  out << "wavelength_um,relative_reflectance,continuum_albedo\n";

  for (double lam = 1.4; lam <= 2.5; lam += 0.01) {
    double cont = albedo_pv * (1.0 - 0.05 * (lam - 1.4));

    // Methane absorption bands
    double abs_166 = 0.55 * std::exp(-std::pow((lam - 1.66) / 0.035, 2.0));
    double abs_172 = 0.65 * std::exp(-std::pow((lam - 1.72) / 0.040, 2.0));
    double abs_220 = 0.45 * std::exp(-std::pow((lam - 2.20) / 0.030, 2.0));
    double abs_232 = 0.75 * std::exp(-std::pow((lam - 2.32) / 0.045, 2.0));

    double refl = cont * (1.0 - (abs_166 + abs_172 + abs_220 + abs_232));
    if (refl < 0.05) refl = 0.05;

    out << lam << "," << refl << "," << cont << "\n";
  }
  out.close();

  std::cout << "Generated Eris Methane Spectrum Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
