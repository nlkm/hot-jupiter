"""
Unit tests for Python ctypes C++ bindings (hot_jupiter/bindings.py).
"""

import pytest
from hot_jupiter.constants import M_JUP, M_EARTH, R_JUP, BAR
from hot_jupiter.bindings import solve_structure_cpp, evaluate_density_cpp


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
