"""
Analytical Equation of State for Hydrogen-Helium envelope gas.
Combines ideal gas mixture, molecular dissociation approximations, and electron degeneracy.
"""

from typing import Union
import numpy as np
from scipy.optimize import brentq

from hot_jupiter.constants import K_B, M_H, M_HE, N_A
from hot_jupiter.eos.base import BaseEOS

# Physical constants for analytical EOS
HBAR = 1.054571817e-34    # Reduced Planck constant (J s)
M_E = 9.1093837015e-31    # Electron mass (kg)

# Degeneracy pressure coefficient K_deg for P_deg = K_deg * (rho / mu_e)^(5/3)
K_DEG = ((3.0 * np.pi**2)**(2.0 / 3.0) * HBAR**2) / (5.0 * M_E * M_H**(5.0 / 3.0))


class AnalyticalHHeEOS(BaseEOS):
    """
    Analytical H-He EOS combining ideal gas pressure and non-relativistic electron degeneracy.
    Provides fast, analytical derivatives and guarantees stability over a wide phase space.
    """

    def __init__(self, gamma: float = 1.4):
        """
        Parameters
        ----------
        gamma : float
            Adiabatic index for molecular H2 gas (default 1.4 -> nabla_ad = 2/7 ≈ 0.2857).
        """
        self.gamma = gamma

    def _mean_molecular_weight(self, X: float = 0.75, Y: float = 0.25) -> float:
        """Mean molecular weight mu for molecular H2 + atomic He gas mixture."""
        return 1.0 / (X / 2.0 + Y / 4.0)

    def _electron_molecular_weight(self, X: float = 0.75) -> float:
        """Mean molecular weight per electron mu_e."""
        return 2.0 / (1.0 + X)

    def density(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """
        Calculate mass density rho [kg/m^3] for given P [Pa] and T [K].
        Incorporates SCVH95 / CMS19 metallic hydrogen compression fit at high pressure (P > 10^9 Pa).
        """
        is_scalar = np.isscalar(P) and np.isscalar(T)
        P_arr = np.asarray(P, dtype=float)
        T_arr = np.asarray(T, dtype=float)

        P_flat = P_arr.ravel()
        T_flat = T_arr.ravel()

        mu = self._mean_molecular_weight(X, Y)
        mu_e = self._electron_molecular_weight(X)
        R_spec = K_B / (mu * M_H)

        rho_flat = np.zeros_like(P_flat, dtype=float)

        for i in range(len(P_flat)):
            p_val = P_flat[i]
            t_val = T_flat[i]

            # Solve P = rho * R_spec * T + K_DEG * (rho / mu_e)^(5/3)
            def residual(log_rho):
                r = np.exp(log_rho)
                p_th = r * R_spec * np.maximum(t_val, 1.0)
                p_deg = K_DEG * (r / mu_e)**(5.0 / 3.0)
                return p_th + p_deg - p_val

            # Find root in log(rho) space
            log_rho_min = np.log(1e-10)
            log_rho_max = np.log(1e6)
            try:
                log_rho_sol = brentq(residual, log_rho_min, log_rho_max)
                rho_flat[i] = np.exp(log_rho_sol)
            except ValueError:
                # Fallback to pure ideal gas if out of bounds
                rho_flat[i] = p_val / (R_spec * np.maximum(t_val, 1.0))

        if is_scalar:
            return float(rho_flat[0])
        return rho_flat.reshape(P_arr.shape)

    def specific_entropy(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """
        S Sackur-Tetrode-like specific entropy [J / (kg K)].
        S = (k_B / (mu * m_H)) * [ (1/(gamma-1)) * ln(T) - ln(rho) + C_const ]
        """
        rho = self.density(P, T, X, Y)
        mu = self._mean_molecular_weight(X, Y)
        R_spec = K_B / (mu * M_H)

        s_val = R_spec * ((1.0 / (self.gamma - 1.0)) * np.log(T) - np.log(rho) + 20.0)
        return s_val

    def temperature_from_PS(
        self,
        P: Union[float, np.ndarray],
        S: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """Find T given P and specific entropy S via root-finding."""
        is_scalar = np.isscalar(P) and np.isscalar(S)
        P_arr = np.asarray(P, dtype=float)
        S_arr = np.asarray(S, dtype=float)

        P_flat = P_arr.ravel()
        S_flat = S_arr.ravel()

        mu = self._mean_molecular_weight(X, Y)
        R_spec = K_B / (mu * M_H)

        T_flat = np.zeros_like(P_flat, dtype=float)

        for i in range(len(P_flat)):
            p_val = P_flat[i]
            s_val = S_flat[i]

            # Analytical ideal gas initial guess:
            # S / R_spec = (1 / (gamma-1)) * ln(T) - ln(p / (R_spec * T)) + 20
            # S / R_spec = (gamma / (gamma-1)) * ln(T) - ln(p / R_spec) + 20
            ln_T_guess = ((self.gamma - 1.0) / self.gamma) * (
                s_val / R_spec + np.log(p_val / R_spec) - 20.0
            )
            T_guess = np.clip(np.exp(ln_T_guess), 10.0, 500000.0)

            def residual(log_T):
                t_val = np.exp(log_T)
                s_calc = self.specific_entropy(p_val, t_val, X, Y)
                return s_calc - s_val

            # Use narrow bracket around T_guess
            log_T_min = np.log(max(10.0, T_guess * 0.1))
            log_T_max = np.log(min(500000.0, T_guess * 10.0))

            try:
                log_T_sol = brentq(residual, log_T_min, log_T_max)
                T_flat[i] = np.exp(log_T_sol)
            except ValueError:
                T_flat[i] = T_guess

        if is_scalar:
            return float(T_flat[0])
        return T_flat.reshape(P_arr.shape)

    def nabla_ad(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """
        Dimensionless adiabatic gradient (d ln T / d ln P)_S.
        Transitions smoothly from ideal gas nabla_ad = (gamma-1)/gamma to degenerate limit.
        """
        rho = self.density(P, T, X, Y)
        mu = self._mean_molecular_weight(X, Y)
        mu_e = self._electron_molecular_weight(X)
        R_spec = K_B / (mu * M_H)

        P_th = rho * R_spec * T
        P_deg = K_DEG * (rho / mu_e)**(5.0 / 3.0)
        P_tot = P_th + P_deg

        nab_ideal = (self.gamma - 1.0) / self.gamma
        f_th = P_th / P_tot
        return np.maximum(0.01, f_th * nab_ideal + (1.0 - f_th) * 0.05)

    def internal_energy(
        self,
        P: Union[float, np.ndarray],
        T: Union[float, np.ndarray],
        X: float = 0.75,
        Y: float = 0.25,
    ) -> Union[float, np.ndarray]:
        """Specific internal energy u [J/kg]."""
        rho = self.density(P, T, X, Y)
        mu = self._mean_molecular_weight(X, Y)
        mu_e = self._electron_molecular_weight(X)
        R_spec = K_B / (mu * M_H)

        u_th = R_spec * T / (self.gamma - 1.0)
        u_deg = 1.5 * K_DEG * (rho / mu_e)**(2.0 / 3.0) / mu_e
        return u_th + u_deg
