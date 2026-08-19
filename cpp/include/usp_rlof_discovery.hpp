// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 3: Ultra-Short-Period (USP) Planet Tidal Fate & Stable Roche Lobe Stripping

#ifndef CPP_INCLUDE_USP_RLOF_DISCOVERY_HPP_
#define CPP_INCLUDE_USP_RLOF_DISCOVERY_HPP_

#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "cpp/include/constants.hpp"

namespace hot_jupiter {

enum class USPFate {
  CATASTROPHIC_COLLISION,
  STABLE_ROCHE_STRIPPED_REMNANT,
  STABLE_ORBITAL_PARKING
};

struct USPEvolutionState {
  double time_myr;
  double semimajor_axis_au;
  double orbital_period_hours;
  double planet_mass_mearth;
  double core_mass_mearth;
  double mantle_mass_mearth;
  double planet_radius_rearth;
  double roche_radius_au;
  double rlof_mass_loss_rate_mearth_myr;
  double tidal_decay_rate_au_myr;
  bool is_overflowing_roche;
};

class USPRLOFDiscoveryEngine {
 public:
  USPRLOFDiscoveryEngine()
      : star_mass_msun_(1.0), star_radius_rsun_(1.0), k2_q_star_(1.0e-6) {}

  USPRLOFDiscoveryEngine(double m_star, double r_star, double k2_q)
      : star_mass_msun_(m_star), star_radius_rsun_(r_star), k2_q_star_(k2_q) {}

  // Compute Roche radius a_Roche [AU]
  // a_Roche = R_p * (3 * M_star / M_p)^(1/3)
  double RocheRadius(double m_planet_me, double r_planet_re) const {
    double m_p_kg = m_planet_me * M_EARTH;
    double r_p_m = r_planet_re * R_EARTH;
    double m_s_kg = star_mass_msun_ * M_SUN;

    double a_roche_m = r_p_m * std::cbrt(3.0 * m_s_kg / m_p_kg);
    return a_roche_m / AU;
  }

  // Pure stellar tidal orbital decay rate da/dt [AU / Myr]
  // da/dt = -9/2 * sqrt(G/M_star) * (k2/Q)_star * M_p * a^(-11/2) * R_star^5
  double TidalDecayRate(double a_au, double m_planet_me) const {
    double m_s_kg = star_mass_msun_ * M_SUN;
    double r_s_m = star_radius_rsun_ * R_SUN;
    double m_p_kg = m_planet_me * M_EARTH;
    double a_m = a_au * AU;

    double da_dt_m_s = -4.5 * std::sqrt(G / m_s_kg) * k2_q_star_ * m_p_kg
                       * std::pow(a_m, -5.5) * std::pow(r_s_m, 5);
    double da_dt_au_myr = (da_dt_m_s * 3.15576e13) / AU;
    return da_dt_au_myr;
  }

  // Roche Lobe Overflow (RLOF) mass loss rate [M_Earth / Myr]
  double RLOFMassLossRate(double a_au, double m_planet_me, double r_planet_re) const {
    double a_roche = RocheRadius(m_planet_me, r_planet_re);
    if (a_au >= a_roche) {
      return 0.0;
    }
    // Mass loss scales with overfilling factor (R_p - R_Roche)^3
    double delta_r_frac = (a_roche - a_au) / a_roche;
    double mdot = 5.0e3 * m_planet_me * std::pow(delta_r_frac, 2.5);  // M_Earth / Myr
    return std::min(1.0e4, mdot);
  }

  // Differentiated Core-Mantle Radius [R_Earth]
  // Iron core + Silicate mantle (Zaslova et al. 2009)
  double DifferentiatedRadius(double m_core_me, double m_mantle_me) const {
    double m_tot = m_core_me + m_mantle_me;
    if (m_tot <= 1.0e-5) return 0.1;
    double iron_fraction = m_core_me / m_tot;
    // Pure iron: R = 0.78 * M^0.30; Pure silicate: R = 1.05 * M^0.27
    double r_pure_iron = 0.78 * std::pow(m_tot, 0.30);
    double r_pure_silicate = 1.05 * std::pow(m_tot, 0.27);
    return iron_fraction * r_pure_iron + (1.0 - iron_fraction) * r_pure_silicate;
  }

  // Full secular coupled Tidal + RLOF orbital evolution over time
  std::vector<USPEvolutionState> EvolveSystem(double m_core_init_me, double m_mantle_init_me,
                                             double a_init_au, double t_max_myr = 5000.0,
                                             double dt_myr = 0.5) const {
    std::vector<USPEvolutionState> history;
    double a = a_init_au;
    double m_c = m_core_init_me;
    double m_m = m_mantle_init_me;

    for (double t = 0.0; t <= t_max_myr; t += dt_myr) {
      double m_tot = m_c + m_m;
      double r_p = DifferentiatedRadius(m_c, m_m);
      double a_roche = RocheRadius(m_tot, r_p);
      double p_hours = 24.0 * std::sqrt(std::pow(a, 3) / star_mass_msun_) * 365.25;

      double mdot_rlof = RLOFMassLossRate(a, m_tot, r_p);
      double da_dt_tide = TidalDecayRate(a, m_tot);

      // Mass transfer torque on orbit: da/dt_RLOF = +2 * a * (M_dot / M_p) * (1 - sqrt(a_roche / a))
      double da_dt_rlof = (mdot_rlof > 0.0) ? (2.0 * a * (mdot_rlof / m_tot) * 0.15) : 0.0;
      double net_da_dt = da_dt_tide + da_dt_rlof;

      USPEvolutionState state;
      state.time_myr = t;
      state.semimajor_axis_au = a;
      state.orbital_period_hours = p_hours;
      state.planet_mass_mearth = m_tot;
      state.core_mass_mearth = m_c;
      state.mantle_mass_mearth = m_m;
      state.planet_radius_rearth = r_p;
      state.roche_radius_au = a_roche;
      state.rlof_mass_loss_rate_mearth_myr = mdot_rlof;
      state.tidal_decay_rate_au_myr = da_dt_tide;
      state.is_overflowing_roche = (a <= a_roche);
      history.push_back(state);

      // Evolve state
      if (mdot_rlof > 0.0) {
        double delta_m = mdot_rlof * dt_myr;
        if (m_m > delta_m) {
          m_m -= delta_m;
        } else {
          double rem = delta_m - m_m;
          m_m = 0.0;
          m_c = std::max(0.01, m_c - rem * 0.2);
        }
      }

      a += net_da_dt * dt_myr;

      // Check collision with stellar surface (R_star)
      double r_star_au = (star_radius_rsun_ * R_SUN) / AU;
      if (a <= r_star_au || m_tot <= 0.05) {
        break;
      }
    }
    return history;
  }

  // Classify final fate of USP system
  USPFate ClassifyFate(const std::vector<USPEvolutionState>& history) const {
    if (history.empty()) return USPFate::CATASTROPHIC_COLLISION;
    const auto& final_state = history.back();
    double r_star_au = (star_radius_rsun_ * R_SUN) / AU;

    if (final_state.semimajor_axis_au <= r_star_au || final_state.planet_mass_mearth <= 0.1) {
      return USPFate::CATASTROPHIC_COLLISION;
    }
    if (final_state.mantle_mass_mearth <= 0.1 * final_state.core_mass_mearth) {
      return USPFate::STABLE_ROCHE_STRIPPED_REMNANT;  // Bare Super-Mercury
    }
    return USPFate::STABLE_ORBITAL_PARKING;
  }

 private:
  double star_mass_msun_;
  double star_radius_rsun_;
  double k2_q_star_;
};

}  // namespace hot_jupiter

#endif  // CPP_INCLUDE_USP_RLOF_DISCOVERY_HPP_
