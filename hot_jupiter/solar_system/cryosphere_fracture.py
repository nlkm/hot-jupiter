"""
Python wrapper for Frontier 6: Ocean-Freezing Pressurization & Viscoelastic Cryosphere Fracture Engine.
"""

from dataclasses import dataclass

import numpy as np


@dataclass
class CryosphereEvolutionStep:
    time_myr: float
    ice_shell_thickness_km: float
    ocean_thickness_km: float
    ocean_overpressure_mpa: float
    surface_hoop_stress_mpa: float
    maxwell_relaxation_time_yr: float
    is_fractured: bool


class CryosphereFractureDiscovery:
    """Python interface to the Viscoelastic Ocean Freezing & Cryosphere Rupture Engine."""

    def __init__(self,
                 body_radius_km: float = 606.0,
                 surface_gravity_m_s2: float = 0.288,
                 bulk_density_kg_m3: float = 1700.0,
                 shear_modulus_gpa: float = 3.5,
                 tensile_strength_mpa: float = 2.0):
        self.body_radius_km = body_radius_km
        self.surface_gravity_m_s2 = surface_gravity_m_s2
        self.bulk_density_kg_m3 = bulk_density_kg_m3
        self.shear_modulus_gpa = shear_modulus_gpa
        self.tensile_strength_mpa = tensile_strength_mpa

    def ice_viscosity_pa_s(self, temperature_k: float) -> float:
        """Temperature-dependent ice viscosity [Pa s]."""
        t_clamped = np.clip(temperature_k, 100.0, 273.0)
        q_act = 50.0e3
        r_gas = 8.314
        eta_0 = 1.0e14
        return float(eta_0 * np.exp(
            (q_act / r_gas) * (1.0 / t_clamped - 1.0 / 273.15)))

    def maxwell_time_years(self, temperature_k: float) -> float:
        """Maxwell viscoelastic relaxation timescale in years."""
        eta = self.ice_viscosity_pa_s(temperature_k)
        mu_pa = self.shear_modulus_gpa * 1.0e9
        return float((eta / mu_pa) / (365.25 * 86400.0))

    def compute_ocean_overpressure_mpa(self, ice_thickness_km: float,
                                       ocean_thickness_km: float,
                                       frozen_layer_km: float) -> float:
        """Compute ocean overpressure from freezing [MPa]."""
        r_ocean_m = (self.body_radius_km - ice_thickness_km) * 1.0e3
        if r_ocean_m <= 1.0e3 or ocean_thickness_km <= 0.1:
            return 0.0

        delta_v_frac = 0.09
        k_water_pa = 2.0e9
        mu_ice_pa = self.shear_modulus_gpa * 1.0e9

        vol_change = 4.0 * np.pi * (r_ocean_m**2) * (frozen_layer_km *
                                                     1.0e3) * delta_v_frac
        ocean_vol = (4.0 / 3.0) * np.pi * (
            r_ocean_m**3 - (r_ocean_m - ocean_thickness_km * 1.0e3)**3)
        compliance = (1.0 / k_water_pa) + (3.0 / (4.0 * mu_ice_pa)) * (
            self.body_radius_km / max(1.0, ice_thickness_km))

        delta_p_pa = (vol_change / ocean_vol) / compliance
        return float(delta_p_pa / 1.0e6)

    def compute_surface_hoop_stress_mpa(self, delta_p_mpa: float,
                                        ice_thickness_km: float) -> float:
        """Compute surface hoop stress [MPa]."""
        h_eff = max(1.0, ice_thickness_km)
        return float(delta_p_mpa * (self.body_radius_km / (2.0 * h_eff)))

    def evolve_cryosphere(self,
                          initial_ice_thickness_km: float,
                          initial_ocean_thickness_km: float,
                          lid_temp_k: float = 120.0,
                          freezing_rate_km_myr: float = 0.05,
                          t_max_myr: float = 1000.0,
                          dt_myr: float = 1.0) -> list[CryosphereEvolutionStep]:
        """Evolve ocean freezing, viscoelastic relaxation, and shell fracture."""
        history = []
        h_ice = initial_ice_thickness_km
        h_ocean = initial_ocean_thickness_km
        tau_m_yr = self.maxwell_time_years(lid_temp_k)
        tau_m_myr = tau_m_yr / 1.0e6

        current_overpressure = 0.0
        fractured = False

        for t in np.arange(0.0, t_max_myr + dt_myr, dt_myr):
            if h_ocean > 0.1:
                delta_freeze = min(h_ocean, freezing_rate_km_myr * dt_myr)
                h_ice += delta_freeze
                h_ocean -= delta_freeze

                delta_p_inc = self.compute_ocean_overpressure_mpa(
                    h_ice, h_ocean, delta_freeze)
                relax_factor = np.exp(-dt_myr /
                                      tau_m_myr) if tau_m_myr > 0 else 0.0
                current_overpressure = current_overpressure * relax_factor + delta_p_inc

            hoop = self.compute_surface_hoop_stress_mpa(current_overpressure,
                                                        h_ice)
            if hoop >= self.tensile_strength_mpa:
                fractured = True

            history.append(
                CryosphereEvolutionStep(
                    time_myr=float(t),
                    ice_shell_thickness_km=float(h_ice),
                    ocean_thickness_km=float(h_ocean),
                    ocean_overpressure_mpa=float(current_overpressure),
                    surface_hoop_stress_mpa=float(hoop),
                    maxwell_relaxation_time_yr=float(tau_m_yr),
                    is_fractured=bool(fractured),
                ))

            if h_ocean <= 0.05:
                break

        return history
