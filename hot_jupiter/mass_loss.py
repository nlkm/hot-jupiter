"""
Roche Lobe Overflow (RLOF) and Atmospheric Mass-Loss Module for Giant Planets.
Implements Eggleton (1983), Paczyński (1971), Rappaport et al. (2013), Jackson et al. (2017).
"""

from dataclasses import dataclass

import numpy as np

from hot_jupiter.constants import YEAR


@dataclass
class RocheLobeMassLoss:
    """
    Evaluates Roche Lobe Radius and Hydrodynamic Mass-Loss Rates during Roche Lobe Overflow (RLOF).
    """
    eta_exponent: float = 4.0  # Hydrodynamic scaling exponent
    M_dot_0: float = 1.0e11  # Onset mass loss rate [kg/s] (~1.5 M_earth / Gyr)
    momentum_fraction_beta: float = 0.5  # Fraction of orbital angular momentum retained

    @staticmethod
    def roche_lobe_radius(a: float, M_p: float, M_star: float) -> float:
        """
        Compute volume-equivalent Roche lobe radius R_Roche [m] using Eggleton (1983).

        Parameters
        ----------
        a : float [m]
            Semi-major axis.
        M_p : float [kg]
            Planet mass.
        M_star : float [kg]
            Host star mass.

        Returns
        -------
        R_Roche : float [m]
        """
        if a <= 0 or M_p <= 0 or M_star <= 0:
            return 0.0

        q = M_p / M_star
        q_13 = q**(1.0 / 3.0)
        q_23 = q**(2.0 / 3.0)

        # Eggleton (1983) formula
        r_roche_ratio = 0.49 * q_23 / (0.6 * q_23 + np.log(1.0 + q_13))
        return float(a * r_roche_ratio)

    def roche_lobe_filling_factor(self, R_p: float, a: float, M_p: float,
                                  M_star: float) -> float:
        """
        Compute Roche lobe filling factor mu_Roche = R_p / R_Roche.
        """
        r_roche = self.roche_lobe_radius(a, M_p, M_star)
        return float(R_p / r_roche) if r_roche > 0 else 0.0

    def evaluate_mass_loss_rate(
        self,
        R_p: float,
        a: float,
        M_p: float,
        M_star: float,
    ) -> tuple[float, float]:
        """
        Evaluate (dM_p_dt_rlof, da_dt_rlof).

        Returns
        -------
        dM_p_dt_rlof : float [kg/s]
            Planet mass loss rate (negative when R_p >= R_Roche).
        da_dt_rlof : float [m/s]
            Semi-major axis rate from RLOF mass loss.
        """
        r_roche = self.roche_lobe_radius(a, M_p, M_star)
        if r_roche <= 0 or R_p <= 0 or M_p <= 0:
            return 0.0, 0.0

        filling_factor = R_p / r_roche

        if filling_factor < 0.95:
            # Below overflow threshold
            return 0.0, 0.0

        # Hydrodynamic RLOF mass-loss rate: dM/dt = - M_dot_0 * exp( eta * (R_p / R_Roche - 1) )
        overflow_excess = max(0.0, filling_factor - 1.0)
        dM_dt = -self.M_dot_0 * np.exp(self.eta_exponent * overflow_excess)

        # Cap maximum mass loss rate to 10% of planet mass per Gyr to ensure numerical stability
        max_dM_dt = -0.10 * M_p / (1.0e9 * YEAR)
        dM_dt = max(dM_dt, max_dM_dt)

        # Orbital back-reaction da/dt |_RLOF = - 2 * a * (dM/dt / M_p) * (1 - beta)
        da_dt_rlof = -2.0 * a * (dM_dt / M_p) * (1.0 -
                                                 self.momentum_fraction_beta)

        return float(dM_dt), float(da_dt_rlof)
