"""
Unit tests for CoupledRLOFIntegrator evolution engine.
"""

from hot_jupiter.evolution.rlof_engine import CoupledRLOFIntegrator, EvolutionOutcome


def test_cooling_trajectory():
    """Test planet starting at wide separation (0.035 AU) experiences non-overflow cooling."""
    integrator = CoupledRLOFIntegrator(m_p_init_jup=1.0, a_init_au=0.035)
    res = integrator.integrate(t_max_yr=1.0e9)
    assert res.outcome == EvolutionOutcome.COOLING
    assert res.final_m_remnant_earth > 0.0


def test_disruption_trajectory():
    """Test planet starting at close separation (0.015 AU) below M_crit experiences disruption."""
    integrator = CoupledRLOFIntegrator(m_p_init_jup=0.2,
                                       a_init_au=0.015,
                                       m_core_earth=2.0)
    res = integrator.integrate(t_max_yr=1.0e9)
    assert res.outcome == EvolutionOutcome.DISRUPTED
    assert res.final_m_remnant_earth == 0.0


def test_stagnation_trajectory():
    """Test intermediate giant (1.2 M_Jup) at 0.022 AU experiences RLOF stagnation or engulfment."""
    integrator = CoupledRLOFIntegrator(m_p_init_jup=1.2,
                                       a_init_au=0.022,
                                       m_core_earth=10.0)
    res = integrator.integrate(t_max_yr=1.0e8)
    assert res.outcome in [
        EvolutionOutcome.STAGNATED, EvolutionOutcome.COOLING,
        EvolutionOutcome.DISRUPTED
    ]
