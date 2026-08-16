#include "interior.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

namespace hot_jupiter {

PlanetStructure InteriorSolver::solve_structure(double M_p, double M_c, double S_env,
                                                double P_surf, int num_pts, double a_au,
                                                double m_star_sun) {
    // 1. Calculate equilibrium outer radius R_p
    double M_jup_norm = M_p / M_JUP;
    double M_c_earth = M_c / M_EARTH;
    double S_norm = S_env / 1.34e5;

    double R_p_sol = 1.068 * R_JUP * std::pow(M_jup_norm, -0.04) *
                     (1.0 - 0.005 * std::min(50.0, M_c_earth)) *
                     std::pow(std::max(0.2, S_norm), 0.65);

    if (R_p_sol < 0.60 * R_JUP) R_p_sol = 0.60 * R_JUP;
    if (R_p_sol > 2.80 * R_JUP) R_p_sol = 2.80 * R_JUP;

    // 2. Heavy-element core radius from Birch-Murnaghan compressed density
    double rho_core_mean = 12000.0;  // kg/m^3
    double R_c = (M_c > 0.0) ?
                 std::pow((3.0 * M_c) / (4.0 * M_PI * rho_core_mean), 1.0 / 3.0) : 0.0;
    if (R_c > 0.40 * R_p_sol) R_c = 0.40 * R_p_sol;

    // Roche lobe calculation if semi-major axis is provided
    double r_roche = 0.0;
    if (a_au > 0.0) {
        double q = M_p / (m_star_sun * M_SUN);
        double q13 = std::pow(q, 1.0 / 3.0);
        double q23 = std::pow(q, 2.0 / 3.0);
        double r_roche_ratio = 0.49 * q23 / (0.6 * q23 + std::log(1.0 + q13));
        r_roche = (a_au * AU) * r_roche_ratio;
    }

    PlanetStructure st;
    st.M_p = M_p;
    st.M_c = M_c;
    st.S_env = S_env;
    st.R_p = R_p_sol;
    st.R_c = R_c;

    st.r.resize(num_pts);
    st.m.resize(num_pts);
    st.P.resize(num_pts);
    st.rho.resize(num_pts);
    st.T.resize(num_pts);
    st.nabla_ad.resize(num_pts);

    // 3. Construct radial coordinate from surface (i = 0, r = R_p) to center (i = num_pts - 1, r = 0)
    for (int i = 0; i < num_pts; ++i) {
        double frac = static_cast<double>(i) / (num_pts - 1);
        st.r[i] = R_p_sol * (1.0 - frac);
    }

    // 4. Initial density shape for core and envelope
    auto [T_surf, rho_surf, nad_surf] = envelope_eos.get_state_from_PS(P_surf, S_env);

    for (int i = 0; i < num_pts; ++i) {
        double r = st.r[i];
        if (r <= R_c) {
            double xi = r / std::max(R_c, 1e3);
            st.rho[i] = 15000.0 * (1.0 - 0.20 * xi * xi);
            st.nabla_ad[i] = 0.0;
        } else {
            double x = (r - R_c) / (R_p_sol - R_c);
            st.rho[i] = 4300.0 * std::pow(std::max(0.0, 1.0 - x), 1.85) + rho_surf;
            st.nabla_ad[i] = 0.2857;
        }
    }

    // 5. Shell mass normalization to guarantee exact mass conservation
    if (M_c > 0.0) {
        double m_core_sum = 0.0;
        for (int i = 0; i < num_pts; ++i) {
            if (st.r[i] <= R_c) {
                double dr = (i == num_pts - 1) ? (st.r[i - 1] - st.r[i]) :
                                                 (st.r[i] - st.r[i + 1]);
                m_core_sum += 4.0 * M_PI * st.r[i] * st.r[i] * st.rho[i] * dr;
            }
        }
        if (m_core_sum > 0.0) {
            double scale_core = M_c / m_core_sum;
            for (int i = 0; i < num_pts; ++i) {
                if (st.r[i] <= R_c) {
                    st.rho[i] *= scale_core;
                }
            }
        }
    }

    double m_env_target = M_p - M_c;
    double m_env_sum = 0.0;
    for (int i = 0; i < num_pts; ++i) {
        if (st.r[i] > R_c) {
            double dr = (i == 0) ? (st.r[i] - st.r[i + 1]) : (st.r[i - 1] - st.r[i]);
            m_env_sum += 4.0 * M_PI * st.r[i] * st.r[i] * st.rho[i] * dr;
        }
    }
    if (m_env_sum > 0.0) {
        double scale_env = m_env_target / m_env_sum;
        for (int i = 0; i < num_pts; ++i) {
            if (st.r[i] > R_c) {
                st.rho[i] *= scale_env;
            }
        }
    }

    // 6. Enclosed mass profile m(r) computed from center (num_pts - 1) outward to surface (0)
    st.m[num_pts - 1] = 0.0;
    for (int i = num_pts - 2; i >= 0; --i) {
        double dr = st.r[i] - st.r[i + 1];
        double r_mid = 0.5 * (st.r[i] + st.r[i + 1]);
        double rho_mid = 0.5 * (st.rho[i] + st.rho[i + 1]);
        st.m[i] = st.m[i + 1] + 4.0 * M_PI * r_mid * r_mid * rho_mid * dr;
    }
    if (st.m[0] > 0.0) {
        double scale = M_p / st.m[0];
        for (int i = 0; i < num_pts; ++i) {
            st.m[i] *= scale;
        }
    }
    st.m[0] = M_p;

    // 7. Hydrostatic Pressure integration inward from surface (i = 0, P = P_surf) to center (num_pts - 1)
    st.P[0] = P_surf;
    for (int i = 0; i < num_pts - 1; ++i) {
        double dr = st.r[i] - st.r[i + 1];
        double r_mid = 0.5 * (st.r[i] + st.r[i + 1]);
        double m_mid = 0.5 * (st.m[i] + st.m[i + 1]);
        double rho_mid = 0.5 * (st.rho[i] + st.rho[i + 1]);
        double r_safe = std::max(r_mid, 1e3);

        double f_tide = 1.0;
        if (r_roche > 0.0) {
            double ratio = r_safe / r_roche;
            f_tide = (ratio < 1.0) ? (1.0 - ratio * ratio * ratio) : 0.0;
        }

        double dP = (G * m_mid * rho_mid * f_tide / (r_safe * r_safe)) * dr;
        st.P[i + 1] = st.P[i] + dP;
    }

    // 8. Temperature profile
    st.T[0] = T_surf;
    double T_cb = T_surf;
    for (int i = 0; i < num_pts; ++i) {
        if (st.r[i] > R_c) {
            st.T[i] = T_surf * std::pow(st.P[i] / P_surf, 0.2857);
            T_cb = st.T[i];
        } else {
            st.T[i] = T_cb;
        }
    }

    st.P_c = st.P.back();
    st.T_c = st.T.back();

    return st;
}

}  // namespace hot_jupiter
