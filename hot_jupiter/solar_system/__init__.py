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


__all__ = [
    "AZ84Binary",
    "AltjiraBinary",
    "AsteroidDynamics",
    "BennuYarkovsky",
    "CA101Binary",
    "CetoPhorcysBinary",
    "CometDynamics",
    "EG138Binary",
    "EnceladusTidalAnalysis",
    "EnceladusTidalOcean",
    "FB128Binary",
    "FM185Binary",
    "GZ31Binary",
    "IoLaplaceTidalAnalysis",
    "JA132Binary",
    "JupiterJunoGravityAnalysis",
    "KP76Binary",
    "KS38Binary",
    "LaplaceLagrangeSecular",
    "MercuryRelativisticPrecession",
    "MoonTidalDynamics",
    "NiceModelResonanceCrossing",
    "OJ67Binary",
    "OJ67TNOBinary",
    "PD149Binary",
    "PlanetNineSecular",
    "PlanetaryRings",
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
    "SeasonalYarkovsky",
    "SilaNunamBinary",
    "TeharonhiawakoBinary",
    "UQ18Binary",
    "UX10Binary",
    "VT130Binary",
    "WC19Binary",
    "YN81Binary",
]
