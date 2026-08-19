// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Unit tests for Frontier 4: Terminator Aerosol Asymmetry & JWST Spectra

#include "cpp/include/terminator_aerosol_discovery.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   TEST SUITE: TERMINATOR AEROSOL ASYMMETRY & JWST DISCOVERY ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TerminatorAerosolDiscoveryEngine engine(1600.0, 10.0, 0.0);

  // 1. Test Silicate Condensation Curve
  double t_cond_1bar = engine.SilicateCondensationTemperature(1.0);
  double t_cond_1mbar = engine.SilicateCondensationTemperature(1.0e-3);
  assert(t_cond_1bar > 1400.0 && t_cond_1bar < 1800.0);
  assert(t_cond_1mbar < t_cond_1bar);
  std::cout << "T_cond(1 bar) = " << t_cond_1bar << " K, T_cond(1 mbar) = " << t_cond_1mbar << " K" << std::endl;

  // 2. Test Limb Temperature Asymmetry (Evening > Morning)
  double t_eve = engine.EvaluateLimbTemperature(0.01, 1);
  double t_mor = engine.EvaluateLimbTemperature(0.01, 2);
  assert(t_eve > t_mor);
  std::cout << "T_evening(10 mbar) = " << t_eve << " K > T_morning(10 mbar) = " << t_mor << " K" << std::endl;

  // 3. Test Cloud Microphysics Condensation (Morning cloudy, Evening clear)
  auto state_eve = engine.ComputeLimbMicrophysics(0.01, 1);
  auto state_mor = engine.ComputeLimbMicrophysics(0.01, 2);
  assert(state_mor.cloud_condensate_mass_frac > 0.0);
  assert(state_mor.optical_depth_slant_1um > 1.0);
  assert(state_eve.cloud_condensate_mass_frac == 0.0);
  std::cout << "Morning cloud condensate mass fraction = " << state_mor.cloud_condensate_mass_frac << std::endl;
  std::cout << "Evening cloud condensate mass fraction = " << state_eve.cloud_condensate_mass_frac << " (Vaporized)" << std::endl;

  // 4. Test JWST Synthetic Spectra
  auto spectrum = engine.ComputeJWSTSpectrum(100);
  assert(spectrum.size() == 100);
  assert(spectrum[0].wavelength_um >= 0.8 && spectrum.back().wavelength_um <= 5.0);

  std::cout << "JWST Spectrum computed across " << spectrum.size() << " spectral bins (0.8 - 5.0 um)" << std::endl;
  std::cout << "================================================================================" << std::endl;
  std::cout << "✅ ALL TERMINATOR AEROSOL DISCOVERY TESTS PASSED!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
