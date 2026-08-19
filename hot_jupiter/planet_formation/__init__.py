"""
Planet Formation & Protoplanetary Disk Physics Sub-Package.
Models Core Accretion, Pebble Accretion, Disk Migration, and Streaming Instability.
"""

import numpy as np


class CoreAccretion:

    def critical_core_mass_kg(self,
                              planetesimal_accretion_rate_kg_s,
                              opacity_cm2_g=0.1):
        m_earth_yr_to_kg_s = 5.972e24 / (365.25 * 86400.0)
        m_dot_normalized = planetesimal_accretion_rate_kg_s / (
            1.0e-6 * m_earth_yr_to_kg_s)
        m_crit_earth = 10.0 * (np.maximum(1.0e-4, m_dot_normalized)**0.25) * (
            (opacity_cm2_g / 0.1)**0.25)
        return m_crit_earth * 5.972e24


class PebbleAccretion:

    def hill_radius_m(self, m_core_kg, a_m, m_star_kg=1.98847e30):
        return a_m * (m_core_kg / (3.0 * m_star_kg))**(1.0 / 3.0)

    def pebble_accretion_rate_kg_s(self,
                                   m_core_kg,
                                   a_m,
                                   surface_density_pebbles_kg_m2,
                                   stokes_number=0.1):
        g = 6.67430e-11
        m_star_kg = 1.98847e30
        n = np.sqrt(g * m_star_kg / a_m**3)
        r_h = self.hill_radius_m(m_core_kg, a_m, m_star_kg)
        accretion_cross_section = r_h**2 * stokes_number**(2.0 / 3.0)
        return accretion_cross_section * surface_density_pebbles_kg_m2 * n


class DiskMigration:

    def type_i_migration_timescale_yr(self,
                                      m_planet_kg,
                                      a_m,
                                      surface_density_gas_kg_m2=1000.0,
                                      aspect_ratio=0.05):
        g = 6.67430e-11
        m_sun = 1.98847e30
        n = np.sqrt(g * m_sun / a_m**3)
        q = m_planet_kg / m_sun
        gamma_type1 = (q / aspect_ratio**2) * (surface_density_gas_kg_m2 *
                                               a_m**2 / m_sun)
        t_migration_s = 1.0 / (gamma_type1 * n)
        return t_migration_s / (365.25 * 86400.0)


class StreamingInstability:

    def critical_dust_to_gas_ratio(self, stokes_number=0.1):
        return 0.01 + 0.05 * (stokes_number - 0.1)**2


from hot_jupiter.planet_formation.resonant_chain import (
    ResonantChainDiscovery,
    ResonantEvolutionStep,
)

__all__ = [
    "CoreAccretion",
    "DiskMigration",
    "PebbleAccretion",
    "ResonantChainDiscovery",
    "ResonantEvolutionStep",
    "StreamingInstability",
]
