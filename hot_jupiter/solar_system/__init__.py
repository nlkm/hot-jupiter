"""
Solar System Bodies & Orbital Dynamics Sub-Package.
Models planets, moons, planetary rings, asteroids, and comets.
"""

import numpy as np


class MoonTidalDynamics:

    def io_tidal_heating_power_watts(self, eccentricity=0.0041):
        g = 6.67430e-11
        m_j = 1.898e27
        r_io = 1.821e6
        a_io = 4.217e8
        k2_over_q = 0.015
        n = np.sqrt(g * m_j / a_io**3)
        factor = 10.5 * k2_over_q * g * m_j**2 * r_io**5 * n / a_io**6
        return factor * eccentricity**2

    def earth_moon_recession_rate_m_s(self, a_moon_m=3.844e8):
        recession_cm_yr = 3.8 * (3.844e8 / a_moon_m)**5.5
        return (recession_cm_yr * 0.01) / (365.25 * 86400.0)


class PlanetaryRings:

    def roche_limit_m(self,
                      r_planet_m,
                      density_planet,
                      density_moon,
                      fluid=True):
        c = 2.456 if fluid else 1.442
        return c * r_planet_m * (density_planet /
                                 np.maximum(10.0, density_moon))**(1.0 / 3.0)

    def shepherd_moon_torque(self, m_moon, m_saturn, a_ring, delta_a):
        g = 6.67430e-11
        n = np.sqrt(g * m_saturn / a_ring**3)
        return (g**2 * m_moon**2) / (a_ring**2 * n * (delta_a / a_ring)**4)


class AsteroidDynamics:

    def yarkovsky_acceleration_m_s2(self, radius_m, density_kg_m3, a_au,
                                    obliquity_deg):
        au = 1.495978707e11
        mass = (4.0 / 3.0) * np.pi * radius_m**3 * density_kg_m3
        l_sun = 3.828e26
        c = 299792458.0
        a_m = a_au * au
        solar_flux = l_sun / (4.0 * np.pi * a_m**2)
        cross_section = np.pi * radius_m**2
        alpha = 0.15
        obl_rad = np.radians(obliquity_deg)
        force = (4.0 /
                 9.0) * alpha * cross_section * solar_flux / c * np.cos(obl_rad)
        return force / np.maximum(1.0e-5, mass)

    def in_kirkwood_gap(self, a_au):
        gaps = [2.50, 2.82, 2.95, 3.27]
        return any(abs(a_au - g) < 0.03 for g in gaps)


class CometDynamics:

    def marsden_sublimation_g_r(self,
                                r_au,
                                r0_au=2.808,
                                m=2.15,
                                n=5.09,
                                k=4.614):
        alpha = 0.11126
        ratio = r_au / r0_au
        return alpha * (ratio**(-m)) * ((1.0 + ratio**n)**(-k))

    def non_gravitational_acceleration_m_s2(self, r_au, a1_au_day2):
        au = 1.495978707e11
        g_r = self.marsden_sublimation_g_r(r_au)
        a1_m_s2 = a1_au_day2 * au / (86400.0**2)
        return a1_m_s2 * g_r


__all__ = [
    "AsteroidDynamics",
    "CometDynamics",
    "MoonTidalDynamics",
    "PlanetaryRings",
]
