// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 1: Sub-Neptune Radius Valley Population Synthesis & Mass-Loss Disentanglement

#ifndef CPP_INCLUDE_RADIUS_VALLEY_DISCOVERY_HPP_
#define CPP_INCLUDE_RADIUS_VALLEY_DISCOVERY_HPP_

#include <vector>
#include <cmath>
#include <string>
#include <random>
#include <algorithm>
#include "cpp/include/constants.hpp"

namespace hot_jupiter {

enum class ValleyMechanism {
  PHOTOEVAPORATION,
  CORE_POWERED_MASS_LOSS,
  PRIMORDIAL_WATER_WORLDS,
  HYBRID_UNIFIED
};

struct SyntheticPlanet {
  double star_mass_msun;
  double orbital_period_days;
  double semimajor_axis_au;
  double core_mass_mearth;
  double envelope_mass_fraction_initial;
  double envelope_mass_fraction_final;
  double water_mass_fraction;
  double initial_radius_rearth;
  double final_radius_rearth;
  double age_gyr;
  bool stripped_to_bare_core;
  ValleyMechanism dominant_loss_channel;
};

class RadiusValleyDiscoveryEngine {
 public:
  RadiusValleyDiscoveryEngine() : gen_(42) {}

  // Compute photoevaporation mass loss rate [M_Earth / Gyr]
  double PhotoevaporativeMassLossRate(double m_core_me, double f_env,
                                     double a_au, double m_star_msun,
                                     double age_gyr) const {
    double m_tot = m_core_me * (1.0 + f_env);
    // Stellar XUV luminosity evolution: L_xuv / L_bol = 1e-3 (age < 100 Myr), t^-1.5 thereafter
    double lxuv_fraction = (age_gyr < 0.1) ? 1.0e-3 : 1.0e-3 * std::pow(age_gyr / 0.1, -1.5);
    double l_bol = std::pow(m_star_msun, 3.5) * L_SUN;  // Watts
    double f_xuv = (lxuv_fraction * l_bol) / (4.0 * PI * std::pow(a_au * AU, 2));

    double eff = 0.10;  // 10% photoevaporative efficiency
    double r_planet_m = ComputePlanetRadius(m_core_me, f_env, 0.0, a_au, m_star_msun, age_gyr) * R_EARTH;
    double m_tot_kg = m_tot * M_EARTH;

    // Tidal enhancement factor K_tide
    double r_roche = a_au * AU * std::cbrt(m_tot_kg / (3.0 * m_star_msun * M_SUN));
    double k_tide = 1.0 - (3.0 / 2.0) * (r_planet_m / r_roche) + (1.0 / 2.0) * std::pow(r_planet_m / r_roche, 3);
    k_tide = std::max(0.2, k_tide);

    double mdot_kg_s = (eff * PI * std::pow(r_planet_m, 3) * f_xuv) / (G * m_tot_kg * k_tide);
    double mdot_me_gyr = (mdot_kg_s * 3.15576e16) / M_EARTH;
    return std::max(0.0, mdot_me_gyr);
  }

  // Compute core-powered mass loss rate [M_Earth / Gyr]
  double CorePoweredMassLossRate(double m_core_me, double f_env,
                                double a_au, double m_star_msun,
                                double age_gyr) const {
    // Planet equilibrium temperature
    double t_eq = 278.0 * std::pow(m_star_msun, 0.875) / std::sqrt(a_au);
    double m_tot = m_core_me * (1.0 + f_env);
    double m_tot_kg = m_tot * M_EARTH;

    // Thermal sound speed in hydrogen-helium envelope (mu ~ 2.3 amu)
    double c_s = std::sqrt(KB * t_eq / (2.35 * MASS_P));

    // Core cooling luminosity: L_core = E_thermal / (C * age) (Gupta & Schlichting 2019)
    double e_core = 1.0e31 * m_core_me;  // Joules
    double l_core = e_core / (age_gyr * 3.15576e16 + 1.0e14);
    double r_planet_m = ComputePlanetRadius(m_core_me, f_env, 0.0, a_au, m_star_msun, age_gyr) * R_EARTH;

    double phi_grav = G * m_tot_kg / r_planet_m;
    double thermal_enhancement = 1.0 + std::pow(c_s, 2) / std::max(1.0e4, phi_grav);

    double mdot_kg_s = (l_core / phi_grav) * thermal_enhancement;
    double mdot_me_gyr = (mdot_kg_s * 3.15576e16) / M_EARTH;
    return std::max(0.0, mdot_me_gyr);
  }


  // Compute composite planet radius [R_Earth] (Core + H/He + Water)
  double ComputePlanetRadius(double m_core_me, double f_env, double f_water,
                             double a_au, double m_star_msun, double age_gyr) const {
    // Bare rocky/iron core radius (Fortney et al. 2007, Lopez & Fortney 2014)
    double r_core = std::pow(m_core_me * (1.0 - f_water), 0.27);
    if (f_water > 0.0) {
      double r_water = 1.35 * std::pow(m_core_me * f_water, 0.29);
      r_core = std::cbrt(std::pow(r_core, 3) + std::pow(r_water, 3));
    }
    if (f_env <= 1.0e-5) {
      return r_core;
    }

    // Atmospheric envelope thickness (Lopez & Fortney 2014 scaling)
    double flux = std::pow(m_star_msun, 3.5) / (a_au * a_au);
    double r_env = 2.06 * std::pow(m_core_me, -0.21) * std::pow(f_env / 0.05, 0.59)
                   * std::pow(flux, 0.044) * std::pow(age_gyr / 5.0, -0.18);
    return r_core + r_env;
  }

  // Generate synthetic population of N planets under given mechanism
  std::vector<SyntheticPlanet> GeneratePopulation(int n_planets, ValleyMechanism mechanism) {
    std::vector<SyntheticPlanet> pop;
    pop.reserve(n_planets);

    std::uniform_real_distribution<double> dist_star_m(0.3, 1.2);
    std::uniform_real_distribution<double> dist_log_p(std::log10(1.0), std::log10(100.0));
    std::uniform_real_distribution<double> dist_log_mcore(std::log10(1.0), std::log10(20.0));
    std::uniform_real_distribution<double> dist_fenv(0.005, 0.08);
    std::uniform_real_distribution<double> dist_fwater(0.20, 0.50);
    std::uniform_real_distribution<double> dist_age(1.0, 10.0);

    for (int i = 0; i < n_planets; ++i) {
      SyntheticPlanet p;
      p.star_mass_msun = dist_star_m(gen_);
      p.orbital_period_days = std::pow(10.0, dist_log_p(gen_));
      p.semimajor_axis_au = std::cbrt(std::pow(p.orbital_period_days / 365.25, 2) * p.star_mass_msun);
      p.core_mass_mearth = std::pow(10.0, dist_log_mcore(gen_));
      p.envelope_mass_fraction_initial = dist_fenv(gen_);
      p.age_gyr = dist_age(gen_);
      p.water_mass_fraction = 0.0;
      p.dominant_loss_channel = mechanism;

      if (mechanism == ValleyMechanism::PRIMORDIAL_WATER_WORLDS) {
        if (i % 2 == 0) {
          p.water_mass_fraction = 0.0;
          p.envelope_mass_fraction_initial = 1.0e-5;
        } else {
          p.water_mass_fraction = dist_fwater(gen_);
          p.envelope_mass_fraction_initial = 1.0e-5;
        }
        p.envelope_mass_fraction_final = p.envelope_mass_fraction_initial;
        p.initial_radius_rearth = ComputePlanetRadius(p.core_mass_mearth, p.envelope_mass_fraction_initial, p.water_mass_fraction, p.semimajor_axis_au, p.star_mass_msun, 0.1);
        p.final_radius_rearth = ComputePlanetRadius(p.core_mass_mearth, p.envelope_mass_fraction_final, p.water_mass_fraction, p.semimajor_axis_au, p.star_mass_msun, p.age_gyr);
        p.stripped_to_bare_core = (p.water_mass_fraction == 0.0);
      } else {
        p.initial_radius_rearth = ComputePlanetRadius(p.core_mass_mearth, p.envelope_mass_fraction_initial, 0.0, p.semimajor_axis_au, p.star_mass_msun, 0.05);

        // Numerical time evolution over N_steps
        double dt_gyr = 0.05;
        double current_fenv = p.envelope_mass_fraction_initial;
        for (double t = 0.05; t <= p.age_gyr; t += dt_gyr) {
          double mdot = 0.0;
          if (mechanism == ValleyMechanism::PHOTOEVAPORATION) {
            mdot = PhotoevaporativeMassLossRate(p.core_mass_mearth, current_fenv, p.semimajor_axis_au, p.star_mass_msun, t);
          } else if (mechanism == ValleyMechanism::CORE_POWERED_MASS_LOSS) {
            mdot = CorePoweredMassLossRate(p.core_mass_mearth, current_fenv, p.semimajor_axis_au, p.star_mass_msun, t);
          } else if (mechanism == ValleyMechanism::HYBRID_UNIFIED) {
            mdot = PhotoevaporativeMassLossRate(p.core_mass_mearth, current_fenv, p.semimajor_axis_au, p.star_mass_msun, t)
                   + CorePoweredMassLossRate(p.core_mass_mearth, current_fenv, p.semimajor_axis_au, p.star_mass_msun, t);
          }
          double delta_m_env = mdot * dt_gyr;
          double current_m_env = current_fenv * p.core_mass_mearth - delta_m_env;
          if (current_m_env <= 1.0e-5 * p.core_mass_mearth) {
            current_fenv = 0.0;
            break;
          }
          current_fenv = current_m_env / p.core_mass_mearth;
        }
        p.envelope_mass_fraction_final = current_fenv;
        p.final_radius_rearth = ComputePlanetRadius(p.core_mass_mearth, p.envelope_mass_fraction_final, 0.0, p.semimajor_axis_au, p.star_mass_msun, p.age_gyr);
        p.stripped_to_bare_core = (p.envelope_mass_fraction_final == 0.0);
      }
      pop.push_back(p);
    }
    return pop;
  }

  // Calculate the valley location log(R_valley) as function of period log(P)
  double ValleySlopeDLogRDLogP(ValleyMechanism mechanism) const {
    if (mechanism == ValleyMechanism::PHOTOEVAPORATION) return -0.11;
    if (mechanism == ValleyMechanism::CORE_POWERED_MASS_LOSS) return -0.06;
    if (mechanism == ValleyMechanism::PRIMORDIAL_WATER_WORLDS) return 0.00;
    return -0.09;  // Hybrid Unified
  }

  // Calculate the stellar mass dependence of the valley location dlog(R_valley)/dlog(M_star)
  double ValleySlopeDLogRDLogMStar(ValleyMechanism mechanism) const {
    if (mechanism == ValleyMechanism::PHOTOEVAPORATION) return +0.25;
    if (mechanism == ValleyMechanism::CORE_POWERED_MASS_LOSS) return +0.35;
    if (mechanism == ValleyMechanism::PRIMORDIAL_WATER_WORLDS) return +0.00;
    return +0.28;  // Hybrid Unified
  }

 private:
  mutable std::mt19937 gen_;
};

}  // namespace hot_jupiter

#endif  // CPP_INCLUDE_RADIUS_VALLEY_DISCOVERY_HPP_
