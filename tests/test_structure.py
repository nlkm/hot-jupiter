"""
Unit tests for 1D hydrostatic interior structure solver.
"""

import pytest
import numpy as np

from thermal_evolution.constants import M_JUP, M_EARTH, R_JUP, BAR
from thermal_evolution.eos import AnalyticalHHeEOS, TabularEOS, BirchMurnaghanCoreEOS
from thermal_evolution.structure import InteriorSolver, PlanetStructure


def test_jupiter_structure_analytical():
    eos = AnalyticalHHeEOS()
    solver = InteriorSolver(envelope_eos=eos)

    M_p = 1.0 * M_JUP
    M_c = 10.0 * M_EARTH
    S_env = eos.specific_entropy(1.0 * BAR, 165.0)  # Present-day Jupiter entropy

    struct = solver.solve_structure(M_p=M_p, M_c=M_c, S_env=S_env)

    assert isinstance(struct, PlanetStructure)
    assert 0.5 * R_JUP < struct.R_p < 4.0 * R_JUP
    assert 0.0 < struct.R_c < struct.R_p
    assert struct.P_c > struct.P_cb > BAR
    assert struct.int_T_dm > 0.0
    assert struct.E_int > 0.0
    assert struct.U_grav < 0.0

    # Profile checks
    prof = struct.profile
    assert prof is not None
    assert np.all(np.diff(prof.m) >= 0)  # Monotonic mass
    assert np.all(np.diff(prof.r) >= 0)  # Monotonic radius
    assert np.all(np.diff(prof.P) <= 0)  # Decreasing pressure


def test_structure_tabular_eos():
    tab_eos = TabularEOS.create_synthetic_grid(n_P=100, n_T=100)
    solver = InteriorSolver(envelope_eos=tab_eos)

    M_p = 1.0 * M_JUP
    M_c = 15.0 * M_EARTH
    S_env = tab_eos.specific_entropy(1.0 * BAR, 165.0)

    struct = solver.solve_structure(M_p=M_p, M_c=M_c, S_env=S_env)

    assert 0.5 * R_JUP < struct.R_p < 4.0 * R_JUP
    assert struct.R_c < struct.R_p
    assert struct.int_T_dm > 0.0
