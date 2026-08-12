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


class RelativisticPrecession:

    def gr_perihelion_precession_rad_s(self,
                                       m_star_kg=1.98847e30,
                                       a_m=5.790905e10,
                                       e=0.20563):
        g = 6.67430e-11
        c = 299792458.0
        n = np.sqrt(g * m_star_kg / a_m**3)
        return (3.0 * g * m_star_kg * n) / (c**2 * a_m *
                                            np.maximum(1.0e-5, 1.0 - e**2))

    def mercury_gr_precession_arcsec_century(self):
        rad_s = self.gr_perihelion_precession_rad_s()
        arcsec_per_rad = (180.0 * 3600.0) / np.pi
        seconds_per_century = 100.0 * 365.25 * 86400.0
        return rad_s * arcsec_per_rad * seconds_per_century


class PlanetNineSecular:

    def planet_nine_secular_precession_rad_yr(self,
                                              a_tno_au,
                                              a_p9_au=500.0,
                                              m_p9_earth=10.0):
        g = 6.67430e-11
        m_sun = 1.98847e30
        au = 1.495978707e11
        m_p9_kg = m_p9_earth * 5.972e24
        n_p9 = np.sqrt(g * m_sun / (a_p9_au * au)**3)
        alpha = a_tno_au / a_p9_au
        b_3_2 = 1.5 * alpha
        dvarpi_dt = (m_p9_kg / m_sun) * n_p9 * alpha * b_3_2
        return dvarpi_dt * (365.25 * 86400.0)


class LaplaceLagrangeSecular:

    def jupiter_secular_g5_arcsec_yr(self):
        return 4.257

    def saturn_secular_g6_arcsec_yr(self):
        return 28.245

    def jupiter_eccentricity_at_time_yr(self, time_yr):
        g5 = np.radians(4.257 / 3600.0)
        g6 = np.radians(28.245 / 3600.0)
        return 0.044 + 0.015 * np.cos((g6 - g5) * time_yr)


class NiceModelResonanceCrossing:

    def ice_giant_eccentricity_kick(self,
                                    delta_t_myr,
                                    m_planetesimal_belt_earth=35.0):
        kick_base = 0.12 * (m_planetesimal_belt_earth / 35.0)
        return kick_base * np.exp(-delta_t_myr / 10.0)


class SeasonalYarkovsky:

    def seasonal_drift_rate_au_myr(self, radius_m, density_kg_m3, a_au,
                                   obliquity_deg):
        au = 1.495978707e11
        m_sun = 1.98847e30
        g = 6.67430e-11
        mass = (4.0 / 3.0) * np.pi * radius_m**3 * density_kg_m3
        l_sun = 3.828e26
        c = 299792458.0
        a_m = a_au * au
        solar_flux = l_sun / (4.0 * np.pi * a_m**2)
        cross_section = np.pi * radius_m**2
        obl_rad = np.radians(obliquity_deg)
        sin_obl = np.sin(obl_rad)
        alpha_seasonal = 0.08
        force = -(4.0 /
                  9.0) * alpha_seasonal * cross_section * solar_flux / c * (
                      sin_obl**2)
        da_dt_m_s = (2.0 / (mass * np.sqrt(g * m_sun / a_m))) * force * a_m
        return da_dt_m_s * (1.0 / au) * (1.0e6 * 365.25 * 86400.0)


class SaturnRingLindbladResonance:

    def lindblad_resonance_torque_nm(self,
                                     m_satellite_kg,
                                     a_satellite_m,
                                     m_saturn_kg=5.683e26,
                                     surface_density_kg_m2=400.0):
        g = 6.67430e-11
        n = np.sqrt(g * m_saturn_kg / a_satellite_m**3)
        q = m_satellite_kg / m_saturn_kg
        return np.pi**2 * surface_density_kg_m2 * a_satellite_m**4 * n**2 * q**2


class EnceladusTidalOcean:

    def enceladus_tidal_power_gw(self, eccentricity=0.0047, k2_over_q=0.001):
        g = 6.67430e-11
        m_saturn = 5.683e26
        r_enc = 2.521e5
        a_enc = 2.38e8
        n = np.sqrt(g * m_saturn / a_enc**3)
        factor = 10.5 * k2_over_q * g * m_saturn**2 * r_enc**5 * n / a_enc**6
        power_watts = factor * eccentricity**2
        return power_watts / 1.0e9


class CetoPhorcysBinary:

    def orbital_period_days(self, a_orb_km=1840.0, m_sys_kg=5.41e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=5.41e18,
                                  r_ceto_km=87.0,
                                  r_phorcys_km=66.0):
        r_eq_m = ((r_ceto_km * 1000.0)**3 + (r_phorcys_km * 1000.0)**3)**(1.0 /
                                                                          3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class AltjiraBinary:

    def orbital_period_days(self, a_orb_km=9900.0, m_sys_kg=3.99e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self, m_sys_kg=3.99e18, r_eq_km=123.0):
        r_m = r_eq_km * 1000.0
        vol = (4.0 / 3.0) * np.pi * r_m**3
        return m_sys_kg / vol


class SilaNunamBinary:

    def orbital_period_days(self, a_orb_km=2777.0, m_sys_kg=1.08e19):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.08e19,
                                  r_sila_km=124.0,
                                  r_nunam_km=118.0):
        r_eq_m = ((r_sila_km * 1000.0)**3 + (r_nunam_km * 1000.0)**3)**(1.0 /
                                                                        3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class TeharonhiawakoBinary:

    def orbital_period_days(self, a_orb_km=27600.0, m_sys_kg=2.44e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=2.44e18,
                                  r_teh_km=89.0,
                                  r_saw_km=61.0):
        r_eq_m = ((r_teh_km * 1000.0)**3 + (r_saw_km * 1000.0)**3)**(1.0 / 3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class KS38Binary:

    def orbital_period_days(self, a_orb_km=15400.0, m_sys_kg=1.43e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.43e18,
                                  r_primary_km=82.0,
                                  r_sec_km=71.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class OJ67Binary:

    def orbital_period_days(self, a_orb_km=11700.0, m_sys_kg=9.20e17):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=9.20e17,
                                  r_primary_km=69.0,
                                  r_sec_km=54.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class EG138Binary:

    def orbital_period_days(self, a_orb_km=14300.0, m_sys_kg=2.25e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=2.25e18,
                                  r_primary_km=94.0,
                                  r_sec_km=72.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class YN81Binary:

    def orbital_period_days(self, a_orb_km=12800.0, m_sys_kg=1.12e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.12e18,
                                  r_primary_km=72.0,
                                  r_sec_km=59.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class WC19Binary:

    def orbital_period_days(self, a_orb_km=4090.0, m_sys_kg=7.68e19):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=7.68e19,
                                  r_primary_km=300.0,
                                  r_sec_km=120.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class KP76Binary:

    def orbital_period_days(self, a_orb_km=8900.0, m_sys_kg=1.23e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.23e18,
                                  r_primary_km=77.0,
                                  r_sec_km=56.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class FB128Binary:

    def orbital_period_days(self, a_orb_km=37500.0, m_sys_kg=1.52e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.52e18,
                                  r_primary_km=80.0,
                                  r_sec_km=60.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


__all__ = [
    "AltjiraBinary",
    "AsteroidDynamics",
    "CetoPhorcysBinary",
    "CometDynamics",
    "EG138Binary",
    "EnceladusTidalOcean",
    "FB128Binary",
    "KP76Binary",
    "KS38Binary",
    "LaplaceLagrangeSecular",
    "MoonTidalDynamics",
    "NiceModelResonanceCrossing",
    "OJ67Binary",
    "PlanetNineSecular",
    "PlanetaryRings",
    "RelativisticPrecession",
    "SaturnRingLindbladResonance",
    "SeasonalYarkovsky",
    "SilaNunamBinary",
    "TeharonhiawakoBinary",
    "WC19Binary",
    "YN81Binary",
]
