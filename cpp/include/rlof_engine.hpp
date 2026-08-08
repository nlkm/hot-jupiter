#ifndef HOT_JUPITER_RLOF_ENGINE_HPP_
#define HOT_JUPITER_RLOF_ENGINE_HPP_

#include <cmath>
#include <string>
#include <vector>
#include "constants.hpp"

namespace hot_jupiter {

enum class EvolutionOutcome {
  DISRUPTED = 0,
  STAGNATED = 1,
  COOLING = 2,
  ENGULFED = 3
};

struct TrajectoryResult {
  std::vector<double> t_arr;
  std::vector<double> a_arr;
  std::vector<double> m_p_arr;
  std::vector<double> m_env_arr;
  std::vector<double> m_core_arr;
  std::vector<double> r_p_arr;
  std::vector<double> r_roche_arr;
  std::vector<double> filling_factor_arr;
  EvolutionOutcome outcome;
  double final_m_remnant_earth;
  double z_bulk;
};

class CoupledRLOFIntegrator {
 public:
  double m_p_init_jup;
  double a_init_au;
  double m_core_earth;
  double m_star_sun;
  double q_star_prime;
  double k2_star;
  double eta_rlof;
  double beta_angular_momentum;

  CoupledRLOFIntegrator(double m_p_init_jup = 1.0,
                        double a_init_au = 0.02,
                        double m_core_earth = 10.0,
                        double m_star_sun = 1.0,
                        double q_star_prime = 1.5e5,
                        double k2_star = 0.03,
                        double eta_rlof = 4.0,
                        double beta_angular_momentum = 0.5)
      : m_p_init_jup(m_p_init_jup),
        a_init_au(a_init_au),
        m_core_earth(m_core_earth),
        m_star_sun(m_star_sun),
        q_star_prime(q_star_prime),
        k2_star(k2_star),
        eta_rlof(eta_rlof),
        beta_angular_momentum(beta_angular_momentum) {}

  static double compute_roche_lobe_radius(double a_m, double m_total_kg, double m_star_sun = 1.0) {
    double m_star_kg = m_star_sun * M_SUN;
    double q = m_total_kg / m_star_kg;
    double q_13 = std::pow(q, 1.0 / 3.0);
    double q_23 = std::pow(q, 2.0 / 3.0);
    double r_roche_ratio = 0.49 * q_23 / (0.6 * q_23 + std::log(1.0 + q_13));
    return a_m * r_roche_ratio;
  }

  TrajectoryResult integrate(double t_max_yr = 5.0e9, int num_pts = 400) const {
    TrajectoryResult res;
    res.t_arr.resize(num_pts);
    res.a_arr.resize(num_pts);
    res.m_p_arr.resize(num_pts);
    res.m_env_arr.resize(num_pts);
    res.m_core_arr.resize(num_pts, m_core_earth);
    res.r_p_arr.resize(num_pts);
    res.r_roche_arr.resize(num_pts);
    res.filling_factor_arr.resize(num_pts);

    double m_core_kg = m_core_earth * M_EARTH;
    double m_env_init_kg = std::max(0.0, (m_p_init_jup * M_JUP) - m_core_kg);
    double m_env_kg = m_env_init_kg;
    double m_total_kg = m_core_kg + m_env_kg;
    double a_curr = a_init_au * AU;

    double log_t_min = std::log10(1.0e6);
    double log_t_max = std::log10(t_max_yr);
    double dlog_t = (log_t_max - log_t_min) / (num_pts - 1);

    bool disrupted = false;
    bool engulfed = false;
    double max_ff = 0.0;

    for (int i = 0; i < num_pts; ++i) {
      double t_yr = std::pow(10.0, log_t_min + i * dlog_t);
      double dt_yr = (i == 0) ? t_yr : (t_yr - res.t_arr[i - 1]);
      double dt_sec = dt_yr * 3.154e7;
      double t_gyr = t_yr / 1.0e9;

      double r_core = 1.0 * R_EARTH * std::pow(m_core_earth / 1.0, 0.27);
      double r_p_curr = r_core;
      if (m_env_kg > 0.1 * M_EARTH) {
        double r_env = 1.25 * R_JUP * std::pow(m_env_kg / M_JUP, 0.15) * std::exp(-0.08 * t_gyr);
        r_p_curr = std::max(r_core, r_env);
      }

      double r_roche_curr = compute_roche_lobe_radius(a_curr, m_total_kg, m_star_sun);
      double ff = (r_roche_curr > 0.0) ? (r_p_curr / r_roche_curr) : 0.0;
      max_ff = std::max(max_ff, ff);

      if (r_p_curr == r_core && ff >= 1.0) {
        disrupted = true;
        m_total_kg = 0.0;
        m_env_kg = 0.0;
        break;
      }

      if (ff >= 0.95 && m_env_kg > 0.0) {
        double m_dot_0 = 1.0e-7 * M_JUP;
        double m_dot = m_dot_0 * std::exp(eta_rlof * (ff - 1.0));
        double est_loss = m_dot * dt_yr;

        int n_sub = std::max(1, std::min(200, static_cast<int>(std::ceil(est_loss / (0.0005 * M_JUP)))));
        double dt_sub_yr = dt_yr / n_sub;

        for (int s = 0; s < n_sub; ++s) {
          if (m_env_kg <= 0.0) break;
          double r_roche_sub = compute_roche_lobe_radius(a_curr, m_total_kg, m_star_sun);
          double ff_sub = (r_roche_sub > 0.0) ? (r_p_curr / r_roche_sub) : 0.0;
          if (ff_sub < 0.95) break;
          double m_dot_sub = m_dot_0 * std::exp(eta_rlof * (ff_sub - 1.0));
          double loss_sub = std::min(m_env_kg, m_dot_sub * dt_sub_yr);

          m_env_kg -= loss_sub;
          m_total_kg = m_core_kg + m_env_kg;
          double da_rlof_sub = -2.0 * a_curr * (-loss_sub / m_total_kg) * (1.0 - beta_angular_momentum);
          a_curr += da_rlof_sub;
        }
      }

      double n_orb = std::sqrt(G * (m_star_sun * M_SUN) / std::max(1.0e6, std::pow(a_curr, 3)));
      double da_tide = -9.0 * (k2_star / q_star_prime) * n_orb *
                       std::pow(R_SUN / std::max(1.0e6, a_curr), 5) *
                       (m_total_kg / (m_star_sun * M_SUN)) * a_curr * dt_sec;
      a_curr += da_tide;

      if (a_curr <= 0.008 * AU || m_total_kg <= 0.0) {
        engulfed = true;
        break;
      }

      res.t_arr[i] = t_yr;
      res.a_arr[i] = a_curr / AU;
      res.m_p_arr[i] = m_total_kg / M_JUP;
      res.m_env_arr[i] = m_env_kg / M_JUP;
      res.r_p_arr[i] = r_p_curr / R_JUP;
      res.r_roche_arr[i] = r_roche_curr / AU;
      res.filling_factor_arr[i] = ff;
    }

    if (disrupted || engulfed || m_total_kg <= 0.0) {
      res.outcome = EvolutionOutcome::DISRUPTED;
      res.final_m_remnant_earth = 0.0;
      res.z_bulk = 0.0;
    } else if (max_ff >= 0.95) {
      double m_crit_jup = 0.50 * std::pow(a_init_au / 0.018, 3.0);
      if (m_p_init_jup < m_crit_jup) {
        res.outcome = EvolutionOutcome::DISRUPTED;
        res.final_m_remnant_earth = 0.0;
        res.z_bulk = 0.0;
      } else {
        res.outcome = EvolutionOutcome::STAGNATED;
        res.final_m_remnant_earth = m_total_kg / M_EARTH;
        res.z_bulk = (m_total_kg > 0.0) ? (m_core_kg / m_total_kg) : 1.0;
      }
    } else {
      res.outcome = EvolutionOutcome::COOLING;
      res.final_m_remnant_earth = m_total_kg / M_EARTH;
      res.z_bulk = (m_total_kg > 0.0) ? (m_core_kg / m_total_kg) : 0.0;
    }

    return res;
  }
};

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_RLOF_ENGINE_HPP_
