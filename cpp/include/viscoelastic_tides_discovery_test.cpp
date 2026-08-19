// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Unit tests for Frontier 8: Frequency-Dependent Andrade Viscoelastic Tidal Dissipation Engine

#include "cpp/include/viscoelastic_tides_discovery.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   TEST SUITE: FREQUENCY-DEPENDENT ANDRADE VISCOELASTIC TIDES ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // Io benchmark (R = 1821.6 km, M = 8.93e22 kg, M_Jup = 1.898e27 kg, a = 4.217e8 m, e = 0.0041)
  hot_jupiter::ViscoelasticTidesDiscoveryEngine io(
      1.8216e6, 8.9319e22, 1.89813e27, 4.217e8, 0.0041, 65.0, 0.30, 1.0);

  // 1. Test Tidal Forcing Frequency (Io orbital period ~ 1.769 days)
  double omega = io.TidalForcingFrequencyRadS();
  double period_days = (2.0 * M_PI / omega) / 86400.0;
  assert(std::abs(period_days - 1.769) < 0.05);
  std::cout << "Io orbital forcing period = " << period_days << " days (omega = " << omega << " rad/s)" << std::endl;

  // 2. Test Mantle Viscosity Scaling
  double eta_1400 = io.MantleViscosityPaS(1400.0);
  double eta_1600 = io.MantleViscosityPaS(1600.0);
  assert(eta_1400 > eta_1600);
  std::cout << "Mantle viscosity at 1400 K = " << eta_1400 << " Pa s, at 1600 K = " << eta_1600 << " Pa s" << std::endl;

  // 3. Test Andrade Complex Love Number & Dissipation Im(k2)
  auto k2_andrade = io.ComputeComplexLoveNumber(omega, 1500.0, hot_jupiter::RheologyModel::ANDRADE);
  assert(k2_andrade.real() > 0.01 && k2_andrade.real() < 1.5);
  assert(std::abs(k2_andrade.imag()) > 1.0e-5 && std::abs(k2_andrade.imag()) < 0.5);
  std::cout << "Andrade k2 = " << k2_andrade.real() << " - i * " << std::abs(k2_andrade.imag()) << std::endl;

  // 4. Test Io Tidal Heating Power (~ 100 TW observational benchmark)
  double power_watts = io.ComputeTidalHeatingPowerWatts(1550.0, hot_jupiter::RheologyModel::ANDRADE);
  double power_tw = power_watts / 1.0e12;
  assert(power_tw > 10.0 && power_tw < 500.0);
  std::cout << "Io tidal heating power at 1550 K = " << power_tw << " TW (Benchmark ~ 100 TW)" << std::endl;

  // 5. Test Thermal Equilibrium Spectrum
  auto spectrum = io.EvaluateThermalSpectrum(1200.0, 1800.0, 25.0, hot_jupiter::RheologyModel::ANDRADE);
  assert(!spectrum.empty());

  std::cout << "================================================================================" << std::endl;
  std::cout << "✅ ALL VISCOELASTIC TIDES DISCOVERY TESTS PASSED!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
