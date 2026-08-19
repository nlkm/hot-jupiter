// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 4 Execution Driver: Terminator Aerosol Asymmetry & JWST Phase-Resolved Transmission

#include <iostream>
#include <iomanip>
#include <fstream>
#include "cpp/include/terminator_aerosol_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   FRONTIER 4 DISCOVERY: TERMINATOR AEROSOL ASYMMETRY & JWST ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TerminatorAerosolDiscoveryEngine engine(1650.0, 10.0, 0.0);

  // 1. Output Pressure-Temperature-Cloud Microphysics Profiles across Day, Evening, Morning, Night
  std::ofstream out_tp("discovery_campaigns/frontier_04_terminator_aerosol/terminator_profiles.csv");
  out_tp << "pressure_bar,t_cond_k,t_day_k,t_evening_k,t_morning_k,t_night_k,cloud_mor,cloud_eve,r_eff_mor_um,tau_mor_1um\n";

  for (double log_p = -5.0; log_p <= 2.0; log_p += 0.05) {
    double p = std::pow(10.0, log_p);
    double t_cond = engine.SilicateCondensationTemperature(p);
    double t_day = engine.EvaluateLimbTemperature(p, 0);
    double t_eve = engine.EvaluateLimbTemperature(p, 1);
    double t_mor = engine.EvaluateLimbTemperature(p, 2);
    double t_nit = engine.EvaluateLimbTemperature(p, 3);

    auto state_mor = engine.ComputeLimbMicrophysics(p, 2);
    auto state_eve = engine.ComputeLimbMicrophysics(p, 1);

    out_tp << p << "," << t_cond << "," << t_day << "," << t_eve << "," << t_mor << "," << t_nit << ","
           << state_mor.cloud_condensate_mass_frac << "," << state_eve.cloud_condensate_mass_frac << ","
           << state_mor.mean_droplet_radius_um << "," << state_mor.optical_depth_slant_1um << "\n";
  }
  out_tp.close();

  // 2. Output High-Resolution JWST Synthetic Transmission Spectrum (0.8 - 5.0 um)
  auto spectrum = engine.ComputeJWSTSpectrum(300);
  std::ofstream out_spec("discovery_campaigns/frontier_04_terminator_aerosol/jwst_transmission_spectrum.csv");
  out_spec << "wavelength_um,depth_morning_ppm,depth_evening_ppm,depth_symmetric_ppm,contrast_ppm\n";

  for (const auto& pt : spectrum) {
    out_spec << pt.wavelength_um << "," << pt.transit_depth_morning_ppm << ","
             << pt.transit_depth_evening_ppm << "," << pt.transit_depth_symmetric_ppm << ","
             << pt.evening_morning_contrast_ppm << "\n";
  }
  out_spec.close();

  std::cout << "Successfully generated Terminator TP Microphysics and JWST Spectra!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
