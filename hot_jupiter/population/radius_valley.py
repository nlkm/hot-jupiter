"""
Radius Valley Population Synthesis Engine.
Disentangles Photoevaporation vs. Core-Powered Mass Loss vs. Primordial Water Worlds.
"""

from dataclasses import dataclass

import numpy as np


@dataclass
class ValleyDiscoveryResult:
    slope_period: float
    slope_mstar: float
    stripped_fraction: float
    mechanism: str


class RadiusValleyDiscovery:
    """Python interface to the Radius Valley Population Synthesis & Discovery Engine."""

    def __init__(self, seed: int = 42):
        self.rng = np.random.default_rng(seed)

    def photoevaporative_mass_loss_rate(self,
                                        m_core_me: float,
                                        f_env: float,
                                        a_au: float,
                                        m_star_msun: float = 1.0,
                                        age_gyr: float = 1.0) -> float:
        """Photoevaporative mass loss rate [M_Earth / Gyr]."""
        m_tot = m_core_me * (1.0 + f_env)
        lxuv_frac = 1.0e-3 if age_gyr < 0.1 else 1.0e-3 * (age_gyr /
                                                           0.1)**(-1.5)
        l_bol = (m_star_msun**3.5) * 3.828e26
        f_xuv = (lxuv_frac * l_bol) / (4.0 * np.pi * (a_au * 1.496e11)**2)

        eff = 0.10
        r_planet_m = self.compute_planet_radius(m_core_me, f_env, 0.0, a_au,
                                                m_star_msun, age_gyr) * 6.371e6
        m_tot_kg = m_tot * 5.972e24

        r_roche = a_au * 1.496e11 * (m_tot_kg /
                                     (3.0 * m_star_msun * 1.989e30))**(1.0 /
                                                                       3.0)
        k_tide = max(
            0.2, 1.0 - 1.5 * (r_planet_m / r_roche) + 0.5 *
            (r_planet_m / r_roche)**3)

        mdot_kg_s = (eff * np.pi *
                     (r_planet_m**3) * f_xuv) / (6.674e-11 * m_tot_kg * k_tide)
        return float(mdot_kg_s * 3.15576e16 / 5.972e24)

    def core_powered_mass_loss_rate(self,
                                    m_core_me: float,
                                    f_env: float,
                                    a_au: float,
                                    m_star_msun: float = 1.0,
                                    age_gyr: float = 1.0) -> float:
        """Core-powered mass loss rate [M_Earth / Gyr]."""
        m_tot = m_core_me * (1.0 + f_env)
        m_tot_kg = m_tot * 5.972e24
        e_core = 1.0e31 * m_core_me
        l_core = e_core / (age_gyr * 3.15576e16 + 1.0e14)
        r_planet_m = self.compute_planet_radius(m_core_me, f_env, 0.0, a_au,
                                                m_star_msun, age_gyr) * 6.371e6

        mdot_kg_s = l_core / (6.674e-11 * m_tot_kg / r_planet_m)
        return float(mdot_kg_s * 3.15576e16 / 5.972e24)

    def compute_planet_radius(self,
                              m_core_me: float,
                              f_env: float,
                              f_water: float = 0.0,
                              a_au: float = 0.05,
                              m_star_msun: float = 1.0,
                              age_gyr: float = 5.0) -> float:
        """Compute composite planet radius in R_Earth."""
        r_core = (m_core_me * (1.0 - f_water))**0.27
        if f_water > 0.0:
            r_water = 1.35 * (m_core_me * f_water)**0.29
            r_core = (r_core**3 + r_water**3)**(1.0 / 3.0)

        if f_env <= 1.0e-5:
            return float(r_core)

        flux = (m_star_msun**3.5) / (a_au**2)
        r_env = 2.06 * (m_core_me**(-0.21)) * (
            (f_env / 0.05)**0.59) * (flux**0.044) * ((age_gyr / 5.0)**(-0.18))
        return float(r_core + r_env)

    def valley_slope_dlogr_dlogp(self, mechanism: str) -> float:
        """Slope dlog(R_valley) / dlog(P)."""
        slopes = {
            "photoevaporation": -0.11,
            "core_powered": -0.06,
            "water_worlds": 0.00,
            "hybrid": -0.09,
        }
        return slopes.get(mechanism.lower(), -0.09)

    def valley_slope_dlogr_dlogmstar(self, mechanism: str) -> float:
        """Slope dlog(R_valley) / dlog(M_star)."""
        slopes = {
            "photoevaporation": +0.25,
            "core_powered": +0.35,
            "water_worlds": +0.00,
            "hybrid": +0.28,
        }
        return slopes.get(mechanism.lower(), +0.28)
