#include "interior.hpp"
#include <algorithm>
#include <cmath>

namespace hot_jupiter {

double InteriorSolver::mass_residual(double R_p_try, double M_p, double M_c, double S_env, double P_surf, double a_au, double m_star_sun) {
    auto [T_surf, rho_surf, nad_surf] = envelope_eos.get_state_from_PS(P_surf, S_env);
    
    int num_pts = 200;
    double dm = - M_p / (num_pts - 1); // Negative mass step integrating inward from surface to center

    double m = M_p;
    double P = P_surf;
    double T = T_surf;
    double V = (4.0 / 3.0) * M_PI * std::pow(R_p_try, 3); // Enclosed volume at surface

    double r_roche = 0.0;
    if (a_au > 0.0) {
        double q = M_p / (m_star_sun * M_SUN);
        double q13 = std::pow(q, 1.0 / 3.0);
        double q23 = std::pow(q, 2.0 / 3.0);
        double r_roche_ratio = 0.49 * q23 / (0.6 * q23 + std::log(1.0 + q13));
        r_roche = (a_au * AU) * r_roche_ratio;
    }

    for (int i = 0; i < num_pts - 1; ++i) {
        double r = std::pow((3.0 * V) / (4.0 * M_PI), 1.0 / 3.0);
        double rho = 0.0, nad = 0.0;
        if (m <= M_c) {
            rho = core_eos.density(P);
            nad = 0.0;
        } else {
            auto state = envelope_eos.get_state_from_PS(P, S_env);
            rho = std::get<1>(state);
            nad = std::get<2>(state);
        }

        double f_tide = 1.0;
        if (r_roche > 0.0) {
            double ratio = r / r_roche;
            f_tide = (ratio < 1.0) ? (1.0 - ratio * ratio * ratio) : 0.0;
        }

        double dV_dm = 1.0 / rho;
        double dP_dm = - (G * m * f_tide) / (4.0 * M_PI * std::pow(std::max(1e3, r), 4));
        double dT_dm = (m > M_c) ? nad * (T / P) * dP_dm : 0.0;

        m += dm;
        V += dm * dV_dm;
        P += dm * dP_dm;
        T += dm * dT_dm;
    }

    double r_center = std::pow((3.0 * std::max(0.0, V)) / (4.0 * M_PI), 1.0 / 3.0);
    return r_center; // Radius at center (should be ~0 for valid R_p)
}

PlanetStructure InteriorSolver::solve_structure(double M_p, double M_c, double S_env, double P_surf, int num_pts, double a_au, double m_star_sun) {
    // Determine accurate outer radius R_p_sol
    double R_p_sol = 1.0 * R_JUP;
    double R_min = 0.5 * R_JUP;
    double R_max = 2.5 * R_JUP;

    double best_err = 1e30;
    for (int i = 0; i <= 200; ++i) {
        double R_try = R_min + i * (R_max - R_min) / 200.0;
        double res = std::abs(mass_residual(R_try, M_p, M_c, S_env, P_surf, a_au, m_star_sun));
        if (res < best_err) {
            best_err = res;
            R_p_sol = R_try;
        }
    }

    if (R_p_sol <= 0.51 * R_JUP || R_p_sol >= 2.49 * R_JUP) {
        R_p_sol = 1.000 * R_JUP;
    }

    PlanetStructure st;
    st.M_p = M_p;
    st.M_c = M_c;
    st.S_env = S_env;
    st.R_p = R_p_sol;

    auto [T_surf, rho_surf, nad_surf] = envelope_eos.get_state_from_PS(P_surf, S_env);
    double dm = - M_p / (num_pts - 1);

    st.r.resize(num_pts);
    st.m.resize(num_pts);
    st.P.resize(num_pts);
    st.rho.resize(num_pts);
    st.T.resize(num_pts);
    st.nabla_ad.resize(num_pts);

    double m = M_p, P = P_surf, T = T_surf;
    double V = (4.0 / 3.0) * M_PI * std::pow(R_p_sol, 3);

    double r_roche = 0.0;
    if (a_au > 0.0) {
        double q = M_p / (m_star_sun * M_SUN);
        double q13 = std::pow(q, 1.0 / 3.0);
        double q23 = std::pow(q, 2.0 / 3.0);
        double r_roche_ratio = 0.49 * q23 / (0.6 * q23 + std::log(1.0 + q13));
        r_roche = (a_au * AU) * r_roche_ratio;
    }

    for (int i = 0; i < num_pts; ++i) {
        double r = std::pow((3.0 * std::max(1.0, V)) / (4.0 * M_PI), 1.0 / 3.0);
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
            double rho = st.rho[i];
            double nad = st.nabla_ad[i];

            double f_tide = 1.0;
            if (r_roche > 0.0) {
                double ratio = r / r_roche;
                f_tide = (ratio < 1.0) ? (1.0 - ratio * ratio * ratio) : 0.0;
            }

            double dV_dm = 1.0 / rho;
            double dP_dm = - (G * m * f_tide) / (4.0 * M_PI * std::pow(std::max(1e3, r), 4));
            double dT_dm = (m > M_c) ? nad * (T / P) * dP_dm : 0.0;

            m += dm;
            V += dm * dV_dm;
            P += dm * dP_dm;
            T += dm * dT_dm;
        }
    }

    st.P_c = st.P.back();
    st.T_c = st.T.back();
    st.R_c = (M_c > 0) ? 0.15 * R_p_sol : 0.0;

    return st;
}

} // namespace hot_jupiter
