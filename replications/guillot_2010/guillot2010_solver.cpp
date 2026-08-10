// C++ Standalone Replication Solver for Guillot (2010) A&A 520, A27
// Computes double-gray 2-stream radiative equilibrium T(tau) and T(P) profiles.

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "atmosphere.hpp"

namespace hot_jupiter {

void run_guillot_tau_sweep(const std::string& output_csv) {
  std::ofstream out(output_csv);
  out << "tau_optical,gamma,t_atm_k\n";

  std::vector<double> gammas = {0.01, 0.1, 1.0, 10.0};
  double t_eq = 1500.0;
  double t_int = 100.0;

  for (double gamma : gammas) {
    for (int k = -40; k <= 30; ++k) {
      double log_tau = k * 0.1;
      double tau = std::pow(10.0, log_tau);

      double term1 = 0.75 * std::pow(t_int, 4) * (tau + 2.0 / 3.0);
      double term2 = 0.75 * std::pow(t_eq, 4) * (2.0 / 3.0 + 1.0 / (gamma * std::sqrt(3.0)) +
                                                (gamma / std::sqrt(3.0) - 1.0 / (gamma * std::sqrt(3.0))) *
                                                    std::exp(-gamma * tau * std::sqrt(3.0)));

      double t_atm = std::pow(term1 + term2, 0.25);
      out << tau << "," << gamma << "," << t_atm << "\n";
    }
  }
  out.close();
  std::cout << "--> Wrote Guillot (2010) T(tau) dataset to " << output_csv << std::endl;
}

void run_guillot_tp_profiles(const std::string& output_csv) {
  std::ofstream out(output_csv);
  out << "target_name,pressure_bar,t_atm_k\n";

  struct Planet { std::string name; double t_eq; double gamma; double kappa_ir; };
  std::vector<Planet> planets = {
      {"HD 209458b", 1450.0, 0.1, 0.01},
      {"HD 189733b", 1200.0, 0.4, 0.01}
  };

  double g_m_s2 = 10.0;
  double t_int = 100.0;

  for (const auto& p : planets) {
    for (int k = -60; k <= 20; ++k) {
      double log_p = k * 0.1;
      double p_bar = std::pow(10.0, log_p);
      double p_pa = p_bar * 1.0e5;
      double tau = (p.kappa_ir * p_pa) / g_m_s2;

      double term1 = 0.75 * std::pow(t_int, 4) * (tau + 2.0 / 3.0);
      double term2 = 0.75 * std::pow(p.t_eq, 4) * (2.0 / 3.0 + 1.0 / (p.gamma * std::sqrt(3.0)) +
                                                (p.gamma / std::sqrt(3.0) - 1.0 / (p.gamma * std::sqrt(3.0))) *
                                                    std::exp(-p.gamma * tau * std::sqrt(3.0)));

      double t_atm = std::pow(term1 + term2, 0.25);
      out << p.name << "," << p_bar << "," << t_atm << "\n";
    }
  }
  out.close();
  std::cout << "--> Wrote Guillot (2010) T(P) dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Guillot (2010) C++ Radiative Equilibrium Solver ===" << std::endl;
  hot_jupiter::run_guillot_tau_sweep("replications/guillot_2010/sim_guillot_tau.csv");
  hot_jupiter::run_guillot_tp_profiles("replications/guillot_2010/sim_guillot_tp.csv");
  std::cout << "✅ Guillot (2010) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
