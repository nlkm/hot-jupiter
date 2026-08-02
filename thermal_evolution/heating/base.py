"""
Abstract interface for extra energy injection sources (tidal dissipation, radiogenic heating, etc.).
"""

from abc import ABC, abstractmethod
from typing import Optional, List
import numpy as np

from thermal_evolution.constants import M_EARTH, YEAR, GYR


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


class RadiogenicHeating(BaseHeatingSource):
    """
    Core radiogenic decay heating (U, Th, K decay in heavy-element core).
    P_radio(t) = P_0 * (M_c / M_Earth) * exp(-t / tau_decay)
    """

    def __init__(self, M_c: float = 10.0 * M_EARTH, P_spec: float = 1.0e-11, tau_decay_gyr: float = 3.0):
        self.M_c = M_c
        self.P_spec = P_spec  # W / kg of core material
        self.tau_decay = tau_decay_gyr * GYR

    def evaluate_power(
        self,
        t: float,
        R_p: float,
        M_p: float,
        S_env: float,
        orbit_params: Optional[dict] = None,
    ) -> float:
        return self.P_spec * self.M_c * np.exp(-t / self.tau_decay)
