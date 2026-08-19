// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 4: Asymmetric Aerosol Rainout & Day-Night Chemical Quenching in Irradiated Gas Giants

#ifndef CPP_INCLUDE_TERMINATOR_AEROSOL_DISCOVERY_HPP_
#define CPP_INCLUDE_TERMINATOR_AEROSOL_DISCOVERY_HPP_

#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "cpp/include/constants.hpp"

namespace hot_jupiter {

struct AtmosphericLimbState {
  double pressure_bar;
  double temperature_k;
  double silicate_vapor_abundance;  // MgSiO3 mole fraction
  double cloud_condensate_mass_frac;  // Condensed cloud density [kg / kg_gas]
  double mean_droplet_radius_um;  // Aerosol particle size [microns]
  double optical_depth_slant_1um;  // Slant optical depth at 1 micron
  double optical_depth_slant_4um;  // Slant optical depth at 4 micron
};

struct JWSTTransmissionPoint {
  double wavelength_um;
  double transit_depth_morning_ppm;
  double transit_depth_evening_ppm;
  double transit_depth_symmetric_ppm;
  double evening_morning_contrast_ppm;
};

class TerminatorAerosolDiscoveryEngine {
 public:
  TerminatorAerosolDiscoveryEngine()
      : t_eq_planet_k_(1600.0), surface_gravity_m_s2_(10.0), metallicity_dex_(0.0) {}

  TerminatorAerosolDiscoveryEngine(double t_eq, double gravity, double metallicity)
      : t_eq_planet_k_(t_eq), surface_gravity_m_s2_(gravity), metallicity_dex_(metallicity) {}

  // Silicate (MgSiO3) condensation temperature as a function of pressure
  // T_cond(P) = 10000 / (6.5 - 0.2 * log10(P_bar))
  double SilicateCondensationTemperature(double p_bar) const {
    double log_p = std::log10(std::max(1.0e-6, p_bar));
    double denom = 6.5 - 0.2 * log_p;
    return (denom > 0.1) ? (10000.0 / denom) : 2000.0;
  }

  // 1D Limb Temperature-Pressure Profile
  // limb_type: 0 = Dayside, 1 = Evening Terminator, 2 = Morning Terminator, 3 = Nightside
  double EvaluateLimbTemperature(double p_bar, int limb_type) const {
    double t_day = t_eq_planet_k_ * std::sqrt(2.0);  // Dayside ~ 1.414 T_eq
    double t_night = t_eq_planet_k_ * 0.65;          // Nightside ~ 0.65 T_eq

    // Evening limb advects hot gas from dayside: T_eve ~ 0.85 T_day + 0.15 T_night
    double t_eve_strat = 0.85 * t_day + 0.15 * t_night;
    // Morning limb advects cold gas from nightside: T_mor ~ 0.20 T_day + 0.80 T_night
    double t_mor_strat = 0.20 * t_day + 0.80 * t_night;

    double base_temp = 0.0;
    if (limb_type == 0) base_temp = t_day;
    else if (limb_type == 1) base_temp = t_eve_strat;
    else if (limb_type == 2) base_temp = t_mor_strat;
    else base_temp = t_night;

    // Adiabatic / radiative gradient: T(P) = T_0 * (P / 0.1)^0.08
    double p_eff = std::max(1.0e-5, p_bar);
    return base_temp * std::pow(p_eff / 0.1, 0.08);
  }

  // Kinetic Cloud Microphysics: Supersaturation, Nucleation, and Sedimentation
  AtmosphericLimbState ComputeLimbMicrophysics(double p_bar, int limb_type) const {
    AtmosphericLimbState state;
    state.pressure_bar = p_bar;
    state.temperature_k = EvaluateLimbTemperature(p_bar, limb_type);

    double t_cond = SilicateCondensationTemperature(p_bar);
    double solar_silicate_abundance = 4.0e-4 * std::pow(10.0, metallicity_dex_);

    if (state.temperature_k < t_cond) {
      // Condensation occurs
      double supersaturation = (t_cond - state.temperature_k) / t_cond;
      state.cloud_condensate_mass_frac = solar_silicate_abundance * (1.0 - std::exp(-3.0 * supersaturation));
      state.silicate_vapor_abundance = solar_silicate_abundance - state.cloud_condensate_mass_frac;

      // Particle size governed by coagulation vs sedimentation: r_eff ~ 0.5 um * (P / 1 bar)^0.3
      state.mean_droplet_radius_um = 0.5 * std::pow(std::max(1.0e-4, p_bar), 0.25);
      
      // Slant optical depth: tau = kappa * rho * L_slant
      // kappa_ext ~ 3 Q_ext / (4 rho_s r_eff) with Mie scattering Q_ext ~ 2
      double kappa_1um = 1.5 / (3.2e3 * state.mean_droplet_radius_um * 1.0e-6);  // m^2 / kg
      double kappa_4um = kappa_1um * std::pow(1.0 / 4.0, 1.5);  // Rayleigh/Mie transition
      
      double rho_gas = (p_bar * 1.0e5 * 2.3e-3) / (8.314 * state.temperature_k);  // kg/m^3
      double scale_height_m = (8.314 * state.temperature_k) / (2.3e-3 * surface_gravity_m_s2_);
      double slant_factor = std::sqrt(2.0 * M_PI * 7.0e7 / scale_height_m);

      state.optical_depth_slant_1um = kappa_1um * (state.cloud_condensate_mass_frac * rho_gas) * scale_height_m * slant_factor;
      state.optical_depth_slant_4um = kappa_4um * (state.cloud_condensate_mass_frac * rho_gas) * scale_height_m * slant_factor;
    } else {
      // Fully vaporized
      state.cloud_condensate_mass_frac = 0.0;
      state.silicate_vapor_abundance = solar_silicate_abundance;
      state.mean_droplet_radius_um = 0.0;
      state.optical_depth_slant_1um = 0.0;
      state.optical_depth_slant_4um = 0.0;
    }
    return state;
  }

  // Generate synthetic JWST transmission spectrum (0.8 - 5.0 microns)
  std::vector<JWSTTransmissionPoint> ComputeJWSTSpectrum(int num_wavelengths = 150) const {
    std::vector<JWSTTransmissionPoint> spectrum;
    double base_transit_depth_ppm = 15000.0;  // 1.5% transit depth for standard Hot Jupiter
    double scale_height_ppm = 180.0;          // Transit depth change per atmospheric scale height

    for (int i = 0; i < num_wavelengths; ++i) {
      double wl = 0.8 + (5.0 - 0.8) * i / (num_wavelengths - 1.0);
      JWSTTransmissionPoint pt;
      pt.wavelength_um = wl;

      // Molecular absorption cross sections (H2O at 1.4, 1.9, 2.7 um; CO2 at 4.3 um; CO at 4.6 um)
      double h2o_cross = 0.8 * std::exp(-std::pow((wl - 1.4) / 0.15, 2)) +
                         1.2 * std::exp(-std::pow((wl - 1.9) / 0.20, 2)) +
                         2.5 * std::exp(-std::pow((wl - 2.7) / 0.35, 2));
      double co2_cross = 4.0 * std::exp(-std::pow((wl - 4.3) / 0.15, 2));
      double co_cross  = 2.0 * std::exp(-std::pow((wl - 4.65) / 0.20, 2));
      double total_gas_feature = h2o_cross + co2_cross + co_cross;

      // Evening limb (cloud-free, prominent gas features)
      pt.transit_depth_evening_ppm = base_transit_depth_ppm + scale_height_ppm * (total_gas_feature + 0.3 * std::pow(wl / 1.0, -1.0));

      // Morning limb (cloud-deck muting features, flat Mie/Rayleigh slope)
      double cloud_truncation = 0.15;  // Mutes 85% of molecular features
      pt.transit_depth_morning_ppm = base_transit_depth_ppm + scale_height_ppm * (0.8 + cloud_truncation * total_gas_feature + 0.8 * std::pow(wl / 1.0, -0.4));

      // Symmetric average
      pt.transit_depth_symmetric_ppm = 0.5 * (pt.transit_depth_evening_ppm + pt.transit_depth_morning_ppm);
      pt.evening_morning_contrast_ppm = pt.transit_depth_evening_ppm - pt.transit_depth_morning_ppm;

      spectrum.push_back(pt);
    }
    return spectrum;
  }

 private:
  double t_eq_planet_k_;
  double surface_gravity_m_s2_;
  double metallicity_dex_;
};

}  // namespace hot_jupiter

#endif  // CPP_INCLUDE_TERMINATOR_AEROSOL_DISCOVERY_HPP_
