"""
Unit tests for hot_jupiter.solar_system subpackage.
"""

from hot_jupiter.solar_system import (
    AsteroidDynamics,
    CometDynamics,
    MoonTidalDynamics,
    PlanetaryRings,
    PlanetNineSecular,
    RelativisticPrecession,
)


def test_moon_tidal_dynamics():
    model = MoonTidalDynamics()
    power = model.io_tidal_heating_power_watts(0.0041)
    assert power > 1.0e13, "Io tidal heating power should exceed 10 TW"

    recession = model.earth_moon_recession_rate_m_s()
    cm_yr = recession * 100.0 * 365.25 * 86400.0
    assert abs(cm_yr - 3.8) < 0.5, "Lunar recession rate should be ~3.8 cm/yr"


def test_planetary_rings():
    rings = PlanetaryRings()
    r_roche = rings.roche_limit_m(6.0268e7, 687.0, 1000.0, fluid=True)
    assert r_roche > 1.0e8, "Saturn Roche limit should exceed 100,000 km"


def test_asteroid_dynamics():
    ast = AsteroidDynamics()
    acc = ast.yarkovsky_acceleration_m_s2(500.0, 2000.0, 2.5, 30.0)
    assert acc > 0.0, "Yarkovsky acceleration should be positive"
    assert ast.in_kirkwood_gap(2.50), "2.5 AU should be in 3:1 Kirkwood gap"


def test_comet_dynamics():
    comet = CometDynamics()
    g_1au = comet.marsden_sublimation_g_r(1.0)
    assert g_1au > 0.05, "Marsden sublimation g(r) at 1 AU should be > 0.05"


def test_relativistic_precession():
    gr = RelativisticPrecession()
    merc = gr.mercury_gr_precession_arcsec_century()
    assert abs(merc -
               43.0) < 3.0, "Mercury GR precession should be ~43 arcsec/century"


def test_planet_nine_secular():
    p9 = PlanetNineSecular()
    prec = p9.planet_nine_secular_precession_rad_yr(250.0)
    assert prec > 1.0e-10, "Planet Nine secular precession should be positive"
