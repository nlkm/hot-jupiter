"""
Guillot (2010) Irradiated Radiative-Convective Atmosphere Model.
Ref: Guillot, T. (2010), A&A, 520, A27.
"""

import numpy as np

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
        bracket = (2.0 / 3.0) + (1.0 / (self.gamma * np.sqrt(3.0))
                                ) + (self.gamma / np.sqrt(3.0) - 1.0 /
                                     (self.gamma * np.sqrt(3.0))) * np.exp(
                                         -self.gamma * tau_arr * np.sqrt(3.0))
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
        R_roche: float = 0.0,
    ) -> AtmosphereResult:
        """
        Find intrinsic temperature T_int and net power L_int matching envelope entropy S_env.
        """
        g_iso = (G * M_p) / (R_p**2)
        f_tide = (1.0 - (R_p / R_roche)**3) if (R_roche > 0 and
                                                R_p < R_roche) else 1.0
        g = max(1e-5, g_iso * f_tide)

        # Absorbed irradiation temperature T_irr
        F_abs = (1.0 - A_b) * F_inc / 4.0
        T_irr = (F_abs / SIGMA_SB)**0.25 if F_abs > 0 else 0.0

        # Residual function: match atmosphere temperature T_rad(P_rcb) to T_adiabat(P_rcb, S_env)
        # RCB optical depth tau_rcb ~ 10 - 100
        tau_rcb = 30.0
        P_rcb = (g * tau_rcb) / self.kappa_th

        # Analytical isentrope boundary matching: T_rad(P_rcb) == T_adiabat(P_rcb, S_env)
        T_target = float(
            self.envelope_eos.temperature_from_PS(P_rcb, S_env, 0.75, 0.25))

        bracket = (2.0 / 3.0) + (1.0 / (self.gamma * np.sqrt(3.0))
                                ) + (self.gamma / np.sqrt(3.0) - 1.0 /
                                     (self.gamma * np.sqrt(3.0))) * np.exp(
                                         -self.gamma * tau_rcb * np.sqrt(3.0))
        term_irr = (3.0 / 4.0) * (T_irr**4) * bracket
        coeff_int = (3.0 / 4.0) * (tau_rcb + 2.0 / 3.0)

        T_int4 = max(1.0, (T_target**4 - term_irr) / coeff_int)
        T_int = float(T_int4**0.25)

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
