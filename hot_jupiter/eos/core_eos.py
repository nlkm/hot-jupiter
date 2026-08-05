"""
Heavy Element Core Equations of State (Rock/Ice mix).
"""

import numpy as np
from scipy.optimize import brentq

from hot_jupiter.constants import GPa


class BaseCoreEOS:
    """Abstract interface for heavy element core EOS."""

    def density(self, P: float | np.ndarray) -> float | np.ndarray:
        """Mass density rho [kg/m^3] given pressure P [Pa]."""
        raise NotImplementedError


class ConstantDensityCoreEOS(BaseCoreEOS):
    """Simple constant density core model."""

    def __init__(self, rho_core: float = 8000.0):
        """
        Parameters
        ----------
        rho_core : float
            Core density in kg/m^3 (default 8000 kg/m^3 = 8 g/cm^3).
        """
        self.rho_core = rho_core

    def density(self, P: float | np.ndarray) -> float | np.ndarray:
        if np.isscalar(P):
            return self.rho_core
        return np.full_like(P, self.rho_core, dtype=float)


class BirchMurnaghanCoreEOS(BaseCoreEOS):
    """
    3rd order Birch-Murnaghan isothermal EOS for high-pressure rock/ice core.
    P(rho) = (3/2) * K_0 * [(rho/rho_0)^(7/3) - (rho/rho_0)^(5/3)] * 
             [ 1 + (3/4)*(K'_0 - 4) * ((rho/rho_0)^(2/3) - 1) ]
    """

    def __init__(
            self,
            rho_0: float = 5500.0,  # Zero-pressure density (kg/m^3)
            K_0: float = 200.0 * GPa,  # Bulk modulus (Pa)
            Kp_0: float = 4.0,  # Pressure derivative K'_0
    ):
        self.rho_0 = rho_0
        self.K_0 = K_0
        self.Kp_0 = Kp_0

    def pressure_from_density(self,
                              rho: float | np.ndarray) -> float | np.ndarray:
        """Compute P [Pa] for given density rho [kg/m^3]."""
        eta = rho / self.rho_0
        eta_23 = eta**(2.0 / 3.0)
        p = (1.5 * self.K_0 * (eta**(7.0 / 3.0) - eta**(5.0 / 3.0)) *
             (1.0 + 0.75 * (self.Kp_0 - 4.0) * (eta_23 - 1.0)))
        return p

    def density(self, P: float | np.ndarray) -> float | np.ndarray:
        """Invert P(rho) to find density rho [kg/m^3] for given pressure P [Pa]."""
        is_scalar = np.isscalar(P)
        P_arr = np.atleast_1d(P)
        rho_out = np.zeros_like(P_arr, dtype=float)

        for i, p_val in enumerate(P_arr):
            if p_val <= 0:
                rho_out[i] = self.rho_0
                continue

            def residual(rho_guess, pv=p_val):
                return self.pressure_from_density(rho_guess) - pv

            try:
                # Core density search range: rho_0 to 10 * rho_0
                rho_out[i] = brentq(residual, self.rho_0, 10.0 * self.rho_0)
            except ValueError:
                # High pressure asymptotic approximation
                rho_out[i] = self.rho_0 * (1.0 + p_val / self.K_0)**0.25

        return float(rho_out[0]) if is_scalar else rho_out
