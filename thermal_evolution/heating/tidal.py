"""
Tidal heating dissipation models for giant planets.
"""

from typing import Optional
import numpy as np

from thermal_evolution.constants import G, M_SUN, AU
from thermal_evolution.heating.base import BaseHeatingSource


class TidalEccentricityHeating(BaseHeatingSource):
    """
    Tidal dissipation powered by orbital eccentricity damping (Goldreich & Soter 1966, Jackson et al. 2008).
    P_tidal = (21/2) * (k2/Q) * (G * M_*^2 * R_p^5 * e^2) / a^6
    """

    def __init__(
        self,
        M_star: float = 1.0 * M_SUN,
        a: float = 0.05 * AU,       # Semi-major axis [m] (e.g. Hot Jupiter at 0.05 AU)
        eccentricity: float = 0.05, # Orbital eccentricity
        k2_over_Q: float = 1.0e-5,  # Tidal dissipation factor k2 / Q
    ):
        self.M_star = M_star
        self.a = a
        self.eccentricity = eccentricity
        self.k2_over_Q = k2_over_Q

    def evaluate_power(
        self,
        t: float,
        R_p: float,
        M_p: float,
        S_env: float,
        orbit_params: Optional[dict] = None,
    ) -> float:
        a = orbit_params.get("a", self.a) if orbit_params else self.a
        e = orbit_params.get("eccentricity", self.eccentricity) if orbit_params else self.eccentricity
        M_star = orbit_params.get("M_star", self.M_star) if orbit_params else self.M_star

        if e <= 0 or a <= 0:
            return 0.0

        # P_tidal = 10.5 * (k2/Q) * G * M_*^2 * R_p^5 * e^2 / a^6
        p_tidal = 10.5 * self.k2_over_Q * G * (M_star**2) * (R_p**5) * (e**2) / (a**6)
        return float(p_tidal)
