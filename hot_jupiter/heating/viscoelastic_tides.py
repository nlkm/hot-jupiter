"""
Python wrapper for Frontier 8: Frequency-Dependent Andrade Viscoelastic Tidal Dissipation Engine.
"""

from dataclasses import dataclass

import numpy as np


@dataclass
class ViscoelasticTidalStep:
    mantle_temperature_k: float
    mantle_viscosity_pa_s: float
    maxwell_time_yr: float
    k2_real: float
    k2_imag: float
    tidal_quality_factor_q: float
    tidal_heating_power_watts: float
    convective_heat_loss_watts: float
    is_thermal_equilibrium: bool


class ViscoelasticTidesDiscovery:
    """Python interface to the Frequency-Dependent Andrade Viscoelastic Tidal Dissipation Engine."""

    def __init__(self,
                 planet_radius_m: float = 1.8216e6,
                 planet_mass_kg: float = 8.9319e22,
                 star_mass_kg: float = 1.89813e27,
                 semi_major_axis_m: float = 4.217e8,
                 eccentricity: float = 0.0041,
                 shear_modulus_gpa: float = 65.0,
                 andrade_alpha: float = 0.30,
                 andrade_zeta: float = 1.0):
        self.planet_radius_m = planet_radius_m
        self.planet_mass_kg = planet_mass_kg
        self.star_mass_kg = star_mass_kg
        self.semi_major_axis_m = semi_major_axis_m
        self.eccentricity = eccentricity
        self.shear_modulus_gpa = shear_modulus_gpa
        self.andrade_alpha = andrade_alpha
        self.andrade_zeta = andrade_zeta

    def tidal_forcing_frequency_rad_s(self) -> float:
        """Orbital mean motion frequency omega [rad / s]."""
        g_const = 6.67430e-11
        return float(
            np.sqrt(g_const * self.star_mass_kg / (self.semi_major_axis_m**3)))

    def mantle_viscosity_pa_s(self, temp_k: float) -> float:
        """Temperature-dependent mantle viscosity [Pa s]."""
        t_clamped = np.clip(temp_k, 800.0, 2200.0)
        e_act = 300.0e3
        r_gas = 8.314
        t_ref = 1600.0
        eta_0 = 1.0e16
        return float(eta_0 * np.exp(
            (e_act / r_gas) * (1.0 / t_clamped - 1.0 / t_ref)))

    def andrade_compliance(self, omega_rad_s: float, temp_k: float) -> complex:
        """Complex Andrade compliance J(omega)."""
        mu = self.shear_modulus_gpa * 1.0e9
        eta = self.mantle_viscosity_pa_s(temp_k)

        j_elastic = 1.0 / mu
        j_fluid = -1j / (omega_rad_s * eta)

        import math
        gamma_val = math.gamma(1.0 + self.andrade_alpha)
        beta = (self.andrade_zeta / mu) * (
            (mu / eta)**self.andrade_alpha) * gamma_val

        cos_t = np.cos(self.andrade_alpha * np.pi / 2.0)
        sin_t = np.sin(self.andrade_alpha * np.pi / 2.0)
        j_andrade = beta * (omega_rad_s**-self.andrade_alpha) * complex(
            cos_t, -sin_t)

        return j_elastic + j_fluid + j_andrade

    def compute_complex_love_number(self,
                                    omega_rad_s: float,
                                    temp_k: float,
                                    model: str = "andrade") -> complex:
        """Compute complex Love number k2(omega)."""
        g_const = 6.67430e-11
        rho = self.planet_mass_kg / ((4.0 / 3.0) * np.pi *
                                     (self.planet_radius_m**3))
        g_surf = g_const * self.planet_mass_kg / (self.planet_radius_m**2)
        mu_base = self.shear_modulus_gpa * 1.0e9
        eta = self.mantle_viscosity_pa_s(temp_k)

        if model == "andrade":
            j_comp = self.andrade_compliance(omega_rad_s, temp_k)
        elif model == "maxwell":
            j_comp = (1.0 / mu_base) - 1j / (omega_rad_s * eta)
        else:
            q_val = 100.0
            j_comp = (1.0 / mu_base) - 1j / (mu_base * q_val)

        mu_eff = 1.0 / j_comp
        hydro_factor = (19.0 / 2.0) / (rho * g_surf * self.planet_radius_m)
        denom = 1.0 + hydro_factor * mu_eff
        return complex(1.5 / denom)

    def compute_tidal_heating_power_watts(self,
                                          temp_k: float,
                                          model: str = "andrade") -> float:
        """Tidal dissipation heating power [W]."""
        g_const = 6.67430e-11
        omega = self.tidal_forcing_frequency_rad_s()
        k2 = self.compute_complex_love_number(omega, temp_k, model)
        im_k2 = abs(k2.imag)

        prefactor = (21.0 / 2.0) * g_const * (self.star_mass_kg**2) * (
            self.planet_radius_m**5) / (self.semi_major_axis_m**6)
        return float(prefactor * (self.eccentricity**2) * omega * im_k2)

    def compute_convective_heat_loss_watts(self,
                                           temp_k: float,
                                           t_surf_k: float = 130.0) -> float:
        """Mantle convective heat loss [W]."""
        delta_t = max(10.0, temp_k - t_surf_k)
        area = 4.0 * np.pi * (self.planet_radius_m**2)
        heat_flux = 2.5 * ((delta_t / (1600.0 - 130.0))**(4.0 / 3.0))
        return float(area * heat_flux)

    def evaluate_thermal_spectrum(
            self,
            t_min_k: float = 1000.0,
            t_max_k: float = 2000.0,
            dt_k: float = 10.0,
            model: str = "andrade") -> list[ViscoelasticTidalStep]:
        """Evaluate dissipation and thermal balance across temperature spectrum."""
        spectrum = []
        omega = self.tidal_forcing_frequency_rad_s()

        for t in np.arange(t_min_k, t_max_k + dt_k, dt_k):
            eta = self.mantle_viscosity_pa_s(t)
            tau_m_yr = (eta /
                        (self.shear_modulus_gpa * 1.0e9)) / (365.25 * 86400.0)
            k2 = self.compute_complex_love_number(omega, t, model)
            q_eff = abs(k2.real) / max(1.0e-10, abs(k2.imag))

            p_tide = self.compute_tidal_heating_power_watts(t, model)
            p_conv = self.compute_convective_heat_loss_watts(t)
            eq = abs(p_tide - p_conv) / max(1.0e10, p_conv) < 0.10

            spectrum.append(
                ViscoelasticTidalStep(
                    mantle_temperature_k=float(t),
                    mantle_viscosity_pa_s=float(eta),
                    maxwell_time_yr=float(tau_m_yr),
                    k2_real=float(k2.real),
                    k2_imag=float(abs(k2.imag)),
                    tidal_quality_factor_q=float(q_eff),
                    tidal_heating_power_watts=float(p_tide),
                    convective_heat_loss_watts=float(p_conv),
                    is_thermal_equilibrium=bool(eq),
                ))

        return spectrum
