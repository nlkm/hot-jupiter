// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 8: Frequency-Dependent Andrade Viscoelastic Tidal Dissipation & Thermal Equilibrium Engine

#ifndef CPP_INCLUDE_VISCOELASTIC_TIDES_DISCOVERY_HPP_
#define CPP_INCLUDE_VISCOELASTIC_TIDES_DISCOVERY_HPP_

#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <complex>
#include "cpp/include/constants.hpp"

namespace hot_jupiter {

enum class RheologyModel {
  MAXWELL,
  ANDRADE,
  SUNDBERG_COOPER,
  CONSTANT_Q
};

struct ViscoelasticTidalState {
  double mantle_temperature_k;
  double mantle_viscosity_pa_s;
  double maxwell_time_yr;
  double k2_real;
  double k2_imag;  // Im(k2) governs dissipation power
  double tidal_quality_factor_q;
  double tidal_heating_power_watts;
  double convective_heat_loss_watts;
  bool is_thermal_equilibrium;
};

class ViscoelasticTidesDiscoveryEngine {
 public:
  ViscoelasticTidesDiscoveryEngine()
      : planet_radius_m(1.8216e6), planet_mass_kg(8.9319e22), star_mass_kg(1.89813e27),
        semi_major_axis_m(4.217e8), eccentricity(0.0041), shear_modulus_gpa(65.0),
        andrade_alpha(0.30), andrade_zeta(1.0) {}

  ViscoelasticTidesDiscoveryEngine(double r_planet, double m_planet, double m_star,
                                   double a_orb, double ecc, double shear_gpa,
                                   double alpha = 0.30, double zeta = 1.0)
      : planet_radius_m(r_planet), planet_mass_kg(m_planet), star_mass_kg(m_star),
        semi_major_axis_m(a_orb), eccentricity(ecc), shear_modulus_gpa(shear_gpa),
        andrade_alpha(alpha), andrade_zeta(zeta) {}

  // Orbital mean motion (tidal forcing frequency) omega = sqrt(G * M_star / a^3)
  double TidalForcingFrequencyRadS() const {
    return std::sqrt(G * star_mass_kg / std::pow(semi_major_axis_m, 3));
  }

  // Olivine/Bridgmanite mantle viscosity: eta(T) = eta_0 * exp( E_act / (R * T) )
  double MantleViscosityPaS(double temp_k) const {
    double t_clamped = std::clamp(temp_k, 800.0, 2200.0);
    double e_act = 300.0e3;  // Activation energy 300 kJ/mol
    double r_gas = 8.314;
    double t_ref = 1600.0;
    double eta_0 = 1.0e16;  // Reference viscosity at 1600 K [Pa s]
    return eta_0 * std::exp((e_act / r_gas) * (1.0 / t_clamped - 1.0 / t_ref));
  }

  // Complex compliance J(omega) = J_1(omega) - i * J_2(omega)
  // Andrade: J(omega) = 1/mu + (1 / (i * omega * eta)) + beta * (i * omega)^(-alpha)
  std::complex<double> AndradeCompliance(double omega_rad_s, double temp_k) const {
    double mu = shear_modulus_gpa * 1.0e9;
    double eta = MantleViscosityPaS(temp_k);
    
    // Elastic compliance
    double j_elastic = 1.0 / mu;
    
    // Fluid viscous compliance
    std::complex<double> j_fluid(0.0, -1.0 / (omega_rad_s * eta));

    // Transient Andrade compliance: beta = (zeta / mu) * (mu / eta)^alpha * Gamma(1 + alpha)
    double gamma_val = std::tgamma(1.0 + andrade_alpha);
    double beta = (andrade_zeta / mu) * std::pow(mu / eta, andrade_alpha) * gamma_val;
    
    double cos_term = std::cos(andrade_alpha * M_PI / 2.0);
    double sin_term = std::sin(andrade_alpha * M_PI / 2.0);
    std::complex<double> j_andrade(beta * std::pow(omega_rad_s, -andrade_alpha) * cos_term,
                                   -beta * std::pow(omega_rad_s, -andrade_alpha) * sin_term);

    return j_elastic + j_fluid + j_andrade;
  }

  // Complex Love number k2(omega) for a homogeneous/core-mantle viscoelastic sphere:
  // k2(omega) = (3/2) / ( 1 + (19/2) * (mu_eff / (rho * g * R)) )
  std::complex<double> ComputeComplexLoveNumber(double omega_rad_s, double temp_k,
                                                RheologyModel model = RheologyModel::ANDRADE) const {
    double rho = planet_mass_kg / ((4.0 / 3.0) * M_PI * std::pow(planet_radius_m, 3));
    double g_surf = G * planet_mass_kg / std::pow(planet_radius_m, 2);
    double mu_base = shear_modulus_gpa * 1.0e9;
    double eta = MantleViscosityPaS(temp_k);

    std::complex<double> j_comp;
    if (model == RheologyModel::ANDRADE) {
      j_comp = AndradeCompliance(omega_rad_s, temp_k);
    } else if (model == RheologyModel::MAXWELL) {
      j_comp = std::complex<double>(1.0 / mu_base, -1.0 / (omega_rad_s * eta));
    } else {
      // Constant Q approximation
      double q_val = 100.0;
      j_comp = std::complex<double>(1.0 / mu_base, -1.0 / (mu_base * q_val));
    }

    std::complex<double> mu_eff = 1.0 / j_comp;
    double hydrostatic_factor = (19.0 / 2.0) / (rho * g_surf * planet_radius_m);
    
    std::complex<double> denom = 1.0 + hydrostatic_factor * mu_eff;
    return 1.5 / denom;
  }

  // Volumetric Tidal Dissipation Heating Power:
  // E_tide = (21/2) * (G * M_star^2 * R_p^5 / a^6) * e^2 * omega * Im(k2)
  double ComputeTidalHeatingPowerWatts(double temp_k, RheologyModel model = RheologyModel::ANDRADE) const {
    double omega = TidalForcingFrequencyRadS();
    std::complex<double> k2 = ComputeComplexLoveNumber(omega, temp_k, model);
    double im_k2 = std::abs(k2.imag());

    double prefactor = (21.0 / 2.0) * G * std::pow(star_mass_kg, 2) * std::pow(planet_radius_m, 5) / std::pow(semi_major_axis_m, 6);
    return prefactor * std::pow(eccentricity, 2) * omega * im_k2;
  }

  // Mantle convective heat loss: F_conv = 4*pi*R_p^2 * q_conv(T) where q_conv ~ C * (T - T_surf)^(4/3)
  double ComputeConvectiveHeatLossWatts(double temp_k, double t_surf_k = 130.0) const {
    double delta_t = std::max(10.0, temp_k - t_surf_k);
    double area = 4.0 * M_PI * std::pow(planet_radius_m, 2);
    // Convective scaling for vigorously convecting silicate mantle (Io ~ 2.5 W/m^2 @ 1600 K)
    double heat_flux = 2.5 * std::pow(delta_t / (1600.0 - 130.0), 4.0 / 3.0);
    return area * heat_flux;
  }

  // Evaluate thermal equilibrium across mantle temperature spectrum (e.g. 1000 K - 2000 K)
  std::vector<ViscoelasticTidalState> EvaluateThermalSpectrum(double t_min_k = 1000.0,
                                                             double t_max_k = 2000.0,
                                                             double dt_k = 10.0,
                                                             RheologyModel model = RheologyModel::ANDRADE) const {
    std::vector<ViscoelasticTidalState> spectrum;
    double omega = TidalForcingFrequencyRadS();

    for (double t = t_min_k; t <= t_max_k; t += dt_k) {
      double eta = MantleViscosityPaS(t);
      double tau_m_yr = (eta / (shear_modulus_gpa * 1.0e9)) / (365.25 * 86400.0);
      auto k2 = ComputeComplexLoveNumber(omega, t, model);
      double q_eff = std::abs(k2.real()) / std::max(1.0e-10, std::abs(k2.imag()));

      double p_tide = ComputeTidalHeatingPowerWatts(t, model);
      double p_conv = ComputeConvectiveHeatLossWatts(t);
      bool eq = std::abs(p_tide - p_conv) / std::max(1.0e10, p_conv) < 0.10;

      ViscoelasticTidalState state;
      state.mantle_temperature_k = t;
      state.mantle_viscosity_pa_s = eta;
      state.maxwell_time_yr = tau_m_yr;
      state.k2_real = k2.real();
      state.k2_imag = std::abs(k2.imag());
      state.tidal_quality_factor_q = q_eff;
      state.tidal_heating_power_watts = p_tide;
      state.convective_heat_loss_watts = p_conv;
      state.is_thermal_equilibrium = eq;
      spectrum.push_back(state);
    }
    return spectrum;
  }

 private:
  double planet_radius_m;
  double planet_mass_kg;
  double star_mass_kg;
  double semi_major_axis_m;
  double eccentricity;
  double shear_modulus_gpa;
  double andrade_alpha;
  double andrade_zeta;
};

}  // namespace hot_jupiter

#endif  // CPP_INCLUDE_VISCOELASTIC_TIDES_DISCOVERY_HPP_
