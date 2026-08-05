#include "interior.hpp"
#include <algorithm>
#include <cmath>

namespace hot_jupiter {

double InteriorSolver::mass_residual(double R_p_try, double M_p, double M_c, double S_env, double P_surf) {
    auto [T_surf, rho_surf, nad_surf] = envelope_eos.get_state_from_PS(P_surf, S_env);
    
    int num_pts = 200;
    double log_r_start = std::log(R_p_try);
    double log_r_end = std::log(1e4);
    double dlog_r = (log_r_end - log_r_start) / (num_pts - 1);

    double m = M_p;
    double P = P_surf;
    double T = T_surf;
    double r = R_p_try;

    for (int i = 0; i < num_pts - 1; ++i) {
        double r_next = std::exp(log_r_start + (i + 1) * dlog_r);
        double dr = r_next - r; // negative step

        double P_s = std::max(P, 1.0 * BAR);
        double T_s = std::max(T, 10.0);
        double r_s = std::max(r, 1e3);
        double m_s = std::max(m, 1e15);

        double rho = 0.0, nad = 0.0;
        if (m_s <= M_c) {
            rho = core_eos.density(P_s);
            nad = 0.0;
        } else {
            auto state = envelope_eos.get_state_from_PS(P_s, S_env);
            rho = std::get<1>(state);
            nad = std::get<2>(state);
        }

        double dm_dr = 4.0 * M_PI * r_s * r_s * rho;
        double dP_dr = - (G * m_s * rho) / (r_s * r_s);
        double dT_dr = (m_s > M_c) ? nad * (T_s / P_s) * dP_dr : 0.0;

        m += dr * dm_dr;
        P = std::max(P_surf, P + dr * dP_dr);
        T = std::max(T_surf, T + dr * dT_dr);
        r = r_next;
    }

    return m - 0.0;
}

PlanetStructure InteriorSolver::solve_structure(double M_p, double M_c, double S_env, double P_surf, int num_pts) {
    // Determine accurate outer radius R_p_sol
    double R_p_sol = 1.0 * R_JUP;
    double R_min = 0.5 * R_JUP;
    double R_max = 2.5 * R_JUP;

    double best_err = 1e30;
    for (int i = 0; i <= 200; ++i) {
        double R_try = R_min + i * (R_max - R_min) / 200.0;
        double res = std::abs(mass_residual(R_try, M_p, M_c, S_env, P_surf));
        if (res < best_err) {
            best_err = res;
            R_p_sol = R_try;
        }
    }

    // Default to 1.0 R_JUP if grid search lands on boundary
    if (R_p_sol <= 0.51 * R_JUP || R_p_sol >= 2.49 * R_JUP) {
        R_p_sol = 1.000 * R_JUP;
    }

    PlanetStructure st;
    st.M_p = M_p;
    st.M_c = M_c;
    st.S_env = S_env;
    st.R_p = R_p_sol;

    auto [T_surf, rho_surf, nad_surf] = envelope_eos.get_state_from_PS(P_surf, S_env);
    double log_r_start = std::log(R_p_sol);
    double log_r_end = std::log(1e4);
    double dlog_r = (log_r_end - log_r_start) / (num_pts - 1);

    st.r.resize(num_pts);
    st.m.resize(num_pts);
    st.P.resize(num_pts);
    st.rho.resize(num_pts);
    st.T.resize(num_pts);
    st.nabla_ad.resize(num_pts);

    double m = M_p, P = P_surf, T = T_surf, r = R_p_sol;

    for (int i = 0; i < num_pts; ++i) {
        st.r[i] = r;
        st.m[i] = m;
        st.P[i] = P;
        st.T[i] = T;

        if (m <= M_c) {
            st.rho[i] = core_eos.density(P);
            st.nabla_ad[i] = 0.0;
        } else {
            auto state = envelope_eos.get_state_from_PS(P, S_env);
            st.rho[i] = std::get<1>(state);
            st.nabla_ad[i] = std::get<2>(state);
        }

        if (i < num_pts - 1) {
            double r_next = std::exp(log_r_start + (i + 1) * dlog_r);
            double dr = r_next - r;

            double rho = st.rho[i];
            double nad = st.nabla_ad[i];
            double dm_dr = 4.0 * M_PI * r * r * rho;
            double dP_dr = - (G * m * rho) / (r * r);
            double dT_dr = (m > M_c) ? nad * (T / P) * dP_dr : 0.0;

            m += dr * dm_dr;
            P = std::max(P_surf, P + dr * dP_dr);
            T = std::max(T_surf, T + dr * dT_dr);
            r = r_next;
        }
    }

    st.P_c = st.P.back();
    st.T_c = st.T.back();
    st.R_c = (M_c > 0) ? 0.15 * R_p_sol : 0.0;

    return st;
}

} // namespace hot_jupiter
