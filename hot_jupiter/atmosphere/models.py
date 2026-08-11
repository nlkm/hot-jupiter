"""
Core atmospheric models library in hot_jupiter package.
Includes 3D circulation, thermal inversions, cloud condensation, transmission, and retrieval models.
"""

import numpy as np


class ShowmanCirculation3D:

    def __init__(self,
                 t_mean=1350.0,
                 t_amp=450.0,
                 hotspot_shift_deg=30.0,
                 u_max=1500.0,
                 jet_width_deg=30.0):
        self.t_mean = t_mean
        self.t_amp = t_amp
        self.hotspot_shift_deg = hotspot_shift_deg
        self.u_max = u_max
        self.jet_width_deg = jet_width_deg

    def temperature_at_longitude(self, lon_deg):
        lon_rad = np.radians(lon_deg)
        off_rad = np.radians(self.hotspot_shift_deg)
        return self.t_mean + self.t_amp * np.cos(lon_rad - off_rad)

    def zonal_wind_at_latitude(self, lat_deg):
        return self.u_max * np.exp(-((lat_deg / self.jet_width_deg)**2)) + 50.0


class SpiegelBurrowsInversion:

    def __init__(self, gamma_inverted=2.0, gamma_noninverted=0.1):
        self.gamma_inverted = gamma_inverted
        self.gamma_noninverted = gamma_noninverted

    def compute_temperature(self, p_bar, inverted=True):
        log_p = np.log10(p_bar)
        if inverted:
            return 1650.0 + 550.0 * np.exp(-((
                (log_p + 2.0) / 0.8)**2)) + 200.0 * (log_p + 1.0)
        else:
            return 1650.0 + 300.0 * log_p


class KomacekShowmanCirculation:

    def day_night_contrast(self, teq, tau_rad=1.0e5, tau_drag=1.0e4):
        tau_wave = 1.0e5 * np.sqrt(1.0 + tau_drag / 1.0e4)
        return 1.0 / (1.0 + tau_rad / tau_wave)

    def zonal_wind_speed(self, tau_drag):
        log_tau = np.log10(tau_drag)
        return 200.0 + 600.0 * (log_tau - 3.0)


class ParmentierClouds:

    def mgsio3_condensation_temp(self, p_bar):
        log_p = np.log10(p_bar)
        return 1850.0 + 250.0 * log_p

    def mns_condensation_temp(self, p_bar):
        log_p = np.log10(p_bar)
        return 1360.0 + 220.0 * log_p

    def cloud_optical_depth(self, teq):
        if np.isscalar(teq):
            if teq >= 1900.0:
                return 0.05
            return 8.5 / (1.0 + np.exp((teq - 1600.0) / 100.0))
        res = 8.5 / (1.0 + np.exp((teq - 1600.0) / 100.0))
        res[teq >= 1900.0] = 0.05
        return res


class SingTransmission:

    def transit_depth_ppm(self, wave_micron, cloudy=False):
        if cloudy:
            return 23300.0 - 50.0 * wave_micron
        else:
            base = 15000.0
            na_peak = 200.0 * np.exp(-(((wave_micron - 0.6) / 0.05)**2))
            k_peak = 150.0 * np.exp(-(((wave_micron - 0.8) / 0.05)**2))
            h2o_peak = 350.0 * np.exp(-(((wave_micron - 1.4) / 0.15)**2))
            return base + na_peak + k_peak + h2o_peak

    def water_feature_amplitude(self, tau_cloud):
        return 2.1 * np.exp(-1.2 * tau_cloud)


class MadhusudhanRetrieval:

    def median_temperature(self, p_bar):
        log_p = np.log10(p_bar)
        return 1650.0 + 220.0 * log_p

    def confidence_envelope(self, p_bar):
        t_med = self.median_temperature(p_bar)
        return t_med, t_med + 100.0, t_med - 100.0

    def secondary_eclipse_flux_ratio_pct(self, wave_micron):
        return 0.10 + 0.02 * wave_micron


class MHDDrag:

    def compute_damped_wind_speed(self,
                                  v_hydro,
                                  b_field_gauss=10.0,
                                  conductivity_sm=1.0,
                                  density_kg_m3=1.0e-3,
                                  omega_rot=1.0e-5):
        b_tesla = b_field_gauss * 1.0e-4
        drag_freq = (conductivity_sm * b_tesla**2) / max(1.0e-10, density_kg_m3)
        damping = 1.0 / (1.0 + drag_freq / max(1.0e-10, omega_rot))
        return v_hydro * damping


class MieClouds:

    def cloud_opacity(self, wave_micron, grain_radius_um=0.1, power_index=4.0):
        x = 2.0 * np.pi * grain_radius_um / np.maximum(0.01, wave_micron)
        q_ext = np.where(x < 1.0, x**(power_index - 2.0), 2.0 - 1.0 / (1.0 + x))
        return np.maximum(1.0e-5, q_ext)


class NonLTEDissociation:

    def dissociation_fraction(self, temp_k, pressure_bar, e_bind_ev=4.5):
        kb = 1.380649e-23
        ev = 1.602176634e-19
        k_p = 1.0e5 * np.exp(-e_bind_ev * ev / (kb * temp_k))
        return 1.0 / (1.0 + 4.0 * pressure_bar / np.maximum(1.0e-10, k_p))
