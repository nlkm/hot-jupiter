"""
Ohmic Dissipation Heating Model for Irradiated Giant Planets.
Ref: Batygin & Stevenson (2010), Thorngren & Fortney (2018).
"""

import numpy as np

from hot_jupiter.constants import SIGMA_SB
from hot_jupiter.heating.base import BaseHeatingSource


class OhmicDissipationHeating(BaseHeatingSource):
    """
    Ohmic dissipation interior heating model powered by thermally ionized atmospheric winds.
    Efficiency peaks around T_eq ~ 1500-1800 K (Thorngren & Fortney 2018).
    P_ohmic = epsilon(T_eq) * P_inc_absorbed
    """

    def __init__(
            self,
            epsilon_max: float = 0.025,  # 2.5% max efficiency
            T_peak: float = 1600.0,  # Peak efficiency temperature [K]
            sigma_T: float = 300.0,  # Gaussian width [K]
    ):
        self.epsilon_max = epsilon_max
        self.T_peak = T_peak
        self.sigma_T = sigma_T

    def evaluate_power(
        self,
        t: float,
        R_p: float,
        M_p: float,
        S_env: float,
        orbit_params: dict | None = None,
    ) -> float:
        if not orbit_params:
            return 0.0

        F_inc = orbit_params.get("F_inc", 0.0)
        A_b = orbit_params.get("A_b", 0.1)

        if F_inc <= 0:
            return 0.0

        # Equilibrium temperature T_eq = (F_inc * (1 - A_b) / (4 * sigma))^(1/4)
        F_abs = F_inc * (1.0 - A_b) / 4.0
        T_eq = (F_abs / SIGMA_SB)**0.25

        # Gaussian efficiency curve
        epsilon = self.epsilon_max * np.exp(-0.5 * (
            (T_eq - self.T_peak) / self.sigma_T)**2)

        # Total absorbed stellar power P_absorbed = pi * R_p^2 * F_inc * (1 - A_b)
        P_abs_total = np.pi * (R_p**2) * F_inc * (1.0 - A_b)
        P_ohmic = epsilon * P_abs_total

        return float(P_ohmic)
