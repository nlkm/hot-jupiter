"""
Tidal heating dissipation models for giant planets.
Includes eccentricity damping and spin-orbit asynchronous dissipation.
"""

import numpy as np

from hot_jupiter.constants import AU, M_SUN, G
from hot_jupiter.heating.base import BaseHeatingSource


class TidalEccentricityHeating(BaseHeatingSource):
    """
    Full equilibrium tidal dissipation model (Hut 1981, Leconte et al. 2010).
    P_tidal = P_eccentricity + P_spin_asynchronous
    """

    def __init__(
            self,
            M_star: float = 1.0 * M_SUN,
            a: float = 0.05 * AU,  # Semi-major axis [m]
            eccentricity: float = 0.05,  # Orbital eccentricity
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
        orbit_params: dict | None = None,
    ) -> float:
        a = orbit_params.get("a", self.a) if orbit_params else self.a
        e = orbit_params.get(
            "eccentricity",
            self.eccentricity) if orbit_params else self.eccentricity
        M_star = orbit_params.get("M_star",
                                  self.M_star) if orbit_params else self.M_star
        Omega_rot = orbit_params.get("Omega_rot",
                                     None) if orbit_params else None
        obliquity = orbit_params.get("obliquity", 0.0) if orbit_params else 0.0

        if a <= 0 or R_p <= 0:
            return 0.0

        n = np.sqrt(G * M_star / (a**3))  # Mean motion

        # 1. Eccentricity tidal heating
        # P_ecc = (21/2) * (k2/Q) * G * M_*^2 * R_p^5 * e^2 / a^6
        p_ecc = 10.5 * self.k2_over_Q * G * (M_star**2) * (R_p**
                                                           5) * (e**2) / (a**6)

        # 2. Spin-orbit asynchronous heating (if Omega_rot provided)
        p_spin = 0.0
        if Omega_rot is not None:
            delta_omega = Omega_rot - n * np.cos(obliquity)
            p_spin = 1.5 * self.k2_over_Q * G * (M_star**2) * (R_p**5) * (
                delta_omega**2) / (a**6)

        return float(p_ecc + p_spin)
