"""
Abstract interface for atmospheric boundary models.
"""

from abc import ABC, abstractmethod
from typing import NamedTuple

from thermal_evolution.constants import SIGMA_SB


class AtmosphereResult(NamedTuple):
    """Output from atmospheric boundary solver."""
    T_int: float       # Intrinsic effective temperature [K]
    T_eff: float       # Total effective temperature [K] (including stellar irradiation)
    L_int: float       # Net outgoing intrinsic power [W]
    P_rcb: float       # Pressure at Radiative-Convective Boundary [Pa]
    T_rcb: float       # Temperature at Radiative-Convective Boundary [K]


class BaseAtmosphere(ABC):
    """
    Abstract interface for planet atmosphere boundary models.
    """

    @abstractmethod
    def evaluate_atmosphere(
        self,
        M_p: float,
        R_p: float,
        S_env: float,
        F_inc: float = 0.0,
        A_b: float = 0.1,
    ) -> AtmosphereResult:
        """
        Compute net outgoing intrinsic power L_int and effective temperatures T_int, T_eff.

        Parameters
        ----------
        M_p : float
            Planet mass [kg].
        R_p : float
            Planet radius [m].
        S_env : float
            Envelope specific entropy [J/(kg K)].
        F_inc : float
            Incident stellar flux at orbital distance [W/m^2].
        A_b : float
            Bond albedo (default 0.1).

        Returns
        -------
        AtmosphereResult namedtuple.
        """
        pass
