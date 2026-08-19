"""
Python wrapper for Frontier 3: Ultra-Short-Period (USP) RLOF & Super-Mercury Formation.
"""

from dataclasses import dataclass

import numpy as np


@dataclass
class USPEvolutionStep:
    time_myr: float
    semimajor_axis_au: float
    orbital_period_hours: float
    planet_mass_mearth: float
    core_mass_mearth: float
    mantle_mass_mearth: float
    planet_radius_rearth: float
    roche_radius_au: float
    is_overflowing: bool


class USPRLOFDiscovery:
    """Python interface to the USP Tidal-RLOF Coupled Evolution Engine."""

    def __init__(self,
                 star_mass_msun: float = 1.0,
                 star_radius_rsun: float = 1.0,
                 k2_q_star: float = 1.0e-6):
        self.star_mass_msun = star_mass_msun
        self.star_radius_rsun = star_radius_rsun
        self.k2_q_star = k2_q_star

    def roche_radius(self, m_planet_me: float, r_planet_re: float) -> float:
        """Roche radius in AU."""
        m_p_kg = m_planet_me * 5.972e24
        r_p_m = r_planet_re * 6.371e6
        m_s_kg = self.star_mass_msun * 1.989e30
        a_roche_m = r_p_m * ((3.0 * m_s_kg / m_p_kg)**(1.0 / 3.0))
        return float(a_roche_m / 1.496e11)

    def tidal_decay_rate(self, a_au: float, m_planet_me: float) -> float:
        """Tidal orbital decay rate da/dt [AU / Myr]."""
        m_s_kg = self.star_mass_msun * 1.989e30
        r_s_m = self.star_radius_rsun * 6.957e8
        m_p_kg = m_planet_me * 5.972e24
        a_m = a_au * 1.496e11

        da_dt_m_s = -4.5 * np.sqrt(
            6.674e-11 / m_s_kg) * self.k2_q_star * m_p_kg * (a_m**-5.5) * (r_s_m
                                                                           **5)
        return float((da_dt_m_s * 3.15576e13) / 1.496e11)

    def differentiated_radius(self, m_core_me: float,
                              m_mantle_me: float) -> float:
        """Differentiated iron core + silicate mantle radius [R_Earth]."""
        m_tot = m_core_me + m_mantle_me
        if m_tot <= 1.0e-5:
            return 0.1
        f_fe = m_core_me / m_tot
        r_fe = 0.78 * (m_tot**0.30)
        r_si = 1.05 * (m_tot**0.27)
        return float(f_fe * r_fe + (1.0 - f_fe) * r_si)

    def evolve(self,
               m_core_init_me: float,
               m_mantle_init_me: float,
               a_init_au: float,
               t_max_myr: float = 5000.0,
               dt_myr: float = 1.0) -> list[USPEvolutionStep]:
        """Evolve coupled tidal decay and mantle stripping."""
        history = []
        a = a_init_au
        m_c = m_core_init_me
        m_m = m_mantle_init_me
        r_star_au = (self.star_radius_rsun * 6.957e8) / 1.496e11

        for t in np.arange(0.0, t_max_myr + dt_myr, dt_myr):
            m_tot = m_c + m_m
            r_p = self.differentiated_radius(m_c, m_m)
            a_roche = self.roche_radius(m_tot, r_p)
            p_hours = 24.0 * np.sqrt((a**3) / self.star_mass_msun) * 365.25

            mdot = 0.0
            if a < a_roche:
                overfill = (a_roche - a) / a_roche
                mdot = min(1.0e4, 5.0e3 * m_tot * (overfill**2.5))

            da_tide = self.tidal_decay_rate(a, m_tot)
            da_rlof = (2.0 * a * (mdot / m_tot) *
                       0.15) if (mdot > 0 and m_tot > 0) else 0.0
            net_da = da_tide + da_rlof

            history.append(
                USPEvolutionStep(
                    time_myr=float(t),
                    semimajor_axis_au=float(a),
                    orbital_period_hours=float(p_hours),
                    planet_mass_mearth=float(m_tot),
                    core_mass_mearth=float(m_c),
                    mantle_mass_mearth=float(m_m),
                    planet_radius_rearth=float(r_p),
                    roche_radius_au=float(a_roche),
                    is_overflowing=(a <= a_roche),
                ))

            if mdot > 0.0:
                dm = mdot * dt_myr
                if m_m > dm:
                    m_m -= dm
                else:
                    rem = dm - m_m
                    m_m = 0.0
                    m_c = max(0.01, m_c - rem * 0.2)

            a += net_da * dt_myr
            if a <= r_star_au or m_tot <= 0.05:
                break

        return history
