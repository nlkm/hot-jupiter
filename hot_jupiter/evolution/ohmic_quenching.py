"""
Python wrapper for Frontier 2: Hot Jupiter Ohmic Inflation & Dynamo Quenching Engine.
"""

from dataclasses import dataclass

import numpy as np


@dataclass
class OhmicQuenchingResult:
    t_eq_k: float
    conductivity_s_m: float
    wind_speed_m_s: float
    ohmic_power_watts: float
    inflated_radius_rjup: float
    is_quenched: bool


class OhmicQuenchingDiscovery:
    """Python interface to the Ohmic Dissipation & Dynamo Quenching Discovery Engine."""

    def __init__(self,
                 b_field_gauss: float = 5.0,
                 planet_mass_mjup: float = 1.0):
        self.b_field_gauss = b_field_gauss
        self.planet_mass_mjup = planet_mass_mjup

    def atmospheric_conductivity(self, t_eq_k: float) -> float:
        """Saha ionization atmospheric electrical conductivity [S/m]."""
        t = max(500.0, t_eq_k)
        e_ion = 4.34 * 1.602e-19  # Potassium ionization in Joules
        n_gas = (0.1 * 1.0e5) / (1.38e-23 * t)
        n_k = n_gas * 1.0e-7

        saha_pref = 2.0 * ((2.0 * np.pi * 9.109e-31 * 1.38e-23 * t) /
                           (6.626e-34**2))**1.5
        n_e = np.sqrt(n_k * saha_pref) * np.exp(-e_ion / (2.0 * 1.38e-23 * t))

        v_th_e = np.sqrt(8.0 * 1.38e-23 * t / (np.pi * 9.109e-31))
        nu_en = n_gas * 1.0e-19 * v_th_e

        sigma = (n_e * (1.602e-19**2)) / (9.109e-31 * (nu_en + 1.0e-5))
        return float(max(1.0e-12, sigma))

    def wind_speed(self, t_eq_k: float, sigma_elec: float) -> float:
        """Self-consistent Lorentz-braked jet speed [m/s]."""
        v_0 = 4000.0 * np.sqrt(t_eq_k / 2000.0)
        b_tesla = self.b_field_gauss * 1.0e-4
        rho_gas = (0.1 * 1.0e5) / ((1.38e-23 / (2.3 * 1.67e-27)) * t_eq_k)

        tau_mag = rho_gas / (sigma_elec * (b_tesla**2) + 1.0e-20)
        tau_rad = 1.0e5
        drag_factor = 1.0 + (tau_rad / tau_mag)
        return float(v_0 / drag_factor)

    def ohmic_power(self, t_eq_k: float) -> float:
        """Ohmic heating power [Watts]."""
        sigma = self.atmospheric_conductivity(t_eq_k)
        v = self.wind_speed(t_eq_k, sigma)
        b_tesla = self.b_field_gauss * 1.0e-4

        r_planet = 7.149e7 * 1.2
        vol = 4.0 * np.pi * (r_planet**2) * 500.0e3
        p_density = sigma * ((v * b_tesla)**2)
        return float(p_density * vol)

    def inflated_radius(self, t_eq_k: float) -> float:
        """Equilibrium inflated radius [R_Jupiter]."""
        r_base = 1.05 * (self.planet_mass_mjup**(-0.05))
        p_ohmic = self.ohmic_power(t_eq_k)
        r_irr = 0.15 * ((t_eq_k / 1500.0)**0.8)
        delta_r = 0.55 * ((p_ohmic / 1.0e19)**0.35)
        return float(r_base + r_irr + delta_r)

    def evaluate(self, t_eq_k: float) -> OhmicQuenchingResult:
        """Evaluate full state."""
        sigma = self.atmospheric_conductivity(t_eq_k)
        v = self.wind_speed(t_eq_k, sigma)
        p = self.ohmic_power(t_eq_k)
        r = self.inflated_radius(t_eq_k)
        return OhmicQuenchingResult(
            t_eq_k=t_eq_k,
            conductivity_s_m=sigma,
            wind_speed_m_s=v,
            ohmic_power_watts=p,
            inflated_radius_rjup=r,
            is_quenched=(t_eq_k > 1850.0),
        )
