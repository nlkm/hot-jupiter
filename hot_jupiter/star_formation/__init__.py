"""
Star Formation & ISM Dynamics Sub-Package.
Models Jeans Instability, Bonnor-Ebert Sphere, Larson Scaling Laws, and IMF.
"""

import numpy as np


class JeansInstability:

    def sound_speed_m_s(self, temp_k, mean_molecular_weight=2.3):
        kb = 1.380649e-23
        m_h = 1.6735575e-27
        return np.sqrt((kb * temp_k) / (mean_molecular_weight * m_h))

    def jeans_length_m(self, temp_k, density_kg_m3, mean_molecular_weight=2.3):
        g = 6.67430e-11
        c_s = self.sound_speed_m_s(temp_k, mean_molecular_weight)
        return c_s * np.sqrt(np.pi / (g * density_kg_m3))

    def jeans_mass_kg(self, temp_k, density_kg_m3, mean_molecular_weight=2.3):
        lambda_j = self.jeans_length_m(temp_k, density_kg_m3,
                                       mean_molecular_weight)
        return (np.pi / 6.0) * density_kg_m3 * lambda_j**3


class BonnorEbertSphere:

    def bonnor_ebert_mass_kg(self,
                             temp_k,
                             external_pressure_pa,
                             mean_molecular_weight=2.3):
        g = 6.67430e-11
        jeans = JeansInstability()
        c_s = jeans.sound_speed_m_s(temp_k, mean_molecular_weight)
        c_be = 1.18
        return (c_be * c_s**4) / np.sqrt(g**3 * external_pressure_pa)


class LarsonScalingLaws:

    def velocity_dispersion_m_s(self, cloud_size_pc):
        return 1100.0 * (cloud_size_pc**0.38)


__all__ = [
    "BonnorEbertSphere",
    "JeansInstability",
    "LarsonScalingLaws",
]
