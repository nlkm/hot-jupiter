"""
Thermal Evolution Time Integrator for Giant Planets.
Solves coupled dS/dt, da/dt, de/dt, dOmega_rot/dt over gigayear timescales.
"""

from dataclasses import dataclass
from typing import Optional, Tuple, Dict, Any
import numpy as np
from scipy.integrate import solve_ivp

from thermal_evolution.constants import YEAR, GYR, R_JUP, L_SUN, SIGMA_SB, M_SUN, AU, HOUR
from thermal_evolution.structure import InteriorSolver, PlanetStructure
from thermal_evolution.atmosphere import BaseAtmosphere, GuillotAtmosphere
from thermal_evolution.heating import BaseHeatingSource, ZeroHeating
from thermal_evolution.orbit import OrbitalState, SpinVectorState, TidalOrbitalSpinRates


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


@dataclass
class CoupledEvolutionResult(EvolutionResult):
    """Output container for coupled thermal-orbital-spin evolution trajectories."""
    a: np.ndarray              # Semi-major axis [m]
    a_au: np.ndarray           # Semi-major axis [AU]
    e: np.ndarray              # Orbital eccentricity
    inc: np.ndarray            # Orbital inclination [rad]
    Omega_rot: np.ndarray      # Spin angular frequency [rad/s]
    P_rot_hrs: np.ndarray      # Rotation period [hours]
    obliquity: np.ndarray      # Spin obliquity [rad]
    obliquity_deg: np.ndarray  # Spin obliquity [deg]
    spin_x: np.ndarray         # 3D Cartesian spin vector X
    spin_y: np.ndarray         # 3D Cartesian spin vector Y
    spin_z: np.ndarray         # 3D Cartesian spin vector Z


class ThermalEvolutionIntegrator:
    """
    Evolves entropy S(t), planet structure, orbital elements, and 3D spin vector over time.
    """

    def __init__(
        self,
        interior_solver: InteriorSolver,
        atmosphere_model: BaseAtmosphere,
        heating_source: Optional[BaseHeatingSource] = None,
    ):
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
        struct = self.interior_solver.solve_structure(M_p=M_p, M_c=M_c, S_env=S_env)

        atmos = self.atmosphere_model.evaluate_atmosphere(
            M_p=M_p, R_p=struct.R_p, S_env=S_env, F_inc=F_inc, A_b=A_b
        )

        p_tidal = self.heating_source.evaluate_power(
            t=t, R_p=struct.R_p, M_p=M_p, S_env=S_env, orbit_params=orbit_params
        )

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
        method: str = "RK23",
    ) -> EvolutionResult:
        """
        Integrate 1D thermal evolution over time.
        """
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

    def evolve_coupled(
        self,
        M_p: float,
        M_c: float,
        S_initial: float,
        orbital_state_initial: OrbitalState,
        spin_state_initial: SpinVectorState,
        M_star: float = 1.0 * M_SUN,
        k2_over_Q: float = 1.0e-5,
        t_span: Tuple[float, float] = (1e6 * YEAR, 4.5e9 * YEAR),
        F_inc_base: float = 0.0,
        A_b: float = 0.1,
        num_eval: int = 100,
        method: str = "RK23",
    ) -> CoupledEvolutionResult:
        """
        Integrate coupled thermal, orbital element (a, e, inc), and spin vector (Omega_rot, obliquity) system.

        State Vector y = [S_env, a, e, inc, Omega_rot, obliquity]
        """
        t_start, t_end = t_span
        t_eval = np.geomspace(t_start, t_end, num_eval)

        rates_evaluator = TidalOrbitalSpinRates(k2_over_Q=k2_over_Q)
        a_init = orbital_state_initial.a

        y0 = [
            S_initial,
            orbital_state_initial.a,
            orbital_state_initial.e,
            orbital_state_initial.inc,
            spin_state_initial.Omega_rot,
            spin_state_initial.obliquity,
        ]

        def ode_func(t_val, y_val):
            S_curr = float(max(1e4, y_val[0]))
            a_curr = float(max(0.001 * AU, y_val[1]))
            e_curr = float(np.clip(y_val[2], 0.0, 0.99))
            inc_curr = float(y_val[3])
            Omega_curr = float(max(1e-10, y_val[4]))
            obl_curr = float(np.clip(y_val[5], 0.0, np.pi))

            # Incident flux scales with 1/a^2
            F_inc_curr = F_inc_base * (a_init / a_curr)**2 if F_inc_base > 0 and a_curr > 0 else F_inc_base

            orbit_dict = {
                "a": a_curr,
                "eccentricity": e_curr,
                "M_star": M_star,
                "Omega_rot": Omega_curr,
                "obliquity": obl_curr,
            }

            dS_dt, struct, _, _, _ = self._entropy_derivative(
                t=t_val,
                S_env=S_curr,
                M_p=M_p,
                M_c=M_c,
                F_inc=F_inc_curr,
                A_b=A_b,
                orbit_params=orbit_dict,
            )

            da_dt, de_dt, dOmega_dt, dobl_dt = rates_evaluator.evaluate_rates(
                M_p=M_p,
                R_p=struct.R_p,
                M_star=M_star,
                a=a_curr,
                e=e_curr,
                Omega_rot=Omega_curr,
                obliquity=obl_curr,
            )

            dinc_dt = 0.0  # Inclination rate under planar tide

            return [dS_dt, da_dt, de_dt, dinc_dt, dOmega_dt, dobl_dt]

        sol = solve_ivp(
            ode_func,
            t_span,
            y0,
            t_eval=t_eval,
            method=method,
            rtol=1e-4,
            atol=1e-6,
        )

        t_out = sol.t
        S_out = sol.y[0]
        a_out = sol.y[1]
        e_out = sol.y[2]
        inc_out = sol.y[3]
        Omega_out = sol.y[4]
        obl_out = sol.y[5]

        N = len(t_out)
        R_p_out = np.zeros(N)
        L_int_out = np.zeros(N)
        T_eff_out = np.zeros(N)
        T_int_out = np.zeros(N)
        P_tidal_out = np.zeros(N)

        spin_x = np.zeros(N)
        spin_y = np.zeros(N)
        spin_z = np.zeros(N)

        for i in range(N):
            a_curr = a_out[i]
            F_inc_curr = F_inc_base * (a_init / a_curr)**2 if F_inc_base > 0 and a_curr > 0 else F_inc_base
            orbit_dict = {
                "a": a_curr,
                "eccentricity": e_out[i],
                "M_star": M_star,
                "Omega_rot": Omega_out[i],
                "obliquity": obl_out[i],
            }

            _, struct, L_int, T_int, P_t = self._entropy_derivative(
                t=t_out[i],
                S_env=S_out[i],
                M_p=M_p,
                M_c=M_c,
                F_inc=F_inc_curr,
                A_b=A_b,
                orbit_params=orbit_dict,
            )
            R_p_out[i] = struct.R_p
            L_int_out[i] = L_int
            T_int_out[i] = T_int
            P_tidal_out[i] = P_t

            F_abs = (1.0 - A_b) * F_inc_curr / 4.0
            T_irr = (F_abs / SIGMA_SB)**0.25 if F_abs > 0 else 0.0
            T_eff_out[i] = (T_int**4 + T_irr**4)**0.25 if T_irr > 0 else T_int

            spin_st = SpinVectorState(Omega_rot=Omega_out[i], obliquity=obl_out[i])
            sp_vec = spin_st.spin_vector
            spin_x[i] = sp_vec[0]
            spin_y[i] = sp_vec[1]
            spin_z[i] = sp_vec[2]

        P_rot_hrs = (2.0 * np.pi / np.maximum(Omega_out, 1e-15)) / HOUR

        return CoupledEvolutionResult(
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
            a=a_out,
            a_au=a_out / AU,
            e=e_out,
            inc=inc_out,
            Omega_rot=Omega_out,
            P_rot_hrs=P_rot_hrs,
            obliquity=obl_out,
            obliquity_deg=np.degrees(obl_out),
            spin_x=spin_x,
            spin_y=spin_y,
            spin_z=spin_z,
        )
