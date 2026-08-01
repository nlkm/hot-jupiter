"""
Thermal Evolution Time Integrator for Giant Planets.
Solves dS/dt = - (L_int - P_tidal) / \int_0^{M_p} T(m) dm over gigayear timescales.
"""

from dataclasses import dataclass
from typing import Optional, Tuple, Dict, Any
import numpy as np
from scipy.integrate import solve_ivp

from thermal_evolution.constants import YEAR, GYR, R_JUP, L_SUN, SIGMA_SB
from thermal_evolution.structure import InteriorSolver, PlanetStructure
from thermal_evolution.atmosphere import BaseAtmosphere, GuillotAtmosphere
from thermal_evolution.heating import BaseHeatingSource, ZeroHeating


@dataclass
class EvolutionResult:
    """Output container for a planet thermal cooling trajectory."""
    t: np.ndarray          # Time array [s]
    t_gyr: np.ndarray      # Time array [Gyr]
    S: np.ndarray          # Specific entropy array [J / (kg K)]
    R_p: np.ndarray        # Total planet radius [m]
    R_p_jup: np.ndarray    # Planet radius in Jupiter radii [R_Jup]
    L_int: np.ndarray      # Intrinsic luminosity [W]
    L_int_sun: np.ndarray  # Intrinsic luminosity in solar units [L_sun]
    T_eff: np.ndarray      # Total effective temperature [K]
    T_int: np.ndarray      # Intrinsic effective temperature [K]
    P_tidal: np.ndarray    # Injected tidal heating power [W]


class ThermalEvolutionIntegrator:
    """
    Evolves entropy S(t) and planet structure over time.
    """

    def __init__(
        self,
        interior_solver: InteriorSolver,
        atmosphere_model: BaseAtmosphere,
        heating_source: Optional[BaseHeatingSource] = None,
    ):
        """
        Parameters
        ----------
        interior_solver : InteriorSolver
            Hydrostatic solver for structure profiles.
        atmosphere_model : BaseAtmosphere
            Atmospheric boundary solver for L_int and T_eff.
        heating_source : BaseHeatingSource, optional
            Internal energy injection model (tidal, radiogenic).
        """
        self.interior_solver = interior_solver
        self.atmosphere_model = atmosphere_model
        self.heating_source = heating_source if heating_source is not None else ZeroHeating()

    def _entropy_derivative(
        self,
        t: float,
        S_env: float,
        M_p: float,
        M_c: float,
        F_inc: float,
        A_b: float,
        orbit_params: Optional[dict],
    ) -> Tuple[float, PlanetStructure, float, float, float]:
        """Compute dS/dt and intermediate structural quantities."""
        # 1. Solve 1D hydrostatic interior structure
        struct = self.interior_solver.solve_structure(M_p=M_p, M_c=M_c, S_env=S_env)

        # 2. Evaluate atmosphere boundary for L_int and T_int
        atmos = self.atmosphere_model.evaluate_atmosphere(
            M_p=M_p, R_p=struct.R_p, S_env=S_env, F_inc=F_inc, A_b=A_b
        )

        # 3. Evaluate internal heating power P_tidal
        p_tidal = self.heating_source.evaluate_power(
            t=t, R_p=struct.R_p, M_p=M_p, S_env=S_env, orbit_params=orbit_params
        )

        # 4. Energy conservation: dS/dt = - (L_int - P_tidal) / \int T dm
        int_T_dm = max(struct.int_T_dm, 1e-10)
        dS_dt = - (atmos.L_int - p_tidal) / int_T_dm

        return dS_dt, struct, atmos.L_int, atmos.T_int, p_tidal

    def evolve(
        self,
        M_p: float,
        M_c: float,
        S_initial: float,
        t_span: Tuple[float, float] = (1e6 * YEAR, 4.5e9 * YEAR),
        F_inc: float = 0.0,
        A_b: float = 0.1,
        orbit_params: Optional[dict] = None,
        num_eval: int = 100,
        method: str = "RK45",
    ) -> EvolutionResult:
        """
        Integrate planet thermal evolution over time.

        Parameters
        ----------
        M_p : float
            Planet mass [kg].
        M_c : float
            Core mass [kg].
        S_initial : float
            Initial specific entropy at t_span[0] [J/(kg K)].
        t_span : tuple of (t_start, t_end) [s]. Default 1 Myr to 4.5 Gyr.
        F_inc : float
            Incident stellar flux [W/m^2].
        A_b : float
            Bond albedo (default 0.1).
        orbit_params : dict, optional
            Orbital parameters passed to heating source.
        num_eval : int
            Number of output time steps.
        method : str
            ODE integration method ("RK45", "Radau").

        Returns
        -------
        EvolutionResult dataclass.
        """
        # Define log-spaced output times
        t_start, t_end = t_span
        t_eval = np.geomspace(t_start, t_end, num_eval)

        def ode_func(t_val, y_val):
            S_curr = float(y_val[0])
            dS_dt, _, _, _, _ = self._entropy_derivative(
                t=t_val,
                S_env=S_curr,
                M_p=M_p,
                M_c=M_c,
                F_inc=F_inc,
                A_b=A_b,
                orbit_params=orbit_params,
            )
            return [dS_dt]

        # Run ODE solver
        sol = solve_ivp(
            ode_func,
            t_span,
            [S_initial],
            t_eval=t_eval,
            method=method,
            rtol=1e-4,
            atol=1e-6,
        )

        t_out = sol.t
        S_out = sol.y[0]

        # Post-process full trajectory details
        R_p_out = np.zeros_like(t_out)
        L_int_out = np.zeros_like(t_out)
        T_eff_out = np.zeros_like(t_out)
        T_int_out = np.zeros_like(t_out)
        P_tidal_out = np.zeros_like(t_out)

        for i in range(len(t_out)):
            _, struct, L_int, T_int, P_t = self._entropy_derivative(
                t=t_out[i],
                S_env=S_out[i],
                M_p=M_p,
                M_c=M_c,
                F_inc=F_inc,
                A_b=A_b,
                orbit_params=orbit_params,
            )
            R_p_out[i] = struct.R_p
            L_int_out[i] = L_int
            T_int_out[i] = T_int
            P_tidal_out[i] = P_t
            
            # Irradiation temperature
            F_abs = (1.0 - A_b) * F_inc / 4.0
            T_irr = (F_abs / SIGMA_SB)**0.25 if F_abs > 0 else 0.0
            T_eff_out[i] = (T_int**4 + (T_irr / np.sqrt(2.0))**4)**0.25 if T_irr > 0 else T_int

        return EvolutionResult(
            t=t_out,
            t_gyr=t_out / GYR,
            S=S_out,
            R_p=R_p_out,
            R_p_jup=R_p_out / R_JUP,
            L_int=L_int_out,
            L_int_sun=L_int_out / L_SUN,
            T_eff=T_eff_out,
            T_int=T_int_out,
            P_tidal=P_tidal_out,
        )
