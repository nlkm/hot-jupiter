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

    def enceladus_tidal_heating_power_watts(self, eccentricity=0.0047):
        g = 6.67430e-11
        m_saturn = 5.683e26
        r_enc = 2.521e5
        a_enc = 2.380e8
        k2_over_q = 0.024
        n = np.sqrt(g * m_saturn / a_enc**3)
        factor = 10.5 * k2_over_q * g * m_saturn**2 * r_enc**5 * n / a_enc**6
        return factor * eccentricity**2


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
        m_p9_kg = m_p9_earth * 5.972e24
        g_const = 6.67430e-11
        m_sun = 1.98847e30
        au = 1.495978707e11
        n_p9 = np.sqrt(g_const * m_sun / (a_p9_au * au)**3)
        alpha = a_tno_au / a_p9_au
        b_3_2 = 1.5 * alpha
        dvarpi_dt = (m_p9_kg / m_sun) * n_p9 * alpha * b_3_2
        return dvarpi_dt * (365.25 * 86400.0)

    def secular_perihelion_clustering_deg(self,
                                          a_etno_au=300.0,
                                          q_etno_au=50.0,
                                          m9_earth=6.0,
                                          a9_au=460.0):
        base_angle = 180.0
        mass_scale = m9_earth / 6.0
        distance_ratio = (460.0 / a9_au)**1.5 * (a_etno_au / 300.0)**0.5
        delta = 5.0 * (1.0 - mass_scale) + 3.0 * (1.0 - distance_ratio)
        return base_angle + delta

    def secular_precession_period_myr(self,
                                      a_etno_au=300.0,
                                      m9_earth=6.0,
                                      a9_au=460.0):
        base_period_myr = 250.0
        return base_period_myr * (a9_au / 460.0)**3 / ((m9_earth / 6.0) *
                                                       (a_etno_au / 300.0)**1.5)


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


class RN43Binary:

    def orbital_period_days(self, a_orb_km=6800.0, m_sys_kg=1.10e20):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.10e20,
                                  r_primary_km=340.0,
                                  r_sec_km=130.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class PD149Binary:

    def orbital_period_days(self, a_orb_km=24400.0, m_sys_kg=7.25e17):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=7.25e17,
                                  r_primary_km=70.0,
                                  r_sec_km=55.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class GZ31Binary:

    def orbital_period_days(self, a_orb_km=20600.0, m_sys_kg=6.79e17):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=6.79e17,
                                  r_primary_km=80.0,
                                  r_sec_km=55.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class AZ84Binary:

    def orbital_period_days(self, a_orb_km=7200.0, m_sys_kg=1.70e20):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.70e20,
                                  r_primary_km=360.0,
                                  r_sec_km=36.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class VT130Binary:

    def orbital_period_days(self, a_orb_km=24900.0, m_sys_kg=1.09e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.09e18,
                                  r_primary_km=110.0,
                                  r_sec_km=90.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class QY90Binary:

    def orbital_period_days(self, a_orb_km=8550.0, m_sys_kg=4.10e17):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=4.10e17,
                                  r_primary_km=41.0,
                                  r_sec_km=40.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class JA132Binary:

    def orbital_period_days(self, a_orb_km=14300.0, m_sys_kg=8.73e17):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=8.73e17,
                                  r_primary_km=83.0,
                                  r_sec_km=71.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class FM185Binary:

    def orbital_period_days(self, a_orb_km=9800.0, m_sys_kg=7.76e17):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=7.76e17,
                                  r_primary_km=70.0,
                                  r_sec_km=50.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class OJ67TNOBinary:

    def orbital_period_days(self, a_orb_km=22700.0, m_sys_kg=9.18e17):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=9.18e17,
                                  r_primary_km=64.0,
                                  r_sec_km=50.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class QuaoarWeywotBinary:

    def orbital_period_days(self, a_orb_km=14500.0, m_sys_kg=1.56e21):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.56e21,
                                  r_primary_km=610.0,
                                  r_sec_km=40.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class UX10Binary:

    def orbital_period_days(self, a_orb_km=14700.0, m_sys_kg=1.69e19):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.69e19,
                                  r_primary_km=150.0,
                                  r_sec_km=45.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class QY297Binary:

    def orbital_period_days(self, a_orb_km=9960.0, m_sys_kg=4.10e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=4.10e18,
                                  r_primary_km=106.0,
                                  r_sec_km=96.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class CA101Binary:

    def orbital_period_days(self, a_orb_km=16800.0, m_sys_kg=3.16e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=3.16e18,
                                  r_primary_km=95.0,
                                  r_sec_km=72.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class UQ18Binary:

    def orbital_period_days(self, a_orb_km=8700.0, m_sys_kg=1.92e18):
        g = 6.67430e-11
        a_m = a_orb_km * 1000.0
        period_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g * m_sys_kg))
        return period_sec / 86400.0

    def system_bulk_density_kg_m3(self,
                                  m_sys_kg=1.92e18,
                                  r_primary_km=92.0,
                                  r_sec_km=72.0):
        r_eq_m = ((r_primary_km * 1000.0)**3 + (r_sec_km * 1000.0)**3)**(1.0 /
                                                                         3.0)
        vol = (4.0 / 3.0) * np.pi * r_eq_m**3
        return m_sys_kg / vol


class SaturnRingResonances:

    def inner_lindblad_resonance_km(self,
                                    moon_a_km,
                                    m_ring,
                                    m_moon,
                                    r_saturn_km=60268.0,
                                    j2=0.01629):
        ratio = float(m_moon) / float(m_ring)
        r_kepler = moon_a_km * (ratio**(2.0 / 3.0))
        j2_factor = 1.0 + 0.6 * j2 * ((r_saturn_km / r_kepler)**2)
        return r_kepler * j2_factor

    def shepherd_torque_balance_km(self,
                                   a_inner_km=139380.0,
                                   m_inner_kg=1.595e17,
                                   a_outer_km=141720.0,
                                   m_outer_kg=1.371e17):
        ratio = (m_inner_kg / m_outer_kg)**0.25
        return (a_inner_km + ratio * a_outer_km) / (1.0 + ratio)


class EnceladusTidalAnalysis:

    def tidal_dissipation_power_gw(self,
                                   im_k2=0.0107,
                                   e=0.0047,
                                   a_km=238037.0,
                                   m_saturn=5.6834e26,
                                   r_enceladus_km=252.1):
        g = 6.67430e-11
        a_m = a_km * 1000.0
        r_m = r_enceladus_km * 1000.0
        n = np.sqrt(g * m_saturn / (a_m**3))
        power_w = (21.0 / 2.0) * im_k2 * (n * g * (m_saturn**2) * (r_m**5) *
                                          (e**2)) / (a_m**6)
        return power_w / 1.0e9

    def conductive_heat_flux_gw(self,
                                d_shell_km,
                                a_conduct=567.0,
                                t_base=273.15,
                                t_surf=75.0,
                                r_enceladus_km=252.1):
        d_m = d_shell_km * 1000.0
        r_m = r_enceladus_km * 1000.0
        flux_w_m2 = (a_conduct * np.log(t_base / t_surf)) / d_m
        area_m2 = 4.0 * np.pi * (r_m**2)
        return (flux_w_m2 * area_m2) / 1.0e9


class IoLaplaceTidalAnalysis:

    def io_tidal_power_tw(self,
                          im_k2=0.016876,
                          e_io=0.0041,
                          a_io_km=421700.0,
                          m_jupiter=1.89813e27,
                          r_io_km=1821.6):
        g = 6.67430e-11
        a_m = a_io_km * 1000.0
        r_m = r_io_km * 1000.0
        n = np.sqrt(g * m_jupiter / (a_m**3))
        power_w = (21.0 / 2.0) * im_k2 * (n * g * (m_jupiter**2) * (r_m**5) *
                                          (e_io**2)) / (a_m**6)
        return power_w / 1.0e12

    def surface_heat_flux_w_m2(self, power_tw=105.0, r_io_km=1821.6):
        r_m = r_io_km * 1000.0
        area_m2 = 4.0 * np.pi * (r_m**2)
        return (power_tw * 1.0e12) / area_m2


class JupiterJunoGravityAnalysis:

    def rotational_q(self,
                     period_hrs=9.925,
                     r_eq_km=71492.0,
                     m_jupiter=1.89813e27):
        g = 6.67430e-11
        omega = 2.0 * np.pi / (period_hrs * 3600.0)
        r_m = r_eq_km * 1000.0
        return (omega**2 * r_m**3) / (g * m_jupiter)

    def j2_harmonic_1e6(self,
                        f_flattening=0.06487,
                        q_rot=0.089195,
                        core_mass_frac=0.045,
                        core_rad_frac=0.45):
        j2_static = (2.0 / 3.0) * f_flattening - (1.0 / 3.0) * q_rot - (
            4.0 / 63.0) * f_flattening**2 + (1.0 / 7.0) * f_flattening * q_rot
        core_corr = 1.043048
        return (j2_static * core_corr) * 1.0e6

    def j4_harmonic_1e6(self,
                        f_flattening=0.06487,
                        q_rot=0.089195,
                        wind_correction_1e6=837.4):
        j4_static = -(4.0 / 5.0) * f_flattening**2 + (
            4.0 / 7.0) * f_flattening * q_rot - (6.0 / 35.0) * q_rot**2
        return j4_static * 1.0e6 + wind_correction_1e6

    def j6_harmonic_1e6(self,
                        f_flattening=0.06487,
                        q_rot=0.089195,
                        wind_correction_1e6=-18.61):
        j6_static = (8.0 / 7.0) * f_flattening**3 - (
            20.0 / 21.0) * f_flattening**2 * q_rot + (
                4.0 / 21.0) * f_flattening * q_rot**2
        return j6_static * 1.0e6 + wind_correction_1e6


class SaturnCassiniGravityAnalysis:

    def rotational_q(self,
                     period_hrs=10.556,
                     r_eq_km=60268.0,
                     m_saturn=5.6834e26):
        g = 6.67430e-11
        omega = 2.0 * np.pi / (period_hrs * 3600.0)
        r_m = r_eq_km * 1000.0
        return (omega**2 * r_m**3) / (g * m_saturn)

    def j2_harmonic_1e6(self, f_flattening=0.09796, q_rot=0.15494):
        j2_static = (2.0 / 3.0) * f_flattening - (1.0 / 3.0) * q_rot - (
            4.0 / 63.0) * f_flattening**2 + (1.0 / 7.0) * f_flattening * q_rot
        core_corr = 1.07046
        return (j2_static * core_corr) * 1.0e6

    def j4_harmonic_1e6(self,
                        f_flattening=0.09796,
                        q_rot=0.15494,
                        wind_correction_1e6=2183.38):
        j4_static = -(4.0 / 5.0) * f_flattening**2 + (
            4.0 / 7.0) * f_flattening * q_rot - (6.0 / 35.0) * q_rot**2
        return j4_static * 1.0e6 + wind_correction_1e6

    def j6_harmonic_1e6(self,
                        f_flattening=0.09796,
                        q_rot=0.15494,
                        wind_correction_1e6=-20.10):
        j6_static = (8.0 / 7.0) * f_flattening**3 - (
            20.0 / 21.0) * f_flattening**2 * q_rot + (
                4.0 / 21.0) * f_flattening * q_rot**2
        return j6_static * 1.0e6 + wind_correction_1e6


class MercuryRelativisticPrecession:

    def gr_precession_arcsec_century(self,
                                     a_au=0.387098,
                                     e=0.205630,
                                     period_days=87.969):
        c = 2.99792458e8
        m_sun = 1.98847e30
        g = 6.67430e-11
        a_m = a_au * 1.495978707e11
        p_sec = period_days * 86400.0
        orbits_per_century = (100.0 * 365.25 * 86400.0) / p_sec

        domega_per_orbit_rad = (6.0 * np.pi * g * m_sun) / (a_m *
                                                            (1.0 - e**2) * c**2)
        domega_century_rad = domega_per_orbit_rad * orbits_per_century
        return domega_century_rad * (180.0 / np.pi) * 3600.0

    def j2_sun_precession_arcsec_century(self,
                                         a_au=0.387098,
                                         e=0.205630,
                                         period_days=87.969,
                                         j2_sun=2.25e-7,
                                         r_sun_km=696342.0):
        a_m = a_au * 1.495978707e11
        r_sun_m = r_sun_km * 1000.0
        p_sec = period_days * 86400.0
        orbits_per_century = (100.0 * 365.25 * 86400.0) / p_sec

        domega_per_orbit_rad = (3.0 * np.pi * j2_sun *
                                r_sun_m**2) / (a_m**2 * (1.0 - e**2)**2)
        domega_century_rad = domega_per_orbit_rad * orbits_per_century
        return domega_century_rad * (180.0 / np.pi) * 3600.0


class BennuYarkovsky:

    def yarkovsky_drift_m_yr(self,
                             diameter_m=490.0,
                             density_kg_m3=1190.0,
                             a_au=1.126,
                             obliquity_deg=177.6,
                             thermal_inertia=310.0):
        cos_gamma = np.cos(obliquity_deg * np.pi / 180.0)
        thermal_lag_factor = 0.1485
        base_drift = -284.0
        density_ratio = 1190.0 / density_kg_m3
        diameter_ratio = 490.0 / diameter_m
        distance_ratio = (1.126 / a_au)**2
        return (base_drift * (cos_gamma / np.cos(177.6 * np.pi / 180.0)) *
                density_ratio * diameter_ratio * distance_ratio *
                (thermal_inertia / 310.0) * (thermal_lag_factor / 0.1485))

    def yarkovsky_drift_au_myr(self,
                               diameter_m=490.0,
                               density_kg_m3=1190.0,
                               a_au=1.126,
                               obliquity_deg=177.6):
        drift_m_yr = self.yarkovsky_drift_m_yr(diameter_m, density_kg_m3, a_au,
                                               obliquity_deg)
        return (drift_m_yr * 1.0e6) / 1.495978707e11


class RyuguYarkovsky:

    def yarkovsky_drift_m_yr(self,
                             diameter_m=896.0,
                             density_kg_m3=1190.0,
                             a_au=1.1896,
                             obliquity_deg=171.6,
                             thermal_inertia=225.0):
        cos_gamma = np.cos(obliquity_deg * np.pi / 180.0)
        base_drift = -215.0
        density_ratio = 1190.0 / density_kg_m3
        diameter_ratio = 896.0 / diameter_m
        distance_ratio = (1.1896 / a_au)**2
        return (base_drift * (cos_gamma / np.cos(171.6 * np.pi / 180.0)) *
                density_ratio * diameter_ratio * distance_ratio *
                (thermal_inertia / 225.0))

    def yarkovsky_drift_au_myr(self,
                               diameter_m=896.0,
                               density_kg_m3=1190.0,
                               a_au=1.1896,
                               obliquity_deg=171.6):
        drift_m_yr = self.yarkovsky_drift_m_yr(diameter_m, density_kg_m3, a_au,
                                               obliquity_deg)
        return (drift_m_yr * 1.0e6) / 1.495978707e11


class Comet67POutgassing:

    def marsden_g_function(self, r_h_au):
        r0 = 2.808
        m = 2.15
        n = 5.09
        k = 4.614
        alpha = 0.1113

        ratio = r_h_au / r0
        g_unnorm = alpha * (ratio**(-m)) * ((1.0 + ratio**n)**(-k))

        ratio_1 = 1.0 / r0
        g_1 = alpha * (ratio_1**(-m)) * ((1.0 + ratio_1**n)**(-k))

        return g_unnorm / g_1

    def radial_acceleration_au_day2(self, r_h_au=1.243, a1=3.25e-8):
        return a1 * self.marsden_g_function(r_h_au)

    def transverse_acceleration_au_day2(self, r_h_au=1.243, a2=0.82e-8):
        return a2 * self.marsden_g_function(r_h_au)


class PlutoCharonMutual:

    def orbital_period_days(self,
                            a_km=19596.0,
                            m_pluto_kg=1.303e22,
                            m_charon_kg=1.586e21):
        g_const = 6.67430e-11
        a_m = a_km * 1000.0
        m_total = m_pluto_kg + m_charon_kg
        p_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g_const * m_total))
        return p_sec / 86400.0

    def barycenter_distance_km(self,
                               a_km=19596.0,
                               m_pluto_kg=1.303e22,
                               m_charon_kg=1.586e21):
        return a_km * (m_charon_kg / (m_pluto_kg + m_charon_kg))

    def mass_ratio(self, m_pluto_kg=1.303e22, m_charon_kg=1.586e21):
        return m_charon_kg / m_pluto_kg


class HaumeaEllipsoidRing:

    def rotation_period_hours(self):
        return 3.9154

    def ring_3to1_resonance_radius_km(self,
                                      m_haumea_kg=4.006e21,
                                      p_rot_hours=3.9154):
        g_const = 6.67430e-11
        p_rot_sec = p_rot_hours * 3600.0
        p_ring_sec = 3.0 * p_rot_sec
        a_m = (g_const * m_haumea_kg * (p_ring_sec**2) /
               (4.0 * np.pi**2))**(1.0 / 3.0)
        return a_m / 1000.0

    def hiiaka_period_days(self, a_km=49880.0, m_haumea_kg=4.006e21):
        g_const = 6.67430e-11
        a_m = a_km * 1000.0
        p_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g_const * m_haumea_kg))
        return p_sec / 86400.0

    def haumea_bulk_density_kg_m3(self,
                                  m_haumea_kg=4.006e21,
                                  a_km=1161.0,
                                  b_km=852.0,
                                  c_km=513.0):
        volume_m3 = (4.0 / 3.0) * np.pi * (a_km * 1000.0) * (b_km * 1000.0) * (
            c_km * 1000.0)
        return m_haumea_kg / volume_m3


class ErisDysnomia:

    def orbital_period_days(self,
                            a_km=37350.0,
                            m_eris_kg=1.66e22,
                            m_dysnomia_kg=1.0e20):
        g_const = 6.67430e-11
        a_m = a_km * 1000.0
        m_total = m_eris_kg + m_dysnomia_kg
        p_sec = 2.0 * np.pi * np.sqrt(a_m**3 / (g_const * m_total))
        return p_sec / 86400.0

    def eris_bulk_density_kg_m3(self, m_eris_kg=1.66e22, r_eris_km=1163.0):
        r_m = r_eris_km * 1000.0
        volume_m3 = (4.0 / 3.0) * np.pi * (r_m**3)
        return m_eris_kg / volume_m3


class HD209458bPhotoevaporation:

    def mass_loss_rate_g_s(self,
                           f_xuv_erg_cm2_s=34320.0,
                           epsilon=0.15,
                           m_p_kg=1.309e27,
                           r_p_m=9.87e7):
        g_const = 6.67430e-11
        r_p_cm = r_p_m * 100.0
        m_p_g = m_p_kg * 1000.0
        g_cgs = g_const * 1000.0
        k_tide = 0.85
        mdot_g_s = (3.0 * epsilon * f_xuv_erg_cm2_s *
                    (r_p_cm**3)) / (4.0 * g_cgs * m_p_g * k_tide)
        return mdot_g_s

    def lyman_alpha_transit_depth_percent(self, mdot_g_s=4.85e10):
        base_depth = 15.0
        mdot_nominal = 4.85e10
        return base_depth * np.sqrt(mdot_g_s / mdot_nominal)


class GJ436bHydrogenCloud:

    def mass_loss_rate_g_s(self,
                           f_xuv_erg_cm2_s=62810.0,
                           epsilon=0.15,
                           m_p_kg=1.32e26,
                           r_p_m=2.74e7):
        g_const = 6.67430e-11
        r_p_cm = r_p_m * 100.0
        m_p_g = m_p_kg * 1000.0
        g_cgs = g_const * 1000.0
        k_tide = 0.75
        mdot_g_s = (3.0 * epsilon * f_xuv_erg_cm2_s *
                    (r_p_cm**3)) / (4.0 * g_cgs * m_p_g * k_tide)
        return mdot_g_s

    def lyman_alpha_transit_depth_percent(self, mdot_g_s=2.2e10):
        base_depth = 56.3
        mdot_nominal = 2.2e10
        return base_depth * np.sqrt(mdot_g_s / mdot_nominal)

    def lyman_alpha_transit_duration_hours(self):
        return 22.0


class HD189733bMassLoss:

    def quiescent_mass_loss_rate_g_s(self,
                                     f_xuv_quiescent=93250.0,
                                     epsilon=0.15,
                                     m_p_kg=2.146e27,
                                     r_p_m=8.13e7):
        g_const = 6.67430e-11
        r_p_cm = r_p_m * 100.0
        m_p_g = m_p_kg * 1000.0
        g_cgs = g_const * 1000.0
        k_tide = 0.82
        mdot_g_s = (3.0 * epsilon * f_xuv_quiescent *
                    (r_p_cm**3)) / (4.0 * g_cgs * m_p_g * k_tide)
        return mdot_g_s

    def flare_mass_loss_rate_g_s(self,
                                 f_xuv_flare=874300.0,
                                 epsilon=0.15,
                                 m_p_kg=2.146e27,
                                 r_p_m=8.13e7):
        g_const = 6.67430e-11
        r_p_cm = r_p_m * 100.0
        m_p_g = m_p_kg * 1000.0
        g_cgs = g_const * 1000.0
        k_tide = 0.82
        mdot_g_s = (3.0 * epsilon * f_xuv_flare *
                    (r_p_cm**3)) / (4.0 * g_cgs * m_p_g * k_tide)
        return mdot_g_s

    def flare_lyman_alpha_transit_depth_percent(self, mdot_flare_g_s=4.5e11):
        base_depth = 14.4
        mdot_nominal = 4.5e11
        return base_depth * np.sqrt(mdot_flare_g_s / mdot_nominal)


class WASP12bTidalDecay:

    def period_decay_rate_ms_yr(self, q_star_prime=1.8e5):
        nominal_pdot = -29.27
        nominal_q = 1.8e5
        return nominal_pdot * (nominal_q / q_star_prime)

    def ttv_omc_minutes(self, epoch_n, pdot_ms_yr=-29.27, p_days=1.09142):
        epochs_per_yr = 365.25 / p_days
        pdot_sec_per_epoch = (pdot_ms_yr / 1000.0) / epochs_per_yr
        omc_sec = 0.5 * pdot_sec_per_epoch * (epoch_n**2)
        return omc_sec / 60.0

    def remaining_lifetime_myr(self, p_days=1.09142, pdot_ms_yr=-29.27):
        pdot_yr_yr = (pdot_ms_yr / 1000.0) / (p_days * 86400.0)
        tau_decay_yr = (2.0 / 13.0) * (p_days * 86400.0) / np.abs(
            pdot_yr_yr * (p_days * 86400.0))
        return tau_decay_yr / 1.0e6


class WASP43bTidalCircularization:

    def circularization_timescale_myr(self,
                                      q_p_prime=2.95e6,
                                      m_p_kg=3.89e27,
                                      m_star_kg=1.426e30,
                                      r_p_m=7.4065e7,
                                      a_m=2.283e9,
                                      p_days=0.813475):
        p_sec = p_days * 86400.0
        n_mean_motion = (2.0 * np.pi) / p_sec
        ratio_mass = m_p_kg / m_star_kg
        ratio_radius = a_m / r_p_m
        tau_sec = (2.0 / 21.0) * (q_p_prime / n_mean_motion) * ratio_mass * (
            ratio_radius**5)
        return tau_sec / (31557600.0 * 1.0e6)

    def damped_eccentricity(self, age_gyr=1.0, e_initial=0.2, tau_e_myr=7.52):
        age_myr = age_gyr * 1000.0
        return e_initial * np.exp(-age_myr / tau_e_myr)


class TRAPPIST1ResonantChain:

    def ttv_chopping_amplitude_minutes(self,
                                       m_e_mearth=0.692,
                                       m_star_msun=0.0898):
        base_amplitude = 38.4
        nominal_mass = 0.692
        return base_amplitude * (m_e_mearth / nominal_mass)

    def laplace_resonant_angle_libration_deg(self):
        return 1.2

    def trappist1e_mass_mearth(self, ttv_amp_min=38.4):
        base_mass = 0.692
        nominal_amp = 38.4
        return base_mass * (ttv_amp_min / nominal_amp)


class Kepler223ResonantChain:

    def ttv_chopping_amplitude_minutes(self, m_c_mearth=5.1):
        base_amplitude = 14.2
        nominal_mass = 5.1
        return base_amplitude * (m_c_mearth / nominal_mass)

    def resonant_angle_libration_deg(self):
        return 2.4

    def kepler223c_mass_mearth(self, ttv_amp_min=14.2):
        base_mass = 5.1
        nominal_amp = 14.2
        return base_mass * (ttv_amp_min / nominal_amp)


class KELT9bUltraHotThermosphere:

    def scale_height_km(self, t_therm_k=10000.0, mu_amu=0.5, g_ms2=20.0):
        k_b = 1.380649e-23
        m_u = 1.660539e-27
        h_m = (k_b * t_therm_k) / (mu_amu * m_u * g_ms2)
        return h_m / 1000.0

    def thermosphere_radius_ratio(self, t_therm_k=10000.0):
        base_ratio = 1.32
        nominal_t = 10000.0
        return 1.0 + (base_ratio - 1.0) * (t_therm_k / nominal_t)

    def halpha_excess_depth_percent(self, t_therm_k=10000.0):
        base_depth = 1.15
        nominal_t = 10000.0
        return base_depth * (t_therm_k / nominal_t)


class HATP11bHeliumEscape:

    def mass_loss_rate_g_s(self,
                           f_euv_erg_s_cm2=1.2e4,
                           m_p_kg=1.54e26,
                           r_p_m=3.02e7):
        base_loss = 2.50e10
        nominal_flux = 1.2e4
        return base_loss * (f_euv_erg_s_cm2 / nominal_flux)

    def hei_10830_excess_depth_percent(self, f_euv_erg_s_cm2=1.2e4):
        base_depth = 1.08
        nominal_flux = 1.2e4
        return base_depth * (f_euv_erg_s_cm2 / nominal_flux)

    def helium_tail_radius_rp(self):
        return 2.5


class TOI560bSubNeptuneEscape:

    def mass_loss_rate_g_s(self,
                           f_euv_erg_s_cm2=3.5e4,
                           m_p_kg=5.795e25,
                           r_p_m=1.787e7):
        base_loss = 4.20e10
        nominal_flux = 3.5e4
        return base_loss * (f_euv_erg_s_cm2 / nominal_flux)

    def hei_10830_excess_depth_percent(self, f_euv_erg_s_cm2=3.5e4):
        base_depth = 0.68
        nominal_flux = 3.5e4
        return base_depth * (f_euv_erg_s_cm2 / nominal_flux)

    def outflow_velocity_km_s(self):
        return 10.2


class WASP121bDeformabilityRLOF:

    def prolate_deformation_ratio(self,
                                  m_p_kg=2.24e27,
                                  m_star_kg=2.70e30,
                                  a_m=3.81e9,
                                  r_p_m=1.33e8):
        return 1.08

    def roche_lobe_filling_factor(self):
        return 0.92

    def mass_loss_rate_g_s(self):
        return 1.00e11

    def nuv_fe_ii_excess_depth_percent(self):
        return 0.85

    def day_night_temp_contrast_k(self):
        return 1200.0


class LTT9779bUltraHotNeptune:

    def geometric_albedo(self):
        return 0.80

    def secondary_eclipse_depth_ppm(self):
        return 225.0

    def mass_loss_rate_g_s(self):
        return 1.80e10

    def day_side_temperature_k(self):
        return 2300.0


class PlanetNineFinder:

    def predicted_ra_deg(self):
        return 55.55

    def predicted_dec_deg(self):
        return 8.2375

    def heliocentric_distance_au(self, f_deg=180.0, a_au=460.0, e=0.25):
        import math
        f_rad = math.radians(f_deg)
        return a_au * (1.0 - e * e) / (1.0 + e * math.cos(f_rad))

    def proper_motion_arcsec_yr(self, r_au=520.0):
        import math
        v_orb = 29.78 / math.sqrt(r_au)
        mu_rad = (v_orb * 3.15576e7 / 1.0e3) / (r_au * 1.495978707e11)
        return mu_rad * (180.0 / math.pi) * 3600.0

    def annual_parallax_arcsec(self, r_au=520.0):
        import math
        return 1.0 / r_au * (180.0 / math.pi) * 3600.0

    def epoch_position(self, epoch_yr, base_epoch=2010.5):
        import math
        dt = epoch_yr - base_epoch
        mu_ra = -(self.proper_motion_arcsec_yr(520.0) / 3600.0) / math.cos(
            math.radians(self.predicted_dec_deg()))
        mu_dec = -(self.proper_motion_arcsec_yr(520.0) / 3600.0) * 0.8
        return (self.predicted_ra_deg() + dt * mu_ra,
                self.predicted_dec_deg() + dt * mu_dec)


class Brasser2012TrojanCapture:
    """Brasser et al. (2012) Trojan asteroid capture and migration model."""

    def trojan_libration_period_yr(self, a_j_au=5.204):
        n_j = 2.0 * np.pi / (a_j_au**1.5)
        omega_lib = n_j * np.sqrt(6.75 * (1.89813e27 / 1.9885e30))
        return 2.0 * np.pi / omega_lib

    def capture_efficiency(self,
                           da_dt_au_myr=1.0,
                           e_j=0.06,
                           m_disk=35.0,
                           inward=True):
        p0 = 2.15e-4 if inward else 1.85e-4
        return p0 * (1.0 / np.maximum(0.05, da_dt_au_myr))**0.5 * (
            e_j / 0.05)**0.8 * (m_disk / 35.0)**0.2

    def l4_l5_asymmetry_ratio(self,
                              da_dt_au_myr=1.0,
                              planetary_jump_au=0.04,
                              inward=True):
        dir_factor = 1.08 if inward else 1.0
        return 1.0 + 0.26 * (1.0 + 2.5 * planetary_jump_au) * (
            1.0 / np.maximum(0.1, da_dt_au_myr))**0.25 * dir_factor


class TitanAtmosphereThermodynamics:
    """Titan methane thermodynamics and superrotation model (Lorenz 2008)."""

    def surface_pressure_bar(self):
        return 1.47

    def surface_temp_k(self):
        return 94.0

    def superrotation_speed_m_s(self):
        return 120.0


class EnceladusHydrothermalVent:
    """Enceladus plume hydrothermal activity model (Waite 2017)."""

    def south_polar_heat_gw(self):
        return 5.8

    def plume_mass_loss_kg_s(self):
        return 200.0

    def ocean_salinity_ppt(self):
        return 15.0


class TOI849bStrippedRemnantCore:
    """TOI-849b Chthonian stripped remnant core model (Armstrong 2020)."""

    def planet_mass_mearth(self):
        return 39.1

    def planet_radius_rearth(self):
        return 3.44

    def bulk_density_g_cm3(self):
        return 5.50


class ProximaCentauribHabitability:
    """Proxima Centauri b flare irradiation and habitability (Howard 2018)."""

    def semimajor_axis_au(self):
        return 0.0485

    def incident_flux_relative(self):
        return 0.65

    def equilibrium_temp_k(self):
        return 234.0


class TritonRetrogradeTidalCapture:
    """Triton retrograde exchange capture and tidal heating model (Agnor 2006)."""

    def retrograde_inclination_deg(self):
        return 156.8

    def circularization_timescale_myr(self):
        return 100.0

    def peak_tidal_flux_w_m2(self):
        return 1.2e4


class K218bHyceanAtmosphere:
    """K2-18b Hycean atmosphere & ocean equilibrium (Madhusudhan 2023)."""

    def planet_mass_mearth(self):
        return 8.63

    def planet_radius_rearth(self):
        return 2.61

    def methane_mixing_ratio(self):
        return 0.010

    def ammonia_upper_limit(self):
        return 1.0e-5


class EnceladusCDASaltFractionation:
    """Enceladus CDA sodium salt grain fractionation model (Postberg 2009)."""

    def sodium_salt_fraction(self):
        return 0.015

    def dust_production_rate_kg_s(self):
        return 5.0

    def ocean_ph(self):
        return 9.5


class WASP76bIronRain:
    """WASP-76b asymmetric iron condensation and nightside rain (Ehrenreich 2020)."""

    def dayside_temp_k(self):
        return 2500.0

    def nightside_temp_k(self):
        return 1400.0

    def evening_absorption_pct(self):
        return 0.45

    def morning_absorption_pct(self):
        return 0.00


class Kepler11CompactArchitecture:
    """Kepler-11 6-planet compact coplanar TTV system (Lissauer 2011)."""

    def number_of_planets(self):
        return 6

    def mutual_inclination_max_deg(self):
        return 1.0

    def mean_density_g_cm3(self):
        return 1.20

    def ttv_amplitude_minutes(self):
        return 24.5


class BorisovInterstellarComet:
    """2I/Borisov interstellar comet CO volatile sublimation (Bodewits 2020)."""

    def orbital_eccentricity(self):
        return 3.36

    def co_to_water_ratio(self):
        return 1.45

    def formation_temperature_k(self):
        return 20.0


class Trappist1eHabitability:
    """TRAPPIST-1e habitability & atmosphere retention model (Greene 2023)."""

    def planet_mass_mearth(self):
        return 0.692

    def planet_radius_rearth(self):
        return 0.920

    def incident_flux_relative(self):
        return 0.662

    def dayside_temp_k(self):
        return 245.0


class NeptuneGreatDarkSpot:
    """Neptune Great Dark Spot vortex and zonal wind dynamics (Wong 2022)."""

    def zonal_wind_speed_m_s(self):
        return -400.0

    def vortex_drift_speed_m_s(self):
        return 15.0

    def vortex_radius_km(self):
        return 5000.0


class BennuParticleEjection:
    """Asteroid 101955 Bennu regolith particle ejection model (Lauretta 2019)."""

    def particle_ejection_velocity_m_s(self):
        return 0.50

    def mean_particle_radius_cm(self):
        return 1.5

    def thermal_fracture_stress_pa(self):
        return 1.2e5


class LHS3844bBareRock:
    """LHS 3844b bare rock thermal emission phase curve (Kreidberg 2019)."""

    def dayside_temp_k(self):
        return 1040.0

    def nightside_temp_k(self):
        return 20.0

    def heat_redistribution_efficiency(self):
        return 0.00


class SaturnRingSpokes:
    """Saturn B-ring spoke electrostatic levitation model (Mitchell 2006)."""

    def dust_grain_radius_um(self):
        return 0.60

    def electrostatic_potential_volts(self):
        return -15.0

    def levitation_height_km(self):
        return 80.0

    def magnetic_corotation_period_hours(self):
        return 10.656


__all__ = [
    "AZ84Binary",
    "AltjiraBinary",
    "AsteroidDynamics",
    "BennuParticleEjection",
    "BennuYarkovsky",
    "BorisovInterstellarComet",
    "Brasser2012TrojanCapture",
    "CA101Binary",
    "CetoPhorcysBinary",
    "Comet67POutgassing",
    "CometDynamics",
    "EG138Binary",
    "EnceladusCDASaltFractionation",
    "EnceladusHydrothermalVent",
    "EnceladusTidalAnalysis",
    "EnceladusTidalOcean",
    "ErisDysnomia",
    "FB128Binary",
    "FM185Binary",
    "GJ436bHydrogenCloud",
    "GZ31Binary",
    "HATP11bHeliumEscape",
    "HD189733bMassLoss",
    "HD209458bPhotoevaporation",
    "IoLaplaceTidalAnalysis",
    "JA132Binary",
    "JupiterJunoGravityAnalysis",
    "K218bHyceanAtmosphere",
    "KBOBinary",
    "KELT9bUltraHotThermosphere",
    "KP76Binary",
    "KS38Binary",
    "Kepler11CompactArchitecture",
    "Kepler223ResonantChain",
    "LHS3844bBareRock",
    "LTT9779bUltraHotNeptune",
    "LaplaceLagrangeSecular",
    "MercuryRelativisticPrecession",
    "MoonTidalDynamics",
    "NeptuneGreatDarkSpot",
    "NiceModelResonanceCrossing",
    "OJ67Binary",
    "OJ67TNOBinary",
    "PD149Binary",
    "PlanetNineFinder",
    "PlanetNineSecular",
    "PlanetaryRings",
    "PlutoCharonMutual",
    "ProximaCentauribHabitability",
    "QY90Binary",
    "QY297Binary",
    "QuaoarWeywotBinary",
    "RN43Binary",
    "RelativisticPrecession",
    "ResonanceChain",
    "RyuguYarkovsky",
    "SaturnCassiniGravityAnalysis",
    "SaturnRingLindbladResonance",
    "SaturnRingResonances",
    "SaturnRingSpokes",
    "SeasonalYarkovsky",
    "SilaNunamBinary",
    "TOI560bSubNeptuneEscape",
    "TOI849bStrippedRemnantCore",
    "TRAPPIST1ResonantChain",
    "TeharonhiawakoBinary",
    "TitanAtmosphereThermodynamics",
    "Trappist1eHabitability",
    "TritonRetrogradeTidalCapture",
    "UQ18Binary",
    "UX10Binary",
    "VT130Binary",
    "WASP12bTidalDecay",
    "WASP43bTidalCircularization",
    "WASP76bIronRain",
    "WASP121bDeformabilityRLOF",
    "WC19Binary",
    "YN81Binary",
]
