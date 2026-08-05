"""
Unit tests for Roche Lobe Overflow (RLOF) mass loss module (hot_jupiter/mass_loss.py).
"""

from hot_jupiter.constants import AU, M_JUP, M_SUN
from hot_jupiter.mass_loss import RocheLobeMassLoss


def test_roche_lobe_radius():
    r_roche = RocheLobeMassLoss.roche_lobe_radius(a=0.02 * AU,
                                                  M_p=1.0 * M_JUP,
                                                  M_star=1.0 * M_SUN)
    assert r_roche > 0.0
    assert r_roche < 0.02 * AU


def test_roche_lobe_mass_loss_rate():
    rlof = RocheLobeMassLoss()
    a = 0.02 * AU
    M_p = 1.0 * M_JUP
    M_star = 1.0 * M_SUN

    r_roche = rlof.roche_lobe_radius(a, M_p, M_star)

    # Planet comfortably inside Roche Lobe -> 0 mass loss
    dM_dt, da_dt = rlof.evaluate_mass_loss_rate(R_p=0.5 * r_roche,
                                                a=a,
                                                M_p=M_p,
                                                M_star=M_star)
    assert dM_dt == 0.0
    assert da_dt == 0.0

    # Planet overflowing Roche Lobe (R_p > R_Roche) -> Negative mass loss
    dM_dt_overflow, da_dt_overflow = rlof.evaluate_mass_loss_rate(R_p=1.1 *
                                                                  r_roche,
                                                                  a=a,
                                                                  M_p=M_p,
                                                                  M_star=M_star)
    assert dM_dt_overflow < 0.0
    assert da_dt_overflow > 0.0
