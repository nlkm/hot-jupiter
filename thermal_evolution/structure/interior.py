"""
1D Hydrostatic Interior Solver for Giant Planets.
Uses ultra-fast RK4 integration and central pressure shooting.
"""

from typing import Tuple, Optional
import numpy as np
from scipy.optimize import brentq

from thermal_evolution.constants import G, BAR, M_JUP, M_EARTH, R_JUP, GPa, MBAR
from thermal_evolution.eos.base import BaseEOS
from thermal_evolution.eos.core_eos import BaseCoreEOS, BirchMurnaghanCoreEOS
from thermal_evolution.structure.planet_state import PlanetStructure, InternalProfile


class InteriorSolver:
    """
    Solves 1D hydrostatic equilibrium for a giant planet given (M_p, M_c, S_env).
    """

    def __init__(self, envelope_eos: BaseEOS, core_eos: Optional[BaseCoreEOS] = None):
        self.envelope_eos = envelope_eos
        self.core_eos = core_eos if core_eos is not None else BirchMurnaghanCoreEOS()

    def _integrate_outward(
        self,
        log10_Pc: float,
        M_p: float,
        M_c: float,
        S_env: float,
        P_surf: float = 1.0 * BAR,
        X: float = 0.75,
        Y: float = 0.25,
        num_pts: int = 150,
    ) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """
        Integrate hydrostatic equilibrium outward from m ~ 0 to m = M_p using vectorized RK4.
        """
        P_c = 10.0**log10_Pc
        rho_c = float(self.core_eos.density(P_c))

        m_start = max(1e-6 * M_p, 1e-12)
        r_start = (3.0 * m_start / (4.0 * np.pi * rho_c))**(1.0 / 3.0)
        P_start = max(100.0, P_c - (2.0 / 3.0) * np.pi * G * (rho_c**2) * (r_start**2))

        # Mass grid (dense near core, extending to M_p)
        if M_c > 0 and m_start < M_c:
            m_core_grid = np.linspace(m_start, M_c, max(20, int(num_pts * (M_c / M_p))))
            m_env_grid = np.linspace(M_c, M_p, num_pts)
            m_grid = np.unique(np.concatenate([m_core_grid, m_env_grid]))
        else:
            m_grid = np.linspace(m_start, M_p, num_pts)

        N = len(m_grid)
        r_arr = np.zeros(N)
        P_arr = np.zeros(N)
        T_arr = np.zeros(N)
        rho_arr = np.zeros(N)
        nad_arr = np.zeros(N)

        r_arr[0] = r_start
        P_arr[0] = P_start
        T_env_start = float(self.envelope_eos.temperature_from_PS(P_start, S_env, X, Y))
        T_arr[0] = T_env_start

        # Vectorized RK4 Stepper
        for i in range(N - 1):
            m = m_grid[i]
            dm = m_grid[i + 1] - m
            r = r_arr[i]
            P = P_arr[i]
            T = T_arr[i]

            if r <= 0 or P <= 10.0 or T <= 10.0:
                # Flow floor
                r_arr[i + 1:] = r
                P_arr[i + 1:] = 10.0
                T_arr[i + 1:] = 10.0
                break

            def derivatives(m_curr, r_curr, P_curr, T_curr):
                P_s = max(P_curr, 10.0)
                T_s = max(T_curr, 10.0)
                r_s = max(r_curr, 1e2)

                if m_curr <= M_c:
                    rho = float(self.core_eos.density(P_s))
                    nad = 0.0
                else:
                    _, rho, nad = self.envelope_eos.get_state_from_PS(P_s, S_env, X, Y)

                dr = 1.0 / (4.0 * np.pi * r_s**2 * max(rho, 1e-4))
                dP = - (G * m_curr) / (4.0 * np.pi * r_s**4)
                dT = - (G * m_curr * T_s * nad) / (4.0 * np.pi * r_s**4 * P_s) if m_curr > M_c else 0.0
                return dr, dP, dT, rho, nad

            # RK4 step
            dr1, dP1, dT1, rho1, nad1 = derivatives(m, r, P, T)
            rho_arr[i] = rho1
            nad_arr[i] = nad1

            dr2, dP2, dT2, _, _ = derivatives(m + 0.5 * dm, r + 0.5 * dm * dr1, P + 0.5 * dm * dP1, T + 0.5 * dm * dT1)
            dr3, dP3, dT3, _, _ = derivatives(m + 0.5 * dm, r + 0.5 * dm * dr2, P + 0.5 * dm * dP2, T + 0.5 * dm * dT2)
            dr4, dP4, dT4, _, _ = derivatives(m + dm, r + dm * dr3, P + dm * dP3, T + dm * dT3)

            r_arr[i + 1] = r + (dm / 6.0) * (dr1 + 2 * dr2 + 2 * dr3 + dr4)
            P_arr[i + 1] = max(10.0, P + (dm / 6.0) * (dP1 + 2 * dP2 + 2 * dP3 + dP4))
            T_arr[i + 1] = max(10.0, T + (dm / 6.0) * (dT1 + 2 * dT2 + 2 * dT3 + dT4))

        # Fill end point
        if m_grid[-1] <= M_c:
            rho_arr[-1] = float(self.core_eos.density(P_arr[-1]))
            nad_arr[-1] = 0.0
        else:
            _, rho_arr[-1], nad_arr[-1] = self.envelope_eos.get_state_from_PS(P_arr[-1], S_env, X, Y)

        return m_grid, r_arr, P_arr, rho_arr, T_arr, nad_arr

    def _surface_pressure_residual(
        self,
        log10_Pc: float,
        M_p: float,
        M_c: float,
        S_env: float,
        P_surf: float,
        X: float,
        Y: float,
    ) -> float:
        """Residual function: log10(P_surface(P_c)) - log10(P_target)."""
        _, _, P_arr, _, _, _ = self._integrate_outward(
            log10_Pc, M_p, M_c, S_env, P_surf, X, Y, num_pts=50
        )
        P_surface_found = max(P_arr[-1], 10.0)
        return np.log10(P_surface_found) - np.log10(P_surf)

    def solve_structure(
        self,
        M_p: float,
        M_c: float,
        S_env: float,
        P_surf: float = 1.0 * BAR,
        X: float = 0.75,
        Y: float = 0.25,
        num_pts: int = 40,
    ) -> PlanetStructure:
        """
        Solve 1D hydrostatic equilibrium for a planet.
        """
        logP_min = 11.0
        logP_max = 13.5

        f_min = self._surface_pressure_residual(logP_min, M_p, M_c, S_env, P_surf, X, Y)
        f_max = self._surface_pressure_residual(logP_max, M_p, M_c, S_env, P_surf, X, Y)

        if f_min * f_max > 0:
            if f_min > 0:
                logP_min = 10.0
            if f_max < 0:
                logP_max = 14.5

        try:
            log10_Pc_sol = brentq(
                self._surface_pressure_residual,
                logP_min,
                logP_max,
                args=(M_p, M_c, S_env, P_surf, X, Y),
                xtol=1e-3,
            )
        except ValueError:
            log10_Pc_sol = logP_min if abs(f_min) < abs(f_max) else logP_max

        m_full, r_full, P_full, rho_full, T_full, nad_full = self._integrate_outward(
            log10_Pc_sol, M_p, M_c, S_env, P_surf, X, Y, num_pts=num_pts
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
        dU_dm = - (G * m_full) / r_safe
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
