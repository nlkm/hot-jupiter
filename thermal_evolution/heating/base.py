"""
Abstract interface for extra energy injection sources (tidal dissipation, radiogenic heating, etc.).
"""

from abc import ABC, abstractmethod
from typing import Optional, List


class BaseHeatingSource(ABC):
    """Abstract interface for interior heating sources."""

    @abstractmethod
    def evaluate_power(
        self,
        t: float,
        R_p: float,
        M_p: float,
        S_env: float,
        orbit_params: Optional[dict] = None,
    ) -> float:
        """
        Return power injected into interior [W] at time t [s].
        """
        pass


class ZeroHeating(BaseHeatingSource):
    """No extra interior heating."""

    def evaluate_power(
        self,
        t: float,
        R_p: float,
        M_p: float,
        S_env: float,
        orbit_params: Optional[dict] = None,
    ) -> float:
        return 0.0


class ConstantHeating(BaseHeatingSource):
    """Constant power injection into planet interior [W]."""

    def __init__(self, P_0: float = 1e18):
        self.P_0 = P_0

    def evaluate_power(
        self,
        t: float,
        R_p: float,
        M_p: float,
        S_env: float,
        orbit_params: Optional[dict] = None,
    ) -> float:
        return self.P_0
