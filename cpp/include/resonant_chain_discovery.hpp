// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 5: Resonant Chain Stability, Resonance Capture, & Chaos in Compact Systems

#ifndef CPP_INCLUDE_RESONANT_CHAIN_DISCOVERY_HPP_
#define CPP_INCLUDE_RESONANT_CHAIN_DISCOVERY_HPP_

#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "cpp/include/constants.hpp"

namespace hot_jupiter {

enum class ResonantChainFate {
  STABLE_RESONANT_LIBRATION,
  CHAOTIC_RESONANCE_OVERLAP,
  CONVERGENT_COLLISION
};

struct ResonantState {
  double time_kyr;
  double semimajor_axis_1_au;
  double semimajor_axis_2_au;
  double eccentricity_1;
  double eccentricity_2;
  double period_ratio;
  double resonant_angle_deg;  // phi = (p+q)*lambda_2 - p*lambda_1 - q*pomega
  double laplace_angle_deg;    // Phi_L = p*lambda_1 - (p+q)*lambda_2 + q*lambda_3
  bool is_librating;
};

class ResonantChainDiscoveryEngine {
 public:
  ResonantChainDiscoveryEngine()
      : star_mass_msun_(0.09), m1_mearth_(1.0), m2_mearth_(1.3), m3_mearth_(0.9) {}

  ResonantChainDiscoveryEngine(double m_star, double m1, double m2, double m3)
      : star_mass_msun_(m_star), m1_mearth_(m1), m2_mearth_(m2), m3_mearth_(m3) {}

  // First-order Mean Motion Resonance (MMR) width in semimajor axis
  // delta_a / a = C * (M_p / M_star)^(2/3)
  double ResonanceWidth(double p_res, double q_res, double a_au, double m_planet_me) const {
    double mu = (m_planet_me * M_EARTH) / (star_mass_msun_ * M_SUN);
    double c_coeff = std::sqrt(1.5 * (p_res + 1.0) / q_res);
    return c_coeff * std::pow(mu, 2.0 / 3.0) * a_au;
  }

  // Chirikov resonance overlap criterion for multi-planet chaos
  // Separation delta_a_crit = 1.4 * a * ( (M1 + M2) / M_star )^(2/7)
  double CriticalOverlapSeparation(double a_au, double m1_me, double m2_me) const {
    double mu_tot = ((m1_me + m2_me) * M_EARTH) / (star_mass_msun_ * M_SUN);
    return 1.40 * a_au * std::pow(mu_tot, 2.0 / 7.0);
  }

  // Equilibrium eccentricity under migration & eccentricity damping: e_eq ~ sqrt(tau_e / tau_mig)
  double EquilibriumEccentricity(double tau_mig_kyr, double tau_e_kyr) const {
    if (tau_mig_kyr <= 0.0) return 0.01;
    double ratio = tau_e_kyr / tau_mig_kyr;
    return std::min(0.25, 0.40 * std::sqrt(ratio));
  }

  // Symplectic secular evolution of 3-planet resonant chain (e.g. TRAPPIST-1 b-c-d 8:5:3 or 3:2:4/3)
  std::vector<ResonantState> EvolveResonantChain(double a1_init_au, double a2_init_au,
                                                double tau_mig_kyr = 100.0,
                                                double k_damp = 100.0,
                                                double t_max_kyr = 200.0,
                                                double dt_kyr = 0.1) const {
    std::vector<ResonantState> history;
    double a1 = a1_init_au;
    double a2 = a2_init_au;
    double e1 = 0.01;
    double e2 = 0.01;
    double tau_e_kyr = tau_mig_kyr / k_damp;

    double p1 = 24.0 * std::sqrt(std::pow(a1, 3) / star_mass_msun_) * 365.25;
    double p2 = 24.0 * std::sqrt(std::pow(a2, 3) / star_mass_msun_) * 365.25;
    double lambda1 = 0.0;
    double lambda2 = 0.0;
    double pomega1 = 0.0;

    double target_ratio = 1.50;  // 3:2 first order MMR target

    for (double t = 0.0; t <= t_max_kyr; t += dt_kyr) {
      double pr = p2 / std::max(1.0e-4, p1);
      
      // Resonant angle for 3:2 MMR: phi = 3*lambda_2 - 2*lambda_1 - pomega_1
      double phi_rad = 3.0 * lambda2 - 2.0 * lambda1 - pomega1;
      double phi_deg = std::fmod(phi_rad * 180.0 / M_PI, 360.0);
      if (phi_deg < 0.0) phi_deg += 360.0;

      // Laplace three-body angle: Phi_L = 3*lambda_1 - 5*lambda_2 + 2*lambda_3
      double phi_l_deg = std::fmod((3.0 * lambda1 - 5.0 * lambda2) * 180.0 / M_PI, 360.0);
      if (phi_l_deg < 0.0) phi_l_deg += 360.0;

      bool in_resonance = std::abs(pr - target_ratio) < 0.03;
      bool librating = in_resonance && (std::abs(phi_deg - 180.0) < 60.0 || std::abs(phi_deg - 0.0) < 60.0);

      ResonantState st;
      st.time_kyr = t;
      st.semimajor_axis_1_au = a1;
      st.semimajor_axis_2_au = a2;
      st.eccentricity_1 = e1;
      st.eccentricity_2 = e2;
      st.period_ratio = pr;
      st.resonant_angle_deg = phi_deg;
      st.laplace_angle_deg = phi_l_deg;
      st.is_librating = librating;
      history.push_back(st);

      // Orbital migration forces
      if (pr > target_ratio) {
        // Convergent migration: planet 2 migrates inward faster
        a2 -= (a2 / tau_mig_kyr) * dt_kyr;
      } else {
        // Resonance capture: planets locked, adiabatic slow migration
        a1 -= (a1 / (3.0 * tau_mig_kyr)) * dt_kyr;
        a2 -= (a2 / (3.0 * tau_mig_kyr)) * dt_kyr;

        // Resonant eccentricity excitation vs damping
        double de_dt_res = 0.05 / tau_mig_kyr;
        double de_dt_damp = -e2 / tau_e_kyr;
        e2 = std::max(0.001, e2 + (de_dt_res + de_dt_damp) * dt_kyr);
        e1 = std::max(0.001, e1 + (0.5 * de_dt_res - e1 / tau_e_kyr) * dt_kyr);
      }

      p1 = 24.0 * std::sqrt(std::pow(a1, 3) / star_mass_msun_) * 365.25;
      p2 = 24.0 * std::sqrt(std::pow(a2, 3) / star_mass_msun_) * 365.25;

      double n1 = 2.0 * M_PI / (p1 / 24.0);
      double n2 = 2.0 * M_PI / (p2 / 24.0);
      lambda1 += n1 * (dt_kyr * 365.25);
      lambda2 += n2 * (dt_kyr * 365.25);

      // Secular precession
      double g_prec = 0.02 * (m1_mearth_ / star_mass_msun_) * n1;
      pomega1 += g_prec * (dt_kyr * 365.25);
    }
    return history;
  }

  // Classify final fate of the multi-planet chain
  ResonantChainFate ClassifyFate(const std::vector<ResonantState>& history) const {
    if (history.empty()) return ResonantChainFate::CHAOTIC_RESONANCE_OVERLAP;
    const auto& f = history.back();
    if (f.eccentricity_1 > 0.35 || f.eccentricity_2 > 0.35) {
      return ResonantChainFate::CHAOTIC_RESONANCE_OVERLAP;
    }
    if (f.is_librating && std::abs(f.period_ratio - 1.50) < 0.02) {
      return ResonantChainFate::STABLE_RESONANT_LIBRATION;
    }
    return ResonantChainFate::CONVERGENT_COLLISION;
  }

 private:
  double star_mass_msun_;
  double m1_mearth_;
  double m2_mearth_;
  double m3_mearth_;
};

}  // namespace hot_jupiter

#endif  // CPP_INCLUDE_RESONANT_CHAIN_DISCOVERY_HPP_
