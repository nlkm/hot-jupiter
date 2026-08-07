"""
Unit test for coupled RLOF mass loss and tidal evolution integrator (hot_jupiter/evolution/rlof_coupled.py).
"""

from hot_jupiter.constants import AU, BAR, M_EARTH, M_JUP, YEAR
from hot_jupiter.eos import AnalyticalHHeEOS
from hot_jupiter.evolution.rlof_coupled import (
    CoupledRLOFEvolutionIntegrator,
    CoupledRLOFEvolutionResult,
)


def test_coupled_rlof_integrator_basic():
    eos = AnalyticalHHeEOS()
    integrator = CoupledRLOFEvolutionIntegrator()

    M_p_init = 1.0 * M_JUP
    M_c = 10.0 * M_EARTH
    a_init = 0.02 * AU
    e_init = 0.05
    S_initial = eos.specific_entropy(1.0 * BAR, 2500.0)

    # 10 Myr short evolutionary run
    t_span = (1.0e6 * YEAR, 1.0e7 * YEAR)
    res = integrator.evolve(
        M_p_init=M_p_init,
        M_c=M_c,
        a_init=a_init,
        e_init=e_init,
        S_initial=S_initial,
        t_span=t_span,
        num_eval=10,
    )

    assert isinstance(res, CoupledRLOFEvolutionResult)
    assert len(res.t) == 10
    assert len(res.M_p) == 10
    assert len(res.a) == 10
    assert res.outcome in [
        "Disrupted/Engulfed", "Stagnated/Survived", "Cooling"
    ]
    assert res.R_roche[0] > 0.0
