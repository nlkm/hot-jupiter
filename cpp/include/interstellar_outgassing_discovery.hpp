// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 7: Interstellar Object Volatile Depletion, Outgassing Torques, & Spin Disruption Engine

#ifndef CPP_INCLUDE_INTERSTELLAR_OUTGASSING_DISCOVERY_HPP_
#define CPP_INCLUDE_INTERSTELLAR_OUTGASSING_DISCOVERY_HPP_

#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "cpp/include/constants.hpp"

namespace hot_jupiter {

enum class VolatileIceType {
  H2O_WATER,
  CO_CARBON_MONOXIDE,
  N2_NITROGEN,
  H2_MOLECULAR_HYDROGEN
};

struct OutgassingState {
  double heliocentric_dist_au;
  double surface_temp_k;
  double sublimation_rate_kg_m2_s;
  double non_grav_accel_m_s2;
  double spin_period_hours;
  double centrifugal_stress_pa;
  bool is_tensile_disrupted;
};

class InterstellarOutgassingDiscoveryEngine {
 public:
  InterstellarOutgassingDiscoveryEngine()
      : eff_radius_m(100.0), axis_ratio_a_over_b(6.0), bulk_density_kg_m3(300.0),
        porosity_fraction(0.70), tensile_strength_pa(10.0), ice_type(VolatileIceType::H2_MOLECULAR_HYDROGEN) {}

  InterstellarOutgassingDiscoveryEngine(double radius_m, double aspect_ratio, double density,
                                        double porosity, double tensile_pa, VolatileIceType ice)
      : eff_radius_m(radius_m), axis_ratio_a_over_b(aspect_ratio), bulk_density_kg_m3(density),
        porosity_fraction(porosity), tensile_strength_pa(tensile_pa), ice_type(ice) {}

  // Sublimation latent heat [J / kg] and molecular mass [kg]
  double LatentHeatJPerKg() const {
    switch (ice_type) {
      case VolatileIceType::H2O_WATER: return 2.8e6;
      case VolatileIceType::CO_CARBON_MONOXIDE: return 2.0e5;
      case VolatileIceType::N2_NITROGEN: return 2.3e5;
      case VolatileIceType::H2_MOLECULAR_HYDROGEN: return 4.5e5;
    }
    return 2.8e6;
  }

  double MeanMolecularMassKg() const {
    double n_a = 6.02214076e23;
    switch (ice_type) {
      case VolatileIceType::H2O_WATER: return 18.015e-3 / n_a;
      case VolatileIceType::CO_CARBON_MONOXIDE: return 28.01e-3 / n_a;
      case VolatileIceType::N2_NITROGEN: return 28.013e-3 / n_a;
      case VolatileIceType::H2_MOLECULAR_HYDROGEN: return 2.016e-3 / n_a;
    }
    return 18.015e-3 / n_a;
  }

  // Equilibrium surface temperature at distance r_au
  double SurfaceTemperatureK(double r_au, double albedo = 0.05) const {
    double solar_const = 1361.0 / (r_au * r_au);
    double t_eq = std::pow((1.0 - albedo) * solar_const / (4.0 * SIGMA_SB), 0.25);
    return std::max(5.0, t_eq);
  }

  // Energy-balance sublimation mass flux: Z = (1 - A) F_sun / (L_sub + C_p * Delta T)
  double SublimationFluxKgM2S(double r_au) const {
    double solar_const = 1361.0 / (r_au * r_au);
    double l_sub = LatentHeatJPerKg();
    return (0.95 * solar_const) / (l_sub * 4.0);
  }

  // Thermal gas exhaust velocity: v_th = sqrt( 8 k_B T / (pi m) )
  double ThermalExhaustVelocityMS(double temp_k) const {
    double m_mol = MeanMolecularMassKg();
    return std::sqrt(8.0 * KB * temp_k / (M_PI * m_mol));
  }


  // Non-gravitational acceleration: a_ng = f_outgas * (Z * A_cross * v_th) / M_body
  double ComputeNonGravAcceleration(double r_au, double f_outgas_anisotropy = 0.25) const {
    double temp = SurfaceTemperatureK(r_au);
    double z_flux = SublimationFluxKgM2S(r_au);
    double v_th = ThermalExhaustVelocityMS(temp);
    
    double body_mass = (4.0 / 3.0) * M_PI * std::pow(eff_radius_m, 3) * bulk_density_kg_m3;
    double cross_area = M_PI * std::pow(eff_radius_m, 2);

    double thrust_force = f_outgas_anisotropy * z_flux * cross_area * v_th;
    return thrust_force / std::max(1.0, body_mass);
  }

  // Outgassing torque and spin-rate evolution: d(omega)/dt = Torque / I_moment
  double ComputeSpinupTorque(double r_au, double lever_arm_frac = 0.20) const {
    double temp = SurfaceTemperatureK(r_au);
    double z_flux = SublimationFluxKgM2S(r_au);
    double v_th = ThermalExhaustVelocityMS(temp);
    double cross_area = M_PI * std::pow(eff_radius_m, 2);
    
    double outgas_force = 0.15 * z_flux * cross_area * v_th;
    double lever_arm = lever_arm_frac * eff_radius_m * axis_ratio_a_over_b;
    return outgas_force * lever_arm;
  }

  // Centrifugal tensile stress on elongated body: sigma_cent = 0.25 * rho * omega^2 * a_long^2
  double CentrifugalTensileStressPa(double omega_rad_s) const {
    double a_long = eff_radius_m * std::sqrt(axis_ratio_a_over_b);
    return 0.25 * bulk_density_kg_m3 * std::pow(omega_rad_s, 2) * std::pow(a_long, 2);
  }

  // Trajectory orbital evolution across perihelion passage (e.g. 'Oumuamua q = 0.255 AU)
  std::vector<OutgassingState> EvolveFlyby(double q_perihelion_au = 0.255,
                                          double initial_spin_period_hrs = 8.14,
                                          double t_span_days = 120.0,
                                          double dt_days = 0.2) const {
    std::vector<OutgassingState> history;
    double omega = 2.0 * M_PI / (initial_spin_period_hrs * 3600.0);
    double body_mass = (4.0 / 3.0) * M_PI * std::pow(eff_radius_m, 3) * bulk_density_kg_m3;
    double a_long = eff_radius_m * std::sqrt(axis_ratio_a_over_b);
    double moment_inertia = 0.20 * body_mass * std::pow(a_long, 2);

    for (double t_day = -t_span_days / 2.0; t_day <= t_span_days / 2.0; t_day += dt_days) {
      // Hyperbolic orbit approximation: r(t) = sqrt( q^2 + (v_inf * t)^2 )
      double v_inf_au_day = 0.15;  // ~26 km/s
      double r_au = std::sqrt(std::pow(q_perihelion_au, 2) + std::pow(v_inf_au_day * t_day, 2));

      double temp = SurfaceTemperatureK(r_au);
      double z_flux = SublimationFluxKgM2S(r_au);
      double a_ng = ComputeNonGravAcceleration(r_au, 0.20);
      double torque = ComputeSpinupTorque(r_au, 0.10);

      // Spin acceleration
      double d_omega_dt = torque / moment_inertia;
      omega += d_omega_dt * (dt_days * 86400.0);
      double current_p_hrs = (2.0 * M_PI / std::max(1.0e-7, omega)) / 3600.0;

      double sigma_cent = CentrifugalTensileStressPa(omega);
      bool disrupted = sigma_cent >= tensile_strength_pa;

      OutgassingState state;
      state.heliocentric_dist_au = r_au;
      state.surface_temp_k = temp;
      state.sublimation_rate_kg_m2_s = z_flux;
      state.non_grav_accel_m_s2 = a_ng;
      state.spin_period_hours = current_p_hrs;
      state.centrifugal_stress_pa = sigma_cent;
      state.is_tensile_disrupted = disrupted;
      history.push_back(state);
    }
    return history;
  }

 private:
  double eff_radius_m;
  double axis_ratio_a_over_b;
  double bulk_density_kg_m3;
  double porosity_fraction;
  double tensile_strength_pa;
  VolatileIceType ice_type;
};

}  // namespace hot_jupiter

#endif  // CPP_INCLUDE_INTERSTELLAR_OUTGASSING_DISCOVERY_HPP_
