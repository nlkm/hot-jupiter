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
    """Dataclass holding complete trajectory results from CoupledRLOFIntegrator."""
    t_arr: np.ndarray  # Time array [yr]
    a_arr: np.ndarray  # Semi-major axis array [AU]
    m_p_arr: np.ndarray  # Total planet mass array [M_Jup]
    m_env_arr: np.ndarray  # Envelope mass array [M_Jup]
    m_core_arr: np.ndarray  # Core mass array [M_Earth]
    r_p_arr: np.ndarray  # Planet physical radius array [R_Jup]
    r_roche_arr: np.ndarray  # Roche lobe radius array [AU]
    filling_factor_arr: np.ndarray  # Roche lobe filling factor mu_Roche = R_p / R_Roche
    outcome: EvolutionOutcome  # Final physical outcome classification
    final_m_remnant_earth: float  # Final remnant mass at t_max [M_Earth]
    z_bulk: float  # Final bulk heavy-element fraction M_core / M_total


class CoupledRLOFIntegrator:
    """
    Unified 4D numerical integrator coupling 1D interior hydrostatic thermal cooling,
    hydrodynamic Roche Lobe Overflow (RLOF) mass loss, and stellar tidal orbital decay.
    """

    def __init__(self,
                 m_p_init_jup: float = 1.0,
                 a_init_au: float = 0.02,
                 m_core_earth: float = 10.0,
                 m_star_sun: float = 1.0,
                 q_star_prime: float = 1.5e5,
                 k2_star: float = 0.03,
                 eta_rlof: float = 4.0,
                 beta_angular_momentum: float = 0.5):
        self.m_p_init_jup = m_p_init_jup
        self.a_init_au = a_init_au
        self.m_core_earth = m_core_earth
        self.m_star_sun = m_star_sun
        self.q_star_prime = q_star_prime
        self.k2_star = k2_star
        self.eta_rlof = eta_rlof
        self.beta_angular_momentum = beta_angular_momentum

    def compute_roche_lobe_radius(self, a_m: float, m_total_kg: float) -> float:
        """Compute volume-equivalent Roche lobe radius using Eggleton (1983)."""
        m_star_kg = self.m_star_sun * M_SUN
        q = m_total_kg / m_star_kg
        q_13 = q**(1.0 / 3.0)
        q_23 = q**(2.0 / 3.0)
        r_roche_ratio = 0.49 * q_23 / (0.6 * q_23 + np.log(1.0 + q_13))
        return float(a_m * r_roche_ratio)

    def integrate(self,
                  t_max_yr: float = 5.0e9,
                  num_pts: int = 400) -> TrajectoryResult:
        """
        Integrate coupled trajectory from t = 1 Myr to t_max_yr.
        """
        m_core_kg = self.m_core_earth * M_EARTH
        m_env_init_kg = max(0.0, (self.m_p_init_jup * M_JUP) - m_core_kg)
        m_env_kg = m_env_init_kg
        m_total_kg = m_core_kg + m_env_kg

        a_curr = self.a_init_au * AU

        t_arr = np.geomspace(1.0e6, t_max_yr, num_pts)
        a_arr = np.zeros(num_pts)
        m_p_arr = np.zeros(num_pts)
        m_env_arr = np.zeros(num_pts)
        r_p_arr = np.zeros(num_pts)
        r_roche_arr = np.zeros(num_pts)
        ff_arr = np.zeros(num_pts)

        disrupted = False
        engulfed = False
        max_ff = 0.0

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

            # Core disruption check: if filling factor at core boundary exceeds 1.0, core is torn apart
            if r_p_curr == r_core and ff >= 1.0:
                disrupted = True
                m_total_kg = 0.0
                m_env_kg = 0.0
                break

            # Hydrodynamic RLOF Mass Loss
            if ff >= 0.95 and m_env_kg > 0.0:
                m_dot_0 = 1.0e-7 * M_JUP  # kg/yr
                m_dot = m_dot_0 * np.exp(self.eta_rlof * (ff - 1.0))
                loss_kg = m_dot * dt_yr

                if loss_kg >= m_env_kg:
                    m_env_kg = 0.0
                else:
                    m_env_kg -= loss_kg

                m_total_kg = m_core_kg + m_env_kg

                # RLOF orbital expansion / decay
                da_rlof = -2.0 * a_curr * (-loss_kg / m_total_kg) * (
                    1.0 - self.beta_angular_momentum)
                a_curr += da_rlof

            # Stellar Tidal Orbital Decay da/dt |_tide
            n_orb = np.sqrt(G * (self.m_star_sun * M_SUN) /
                            max(1.0e6, a_curr**3))
            da_tide = (-9.0 * (self.k2_star / self.q_star_prime) * n_orb *
                       ((R_SUN / max(1.0e6, a_curr))**5) *
                       (m_total_kg /
                        (self.m_star_sun * M_SUN)) * a_curr * dt_sec)
            a_curr += da_tide

            if a_curr <= 0.008 * AU or m_total_kg <= 0:
                engulfed = True
                break

            a_arr[idx] = a_curr / AU
            m_p_arr[idx] = m_total_kg / M_JUP
            m_env_arr[idx] = m_env_kg / M_JUP
            r_p_arr[idx] = r_p_curr / R_JUP
            r_roche_arr[idx] = r_roche_curr / AU
            ff_arr[idx] = ff

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
