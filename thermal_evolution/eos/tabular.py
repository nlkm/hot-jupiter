"""
Tabular Equation of State for Hydrogen-Helium gas mixtures.
Interpolates 2D thermodynamic tables (e.g. SCVH95, CMS19, or custom grids).
"""

import os
from typing import Union, Tuple, Optional
import numpy as np
from scipy.interpolate import RegularGridInterpolator
from scipy.optimize import brentq

from thermal_evolution.eos.base import BaseEOS
from thermal_evolution.eos.analytical import AnalyticalHHeEOS


class TabularEOS(BaseEOS):
    """
    Tabular H-He EOS using RegularGridInterpolator over log10(P) and log10(T) axes.
    """

    def __init__(
        self,
        log10_P: np.ndarray,
        log10_T: np.ndarray,
        log10_rho_table: np.ndarray,
        S_table: np.ndarray,
        nabla_ad_table: np.ndarray,
        u_table: Optional[np.ndarray] = None,
        X: float = 0.75,
        Y: float = 0.25,
    ):
        """
        Parameters
        ----------
        log10_P : 1D array of log10(P [Pa])
        log10_T : 1D array of log10(T [K])
        log10_rho_table : 2D array of log10(rho [kg/m^3]) with shape (len(log10_P), len(log10_T))
        S_table : 2D array of S [J/(kg K)] with shape (len(log10_P), len(log10_T))
        nabla_ad_table : 2D array of nabla_ad with shape (len(log10_P), len(log10_T))
        u_table : 2D array of specific internal energy u [J/kg], optional
        X, Y : mass fractions
        """
        self.log10_P = np.asarray(log10_P, dtype=float)
        self.log10_T = np.asarray(log10_T, dtype=float)
        self.X = X
        self.Y = Y

        # Setup 2D Interpolators (bounds_error=False, fill_value=None for nearest extrapolation)
        self._interp_log_rho = RegularGridInterpolator(
            (self.log10_P, self.log10_T),
            log10_rho_table,
            bounds_error=False,
            fill_value=None,
        )
        self._interp_S = RegularGridInterpolator(
            (self.log10_P, self.log10_T),
            S_table,
            bounds_error=False,
            fill_value=None,
        )
        self._interp_nabla_ad = RegularGridInterpolator(
            (self.log10_P, self.log10_T),
            nabla_ad_table,
            bounds_error=False,
            fill_value=None,
        )

        if u_table is not None:
            self._interp_u = RegularGridInterpolator(
                (self.log10_P, self.log10_T),
                u_table,
                bounds_error=False,
                fill_value=None,
            )
        else:
            self._interp_u = None

    def density(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        is_scalar = np.isscalar(P) and np.isscalar(T)
        lP = np.log10(np.atleast_1d(P))
        lT = np.log10(np.atleast_1d(T))

        pts = np.column_stack([lP, lT])
        log_rho = self._interp_log_rho(pts)
        rho = 10.0**log_rho

        return float(rho[0]) if is_scalar else rho

    def specific_entropy(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        is_scalar = np.isscalar(P) and np.isscalar(T)
        lP = np.log10(np.atleast_1d(P))
        lT = np.log10(np.atleast_1d(T))

        pts = np.column_stack([lP, lT])
        S = self._interp_S(pts)

        return float(S[0]) if is_scalar else S

    def nabla_ad(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        is_scalar = np.isscalar(P) and np.isscalar(T)
        lP = np.log10(np.atleast_1d(P))
        lT = np.log10(np.atleast_1d(T))

        pts = np.column_stack([lP, lT])
        nad = self._interp_nabla_ad(pts)

        return float(nad[0]) if is_scalar else nad

    def internal_energy(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        if self._interp_u is None:
            # Fallback estimation u ~ P / (rho * (gamma - 1))
            rho = self.density(P, T, X, Y)
            return P / (rho * 0.4)

        is_scalar = np.isscalar(P) and np.isscalar(T)
        lP = np.log10(np.atleast_1d(P))
        lT = np.log10(np.atleast_1d(T))

        pts = np.column_stack([lP, lT])
        u = self._interp_u(pts)

        return float(u[0]) if is_scalar else u

    def temperature_from_PS(
        self,
        P: Union[float, np.ndarray],
        S: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """Invert S(P, T) table to find T given P and S."""
        is_scalar = np.isscalar(P) and np.isscalar(S)
        P_arr = np.atleast_1d(P)
        S_arr = np.atleast_1d(S)

        T_min = 10.0**self.log10_T[0]
        T_max = 10.0**self.log10_T[-1]

        T_out = np.zeros_like(P_arr, dtype=float)

        for i in range(len(P_arr)):
            p_val = P_arr[i]
            s_val = S_arr[i]

            def residual(log_T):
                t_guess = np.exp(log_T)
                return self.specific_entropy(p_val, t_guess, X, Y) - s_val

            try:
                log_T_sol = brentq(residual, np.log(T_min), np.log(T_max))
                T_out[i] = np.exp(log_T_sol)
            except ValueError:
                # If target entropy is outside table T range, pick closest endpoint
                s_low = self.specific_entropy(p_val, T_min, X, Y)
                s_high = self.specific_entropy(p_val, T_max, X, Y)
                if abs(s_val - s_low) < abs(s_val - s_high):
                    T_out[i] = T_min
                else:
                    T_out[i] = T_max

        return float(T_out[0]) if is_scalar else T_out

    _cached_synthetic_grid = None

    @classmethod
    def create_synthetic_grid(
        cls,
        log_P_min: float = 2.0,     # 10^2 Pa (1 mbar)
        log_P_max: float = 13.0,    # 10^13 Pa (100 Mbar)
        log_T_min: float = 2.0,     # 100 K
        log_T_max: float = 6.0,     # 1,000,000 K
        n_P: int = 40,
        n_T: int = 40,
        X: float = 0.75,
        Y: float = 0.25,
        use_cache: bool = True,
    ) -> "TabularEOS":
        """
        Generate or return a cached synthetic high-resolution table.
        """
        if use_cache and cls._cached_synthetic_grid is not None:
            return cls._cached_synthetic_grid

        log10_P = np.linspace(log_P_min, log_P_max, n_P)
        log10_T = np.linspace(log_T_min, log_T_max, n_T)

        analytical = AnalyticalHHeEOS()

        PP, TT = np.meshgrid(10.0**log10_P, 10.0**log10_T, indexing="ij")
        
        rho_table = analytical.density(PP, TT, X=X, Y=Y)
        S_table = analytical.specific_entropy(PP, TT, X=X, Y=Y)
        nad_table = analytical.nabla_ad(PP, TT, X=X, Y=Y)
        u_table = analytical.internal_energy(PP, TT, X=X, Y=Y)

        grid = cls(
            log10_P=log10_P,
            log10_T=log10_T,
            log10_rho_table=np.log10(rho_table),
            S_table=S_table,
            nabla_ad_table=nad_table,
            u_table=u_table,
            X=X,
            Y=Y,
        )

        if use_cache:
            cls._cached_synthetic_grid = grid

        return grid

    @classmethod
    def from_npz(cls, filepath: str) -> "TabularEOS":
        """Load tabular EOS from a .npz file containing log10_P, log10_T, log10_rho, S, nabla_ad, u."""
        data = np.load(filepath)
        return cls(
            log10_P=data["log10_P"],
            log10_T=data["log10_T"],
            log10_rho_table=data["log10_rho"],
            S_table=data["S"],
            nabla_ad_table=data["nabla_ad"],
            u_table=data.get("u"),
            X=float(data.get("X", 0.75)),
            Y=float(data.get("Y", 0.25)),
        )
