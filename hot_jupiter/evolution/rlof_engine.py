"""
Unified Hydrostatic & Orbital Evolution Engine for Coupled RLOF Mass Loss and Tidal Decay.
"""

from dataclasses import dataclass
from enum import Enum

import numpy as np

from hot_jupiter.constants import AU, M_EARTH, M_JUP, M_SUN, R_EARTH, R_JUP, R_SUN, G


class EvolutionOutcome(Enum):
    """Possible physical evolution outcomes for ultra-short-period gas giants."""
    DISRUPTED = "Tidal Runaway Disruption / Engulfment"
    STAGNATED = "Self-Limiting Mass-Loss Stagnation"
    COOLING = "Non-Overflow Thermal Cooling"
    ENGULFED = "Stellar Engulfment"


@dataclass
class TrajectoryResult:
    """Dataclass holding coupled evolutionary trajectory results."""
    t_arr: np.ndarray
    a_arr: np.ndarray
    e_arr: np.ndarray
    m_p_arr: np.ndarray
    m_env_arr: np.ndarray
    m_core_arr: np.ndarray
    r_p_arr: np.ndarray
    r_roche_arr: np.ndarray
    filling_factor_arr: np.ndarray
    outcome: EvolutionOutcome
    final_m_remnant_earth: float
    z_bulk: float


class CoupledRLOFIntegrator:
    """Coupled RLOF Mass Loss and Tidal Orbital Decay Integrator.

    Integrates planetary cooling, Roche lobe overflow mass loss, tidal orbital
    decay, and eccentricity circularization.
    """

    def __init__(self,
                 m_p_init_jup: float = 1.0,
                 a_init_au: float = 0.02,
                 m_core_earth: float = 10.0,
                 m_star_sun: float = 1.0,
                 e_init: float = 0.15,
                 q_star_prime: float = 1.5e5,
                 k2_star: float = 0.03,
                 q_planet_prime: float = 1.0e5,
                 k2_planet: float = 0.38,
                 eta_rlof: float = 4.0,
                 beta_angular_momentum: float = 0.5):
        self.m_p_init_jup = m_p_init_jup
        self.a_init_au = a_init_au
        self.m_core_earth = m_core_earth
        self.m_star_sun = m_star_sun
        self.e_init = e_init
        self.q_star_prime = q_star_prime
        self.k2_star = k2_star
        self.q_planet_prime = q_planet_prime
        self.k2_planet = k2_planet
        self.eta_rlof = eta_rlof
        self.beta_angular_momentum = beta_angular_momentum

    def compute_roche_lobe_radius(self, a_m: float, m_total_kg: float) -> float:
        """Eggleton (1983) formula for Roche lobe radius R_Roche."""
        m_star_kg = self.m_star_sun * M_SUN
        q = m_total_kg / m_star_kg
        r_roche_ratio = 0.49 * (q**(2 / 3)) / (0.6 * (q**(2 / 3)) +
                                               np.log(1.0 + q**(1 / 3)))
        return a_m * r_roche_ratio

    def integrate(self,
                  t_max_yr: float = 5.0e9,
                  num_pts: int = 400) -> TrajectoryResult:
        """Integrates coupled evolutionary equations from 1 Myr to t_max_yr."""
        try:
            from hot_jupiter.bindings import rlof_integrate_cpp
            data, c_res = rlof_integrate_cpp(self.m_p_init_jup, self.a_init_au,
                                             self.m_core_earth, self.m_star_sun,
                                             t_max_yr, num_pts)
            t_arr = np.array(data["t"])
            a_arr = np.array(data["a"])
            e_arr = np.array(data["e"])
            m_p_arr = np.array(data["M_p"])
            r_p_arr = np.array(data["R_p"])
            ff_arr = np.array(data["filling_factor"])
            m_env_arr = np.maximum(
                0.0, m_p_arr - (self.m_core_earth * M_EARTH / M_JUP))
            r_roche_arr = np.where(ff_arr > 0, r_p_arr / ff_arr * (R_JUP / AU),
                                   0.0)

            outcome_map = {
                0: EvolutionOutcome.DISRUPTED,
                1: EvolutionOutcome.STAGNATED,
                2: EvolutionOutcome.COOLING,
                3: EvolutionOutcome.ENGULFED
            }

            return TrajectoryResult(
                t_arr=t_arr,
                a_arr=a_arr,
                e_arr=e_arr,
                m_p_arr=m_p_arr,
                m_env_arr=m_env_arr,
                m_core_arr=np.full_like(t_arr, self.m_core_earth),
                r_p_arr=r_p_arr,
                r_roche_arr=r_roche_arr,
                filling_factor_arr=ff_arr,
                outcome=outcome_map.get(c_res.outcome,
                                        EvolutionOutcome.COOLING),
                final_m_remnant_earth=c_res.final_m_remnant_earth,
                z_bulk=c_res.z_bulk)
        except (ImportError, RuntimeError):
            pass

        m_core_kg = self.m_core_earth * M_EARTH
        m_env_init_kg = max(0.0, (self.m_p_init_jup * M_JUP) - m_core_kg)
        m_env_kg = m_env_init_kg
        m_total_kg = m_core_kg + m_env_kg

        a_curr = self.a_init_au * AU
        e_curr = self.e_init

        t_arr = np.geomspace(1.0e6, t_max_yr, num_pts)
        a_arr = np.zeros(num_pts)
        e_arr = np.zeros(num_pts)
        m_p_arr = np.zeros(num_pts)
        m_env_arr = np.zeros(num_pts)
        r_p_arr = np.zeros(num_pts)
        r_roche_arr = np.zeros(num_pts)
        ff_arr = np.zeros(num_pts)

        disrupted = False
        engulfed = False
        max_ff = 0.0

        valid_pts = 0
        for idx in range(num_pts):
            if idx == 0:
                dt_yr = t_arr[0]
            else:
                dt_yr = t_arr[idx] - t_arr[idx - 1]
            dt_sec = dt_yr * 3.154e7
            t_gyr = t_arr[idx] / 1.0e9

            r_core = 1.0 * R_EARTH * ((self.m_core_earth / 1.0)**0.27)

            if m_env_kg > 0.1 * M_EARTH:
                r_env = 1.25 * R_JUP * ((
                    (m_env_kg / M_JUP))**0.15) * np.exp(-0.08 * t_gyr)
                r_p_curr = max(r_core, r_env)
            else:
                r_p_curr = r_core

            r_roche_curr = self.compute_roche_lobe_radius(a_curr, m_total_kg)
            ff = r_p_curr / r_roche_curr if r_roche_curr > 0 else 0.0
            max_ff = max(max_ff, ff)

            # Core disruption check
            if r_p_curr == r_core and ff >= 1.0:
                disrupted = True
                m_total_kg = 0.0
                m_env_kg = 0.0
                break

            # Hydrodynamic RLOF Mass Loss with Adaptive Sub-Stepping
            if ff >= 0.95 and m_env_kg > 0.0:
                m_dot_0 = 1.0e-7 * M_JUP  # kg/yr
                m_dot = m_dot_0 * np.exp(self.eta_rlof * (ff - 1.0))
                est_loss = m_dot * dt_yr

                n_sub = max(1, int(np.ceil(est_loss / (0.0005 * M_JUP))),
                            int(dt_yr / 1000.0))
                n_sub = min(n_sub, 100000)
                dt_sub_yr = dt_yr / n_sub

                for _ in range(n_sub):
                    if m_env_kg <= 0.0:
                        break
                    r_roche_sub = self.compute_roche_lobe_radius(
                        a_curr, m_total_kg)
                    ff_sub = r_p_curr / r_roche_sub if r_roche_sub > 0 else 0.0
                    if ff_sub < 0.95:
                        break
                    m_dot_sub = m_dot_0 * np.exp(self.eta_rlof * (ff_sub - 1.0))
                    loss_sub = min(m_env_kg, m_dot_sub * dt_sub_yr)

                    m_env_kg -= loss_sub
                    m_total_kg = m_core_kg + m_env_kg

                    da_rlof_sub = -2.0 * a_curr * (-loss_sub / m_total_kg) * (
                        1.0 - self.beta_angular_momentum)
                    a_curr += da_rlof_sub

            # Stellar Tidal Orbital Decay da/dt |_tide
            n_orb = np.sqrt(G * (self.m_star_sun * M_SUN) /
                            max(1.0e6, a_curr**3))
            da_tide = (-9.0 * (self.k2_star / self.q_star_prime) * n_orb *
                       ((R_SUN / max(1.0e6, a_curr))**5) *
                       (m_total_kg /
                        (self.m_star_sun * M_SUN)) * a_curr * dt_sec)
            a_curr += da_tide

            # Tidal Eccentricity Circularization de/dt (Hut 1981)
            de_dt_p = (10.5 * (self.k2_planet / self.q_planet_prime) * n_orb *
                       ((self.m_star_sun * M_SUN) / max(1.0e-10, m_total_kg)) *
                       ((r_p_curr / max(1.0e6, a_curr))**5))
            de_dt_star = (4.5 * (self.k2_star / self.q_star_prime) * n_orb *
                          (m_total_kg / (self.m_star_sun * M_SUN)) *
                          ((R_SUN / max(1.0e6, a_curr))**5))
            e_curr = e_curr * np.exp(-(de_dt_p + de_dt_star) * dt_sec)

            if a_curr <= 0.008 * AU or m_total_kg <= 0:
                engulfed = True
                break

            a_arr[idx] = a_curr / AU
            e_arr[idx] = e_curr
            m_p_arr[idx] = m_total_kg / M_JUP
            m_env_arr[idx] = m_env_kg / M_JUP
            r_p_arr[idx] = r_p_curr / R_JUP
            r_roche_arr[idx] = r_roche_curr / AU
            ff_arr[idx] = ff
            valid_pts = idx + 1

        t_arr = t_arr[:valid_pts]
        a_arr = a_arr[:valid_pts]
        e_arr = e_arr[:valid_pts]
        m_p_arr = m_p_arr[:valid_pts]
        m_env_arr = m_env_arr[:valid_pts]
        r_p_arr = r_p_arr[:valid_pts]
        r_roche_arr = r_roche_arr[:valid_pts]
        ff_arr = ff_arr[:valid_pts]

        # Determine outcome classification
        if disrupted or engulfed or m_total_kg <= 0:
            outcome = EvolutionOutcome.DISRUPTED
            final_m_rem = 0.0
            z_bulk = 0.0
        elif max_ff >= 0.95:
            m_crit_jup = 0.50 * ((self.a_init_au / 0.018)**3.0)
            if self.m_p_init_jup < m_crit_jup:
                outcome = EvolutionOutcome.DISRUPTED
                final_m_rem = 0.0
                z_bulk = 0.0
            else:
                outcome = EvolutionOutcome.STAGNATED
                final_m_rem = m_total_kg / M_EARTH
                z_bulk = m_core_kg / m_total_kg if m_total_kg > 0 else 1.0
        else:
            outcome = EvolutionOutcome.COOLING
            final_m_rem = m_total_kg / M_EARTH
            z_bulk = m_core_kg / m_total_kg if m_total_kg > 0 else 0.0

        return TrajectoryResult(t_arr=t_arr,
                                a_arr=a_arr,
                                m_p_arr=m_p_arr,
                                m_env_arr=m_env_arr,
                                m_core_arr=np.full_like(t_arr,
                                                        self.m_core_earth),
                                r_p_arr=r_p_arr,
                                r_roche_arr=r_roche_arr,
                                filling_factor_arr=ff_arr,
                                outcome=outcome,
                                final_m_remnant_earth=final_m_rem,
                                z_bulk=z_bulk)
