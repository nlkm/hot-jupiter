"""
Guillot (2010) Irradiated Radiative-Convective Atmosphere Model.
Ref: Guillot, T. (2010), A&A, 520, A27.
"""

import numpy as np
from scipy.optimize import brentq

from hot_jupiter.atmosphere.base import AtmosphereResult, BaseAtmosphere
from hot_jupiter.constants import SIGMA_SB, G
from hot_jupiter.eos.base import BaseEOS


class GuillotAtmosphere(BaseAtmosphere):
    """
    Guillot (2010) semi-analytical irradiated atmosphere boundary model.
    """

    def __init__(
            self,
            envelope_eos: BaseEOS,
            kappa_th: float = 1e-2,  # Thermal opacity [m^2/kg] (0.1 cm^2/g)
            gamma:
        float = 0.1,  # Ratio of visible to thermal opacity kappa_v / kappa_th
    ):
        self.envelope_eos = envelope_eos
        self.kappa_th = kappa_th
        self.gamma = gamma

    def temperature_profile(
        self,
        tau: float | np.ndarray,
        T_int: float,
        T_irr: float,
    ) -> float | np.ndarray:
        """
        Guillot (2010) T(tau) temperature profile equation (Eq. 29).
        """
        tau_arr = np.asarray(tau, dtype=float)

        # Term 1: Intrinsic flux term
        term_int = (3.0 / 4.0) * (T_int**4) * (tau_arr + 2.0 / 3.0)

        # Term 2: Irradiated stellar flux term
        g_tau = self.gamma * tau_arr
        bracket = (2.0 / 3.0) + (2.0 / (3.0 * self.gamma)) * (
            1.0 + (g_tau / 2.0 - 1.0) * np.exp(-g_tau))
        term_irr = (3.0 / 4.0) * (T_irr**4) * bracket

        T4 = np.maximum(1.0, term_int + term_irr)
        T_profile = T4**0.25

        if np.isscalar(tau):
            return float(T_profile)
        return T_profile

    def evaluate_atmosphere(
        self,
        M_p: float,
        R_p: float,
        S_env: float,
        F_inc: float = 0.0,
        A_b: float = 0.1,
    ) -> AtmosphereResult:
        """
        Find intrinsic temperature T_int and net power L_int matching envelope entropy S_env.
        """
        g = (G * M_p) / (R_p**2)

        # Absorbed irradiation temperature T_irr
        F_abs = (1.0 - A_b) * F_inc / 4.0
        T_irr = (F_abs / SIGMA_SB)**0.25 if F_abs > 0 else 0.0

        # Residual function: match atmosphere temperature T_rad(P_rcb) to T_adiabat(P_rcb, S_env)
        # RCB optical depth tau_rcb ~ 10 - 100
        tau_rcb = 30.0
        P_rcb = (g * tau_rcb) / self.kappa_th

        def residual(T_int_guess):
            T_rad_rcb = self.temperature_profile(tau_rcb, T_int_guess, T_irr)
            # Find entropy of adiabat at (P_rcb, T_rad_rcb)
            S_rad_rcb = self.envelope_eos.specific_entropy(P_rcb, T_rad_rcb)
            return S_rad_rcb - S_env

        # Solve for T_int between 5 K and 3000 K
        try:
            T_int = brentq(residual, 5.0, 3000.0)
        except ValueError:
            # Minimize absolute residual if boundary bracket is violated
            from scipy.optimize import minimize_scalar
            res_opt = minimize_scalar(lambda t: abs(residual(t)),
                                      bounds=(5.0, 3000.0),
                                      method="bounded")
            T_int = float(res_opt.x)

        T_eff = (T_int**4 + T_irr**4)**0.25 if T_irr > 0 else T_int
        L_int = 4.0 * np.pi * (R_p**2) * SIGMA_SB * (T_int**4)
        T_rcb = self.temperature_profile(tau_rcb, T_int, T_irr)

        return AtmosphereResult(
            T_int=T_int,
            T_eff=T_eff,
            L_int=L_int,
            P_rcb=P_rcb,
            T_rcb=T_rcb,
        )
