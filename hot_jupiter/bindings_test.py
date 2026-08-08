"""
Unit tests for Python ctypes C++ bindings (hot_jupiter/bindings.py).
"""

from hot_jupiter.bindings import (
    evaluate_density_cpp,
    rlof_integrate_cpp,
    simulate_population_cpp,
    solve_interior_profile_detailed_cpp,
    solve_structure_cpp,
)
from hot_jupiter.constants import BAR, M_EARTH, M_JUP, R_JUP


def test_cpp_bindings_solve_structure():
    M_p = 1.0 * M_JUP
    M_c = 10.0 * M_EARTH
    S_env = 1.0e8
    P_surf = 1.0 * BAR

    res = solve_structure_cpp(M_p, M_c, S_env, P_surf)
    assert res.R_p > 0.5 * R_JUP
    assert res.R_p < 3.0 * R_JUP
    assert res.num_layers == 300
    assert res.P_center > 0.0
    assert res.T_center > 0.0


def test_cpp_bindings_evaluate_density():
    P = 1.0e10  # 10 GPa
    T = 5000.0  # 5000 K
    rho = evaluate_density_cpp(P, T, X=0.75)
    assert rho > 100.0
    assert rho < 10000.0


def test_cpp_bindings_rlof_integrate():
    data, res = rlof_integrate_cpp(m_p_init_jup=1.0,
                                   a_init_au=0.035,
                                   m_core_earth=10.0,
                                   t_max_yr=1.0e9,
                                   num_pts=100)
    assert len(data["t"]) == 100
    assert res.outcome == 2  # COOLING
    assert res.final_m_remnant_earth > 0.0


def test_cpp_bindings_solve_interior_profile_detailed():
    data, res = solve_interior_profile_detailed_cpp(1.0 * M_JUP,
                                                    10.0 * M_EARTH,
                                                    1.0e8,
                                                    1.0 * BAR,
                                                    num_pts=300)
    assert len(data["r"]) == 300
    assert data["r"][0] > 0.5 * R_JUP
    assert data["rho"][0] > 0.0
    assert res.R_p > 0.0


def test_cpp_bindings_simulate_population():
    pop = simulate_population_cpp(num_planets=50, seed=42)
    assert len(pop["m_p_init"]) == 50
    assert len(pop["outcome"]) == 50
