"""
Unit tests for thermal_evolution.eos module.
"""

import pytest
import numpy as np

from thermal_evolution.constants import BAR, MBAR, GPa
from thermal_evolution.eos import (
    BaseEOS,
    AnalyticalHHeEOS,
    TabularEOS,
    ConstantDensityCoreEOS,
    BirchMurnaghanCoreEOS,
)


def test_analytical_eos_basic():
    eos = AnalyticalHHeEOS()
    
    P = 1.0 * BAR
    T = 1000.0
    
    rho = eos.density(P, T)
    assert rho > 0.0
    assert 0.01 < rho < 10.0  # Atmospheric range

    S = eos.specific_entropy(P, T)
    assert np.isfinite(S)

    T_inv = eos.temperature_from_PS(P, S)
    assert pytest.approx(T, rel=1e-3) == T_inv

    nad = eos.nabla_ad(P, T)
    assert 0.01 < nad <= 0.4


def test_tabular_eos_abstract_interface():
    # Test tabular EOS using synthetic grid
    tab_eos: BaseEOS = TabularEOS.create_synthetic_grid(n_P=100, n_T=100)
    
    P = 10.0 * BAR
    T = 1000.0
    
    rho = tab_eos.density(P, T)
    assert rho > 0.0

    S = tab_eos.specific_entropy(P, T)
    assert np.isfinite(S)

    T_inv = tab_eos.temperature_from_PS(P, S)
    assert pytest.approx(T, rel=1e-2) == T_inv

    nad = tab_eos.nabla_ad(P, T)
    assert 0.01 < nad <= 0.4


def test_interchangeable_eos():
    analytical: BaseEOS = AnalyticalHHeEOS()
    tabular: BaseEOS = TabularEOS.create_synthetic_grid(n_P=100, n_T=100)

    P = 10.0 * BAR
    T_target = 2000.0

    # Direct evaluation at same (P, T)
    rho_analytical = analytical.density(P, T_target)
    rho_tabular = tabular.density(P, T_target)
    assert pytest.approx(rho_analytical, rel=0.02) == rho_tabular

    # State from (P, S) API
    S_target = analytical.specific_entropy(P, T_target)
    T1, rho1, nad1 = analytical.get_state_from_PS(P, S_target)
    T2, rho2, nad2 = tabular.get_state_from_PS(P, S_target)

    assert pytest.approx(T1, rel=0.15) == T2
    assert pytest.approx(rho1, rel=0.20) == rho2


def test_core_eos():
    c_const = ConstantDensityCoreEOS(rho_core=8000.0)
    assert c_const.density(1e10) == 8000.0

    c_bm = BirchMurnaghanCoreEOS(rho_0=5500.0, K_0=200.0 * GPa)
    rho_zero = c_bm.density(0.0)
    assert pytest.approx(rho_zero, abs=1.0) == 5500.0

    rho_high_p = c_bm.density(100.0 * GPa)
    assert rho_high_p > 5500.0
