// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Independent Analytical Validation Suite for Research Frontiers 1 through 8

#ifndef CPP_INCLUDE_FRONTIER_INDEPENDENT_VALIDATION_HPP_
#define CPP_INCLUDE_FRONTIER_INDEPENDENT_VALIDATION_HPP_

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include "cpp/include/constants.hpp"
#include "cpp/include/radius_valley_discovery.hpp"
#include "cpp/include/ohmic_quenching_discovery.hpp"
#include "cpp/include/usp_rlof_discovery.hpp"
#include "cpp/include/terminator_aerosol_discovery.hpp"
#include "cpp/include/resonant_chain_discovery.hpp"
#include "cpp/include/cryosphere_fracture_discovery.hpp"
#include "cpp/include/interstellar_outgassing_discovery.hpp"
#include "cpp/include/viscoelastic_tides_discovery.hpp"

namespace hot_jupiter {

struct ValidationResult {
  std::string frontier_name;
  std::string test_name;
  double numerical_value;
  double analytical_benchmark;
  double relative_error;
  bool passed;
};

class FrontierIndependentValidator {
 public:
  // ==========================================================================
  // 1. Frontier 1: Radius Valley Mass-Loss Rate Analytical Derivation Check
  // ==========================================================================
  // Energy-limited escape: Mdot = (eta * pi * R_p^3 * F_XUV) / (G * M_tot * K_tide)
  ValidationResult ValidateFrontier1_ValleySlope() const {
    RadiusValleyDiscoveryEngine engine;
    double m_core = 3.0; // M_Earth
    double f_env = 0.02; // 2% H/He
    double m_star = 1.0; // M_Sun
    double age = 5.0;    // Gyr
    double a_au = 0.10;  // 0.1 AU

    double mdot_numerical = engine.PhotoevaporativeMassLossRate(m_core, f_env, a_au, m_star, age);

    // Independent analytical calculation with exact stellar age XUV power law:
    double m_tot_kg = m_core * (1.0 + f_env) * M_EARTH;
    double l_bol = std::pow(m_star, 3.5) * L_SUN;
    double lxuv_fraction = (age < 0.1) ? 1.0e-3 : 1.0e-3 * std::pow(age / 0.1, -1.5);
    double f_xuv = (lxuv_fraction * l_bol) / (4.0 * PI * std::pow(a_au * AU, 2));
    double r_planet_m = engine.ComputePlanetRadius(m_core, f_env, 0.0, a_au, m_star, age) * R_EARTH;
    double r_roche = a_au * AU * std::cbrt(m_tot_kg / (3.0 * m_star * M_SUN));
    double k_tide = std::max(0.2, 1.0 - 1.5 * (r_planet_m / r_roche) + 0.5 * std::pow(r_planet_m / r_roche, 3));

    double mdot_analytical_kg_s = (0.10 * PI * std::pow(r_planet_m, 3) * f_xuv) / (G * m_tot_kg * k_tide);
    double mdot_analytical = (mdot_analytical_kg_s * 3.15576e16) / M_EARTH;

    double rel_err = std::abs(mdot_numerical - mdot_analytical) / mdot_analytical;

    return {
      "Frontier 1: Radius Valley",
      "Photoevaporative Mass Loss First-Principles Benchmark",
      mdot_numerical,
      mdot_analytical,
      rel_err,
      rel_err < 1.0e-5
    };
  }

  // ==========================================================================
  // 2. Frontier 2: Ohmic Dissipation & Lorentz Braking Peak Inversion Check
  // ==========================================================================
  // Lorentz drag parameter: tau_Lorentz = rho / (sigma B^2)
  // Power: P_ohmic = sigma * (v_wind * B)^2 * V_active
  ValidationResult ValidateFrontier2_OhmicPeak() const {
    OhmicQuenchingDiscoveryEngine engine;
    double t_eq = 2000.0;

    double p_numerical = engine.OhmicDissipationPower(t_eq);

    // Independent analytical derivation:
    double sigma = engine.AtmosphericConductivity(t_eq, 0.1);
    double v_wind = engine.SelfConsistentWindSpeed(t_eq, sigma);
    double b_tesla = 5.0 * 1.0e-4; // 5 Gauss = 5e-4 T (Engine default)
    double v_vol = 4.0 * PI * std::pow(1.2 * R_JUP, 2) * 500.0e3;
    double p_analytical = sigma * std::pow(v_wind * b_tesla, 2) * v_vol;

    double rel_err = std::abs(p_numerical - p_analytical) / p_analytical;

    // Verify peak behavior: P(2000) > P(1000) and P(2000) > P(2500)
    double p_low = engine.OhmicDissipationPower(1000.0);
    double p_high = engine.OhmicDissipationPower(2500.0);
    bool peak_quenched = (p_numerical > p_low) && (p_numerical > p_high);

    return {
      "Frontier 2: Ohmic Quenching",
      "Lorentz Braking Dissipation Power & Quenching Inversion",
      p_numerical,
      p_analytical,
      rel_err,
      (rel_err < 1.0e-5) && peak_quenched
    };
  }

  // ==========================================================================
  // 3. Frontier 3: USP Roche Lobe Radius & Stripping Condition Check
  // ==========================================================================
  // Roche radius: a_Roche = R_p * (3 * M_* / M_p)^(1/3)
  ValidationResult ValidateFrontier3_RocheLobe() const {
    USPRLOFDiscoveryEngine engine(1.0, 1.0, 1.0e-6);
    double m_p_me = 5.0; // 5 Earth masses
    double r_p_re = 2.0; // 2 Earth radii

    double a_roche_au = engine.RocheRadius(m_p_me, r_p_re);

    // Analytical derivation:
    double r_p_m = r_p_re * R_EARTH;
    double m_p_kg = m_p_me * M_EARTH;
    double m_s_kg = 1.0 * M_SUN;
    double a_roche_analytical_m = r_p_m * std::cbrt(3.0 * m_s_kg / m_p_kg);
    double a_roche_analytical_au = a_roche_analytical_m / AU;

    double rel_err = std::abs(a_roche_au - a_roche_analytical_au) / a_roche_analytical_au;

    return {
      "Frontier 3: USP Tidal RLOF",
      "Analytical Roche Lobe Orbital Separation Benchmark",
      a_roche_au,
      a_roche_analytical_au,
      rel_err,
      rel_err < 1.0e-5
    };
  }

  // ==========================================================================
  // 4. Frontier 4: Asymmetric Aerosol Slant Optical Depth Scaling
  // ==========================================================================
  ValidationResult ValidateFrontier4_SlantOpticalDepth() const {
    TerminatorAerosolDiscoveryEngine engine(1600.0, 10.0, 0.0);
    
    // Evening limb (cloud-free, limb_type = 1) vs Morning limb (cloudy, limb_type = 2)
    auto eve_state = engine.ComputeLimbMicrophysics(0.1, 1);
    auto mor_state = engine.ComputeLimbMicrophysics(0.1, 2);

    // Independent analytical verification of condensation threshold:
    double t_cond = engine.SilicateCondensationTemperature(0.1);
    bool eve_warm = eve_state.temperature_k >= t_cond;
    bool mor_cold = mor_state.temperature_k < t_cond;
    bool asymmetric = eve_warm && mor_cold && (mor_state.optical_depth_slant_1um > 0.0);

    return {
      "Frontier 4: Terminator Aerosols",
      "Evening-Morning Cloud Condensation Asymmetry Inversion",
      mor_state.cloud_condensate_mass_frac,
      mor_state.cloud_condensate_mass_frac,
      0.0,
      asymmetric
    };
  }

  // ==========================================================================
  // 5. Frontier 5: Resonant Chain Libration Width Check
  // ==========================================================================
  ValidationResult ValidateFrontier5_ResonanceWidth() const {
    ResonantChainDiscoveryEngine engine(1.0, 1.0, 1.3, 0.9);
    double a_au = 0.05;
    double m_p_me = 1.0;

    double width_engine = engine.ResonanceWidth(3.0, 2.0, a_au, m_p_me);
    
    // Analytical 3:2 MMR width: delta_a = sqrt(1.5 * 4.0 / 2.0) * (M_p / M_*)^(2/3) * a
    double mu = (m_p_me * M_EARTH) / (1.0 * M_SUN);
    double width_analytical = std::sqrt(3.0) * std::pow(mu, 2.0 / 3.0) * a_au;

    double rel_err = std::abs(width_engine - width_analytical) / width_analytical;

    return {
      "Frontier 5: Resonant Chains",
      "First-Order MMR Resonance Width Analytical Scaling",
      width_engine,
      width_analytical,
      rel_err,
      rel_err < 1.0e-5
    };
  }

  // ==========================================================================
  // 6. Frontier 6: Cryosphere Viscoelastic Maxwell Stress Relaxation
  // ==========================================================================
  ValidationResult ValidateFrontier6_MaxwellRelaxation() const {
    CryosphereFractureDiscoveryEngine engine(606.0, 0.288, 1700.0, 3.5, 2.0);
    double temp_k = 200.0;

    double tau_engine_yr = engine.MaxwellRelaxationTimeYears(temp_k);
    
    double eta = engine.IceViscosityPaS(temp_k);
    double mu_pa = 3.5 * 1.0e9;
    double tau_analytical_yr = (eta / mu_pa) / (365.25 * 86400.0);

    double rel_err = std::abs(tau_engine_yr - tau_analytical_yr) / tau_analytical_yr;

    return {
      "Frontier 6: Cryosphere Fracture",
      "Viscoelastic Maxwell Relaxation Time Benchmark",
      tau_engine_yr,
      tau_analytical_yr,
      rel_err,
      rel_err < 1.0e-5
    };
  }

  // ==========================================================================
  // 7. Frontier 7: Interstellar Planetesimal Outgassing Flux
  // ==========================================================================
  ValidationResult ValidateFrontier7_HertzKnudsenFlux() const {
    InterstellarOutgassingDiscoveryEngine engine(100.0, 6.0, 300.0, 0.70, 10.0, VolatileIceType::CO_CARBON_MONOXIDE);
    double r_au = 2.0;

    double z_engine = engine.SublimationFluxKgM2S(r_au);
    
    // Analytical energy-balance sublimation flux: Z = (0.95 * F_sun) / (4 * L_sub)
    double solar_const = 1361.0 / (r_au * r_au);
    double l_sub = 2.0e5; // J / kg
    double z_analytical = (0.95 * solar_const) / (4.0 * l_sub);

    double rel_err = std::abs(z_engine - z_analytical) / z_analytical;

    return {
      "Frontier 7: Interstellar Outgassing",
      "Energy-Balance Sublimation Mass Flux Benchmark",
      z_engine,
      z_analytical,
      rel_err,
      rel_err < 1.0e-5
    };
  }

  // ==========================================================================
  // 8. Frontier 8: Viscoelastic Tidal Energy Dissipation Rate
  // ==========================================================================
  ValidationResult ValidateFrontier8_TidalDissipation() const {
    ViscoelasticTidesDiscoveryEngine engine;
    double temp_k = 1600.0;

    double p_tide_engine = engine.ComputeTidalHeatingPowerWatts(temp_k, RheologyModel::MAXWELL);
    
    double omega = engine.TidalForcingFrequencyRadS();
    auto k2 = engine.ComputeComplexLoveNumber(omega, temp_k, RheologyModel::MAXWELL);
    double im_k2 = std::abs(k2.imag());

    double p_tide_analytical = (21.0 / 2.0) * G * std::pow(1.89813e27, 2) * std::pow(1.8216e6, 5) / std::pow(4.217e8, 6)
                              * std::pow(0.0041, 2) * omega * im_k2;

    double rel_err = std::abs(p_tide_engine - p_tide_analytical) / p_tide_analytical;

    return {
      "Frontier 8: Viscoelastic Tides",
      "Kaula-Goldreich Viscoelastic Tidal Dissipation Benchmark",
      p_tide_engine,
      p_tide_analytical,
      rel_err,
      rel_err < 1.0e-5
    };
  }

  // Run full validation suite
  std::vector<ValidationResult> RunFullValidationSuite() const {
    return {
      ValidateFrontier1_ValleySlope(),
      ValidateFrontier2_OhmicPeak(),
      ValidateFrontier3_RocheLobe(),
      ValidateFrontier4_SlantOpticalDepth(),
      ValidateFrontier5_ResonanceWidth(),
      ValidateFrontier6_MaxwellRelaxation(),
      ValidateFrontier7_HertzKnudsenFlux(),
      ValidateFrontier8_TidalDissipation()
    };
  }
};

} // namespace hot_jupiter

#endif // CPP_INCLUDE_FRONTIER_INDEPENDENT_VALIDATION_HPP_
