#ifndef THERMAL_EVOLUTION_MULTI_PLANET_HPP
#define THERMAL_EVOLUTION_MULTI_PLANET_HPP

#include <vector>
#include <cmath>
#include <algorithm>

#include "constants.hpp"

namespace thermal_evolution {

struct PlanetSystemMember {
    double M_p;
    double M_c;
    double a;
    double e;
    double inc = 0.0;
    double Omega_node = 0.0;
    double omega_arg = 0.0;
    double S_env = 1.34e5;
};

class MultiPlanetSystem {
public:
    double M_star = M_SUN;
    std::vector<PlanetSystemMember> planets;

    static double laplace_b(int s, double alpha) {
        // Laplace coefficient b_{3/2}^{(s)}(alpha) approximation
        if (s == 1) {
            return 3.0 * alpha * (1.0 + 0.375 * alpha * alpha);
        } else if (s == 2) {
            return 0.75 * alpha * alpha * (1.0 + 0.4 * alpha * alpha);
        }
        return 0.0;
    }

    std::vector<std::vector<double>> compute_secular_matrix() const {
        int N = planets.size();
        std::vector<std::vector<double>> A(N, std::vector<double>(N, 0.0));

        for (int i = 0; i < N; ++i) {
            double n_i = std::sqrt(G * M_star / std::pow(planets[i].a, 3));
            for (int j = 0; j < N; ++j) {
                if (i == j) continue;
                double a_i = planets[i].a;
                double a_j = planets[j].a;
                double alpha = std::min(a_i, a_j) / std::max(a_i, a_j);
                double alpha_bar = (a_i < a_j) ? alpha : 1.0;

                A[i][j] = - 0.25 * n_i * (planets[j].M_p / M_star) * alpha * alpha_bar * laplace_b(2, alpha);
            }
            double sum_diag = 0.0;
            for (int j = 0; j < N; ++j) {
                if (i == j) continue;
                double a_i = planets[i].a;
                double a_j = planets[j].a;
                double alpha = std::min(a_i, a_j) / std::max(a_i, a_j);
                double alpha_bar = (a_i < a_j) ? alpha : 1.0;
                sum_diag += 0.25 * n_i * (planets[j].M_p / M_star) * alpha * alpha_bar * laplace_b(1, alpha);
            }
            A[i][i] = sum_diag;
        }
        return A;
    }

    std::vector<double> evaluate_secular_de_dt() const {
        int N = planets.size();
        auto A = compute_secular_matrix();
        std::vector<double> de_dt(N, 0.0);

        for (int i = 0; i < N; ++i) {
            double sum_terms = 0.0;
            for (int j = 0; j < N; ++j) {
                if (i == j) continue;
                sum_terms += A[i][j] * planets[j].e;
            }
            de_dt[i] = sum_terms;
        }
        return de_dt;
    }
};

} // namespace thermal_evolution

#endif // THERMAL_EVOLUTION_MULTI_PLANET_HPP
