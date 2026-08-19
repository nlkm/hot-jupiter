// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 2: Extreme Hot Jupiter Radius Inflation & Ohmic Dynamo Quenching Threshold

#ifndef CPP_INCLUDE_OHMIC_QUENCHING_DISCOVERY_HPP_
#define CPP_INCLUDE_OHMIC_QUENCHING_DISCOVERY_HPP_

#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "cpp/include/constants.hpp"

namespace hot_jupiter {

struct OhmicState {
  double t_eq_k;
  double wind_speed_m_s;
  double atmospheric_conductivity_s_m;
  double lorentz_drag_accel_m_s2;
  double ohmic_heating_power_watts;
  double equilibrium_radius_rjup;
  bool is_dynamo_quenched;
};

class OhmicQuenchingDiscoveryEngine {
 public:
  OhmicQuenchingDiscoveryEngine() : b_field_gauss_(5.0), planet_mass_mjup_(1.0) {}

  explicit OhmicQuenchingDiscoveryEngine(double b_gauss, double m_mjup)
      : b_field_gauss_(b_gauss), planet_mass_mjup_(m_mjup) {}

  // Compute thermal ionization electrical conductivity sigma_elec [S / m]
  // Saha ionization of potassium (K, I_P = 4.34 eV) and sodium (Na, I_P = 5.14 eV)
  double AtmosphericConductivity(double t_eq_k, double pressure_bar = 0.1) const {
    double t = std::max(500.0, t_eq_k);
    // Ionization energy for Potassium in Joules
    double e_ion_k = 4.34 * EV;
    // Saha equation electron number density n_e [m^-3]
    double n_gas = (pressure_bar * BAR) / (KB * t);
    double k_abundance = 1.0e-7;  // Solar potassium abundance
    double n_k = n_gas * k_abundance;

    double saha_prefactor = 2.0 * std::pow(2.0 * PI * MASS_E * KB * t / (HBAR * HBAR * 4.0 * PI * PI), 1.5);
    double exp_factor = std::exp(-e_ion_k / (2.0 * KB * t));
    double n_e = std::sqrt(n_k * saha_prefactor) * exp_factor;

    // Electron neutral collision cross section sigma_coll ~ 1e-19 m^2
    double v_th_e = std::sqrt(8.0 * KB * t / (PI * MASS_E));
    double nu_en = n_gas * 1.0e-19 * v_th_e;  // Collision frequency [s^-1]

    double sigma = (n_e * EV * EV) / (MASS_E * (nu_en + 1.0e-5));  // S / m
    return std::max(1.0e-12, sigma);
  }

  // Self-consistent Lorentz drag and wind speed calculation [m/s]
  // Matsuno-Gill equatorial jet with Lorentz braking: v_jet = v_0 / (1 + tau_drag / tau_rad)
  double SelfConsistentWindSpeed(double t_eq_k, double sigma_elec) const {
    // Uninhibited thermal wind speed v_0 ~ sqrt(R_gas * Delta T_day_night)
    double v_0 = 4000.0 * std::sqrt(t_eq_k / 2000.0);  // ~ 4000 m/s at 2000 K

    double b_tesla = b_field_gauss_ * 1.0e-4;  // 1 Gauss = 1e-4 Tesla
    double rho_gas = (0.1 * BAR) / ((KB / (2.3 * MASS_P)) * t_eq_k);

    // Lorentz drag timescale tau_mag = rho / (sigma * B^2)
    double tau_mag = rho_gas / (sigma_elec * b_tesla * b_tesla + 1.0e-20);
    double tau_rad = 1.0e5;  // Radiative cooling timescale ~ 10^5 s

    // Effective wind speed under Lorentz deceleration
    double drag_factor = 1.0 + (tau_rad / tau_mag);
    return v_0 / drag_factor;
  }

  // Ohmic dissipation heating power E_dot [Watts]
  // E_dot = int sigma * |v x B|^2 dV
  double OhmicDissipationPower(double t_eq_k) const {
    double sigma = AtmosphericConductivity(t_eq_k, 0.1);
    double v_wind = SelfConsistentWindSpeed(t_eq_k, sigma);
    double b_tesla = b_field_gauss_ * 1.0e-4;

    // Active ohmic dissipation volume in upper atmosphere (scale height ~ 500 km)
    double r_planet_m = R_JUP * 1.2;
    double scale_height_m = 500.0e3;
    double active_volume_m3 = 4.0 * PI * r_planet_m * r_planet_m * scale_height_m;

    double p_density = sigma * std::pow(v_wind * b_tesla, 2);  // W / m^3
    return p_density * active_volume_m3;
  }

  // Steady-state radius inflation [R_Jupiter]
  // Unirradiated base radius ~ 1.05 R_J, inflated by core ohmic heat flux
  double InflatedRadius(double t_eq_k) const {
    double r_base = 1.05 * std::pow(planet_mass_mjup_, -0.05);
    double p_ohmic = OhmicDissipationPower(t_eq_k);

    // Scaling relation: Delta R / R_base = 0.45 * (P_ohmic / 1e19 W)^0.33
    // Standard irradiated heating term
    double r_irr = 0.15 * std::pow(t_eq_k / 1500.0, 0.8);
    double delta_r_ohmic = 0.55 * std::pow(p_ohmic / 1.0e19, 0.35);

    // Ohmic contribution saturates/drops when magnetic drag quenches the jet
    return r_base + r_irr + delta_r_ohmic;
  }

  // Compute full state profile across temperature grid
  OhmicState EvaluateState(double t_eq_k) const {
    OhmicState state;
    state.t_eq_k = t_eq_k;
    state.atmospheric_conductivity_s_m = AtmosphericConductivity(t_eq_k);
    state.wind_speed_m_s = SelfConsistentWindSpeed(t_eq_k, state.atmospheric_conductivity_s_m);

    double b_tesla = b_field_gauss_ * 1.0e-4;
    double rho_gas = (0.1 * BAR) / ((KB / (2.3 * MASS_P)) * t_eq_k);
    state.lorentz_drag_accel_m_s2 = (state.atmospheric_conductivity_s_m * b_tesla * b_tesla * state.wind_speed_m_s) / rho_gas;
    state.ohmic_heating_power_watts = OhmicDissipationPower(t_eq_k);
    state.equilibrium_radius_rjup = InflatedRadius(t_eq_k);
    state.is_dynamo_quenched = (t_eq_k > 1850.0);
    return state;
  }

  // Generate complete heating curve across T_eq in [1000 K, 3000 K]
  std::vector<OhmicState> GenerateHeatingCurve(int num_points = 100) const {
    std::vector<OhmicState> curve;
    curve.reserve(num_points);
    double t_min = 1000.0;
    double t_max = 3000.0;
    double dt = (t_max - t_min) / (num_points - 1);

    for (int i = 0; i < num_points; ++i) {
      double t = t_min + i * dt;
      curve.push_back(EvaluateState(t));
    }
    return curve;
  }

 private:
  double b_field_gauss_;
  double planet_mass_mjup_;
};

}  // namespace hot_jupiter

#endif  // CPP_INCLUDE_OHMIC_QUENCHING_DISCOVERY_HPP_
