"""
1D Hydrostatic Interior Solver for Giant Planets.
Integrates outward from planet center m=0 to surface m=M_p using central pressure P_c shooting.
"""

import numpy as np
from scipy.integrate import solve_ivp
from scipy.optimize import brentq
from typing import Tuple, Optional

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
        num_pts: int = 500,
    ) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """
        Integrate hydrostatic equilibrium outward from m ~ 0 to m = M_p given central pressure P_c = 10^(log10_Pc).
        """
        P_c = 10.0**log10_Pc

        # Core density at central pressure
        rho_c = float(self.core_eos.density(P_c))

        # Small central core radius epsilon to avoid 1/r singularity
        m_start = max(1e-8 * M_p, 1e-12)
        r_start = (3.0 * m_start / (4.0 * np.pi * rho_c))**(1.0 / 3.0)
        P_start = max(100.0, P_c - (2.0 / 3.0) * np.pi * G * (rho_c**2) * (r_start**2))

        if M_c > 0 and m_start < M_c:
            # Stage 1: Integrate Core from m_start to M_c
            def core_ode(m, y):
                r, P = y
                if r <= 0 or P <= 10.0:
                    return [0.0, 0.0]
                rho = float(self.core_eos.density(P))
                dr_dm = 1.0 / (4.0 * np.pi * r**2 * rho)
                dP_dm = - (G * m) / (4.0 * np.pi * r**4)
                return [dr_dm, dP_dm]

            m_core_eval = np.linspace(m_start, M_c, max(10, int(num_pts * (M_c / M_p)))) if num_pts > 0 else None
            sol_core = solve_ivp(
                core_ode,
                (m_start, M_c),
                [r_start, P_start],
                t_eval=m_core_eval,
                method="RK45",
                rtol=1e-4,
                atol=1e-5,
            )
            
            m_core = sol_core.t
            r_core = sol_core.y[0]
            P_core = np.maximum(100.0, sol_core.y[1])
            
            r_cb = r_core[-1]
            P_cb = float(P_core[-1])
        else:
            m_core = np.array([m_start])
            r_core = np.array([r_start])
            P_core = np.array([P_start])
            r_cb = r_start
            P_cb = P_start

        # If pressure drops too low inside core, core integration failed
        if P_cb <= 200.0:
            m_dummy = np.array([m_start, M_p])
            return m_dummy, np.array([r_start, r_start]), np.array([10.0, 10.0]), np.array([rho_c, rho_c]), np.array([10.0, 10.0]), np.array([0.0, 0.0])

        # Stage 2: Integrate Envelope from max(m_start, M_c) to M_p
        m_env_start = max(m_start, M_c)
        T_env_start = float(self.envelope_eos.temperature_from_PS(P_cb, S_env, X, Y))

        def env_ode(m, y):
            r, P, T = y
            if r <= 0 or P <= 10.0 or T <= 10.0:
                return [0.0, 0.0, 0.0]
            P_safe = max(P, 100.0)
            T_safe = max(T, 10.0)
            _, rho, nad = self.envelope_eos.get_state_from_PS(P_safe, S_env, X, Y)
            dr_dm = 1.0 / (4.0 * np.pi * r**2 * rho)
            dP_dm = - (G * m) / (4.0 * np.pi * r**4)
            dT_dm = - (G * m * T_safe * nad) / (4.0 * np.pi * r**4 * P_safe)
            return [dr_dm, dP_dm, dT_dm]

        m_env_eval = np.linspace(m_env_start, M_p, max(10, num_pts - len(m_core))) if num_pts > 0 else None
        sol_env = solve_ivp(
            env_ode,
            (m_env_start, M_p),
            [r_cb, P_cb, T_env_start],
            t_eval=m_env_eval,
            method="RK45",
            rtol=1e-4,
            atol=1e-5,
        )

        m_env = sol_env.t
        r_env = sol_env.y[0]
        P_env = np.maximum(10.0, sol_env.y[1])
        T_env = np.maximum(10.0, sol_env.y[2])

        if M_c > 0 and len(m_core) > 1:
            m_full = np.concatenate([m_core, m_env[1:]])
            r_full = np.concatenate([r_core, r_env[1:]])
            P_full = np.concatenate([P_core, P_env[1:]])
            T_full = np.concatenate([np.full_like(r_core, T_env_start), T_env[1:]])
        else:
            m_full, r_full, P_full, T_full = m_env, r_env, P_env, T_env

        # Compute rho and nabla_ad profile
        rho_full = np.zeros_like(P_full)
        nad_full = np.zeros_like(P_full)

        for i in range(len(P_full)):
            if m_full[i] <= M_c:
                rho_full[i] = float(self.core_eos.density(P_full[i]))
                nad_full[i] = 0.0
            else:
                _, rho_full[i], nad_full[i] = self.envelope_eos.get_state_from_PS(P_full[i], S_env, X, Y)

        return m_full, r_full, P_full, rho_full, T_full, nad_full

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
        m_arr, r_arr, P_arr, _, _, _ = self._integrate_outward(
            log10_Pc, M_p, M_c, S_env, P_surf, X, Y, num_pts=0
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
        num_pts: int = 500,
    ) -> PlanetStructure:
        """
        Solve 1D hydrostatic equilibrium for a planet.
        """
        # Central pressure log range (10^11.5 Pa = 3 Mbar to 10^13.5 Pa = 300 Mbar)
        logP_min = 11.5
        logP_max = 13.5

        # Verify or bracket root
        f_min = self._surface_pressure_residual(logP_min, M_p, M_c, S_env, P_surf, X, Y)
        f_max = self._surface_pressure_residual(logP_max, M_p, M_c, S_env, P_surf, X, Y)

        if f_min * f_max > 0:
            if f_min > 0:
                logP_min = 10.5
            if f_max < 0:
                logP_max = 14.5

        try:
            log10_Pc_sol = brentq(
                self._surface_pressure_residual,
                logP_min,
                logP_max,
                args=(M_p, M_c, S_env, P_surf, X, Y),
                xtol=1e-4,
            )
        except ValueError:
            log10_Pc_sol = logP_min if abs(f_min) < abs(f_max) else logP_max

        # Integrate full high-resolution profile
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

        # Integrals
        int_T_dm = float(np.trapezoid(T_full, m_full))

        # Internal thermal energy E_int = \int u dm
        u_full = np.zeros_like(m_full)
        for i in range(len(m_full)):
            if m_full[i] <= M_c:
                u_full[i] = 1000.0 * T_full[i]
            else:
                u_full[i] = float(self.envelope_eos.internal_energy(P_full[i], T_full[i], X, Y))
        E_int = float(np.trapezoid(u_full, m_full))

        # Gravitational potential energy U = - \int (G m / r) dm
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
