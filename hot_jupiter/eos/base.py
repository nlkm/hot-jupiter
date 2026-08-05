"""
Abstract Base Class for Equations of State (EOS).
"""

from abc import ABC, abstractmethod
from typing import Tuple, Union
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
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """Return mass density rho [kg/m^3] given P [Pa] and T [K]."""
        pass

    @abstractmethod
    def specific_entropy(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """Return specific entropy S [J/(kg K)] given P [Pa] and T [K]."""
        pass

    @abstractmethod
    def temperature_from_PS(
        self,
        P: Union[float, np.ndarray],
        S: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """Return temperature T [K] given P [Pa] and specific entropy S [J/(kg K)]."""
        pass

    @abstractmethod
    def nabla_ad(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """Return adiabatic temperature gradient nabla_ad = (d ln T / d ln P)_S."""
        pass

    @abstractmethod
    def internal_energy(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """Return specific internal energy u [J/kg] given P [Pa] and T [K]."""
        pass

    def get_state_from_PS(
        self,
        P: float,
        S: float,
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Tuple[float, float, float]:
        """
        Convenience method: returns (T, rho, nabla_ad) at a given pressure P and entropy S.
        """
        T = float(self.temperature_from_PS(P, S, X, Y))
        rho = float(self.density(P, T, X, Y))
        nad = float(self.nabla_ad(P, T, X, Y))
        return T, rho, nad
