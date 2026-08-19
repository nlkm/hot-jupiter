// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 8 Execution Driver: Frequency-Dependent Andrade Viscoelastic Tidal Dissipation Engine

#include <iostream>
#include <iomanip>
#include <fstream>
#include "cpp/include/viscoelastic_tides_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   FRONTIER 8 DISCOVERY: VISCOELASTIC ANDRADE TIDAL DISSIPATION ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // 1. Io Benchmark Spectrum (T = 1000 K - 2000 K) comparing Andrade, Maxwell, and Constant Q
  hot_jupiter::ViscoelasticTidesDiscoveryEngine io(
      1.8216e6, 8.9319e22, 1.89813e27, 4.217e8, 0.0041, 65.0, 0.30, 1.0);

  auto spec_andrade = io.EvaluateThermalSpectrum(1000.0, 2000.0, 10.0, hot_jupiter::RheologyModel::ANDRADE);
  auto spec_maxwell = io.EvaluateThermalSpectrum(1000.0, 2000.0, 10.0, hot_jupiter::RheologyModel::MAXWELL);

  std::ofstream out_io("discovery_campaigns/frontier_08_viscoelastic_tides/io_thermal_spectrum.csv");
  out_io << "temp_k,im_k2_andrade,p_tide_andrade_tw,im_k2_maxwell,p_tide_maxwell_tw,p_conv_tw\n";

  for (size_t i = 0; i < spec_andrade.size(); ++i) {
    double t = spec_andrade[i].mantle_temperature_k;
    double im_and = spec_andrade[i].k2_imag;
    double p_and = spec_andrade[i].tidal_heating_power_watts / 1.0e12;
    double im_max = spec_maxwell[i].k2_imag;
    double p_max = spec_maxwell[i].tidal_heating_power_watts / 1.0e12;
    double p_conv = spec_andrade[i].convective_heat_loss_watts / 1.0e12;

    out_io << t << "," << im_and << "," << p_and << "," << im_max << "," << p_max << "," << p_conv << "\n";
  }
  out_io.close();

  // 2. Frequency Response across Tidal Periods (0.1 - 100 days)
  std::ofstream out_freq("discovery_campaigns/frontier_08_viscoelastic_tides/frequency_response.csv");
  out_freq << "period_days,im_k2_1400k,im_k2_1600k,im_k2_1800k\n";

  for (double log_p = -1.0; log_p <= 2.0; log_p += 0.05) {
    double p_days = std::pow(10.0, log_p);
    double omega = (2.0 * M_PI) / (p_days * 86400.0);
    double im_1400 = std::abs(io.ComputeComplexLoveNumber(omega, 1400.0, hot_jupiter::RheologyModel::ANDRADE).imag());
    double im_1600 = std::abs(io.ComputeComplexLoveNumber(omega, 1600.0, hot_jupiter::RheologyModel::ANDRADE).imag());
    double im_1800 = std::abs(io.ComputeComplexLoveNumber(omega, 1800.0, hot_jupiter::RheologyModel::ANDRADE).imag());

    out_freq << p_days << "," << im_1400 << "," << im_1600 << "," << im_1800 << "\n";
  }
  out_freq.close();

  // 3. TRAPPIST-1e / Super-Earth Tidal Heating Map across (a_au, eccentricity)
  std::ofstream out_map("discovery_campaigns/frontier_08_viscoelastic_tides/trappist1e_heating_map.csv");
  out_map << "semi_major_axis_au,eccentricity,heat_flux_w_m2,is_runaway\n";

  // TRAPPIST-1 star mass = 0.0898 M_sun, TRAPPIST-1e R = 0.92 R_earth, M = 0.69 M_earth
  double r_t1e = 0.92 * hot_jupiter::R_EARTH;
  double m_t1e = 0.69 * hot_jupiter::M_EARTH;
  double m_star = 0.0898 * hot_jupiter::M_SUN;

  for (double a_au = 0.015; a_au <= 0.060; a_au += 0.002) {
    for (double ecc = 0.001; ecc <= 0.150; ecc += 0.005) {
      hot_jupiter::ViscoelasticTidesDiscoveryEngine t1e(
          r_t1e, m_t1e, m_star, a_au * hot_jupiter::AU, ecc, 75.0, 0.30, 1.0);
      double p_tide = t1e.ComputeTidalHeatingPowerWatts(1600.0, hot_jupiter::RheologyModel::ANDRADE);
      double flux = p_tide / (4.0 * M_PI * std::pow(r_t1e, 2));
      bool runaway = flux > 10.0;  // > 10 W/m^2 triggers catastrophic runaway volcanism
      out_map << a_au << "," << ecc << "," << flux << "," << (runaway ? 1 : 0) << "\n";
    }
  }
  out_map.close();

  std::cout << "Successfully generated Viscoelastic Tides Spectrum and TRAPPIST-1 Heating Maps!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
