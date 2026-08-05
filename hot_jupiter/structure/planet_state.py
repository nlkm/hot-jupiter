"""
Container for planet state parameters and solved 1D internal profile.
"""

from dataclasses import dataclass
from typing import Optional
import numpy as np


@dataclass
class InternalProfile:
    """Detailed 1D profile of the planet interior."""
    m: np.ndarray        # Enclosed mass profile [kg] (from 0 to M_p)
    r: np.ndarray        # Radius profile [m] (from 0 to R_p)
    P: np.ndarray        # Pressure profile [Pa]
    rho: np.ndarray      # Mass density profile [kg/m^3]
    T: np.ndarray        # Temperature profile [K]
    nabla_ad: np.ndarray # Adiabatic temperature gradient


@dataclass
class PlanetStructure:
    """Summary metrics of a solved 1D hydrostatic planet structure."""
    M_p: float           # Total planet mass [kg]
    M_c: float           # Heavy element core mass [kg]
    S_env: float         # Convective envelope specific entropy [J / (kg K)]
    R_p: float           # Total planet radius [m]
    R_c: float           # Core radius [m]
    P_c: float           # Central core pressure [Pa]
    T_c: float           # Core temperature [K]
    T_cb: float          # Core-envelope boundary temperature [K]
    P_cb: float          # Core-envelope boundary pressure [Pa]
    int_T_dm: float      # Thermal energy integral \int_0^{M_p} T dm [K kg]
    E_int: float         # Total internal thermal energy [J]
    U_grav: float        # Gravitational potential energy [J]
    profile: Optional[InternalProfile] = None

    @property
    def E_tot(self) -> float:
        """Total planet energy E_tot = E_int + U_grav [J]."""
        return self.E_int + self.U_grav
