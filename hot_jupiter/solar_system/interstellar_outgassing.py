"""
Python wrapper for Frontier 7: Interstellar Object Volatile Depletion & Outgassing Torques Engine.
"""

from dataclasses import dataclass

import numpy as np


@dataclass
class OutgassingEvolutionStep:
    heliocentric_dist_au: float
    surface_temp_k: float
    sublimation_rate_kg_m2_s: float
    non_grav_accel_m_s2: float
    spin_period_hours: float
    centrifugal_stress_pa: float
    is_tensile_disrupted: bool


class InterstellarOutgassingDiscovery:
    """Python interface to the Interstellar Object Outgassing & Structural Integrity Engine."""

    def __init__(self,
                 eff_radius_m: float = 100.0,
                 axis_ratio_a_over_b: float = 6.0,
                 bulk_density_kg_m3: float = 300.0,
                 porosity_fraction: float = 0.70,
                 tensile_strength_pa: float = 10.0,
                 ice_type: str = "H2"):
        self.eff_radius_m = eff_radius_m
        self.axis_ratio_a_over_b = axis_ratio_a_over_b
        self.bulk_density_kg_m3 = bulk_density_kg_m3
        self.porosity_fraction = porosity_fraction
        self.tensile_strength_pa = tensile_strength_pa
        self.ice_type = ice_type

    def latent_heat_j_per_kg(self) -> float:
        """Sublimation latent heat [J / kg]."""
        mapping = {"H2O": 2.8e6, "CO": 2.0e5, "N2": 2.3e5, "H2": 4.5e5}
        return float(mapping.get(self.ice_type, 2.8e6))

    def mean_molecular_mass_kg(self) -> float:
        """Molecular mass in kg."""
        n_a = 6.02214076e23
        mapping = {
            "H2O": 18.015e-3 / n_a,
            "CO": 28.01e-3 / n_a,
            "N2": 28.013e-3 / n_a,
            "H2": 2.016e-3 / n_a,
        }
        return float(mapping.get(self.ice_type, 18.015e-3 / n_a))

    def surface_temperature_k(self, r_au: float, albedo: float = 0.05) -> float:
        """Equilibrium surface temperature at distance r_au."""
        solar_const = 1361.0 / (r_au * r_au)
        sigma_sb = 5.670374419e-8
        t_eq = ((1.0 - albedo) * solar_const / (4.0 * sigma_sb))**0.25
        return float(max(5.0, t_eq))

    def sublimation_flux_kg_m2_s(self, r_au: float) -> float:
        """Energy-balance sublimation mass flux [kg / m^2 / s]."""
        solar_const = 1361.0 / (r_au * r_au)
        l_sub = self.latent_heat_j_per_kg()
        return float((0.95 * solar_const) / (l_sub * 4.0))

    def thermal_exhaust_velocity_m_s(self, temp_k: float) -> float:
        """Thermal gas exhaust velocity [m / s]."""
        kb = 1.380649e-23
        m_mol = self.mean_molecular_mass_kg()
        return float(np.sqrt(8.0 * kb * temp_k / (np.pi * m_mol)))

    def compute_non_grav_acceleration(self,
                                      r_au: float,
                                      f_anisotropy: float = 0.25) -> float:
        """Non-gravitational acceleration [m / s^2]."""
        temp = self.surface_temperature_k(r_au)
        z_flux = self.sublimation_flux_kg_m2_s(r_au)
        v_th = self.thermal_exhaust_velocity_m_s(temp)

        body_mass = (4.0 / 3.0) * np.pi * (self.eff_radius_m**
                                           3) * self.bulk_density_kg_m3
        cross_area = np.pi * (self.eff_radius_m**2)
        thrust_force = f_anisotropy * z_flux * cross_area * v_th
        return float(thrust_force / max(1.0, body_mass))

    def evolve_flyby(self,
                     q_perihelion_au: float = 0.255,
                     initial_spin_period_hrs: float = 8.14,
                     t_span_days: float = 120.0,
                     dt_days: float = 0.5) -> list[OutgassingEvolutionStep]:
        """Simulate trajectory, non-gravitational acceleration, and spin evolution."""
        history = []
        omega = 2.0 * np.pi / (initial_spin_period_hrs * 3600.0)
        body_mass = (4.0 / 3.0) * np.pi * (self.eff_radius_m**
                                           3) * self.bulk_density_kg_m3
        a_long = self.eff_radius_m * np.sqrt(self.axis_ratio_a_over_b)
        moment_inertia = 0.20 * body_mass * (a_long**2)
        v_inf_au_day = 0.15

        for t_day in np.arange(-t_span_days / 2.0, t_span_days / 2.0 + dt_days,
                               dt_days):
            r_au = np.sqrt(q_perihelion_au**2 + (v_inf_au_day * t_day)**2)
            temp = self.surface_temperature_k(r_au)
            z_flux = self.sublimation_flux_kg_m2_s(r_au)
            a_ng = self.compute_non_grav_acceleration(r_au, 0.20)

            # Spinup torque
            v_th = self.thermal_exhaust_velocity_m_s(temp)
            cross_area = np.pi * (self.eff_radius_m**2)
            torque = (0.15 * z_flux * cross_area * v_th) * (
                0.10 * self.eff_radius_m * self.axis_ratio_a_over_b)

            d_omega_dt = torque / moment_inertia
            omega += d_omega_dt * (dt_days * 86400.0)
            p_hrs = (2.0 * np.pi / max(1.0e-7, omega)) / 3600.0

            sigma_cent = 0.25 * self.bulk_density_kg_m3 * (omega**2) * (a_long**
                                                                        2)
            disrupted = sigma_cent >= self.tensile_strength_pa

            history.append(
                OutgassingEvolutionStep(
                    heliocentric_dist_au=float(r_au),
                    surface_temp_k=float(temp),
                    sublimation_rate_kg_m2_s=float(z_flux),
                    non_grav_accel_m_s2=float(a_ng),
                    spin_period_hours=float(p_hrs),
                    centrifugal_stress_pa=float(sigma_cent),
                    is_tensile_disrupted=bool(disrupted),
                ))

        return history
