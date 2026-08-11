"""
Coupled Roche Lobe Overflow (RLOF) Mass Loss and Tidal Evolution Integrator.
Integrates simultaneous planet mass loss dM_p/dt, orbital semi-major axis da/dt,
eccentricity de/dt, and interior thermal entropy dS_env/dt.
"""

from dataclasses import dataclass

import numpy as np
from scipy.integrate import solve_ivp

from hot_jupiter.atmosphere import BaseAtmosphere, GuillotAtmosphere
from hot_jupiter.constants import (
    AU,
    GYR,
    L_SUN,
    M_EARTH,
    M_JUP,
    M_SUN,
    R_JUP,
    R_SUN,
    YEAR,
    G,
)
from hot_jupiter.eos import AnalyticalHHeEOS
from hot_jupiter.heating import BaseHeatingSource, ZeroHeating
from hot_jupiter.mass_loss import RocheLobeMassLoss
from hot_jupiter.orbit.orbital_elements import TidalOrbitalSpinRates
from hot_jupiter.structure import InteriorSolver


@dataclass
class CoupledRLOFEvolutionResult:
    """Container for coupled RLOF mass loss and tidal evolution trajectory."""
    t: np.ndarray  # Time array [years]
    M_p: np.ndarray  # Planet mass [M_Jup]
    R_p: np.ndarray  # Planet radius [R_Jup]
    R_roche: np.ndarray  # Roche lobe radius [R_Jup]
    a: np.ndarray  # Semi-major axis [AU]
    e: np.ndarray  # Eccentricity
    S_env: np.ndarray  # Specific entropy [J/(kg K)]
    L_int: np.ndarray  # Intrinsic luminosity [L_Sun]
    P_tidal: np.ndarray  # Tidal power [W]
    dM_dt: np.ndarray  # Mass loss rate [M_Earth / Gyr]
    filling_factor: np.ndarray  # R_p / R_roche
    outcome: str  # "Disrupted/Engulfed", "Stagnated/Survived", "Cooling"


class CoupledRLOFEvolutionIntegrator:
    """
    Simultaneously integrates giant planet interior cooling, tidal orbital decay,
    and hydrodynamic Roche Lobe Overflow (RLOF) mass loss.
    """

    def __init__(
        self,
        interior_solver: InteriorSolver | None = None,
        atmosphere_model: BaseAtmosphere | None = None,
        heating_source: BaseHeatingSource | None = None,
        mass_loss_model: RocheLobeMassLoss | None = None,
        M_star: float = 1.0 * M_SUN,
        R_star: float = 1.0 * R_SUN,
        k2_p: float = 0.38,
        Q_p: float = 1.0e6,
    ):
        self.eos = AnalyticalHHeEOS(
        ) if interior_solver is None else interior_solver.envelope_eos
        self.interior_solver = InteriorSolver(
            envelope_eos=self.eos
        ) if interior_solver is None else interior_solver
        self.atmosphere_model = GuillotAtmosphere(
            envelope_eos=self.eos
        ) if atmosphere_model is None else atmosphere_model
        self.heating_source = ZeroHeating(
        ) if heating_source is None else heating_source
        self.mass_loss_model = RocheLobeMassLoss(
        ) if mass_loss_model is None else mass_loss_model

        self.M_star = M_star
        self.R_star = R_star
        self.k2_p = k2_p
        self.Q_p = Q_p

    def evolve(
        self,
        M_p_init: float,
        M_c: float,
        a_init: float,
        e_init: float,
        S_initial: float,
        t_span: tuple[float, float] = (1.0e6 * YEAR, 5.0e9 * YEAR),
        num_eval: int = 200,
    ) -> CoupledRLOFEvolutionResult:
        """
        Evolve planet mass, radius, semi-major axis, eccentricity, and thermal entropy.
        """
        t_start, t_end = t_span
        t_eval = np.linspace(t_start, t_end, num_eval)

        # State vector: [S_env, a, e, M_p]
        y0 = [S_initial, a_init, e_init, M_p_init]

        # Auxiliary history storage
        history = {
            "t": [],
            "M_p": [],
            "R_p": [],
            "R_roche": [],
            "a": [],
            "e": [],
            "S_env": [],
            "L_int": [],
            "P_tidal": [],
            "dM_dt": [],
            "filling_factor": [],
        }

        def rhs(t_sec: float, y: list[float]) -> list[float]:
            S_curr = float(max(1.0e4, y[0]))
            a_curr = float(max(0.005 * AU, y[1]))
            e_curr = float(np.clip(y[2], 0.0, 0.95))
            M_p_curr = float(max(M_c + 0.1 * M_EARTH, y[3]))

            # 1. Hydrostatic interior structure
            try:
                struct = self.interior_solver.solve_structure(M_p=M_p_curr,
                                                              M_c=M_c,
                                                              S_env=S_curr)
                R_p = struct.R_p
                int_T_dm = max(1.0e20, struct.int_T_dm)
            except Exception:  # noqa: BLE001
                R_p = 1.0 * R_JUP
                int_T_dm = 1.0e25

            # 2. Roche Lobe Radius & Filling Factor
            r_roche = self.mass_loss_model.roche_lobe_radius(a=a_curr,
                                                             M_p=M_p_curr,
                                                             M_star=self.M_star)
            _ff = R_p / r_roche if r_roche > 0 else 0.0

            # 3. Mass Loss Rate dM/dt & da/dt |_RLOF
            dM_dt_rlof, da_dt_rlof = self.mass_loss_model.evaluate_mass_loss_rate(
                R_p=R_p, a=a_curr, M_p=M_p_curr, M_star=self.M_star)

            # Cap mass loss if planet envelope is almost completely stripped
            if M_p_curr <= M_c + 0.5 * M_EARTH:
                dM_dt_rlof = 0.0
                da_dt_rlof = 0.0

            # 4. Stellar irradiation & Atmosphere model
            F_inc = (L_SUN *
                     (1.0 * R_SUN / self.R_star)**2) / (4.0 * np.pi * a_curr**2)
            atmos = self.atmosphere_model.evaluate_atmosphere(M_p=M_p_curr,
                                                              R_p=R_p,
                                                              S_env=S_curr,
                                                              F_inc=F_inc)
            L_int = atmos.L_int

            # 5. Tidal Orbital Decay & Tidal Power
            rates = TidalOrbitalSpinRates(k2_over_Q=self.k2_p / self.Q_p)
            n_orb = np.sqrt(G * self.M_star / max(a_curr**3, 1.0e10))
            da_dt_tide, de_dt_tide, _, _ = rates.evaluate_rates(
                M_p=M_p_curr,
                R_p=R_p,
                M_star=self.M_star,
                a=a_curr,
                e=e_curr,
                Omega_rot=n_orb,
                obliquity=0.0)
            P_tidal = 10.5 * (self.k2_p / self.Q_p) * (G * self.M_star**2) * (
                R_p**5 / max(a_curr**6, 1.0e15)) * (e_curr**2)

            # Extra heating sources (e.g. Ohmic)
            P_extra = self.heating_source.evaluate_power(t=t_sec,
                                                         R_p=R_p,
                                                         M_p=M_p_curr,
                                                         S_env=S_curr)

            # 6. Thermal Entropy Derivative dS/dt
            # dS/dt = - (L_int - P_tidal - P_extra) / int_T_dm
            net_luminosity = L_int - P_tidal - P_extra
            dS_dt = -net_luminosity / int_T_dm

            # Total semi-major axis derivative: da/dt = da/dt |_tide + da/dt |_RLOF
            da_dt_total = da_dt_tide + da_dt_rlof

            return [dS_dt, da_dt_total, de_dt_tide, dM_dt_rlof]

        # Execute IVP integration
        sol = solve_ivp(
            fun=rhs,
            t_span=(t_start, t_end),
            y0=y0,
            t_eval=t_eval,
            method="RK45",
            rtol=1.0e-4,
            atol=1.0e-6,
        )

        # Reconstruct detailed physical outputs along solution trajectory
        for idx in range(len(sol.t)):
            t_sec = sol.t[idx]
            S_c = float(sol.y[0, idx])
            a_c = float(sol.y[1, idx])
            e_c = float(sol.y[2, idx])
            M_c_val = float(sol.y[3, idx])

            try:
                st = self.interior_solver.solve_structure(M_p=M_c_val,
                                                          M_c=M_c,
                                                          S_env=S_c)
                R_p_val = st.R_p
            except Exception:  # noqa: BLE001
                R_p_val = 1.0 * R_JUP

            r_roche_val = self.mass_loss_model.roche_lobe_radius(
                a_c, M_c_val, self.M_star)
            ff_val = R_p_val / r_roche_val if r_roche_val > 0 else 0.0
            dM_dt_val, _ = self.mass_loss_model.evaluate_mass_loss_rate(
                R_p_val, a_c, M_c_val, self.M_star)

            F_inc_val = L_SUN / (4.0 * np.pi * a_c**2)
            atmos_val = self.atmosphere_model.evaluate_atmosphere(
                M_p=M_c_val, R_p=R_p_val, S_env=S_c, F_inc=F_inc_val)
            rates_val = TidalOrbitalSpinRates(k2_over_Q=self.k2_p / self.Q_p)
            n_c = np.sqrt(G * self.M_star / max(a_c**3, 1.0e10))
            _, _, _, _ = rates_val.evaluate_rates(M_p=M_c_val,
                                                  R_p=R_p_val,
                                                  M_star=self.M_star,
                                                  a=a_c,
                                                  e=e_c,
                                                  Omega_rot=n_c,
                                                  obliquity=0.0)
            P_tid_val = 10.5 * (self.k2_p / self.Q_p) * (G * self.M_star**2) * (
                R_p_val**5 / max(a_c**6, 1.0e15)) * (e_c**2)

            history["t"].append(t_sec / YEAR)
            history["M_p"].append(M_c_val / M_JUP)
            history["R_p"].append(R_p_val / R_JUP)
            history["R_roche"].append(r_roche_val / R_JUP)
            history["a"].append(a_c / AU)
            history["e"].append(e_c)
            history["S_env"].append(S_c)
            history["L_int"].append(atmos_val.L_int / L_SUN)
            history["P_tidal"].append(P_tid_val)
            history["dM_dt"].append(dM_dt_val * (1.0e9 * GYR / M_EARTH))
            history["filling_factor"].append(ff_val)

        # Determine overall trajectory outcome classification
        final_a = history["a"][-1]
        _final_ff = history["filling_factor"][-1]
        max_ff = max(history["filling_factor"])

        if final_a <= 0.008 or (max_ff > 1.2 and final_a < 0.012):
            outcome = "Disrupted/Engulfed"
        elif max_ff >= 0.95:
            outcome = "Stagnated/Survived"
        else:
            outcome = "Cooling"

        return CoupledRLOFEvolutionResult(
            t=np.array(history["t"]),
            M_p=np.array(history["M_p"]),
            R_p=np.array(history["R_p"]),
            R_roche=np.array(history["R_roche"]),
            a=np.array(history["a"]),
            e=np.array(history["e"]),
            S_env=np.array(history["S_env"]),
            L_int=np.array(history["L_int"]),
            P_tidal=np.array(history["P_tidal"]),
            dM_dt=np.array(history["dM_dt"]),
            filling_factor=np.array(history["filling_factor"]),
            outcome=outcome,
        )
