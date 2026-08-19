"""
Python wrapper for Frontier 4: Terminator Aerosol Asymmetry & JWST Spectra.
"""

from dataclasses import dataclass

import numpy as np


@dataclass
class LimbMicrophysicsResult:
    pressure_bar: float
    temperature_k: float
    silicate_vapor_abundance: float
    cloud_condensate_mass_frac: float
    mean_droplet_radius_um: float
    optical_depth_slant_1um: float
    optical_depth_slant_4um: float


@dataclass
class JWSTTransmissionSpectrum:
    wavelength_um: np.ndarray
    transit_depth_morning_ppm: np.ndarray
    transit_depth_evening_ppm: np.ndarray
    transit_depth_symmetric_ppm: np.ndarray
    evening_morning_contrast_ppm: np.ndarray


class TerminatorAerosolDiscovery:
    """Python interface to the 3D GCM Terminator Aerosol Rainout & JWST Spectral Engine."""

    def __init__(self,
                 t_eq_planet_k: float = 1600.0,
                 surface_gravity_m_s2: float = 10.0,
                 metallicity_dex: float = 0.0):
        self.t_eq_planet_k = t_eq_planet_k
        self.surface_gravity_m_s2 = surface_gravity_m_s2
        self.metallicity_dex = metallicity_dex

    def silicate_condensation_temp(self, p_bar: float) -> float:
        """Silicate (MgSiO3) condensation temperature as function of pressure."""
        log_p = np.log10(max(1.0e-6, p_bar))
        denom = 6.5 - 0.2 * log_p
        return float(10000.0 / denom if denom > 0.1 else 2000.0)

    def limb_temperature(self, p_bar: float, limb_type: int) -> float:
        """Evaluate temperature on Dayside (0), Evening (1), Morning (2), Nightside (3)."""
        t_day = self.t_eq_planet_k * np.sqrt(2.0)
        t_night = self.t_eq_planet_k * 0.65

        t_eve_strat = 0.85 * t_day + 0.15 * t_night
        t_mor_strat = 0.20 * t_day + 0.80 * t_night

        if limb_type == 0:
            base_t = t_day
        elif limb_type == 1:
            base_t = t_eve_strat
        elif limb_type == 2:
            base_t = t_mor_strat
        else:
            base_t = t_night

        p_eff = max(1.0e-5, p_bar)
        return float(base_t * ((p_eff / 0.1)**0.08))

    def evaluate_microphysics(self, p_bar: float,
                              limb_type: int) -> LimbMicrophysicsResult:
        """Compute aerosol cloud mass fraction, droplet size, and slant optical depths."""
        temp = self.limb_temperature(p_bar, limb_type)
        t_cond = self.silicate_condensation_temp(p_bar)
        sol_silicate = 4.0e-4 * (10.0**self.metallicity_dex)

        if temp < t_cond:
            supersat = (t_cond - temp) / t_cond
            cloud_frac = sol_silicate * (1.0 - np.exp(-3.0 * supersat))
            vap_frac = sol_silicate - cloud_frac
            r_eff = 0.5 * (max(1.0e-4, p_bar)**0.25)

            kappa_1 = 1.5 / (3.2e3 * r_eff * 1.0e-6)
            kappa_4 = kappa_1 * ((1.0 / 4.0)**1.5)

            rho_g = (p_bar * 1.0e5 * 2.3e-3) / (8.314 * temp)
            h_m = (8.314 * temp) / (2.3e-3 * self.surface_gravity_m_s2)
            slant_factor = np.sqrt(2.0 * np.pi * 7.0e7 / h_m)

            tau_1 = kappa_1 * (cloud_frac * rho_g) * h_m * slant_factor
            tau_4 = kappa_4 * (cloud_frac * rho_g) * h_m * slant_factor
        else:
            cloud_frac = 0.0
            vap_frac = sol_silicate
            r_eff = 0.0
            tau_1 = 0.0
            tau_4 = 0.0

        return LimbMicrophysicsResult(
            pressure_bar=float(p_bar),
            temperature_k=float(temp),
            silicate_vapor_abundance=float(vap_frac),
            cloud_condensate_mass_frac=float(cloud_frac),
            mean_droplet_radius_um=float(r_eff),
            optical_depth_slant_1um=float(tau_1),
            optical_depth_slant_4um=float(tau_4),
        )

    def compute_jwst_spectrum(
            self, num_points: int = 150) -> JWSTTransmissionSpectrum:
        """Compute synthetic JWST transmission spectra for morning, evening, and contrast."""
        wl = np.linspace(0.8, 5.0, num_points)
        base_depth = 15000.0
        h_scale = 180.0

        h2o = 0.8 * np.exp(-((wl - 1.4) / 0.15)**2) + 1.2 * np.exp(-(
            (wl - 1.9) / 0.20)**2) + 2.5 * np.exp(-((wl - 2.7) / 0.35)**2)
        co2 = 4.0 * np.exp(-((wl - 4.3) / 0.15)**2)
        co = 2.0 * np.exp(-((wl - 4.65) / 0.20)**2)
        gas_feat = h2o + co2 + co

        eve = base_depth + h_scale * (gas_feat + 0.3 * (wl / 1.0)**-1.0)
        mor = base_depth + h_scale * (0.8 + 0.15 * gas_feat + 0.8 *
                                      (wl / 1.0)**-0.4)
        sym = 0.5 * (eve + mor)
        contrast = eve - mor

        return JWSTTransmissionSpectrum(
            wavelength_um=wl,
            transit_depth_morning_ppm=mor,
            transit_depth_evening_ppm=eve,
            transit_depth_symmetric_ppm=sym,
            evening_morning_contrast_ppm=contrast,
        )
