// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #55: TRAPPIST-1e Viscoelastic Tidal Dissipation Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/viscoelastic_tides_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #55: TRAPPIST-1e VISCOELASTIC TIDAL DISSIPATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // TRAPPIST-1e parameters: R = 0.920 R_earth, M = 0.692 M_earth, M_star = 0.0898 M_sun, a = 0.02928 AU, e = 0.0051
  const double r_planet = 0.920 * 6.371e6;
  const double m_planet = 0.692 * 5.972e24;
  const double m_star = 0.0898 * 1.989e30;
  const double a_orb = 0.02928 * 1.496e11;
  const double ecc = 0.0051;

  hot_jupiter::ViscoelasticTidesDiscoveryEngine trappist1e(
      r_planet, m_planet, m_star, a_orb, ecc, 75.0, 0.30, 1.0);

  // Sweep mantle temperature from 1000 K to 2000 K
  auto thermal_curve = trappist1e.EvaluateThermalSpectrum(1000.0, 2000.0, 20.0, hot_jupiter::RheologyModel::ANDRADE);


  std::ofstream out("replications_observational/paper_55/trappist1e_tidal_dissipation.csv");
  out << "temp_k,viscosity_pa_s,im_k2,q_factor,tidal_power_watts,tidal_flux_w_m2,heat_loss_watts\n";
  const double surface_area = 4.0 * M_PI * std::pow(r_planet, 2);

  for (const auto& s : thermal_curve) {
    double tidal_flux = s.tidal_heating_power_watts / surface_area;
    out << s.mantle_temperature_k << "," << s.mantle_viscosity_pa_s << ","
        << s.k2_imag << "," << s.tidal_quality_factor_q << ","
        << s.tidal_heating_power_watts << "," << tidal_flux << ","
        << s.convective_heat_loss_watts << "\n";
  }
  out.close();

  std::cout << "Generated TRAPPIST-1e viscoelastic tidal dissipation simulation data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
