"""
Unit tests for multi-domain astrophysics subpackages.
"""

import numpy as np

from hot_jupiter.planet_formation import CoreAccretion, DiskMigration
from hot_jupiter.star_formation import JeansInstability, LarsonScalingLaws
from hot_jupiter.stellar_evolution import EddingtonLimit, StellarMainSequence


def test_planet_formation():
    core = CoreAccretion()
    m_crit = core.critical_core_mass_kg(1.0e-6 * 5.972e24 / (365.25 * 86400.0))
    assert abs(m_crit / 5.972e24 - 10.0) < 1.0, "Critical core mass ~10 M_earth"

    migration = DiskMigration()
    t_mig = migration.type_i_migration_timescale_yr(5.972e24, 1.496e11)
    assert t_mig > 1.0e4, "Type I migration timescale should be > 10,000 yr"


def test_stellar_evolution():
    stellar = StellarMainSequence()
    l_solar = stellar.zams_luminosity_watts(1.98847e30)
    assert abs(l_solar - 3.828e26) < 1.0e25, "Solar ZAMS luminosity match"

    edd = EddingtonLimit()
    l_edd = edd.eddington_luminosity_watts(1.98847e30)
    assert l_edd > 1.0e31, "Solar Eddington limit > 1e31 W"


def test_star_formation():
    jeans = JeansInstability()
    m_j = jeans.jeans_mass_kg(10.0, 1.0e-16) / 1.98847e30
    assert 0.1 < m_j < 100.0, "Jeans mass at 10K should be solar order"

    larson = LarsonScalingLaws()
    v_disp = larson.velocity_dispersion_m_s(1.0)
    assert abs(v_disp - 1100.0) < 50.0, "Larson velocity dispersion ~1.1 km/s"


def test_terminator_aerosol_discovery():
    from hot_jupiter.atmosphere import TerminatorAerosolDiscovery
    engine = TerminatorAerosolDiscovery(t_eq_planet_k=1600.0,
                                        surface_gravity_m_s2=10.0,
                                        metallicity_dex=0.0)

    # 1. Condensation curve
    t_cond = engine.silicate_condensation_temp(1.0)
    assert 1400.0 < t_cond < 1800.0

    # 2. Temperature asymmetry
    t_eve = engine.limb_temperature(0.01, 1)
    t_mor = engine.limb_temperature(0.01, 2)
    assert t_eve > t_mor

    # 3. Microphysics
    mor = engine.evaluate_microphysics(0.01, 2)
    eve = engine.evaluate_microphysics(0.01, 1)
    assert mor.cloud_condensate_mass_frac > 0.0
    assert eve.cloud_condensate_mass_frac == 0.0

    # 4. JWST spectrum
    spec = engine.compute_jwst_spectrum(50)
    assert len(spec.wavelength_um) == 50
    amp_eve = np.ptp(spec.transit_depth_evening_ppm)
    amp_mor = np.ptp(spec.transit_depth_morning_ppm)
    assert amp_eve > amp_mor  # Evening limb exhibits unmuted molecular features


def test_resonant_chain_discovery():
    from hot_jupiter.planet_formation import ResonantChainDiscovery
    engine = ResonantChainDiscovery(star_mass_msun=0.09,
                                    m1_mearth=1.0,
                                    m2_mearth=1.3,
                                    m3_mearth=0.9)

    # 1. Resonance width
    w_32 = engine.resonance_width(2.0, 1.0, 0.015, 1.3)
    assert 1.0e-5 < w_32 < 1.0e-2

    # 2. Critical separation
    da_crit = engine.critical_overlap_separation(0.015, 1.0, 1.3)
    assert 1.0e-4 < da_crit < 5.0e-3

    # 3. Evolution
    hist = engine.evolve_chain(0.012,
                               0.018,
                               tau_mig_kyr=50.0,
                               k_damp=100.0,
                               t_max_kyr=100.0,
                               dt_kyr=0.2)
    assert len(hist) > 0
    assert abs(hist[-1].period_ratio - 1.50) < 0.05


def test_cryosphere_fracture_discovery():
    from hot_jupiter.solar_system import CryosphereFractureDiscovery
    engine = CryosphereFractureDiscovery(body_radius_km=606.0,
                                         surface_gravity_m_s2=0.288,
                                         bulk_density_kg_m3=1700.0,
                                         shear_modulus_gpa=3.5,
                                         tensile_strength_mpa=2.0)

    # 1. Viscosity & relaxation
    eta = engine.ice_viscosity_pa_s(260.0)
    tau_m = engine.maxwell_time_years(260.0)
    assert 1.0e13 < eta < 1.0e17
    assert 1.0e-5 < tau_m < 1.0e4

    # 2. Overpressure
    dp = engine.compute_ocean_overpressure_mpa(50.0, 100.0, 5.0)
    assert 0.1 < dp < 50.0

    # 3. Evolution
    hist = engine.evolve_cryosphere(30.0,
                                    80.0,
                                    lid_temp_k=120.0,
                                    freezing_rate_km_myr=0.1,
                                    t_max_myr=200.0,
                                    dt_myr=2.0)
    assert len(hist) > 0
    assert hist[-1].is_fractured
