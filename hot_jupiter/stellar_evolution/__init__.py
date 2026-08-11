"""
Stellar Evolution & Interior Structure Sub-Package.
Models ZAMS Mass-Luminosity, Eddington Limit, Reimers Mass Loss, and Polytropic Interiors.
"""

import numpy as np


class StellarMainSequence:

    def zams_luminosity_watts(self, m_star_kg):
        m_solar = m_star_kg / 1.98847e30
        l_solar = 3.828e26
        if m_solar < 0.43:
            ratio = 0.23 * m_solar**2.3
        elif m_solar < 2.0:
            ratio = m_solar**4.0
        elif m_solar < 20.0:
            ratio = 1.5 * m_solar**3.5
        else:
            ratio = 32000.0 * m_solar
        return ratio * l_solar

    def zams_radius_m(self, m_star_kg):
        m_solar = m_star_kg / 1.98847e30
        r_solar = 6.957e8
        ratio = m_solar**0.8 if m_solar < 1.0 else m_solar**0.57
        return ratio * r_solar


class EddingtonLimit:

    def eddington_luminosity_watts(self,
                                   m_star_kg,
                                   opacity_electron_scattering=0.04):
        g = 6.67430e-11
        c = 299792458.0
        return (4.0 * np.pi * g * m_star_kg * c) / opacity_electron_scattering


class ReimersStellarWind:

    def reimers_mass_loss_rate_kg_s(self,
                                    m_star_kg,
                                    r_star_m,
                                    l_star_watts,
                                    eta_reimers=0.5):
        m_solar = m_star_kg / 1.98847e30
        r_solar = r_star_m / 6.957e8
        l_solar = l_star_watts / 3.828e26
        m_dot_sun_yr = 4.0e-13 * eta_reimers * (l_solar * r_solar /
                                                np.maximum(0.1, m_solar))
        kg_per_sun_yr = 1.98847e30 / (365.25 * 86400.0)
        return m_dot_sun_yr * kg_per_sun_yr


__all__ = [
    "EddingtonLimit",
    "ReimersStellarWind",
    "StellarMainSequence",
]
