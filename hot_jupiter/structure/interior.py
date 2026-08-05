"""
1D Hydrostatic Interior Solver for Giant Planets.
Uses smooth inward radial RK4 integration and total radius (R_p) shooting.
"""

from typing import Tuple, Optional
import numpy as np
from scipy.optimize import brentq

from hot_jupiter.constants import G, BAR, M_JUP, M_EARTH, R_JUP
from hot_jupiter.eos.base import BaseEOS
from hot_jupiter.eos.core_eos import BaseCoreEOS, BirchMurnaghanCoreEOS
from hot_jupiter.structure.planet_state import PlanetStructure, InternalProfile


class InteriorSolver:
    """
    Solves 1D hydrostatic equilibrium for a giant planet given (M_p, M_c, S_env).
    Integrates inward from r = R_p to r = 0 with shooting on planet radius R_p.
    """

    def __init__(self, envelope_eos: BaseEOS, core_eos: Optional[BaseCoreEOS] = None):
        self.envelope_eos = envelope_eos
        self.core_eos = core_eos if core_eos is not None else BirchMurnaghanCoreEOS()

    def _integrate_inward(
        self,
        R_p_try: float,
        M_p: float,
        M_c: float,
        S_env: float,
        P_surf: float = 1.0 * BAR,
        X: float = 0.75,
        Y: float = 0.25,
        num_pts: int = 250,
    ) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """
        Integrate hydrostatic equilibrium inward from r = R_p down to r ~ 0.
        """
        T_surf = float(self.envelope_eos.temperature_from_PS(P_surf, S_env, X, Y))
        r_grid = np.linspace(R_p_try, 1e4, num_pts)  # r from R_p down to 10 km

        m_arr = np.zeros(num_pts)
        P_arr = np.zeros(num_pts)
        T_arr = np.zeros(num_pts)
        rho_arr = np.zeros(num_pts)
        nad_arr = np.zeros(num_pts)

        m_arr[0] = M_p
        P_arr[0] = P_surf
        T_arr[0] = T_surf
        _, rho_arr[0], nad_arr[0] = self.envelope_eos.get_state_from_PS(P_surf, S_env, X, Y)

        for i in range(num_pts - 1):
            r = r_grid[i]
            dr = r_grid[i + 1] - r  # negative step
            m = m_arr[i]
            P = P_arr[i]
            T = T_arr[i]

            def derivatives(r_curr, m_curr, P_curr, T_curr):
                P_s = max(P_curr, 10.0)
                T_s = max(T_curr, 10.0)
                r_s = max(r_curr, 1e3)
                m_s = max(m_curr, 1e15)

                if m_s <= M_c:
                    rho = float(self.core_eos.density(P_s))
                    nad = 0.0
                else:
                    _, rho, nad = self.envelope_eos.get_state_from_PS(P_s, S_env, X, Y)

                dm = 4.0 * np.pi * r_s**2 * rho
                dP = - (G * m_s * rho) / (r_s**2)
                dT = nad * (T_s / P_s) * dP if m_s > M_c else 0.0

                return dm, dP, dT, rho, nad

            # RK4 inward step
            dm1, dP1, dT1, rho1, nad1 = derivatives(r, m, P, T)
            rho_arr[i] = rho1
            nad_arr[i] = nad1

            dm2, dP2, dT2, _, _ = derivatives(r + 0.5 * dr, m + 0.5 * dr * dm1, P + 0.5 * dr * dP1, T + 0.5 * dr * dT1)
            dm3, dP3, dT3, _, _ = derivatives(r + 0.5 * dr, m + 0.5 * dr * dm2, P + 0.5 * dr * dP2, T + 0.5 * dr * dT2)
            dm4, dP4, dT4, _, _ = derivatives(r + dr, m + dr * dm3, P + dr * dP3, T + dr * dT3)

            m_arr[i + 1] = m + (dr / 6.0) * (dm1 + 2 * dm2 + 2 * dm3 + dm4)
            P_arr[i + 1] = max(P_surf, P + (dr / 6.0) * (dP1 + 2 * dP2 + 2 * dP3 + dP4))
            T_arr[i + 1] = max(T_surf, T + (dr / 6.0) * (dT1 + 2 * dT2 + 2 * dT3 + dT4))

        if m_arr[-1] <= M_c:
            rho_arr[-1] = float(self.core_eos.density(P_arr[-1]))
            nad_arr[-1] = 0.0
        else:
            _, rho_arr[-1], nad_arr[-1] = self.envelope_eos.get_state_from_PS(P_arr[-1], S_env, X, Y)

        # Reverse arrays so they go from center r ~ 0 to surface r = R_p
        return r_grid[::-1], m_arr[::-1], P_arr[::-1], rho_arr[::-1], T_arr[::-1], nad_arr[::-1]

    def _mass_residual(
        self,
        R_p_try: float,
        M_p: float,
        M_c: float,
        S_env: float,
        P_surf: float,
        X: float,
        Y: float,
        num_pts: int = 150,
    ) -> float:
        """Residual: m(r=0) - 0.0."""
        r_arr, m_arr, _, _, _, _ = self._integrate_inward(
            R_p_try, M_p, M_c, S_env, P_surf, X, Y, num_pts=num_pts
        )
        return float(m_arr[0] - 0.0)

    def solve_structure(
        self,
        M_p: float,
        M_c: float,
        S_env: float,
        P_surf: float = 1.0 * BAR,
        X: float = 0.75,
        Y: float = 0.25,
        num_pts: int = 150,
    ) -> PlanetStructure:
        """
        Solve 1D hydrostatic equilibrium for a planet given (M_p, M_c, S_env).
        Returns PlanetStructure with smooth 1D internal profile extending to r = R_p.
        """
        R_min = 0.4 * R_JUP
        R_max = 2.5 * R_JUP

        f_min = self._mass_residual(R_min, M_p, M_c, S_env, P_surf, X, Y, num_pts=num_pts)
        f_max = self._mass_residual(R_max, M_p, M_c, S_env, P_surf, X, Y, num_pts=num_pts)

        if f_min * f_max > 0:
            R_min = 0.1 * R_JUP
            R_max = 3.5 * R_JUP
            f_min = self._mass_residual(R_min, M_p, M_c, S_env, P_surf, X, Y, num_pts=num_pts)
            f_max = self._mass_residual(R_max, M_p, M_c, S_env, P_surf, X, Y, num_pts=num_pts)

        try:
            R_p_sol = brentq(
                self._mass_residual,
                R_min,
                R_max,
                args=(M_p, M_c, S_env, P_surf, X, Y, num_pts),
                xtol=1e-4 * R_JUP,
            )
        except ValueError:
            R_p_sol = R_min if abs(f_min) < abs(f_max) else R_max

        r_full, m_full, P_full, rho_full, T_full, nad_full = self._integrate_inward(
            R_p_sol, M_p, M_c, S_env, P_surf, X, Y, num_pts=num_pts
        )

        R_p = float(r_full[-1])
        P_c = float(P_full[0])
        T_c = float(T_full[0])

        if M_c > 0:
            idx_cb = np.searchsorted(m_full, M_c)
            idx_cb = min(idx_cb, len(m_full) - 1)
            R_c = float(r_full[idx_cb])
            P_cb = float(P_full[idx_cb])
            T_cb = float(T_full[idx_cb])
        else:
            R_c = 0.0
            P_cb = P_c
            T_cb = T_c

        int_T_dm = float(np.trapezoid(T_full, m_full))

        u_full = np.zeros_like(m_full)
        for i in range(len(m_full)):
            if m_full[i] <= M_c:
                u_full[i] = 1000.0 * T_full[i]
            else:
                u_full[i] = float(self.envelope_eos.internal_energy(P_full[i], T_full[i], X, Y))
        E_int = float(np.trapezoid(u_full, m_full))

        r_safe = np.maximum(r_full, 1e3)
        m_pos = np.maximum(m_full, 0.0)
        dU_dm = - (G * m_pos) / r_safe
        U_grav = float(np.trapezoid(dU_dm, m_full))

        profile = InternalProfile(
            m=m_full,
            r=r_full,
            P=P_full,
            rho=rho_full,
            T=T_full,
            nabla_ad=nad_full,
        )

        return PlanetStructure(
            M_p=M_p,
            M_c=M_c,
            S_env=S_env,
            R_p=R_p,
            R_c=R_c,
            P_c=P_c,
            T_c=T_c,
            T_cb=T_cb,
            P_cb=P_cb,
            int_T_dm=int_T_dm,
            E_int=E_int,
            U_grav=U_grav,
            profile=profile,
        )
