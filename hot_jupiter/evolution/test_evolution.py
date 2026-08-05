"""
Unit tests for atmosphere, heating, and thermal evolution time integrator.
"""

import pytest
import numpy as np

from hot_jupiter.constants import M_JUP, M_EARTH, R_JUP, BAR, YEAR, GYR, AU, M_SUN
from hot_jupiter.eos import AnalyticalHHeEOS, TabularEOS
from hot_jupiter.structure import InteriorSolver
from hot_jupiter.atmosphere import GuillotAtmosphere, AtmosphereResult
from hot_jupiter.heating import TidalEccentricityHeating, ConstantHeating, ZeroHeating
from hot_jupiter.evolution import ThermalEvolutionIntegrator, EvolutionResult


def test_guillot_atmosphere():
    eos = AnalyticalHHeEOS()
    atmos = GuillotAtmosphere(envelope_eos=eos)

    M_p = 1.0 * M_JUP
    R_p = 1.0 * R_JUP
    S_env = eos.specific_entropy(1.0 * BAR, 165.0)

    # Isolated planet (no stellar irradiation)
    res_iso = atmos.evaluate_atmosphere(M_p=M_p, R_p=R_p, S_env=S_env, F_inc=0.0)
    assert isinstance(res_iso, AtmosphereResult)
    assert res_iso.T_int > 0.0
    assert res_iso.L_int > 0.0
    assert res_iso.T_eff == res_iso.T_int

    # Irradiated planet (F_inc = 1e5 W/m^2 ~ Hot Jupiter at 0.05 AU)
    res_irr = atmos.evaluate_atmosphere(M_p=M_p, R_p=R_p, S_env=S_env, F_inc=1.0e5)
    assert res_irr.T_eff > res_irr.T_int


def test_tidal_heating():
    heating = TidalEccentricityHeating(M_star=1.0 * M_SUN, a=0.05 * AU, eccentricity=0.05)
    p_tidal = heating.evaluate_power(t=0.0, R_p=1.2 * R_JUP, M_p=1.0 * M_JUP, S_env=100000.0)
    assert p_tidal > 0.0


def test_hot_jupiter_cooling_track():
    eos = AnalyticalHHeEOS()
    solver = InteriorSolver(envelope_eos=eos)
    atmos = GuillotAtmosphere(envelope_eos=eos)
    integrator = ThermalEvolutionIntegrator(
        interior_solver=solver,
        atmosphere_model=atmos,
        heating_source=ZeroHeating(),
    )

    M_p = 1.0 * M_JUP
    M_c = 10.0 * M_EARTH
    S_initial = eos.specific_entropy(1.0 * BAR, 2500.0)  # Hot young planet

    # Short 10 Myr evolution run
    t_span = (1.0e6 * YEAR, 1.0e7 * YEAR)
    result = integrator.evolve(
        M_p=M_p,
        M_c=M_c,
        S_initial=S_initial,
        t_span=t_span,
        num_eval=5,
    )

    assert isinstance(result, EvolutionResult)
    assert len(result.t) == 5
    assert result.S[0] > result.S[-1]        # Entropy decreases (cooling)
    assert result.R_p[0] >= result.R_p[-1]    # Planet contracts as it cools
    assert result.L_int[0] > result.L_int[-1] # Intrinsic luminosity decreases
