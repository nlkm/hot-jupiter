"""
Abstract Base Class for Equations of State (EOS).
"""

from abc import ABC, abstractmethod

import numpy as np


class BaseEOS(ABC):
    """
    Abstract interface for Hydrogen-Helium envelope equations of state.
    
    All quantities use SI units:
    - Pressure P: Pa
    - Temperature T: K
    - Density rho: kg/m^3
    - Specific Entropy S: J / (kg K)
    - Specific Internal Energy u: J / kg
    - Hydrogen mass fraction X (default 0.75)
    - Helium mass fraction Y (default 0.25, X + Y + Z = 1)
    """

    @abstractmethod
    def density(
        self,
        P: float | np.ndarray,
        T: float | np.ndarray,
        X: float = 0.75,
        Y: float = 0.25,
    ) -> float | np.ndarray:
        """Return mass density rho [kg/m^3] given P [Pa] and T [K]."""

    @abstractmethod
    def specific_entropy(
        self,
        P: float | np.ndarray,
        T: float | np.ndarray,
        X: float = 0.75,
        Y: float = 0.25,
    ) -> float | np.ndarray:
        """Return specific entropy S [J/(kg K)] given P [Pa] and T [K]."""

    @abstractmethod
    def temperature_from_PS(
        self,
        P: float | np.ndarray,
        S: float | np.ndarray,
        X: float = 0.75,
        Y: float = 0.25,
    ) -> float | np.ndarray:
        """Return temperature T [K] given P [Pa] and specific entropy S [J/(kg K)]."""

    @abstractmethod
    def nabla_ad(
        self,
        P: float | np.ndarray,
        T: float | np.ndarray,
        X: float = 0.75,
        Y: float = 0.25,
    ) -> float | np.ndarray:
        """Return adiabatic temperature gradient nabla_ad = (d ln T / d ln P)_S."""

    @abstractmethod
    def internal_energy(
        self,
        P: float | np.ndarray,
        T: float | np.ndarray,
        X: float = 0.75,
        Y: float = 0.25,
    ) -> float | np.ndarray:
        """Return specific internal energy u [J/kg] given P [Pa] and T [K]."""

    def get_state_from_PS(
        self,
        P: float,
        S: float,
        X: float = 0.75,
        Y: float = 0.25,
    ) -> tuple[float, float, float]:
        """
        Convenience method: returns (T, rho, nabla_ad) at a given pressure P and entropy S.
        """
        T = float(self.temperature_from_PS(P, S, X, Y))
        rho = float(self.density(P, T, X, Y))
        nad = float(self.nabla_ad(P, T, X, Y))
        return T, rho, nad
