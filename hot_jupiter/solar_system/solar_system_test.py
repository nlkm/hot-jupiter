"""
Unit tests for hot_jupiter.solar_system subpackage.
"""

from hot_jupiter.solar_system import (
    AltjiraBinary,
    AsteroidDynamics,
    AZ84Binary,
    CetoPhorcysBinary,
    CometDynamics,
    EG138Binary,
    EnceladusTidalOcean,
    FB128Binary,
    FM185Binary,
    GZ31Binary,
    JA132Binary,
    KP76Binary,
    KS38Binary,
    LaplaceLagrangeSecular,
    MoonTidalDynamics,
    NiceModelResonanceCrossing,
    OJ67Binary,
    PD149Binary,
    PlanetaryRings,
    PlanetNineSecular,
    QY90Binary,
    RelativisticPrecession,
    RN43Binary,
    SaturnRingLindbladResonance,
    SeasonalYarkovsky,
    SilaNunamBinary,
    TeharonhiawakoBinary,
    VT130Binary,
    WC19Binary,
    YN81Binary,
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


def test_laplace_lagrange_secular():
    ll = LaplaceLagrangeSecular()
    g5 = ll.jupiter_secular_g5_arcsec_yr()
    g6 = ll.saturn_secular_g6_arcsec_yr()
    assert abs(g5 - 4.257) < 0.01, "g5 frequency should be ~4.257 arcsec/yr"
    assert abs(g6 - 28.245) < 0.01, "g6 frequency should be ~28.245 arcsec/yr"


def test_nice_model_resonance():
    nice = NiceModelResonanceCrossing()
    kick = nice.ice_giant_eccentricity_kick(0.0)
    assert kick > 0.10, "Nice model kick should be > 0.10"


def test_seasonal_yarkovsky():
    sy = SeasonalYarkovsky()
    drift = sy.seasonal_drift_rate_au_myr(500.0, 2000.0, 2.5, 90.0)
    assert drift < 0.0, "Seasonal Yarkovsky drift rate should be negative"


def test_saturn_ring_lindblad():
    lind = SaturnRingLindbladResonance()
    torque = lind.lindblad_resonance_torque_nm(1.4e17, 1.3935e8)
    assert torque > 1.0e8, "Lindblad ring torque should exceed 1.0e8 N m"


def test_enceladus_tidal_ocean():
    enc = EnceladusTidalOcean()
    power_gw = enc.enceladus_tidal_power_gw()
    assert power_gw > 0.1, "Enceladus tidal power should exceed 0.1 GW"


def test_ceto_phorcys_binary():
    ceto = CetoPhorcysBinary()
    p_days = ceto.orbital_period_days()
    rho_kg_m3 = ceto.system_bulk_density_kg_m3()
    assert abs(p_days - 9.554) < 0.05, "Ceto orbital period mismatch"
    assert abs(rho_kg_m3 - 1370.0) < 50.0, "Ceto bulk density mismatch"


def test_altjira_binary():
    altjira = AltjiraBinary()
    p_days = altjira.orbital_period_days()
    rho_kg_m3 = altjira.system_bulk_density_kg_m3()
    assert abs(p_days - 139.6) < 2.0, "Altjira orbital period mismatch"
    assert abs(rho_kg_m3 - 510.0) < 30.0, "Altjira bulk density mismatch"


def test_sila_nunam_binary():
    sila = SilaNunamBinary()
    p_days = sila.orbital_period_days()
    rho_kg_m3 = sila.system_bulk_density_kg_m3()
    assert abs(p_days - 12.51) < 0.1, "Sila-Nunam orbital period mismatch"
    assert abs(rho_kg_m3 - 720.0) < 30.0, "Sila-Nunam bulk density mismatch"


def test_teharonhiawako_binary():
    teh = TeharonhiawakoBinary()
    p_days = teh.orbital_period_days()
    rho_kg_m3 = teh.system_bulk_density_kg_m3()
    assert abs(p_days - 828.7) < 5.0, "Teharonhiawako orbital period mismatch"
    assert abs(rho_kg_m3 - 620.0) < 30.0, "Teharonhiawako bulk density mismatch"


def test_ks38_binary():
    ks38 = KS38Binary()
    p_days = ks38.orbital_period_days()
    rho_kg_m3 = ks38.system_bulk_density_kg_m3()
    assert abs(p_days - 450.0) < 3.0, "2000 KS38 orbital period mismatch"
    assert abs(rho_kg_m3 - 375.0) < 30.0, "2000 KS38 bulk density mismatch"


def test_oj67_binary():
    oj67 = OJ67Binary()
    p_days = oj67.orbital_period_days()
    rho_kg_m3 = oj67.system_bulk_density_kg_m3()
    assert abs(p_days - 380.0) < 10.0, "2000 OJ67 orbital period mismatch"
    assert abs(rho_kg_m3 - 450.0) < 30.0, "2000 OJ67 bulk density mismatch"


def test_eg138_binary():
    eg138 = EG138Binary()
    p_days = eg138.orbital_period_days()
    rho_kg_m3 = eg138.system_bulk_density_kg_m3()
    assert abs(p_days - 360.0) < 45.0, "2000 EG138 orbital period mismatch"
    assert abs(rho_kg_m3 - 450.0) < 30.0, "2000 EG138 bulk density mismatch"


def test_yn81_binary():
    yn81 = YN81Binary()
    p_days = yn81.orbital_period_days()
    rho_kg_m3 = yn81.system_bulk_density_kg_m3()
    assert abs(p_days - 410.0) < 30.0, "2000 YN81 orbital period mismatch"
    assert abs(rho_kg_m3 - 470.0) < 30.0, "2000 YN81 bulk density mismatch"


def test_wc19_binary():
    wc19 = WC19Binary()
    p_days = wc19.orbital_period_days()
    rho_kg_m3 = wc19.system_bulk_density_kg_m3()
    assert abs(p_days - 8.40) < 0.1, "2002 WC19 orbital period mismatch"
    assert abs(rho_kg_m3 - 638.0) < 30.0, "2002 WC19 bulk density mismatch"


def test_kp76_binary():
    kp76 = KP76Binary()
    p_days = kp76.orbital_period_days()
    rho_kg_m3 = kp76.system_bulk_density_kg_m3()
    assert abs(p_days - 240.0) < 30.0, "2001 KP76 orbital period mismatch"
    assert abs(rho_kg_m3 - 460.0) < 30.0, "2001 KP76 bulk density mismatch"


def test_fb128_binary():
    fb128 = FB128Binary()
    p_days = fb128.orbital_period_days()
    rho_kg_m3 = fb128.system_bulk_density_kg_m3()
    assert abs(p_days - 1660.0) < 10.0, "2003 FB128 orbital period mismatch"
    assert abs(rho_kg_m3 - 498.0) < 30.0, "2003 FB128 bulk density mismatch"


def test_rn43_binary():
    rn43 = RN43Binary()
    p_days = rn43.orbital_period_days()
    rho_kg_m3 = rn43.system_bulk_density_kg_m3()
    assert abs(p_days - 14.80) < 1.0, "2005 RN43 orbital period mismatch"
    assert abs(rho_kg_m3 - 635.0) < 30.0, "2005 RN43 bulk density mismatch"


def test_pd149_binary():
    pd149 = PD149Binary()
    p_days = pd149.orbital_period_days()
    rho_kg_m3 = pd149.system_bulk_density_kg_m3()
    assert abs(p_days - 1260.0) < 10.0, "2002 PD149 orbital period mismatch"
    assert abs(rho_kg_m3 - 340.0) < 30.0, "2002 PD149 bulk density mismatch"


def test_gz31_binary():
    gz31 = GZ31Binary()
    p_days = gz31.orbital_period_days()
    rho_kg_m3 = gz31.system_bulk_density_kg_m3()
    assert abs(p_days - 1010.0) < 10.0, "2002 GZ31 orbital period mismatch"
    assert abs(rho_kg_m3 - 238.0) < 30.0, "2002 GZ31 bulk density mismatch"


def test_az84_binary():
    az84 = AZ84Binary()
    p_days = az84.orbital_period_days()
    rho_kg_m3 = az84.system_bulk_density_kg_m3()
    assert abs(p_days - 12.25) < 1.5, "2003 AZ84 orbital period mismatch"
    assert abs(rho_kg_m3 - 870.0) < 30.0, "2003 AZ84 bulk density mismatch"


def test_vt130_binary():
    vt130 = VT130Binary()
    p_days = vt130.orbital_period_days()
    rho_kg_m3 = vt130.system_bulk_density_kg_m3()
    assert abs(p_days - 1060.0) < 10.0, "2002 VT130 orbital period mismatch"
    assert abs(rho_kg_m3 - 126.0) < 30.0, "2002 VT130 bulk density mismatch"


def test_qy90_binary():
    qy90 = QY90Binary()
    p_days = qy90.orbital_period_days()
    rho_kg_m3 = qy90.system_bulk_density_kg_m3()
    assert abs(p_days - 320.0) < 30.0, "2003 QY90 orbital period mismatch"
    assert abs(rho_kg_m3 - 740.0) < 30.0, "2003 QY90 bulk density mismatch"


def test_ja132_binary():
    ja132 = JA132Binary()
    p_days = ja132.orbital_period_days()
    rho_kg_m3 = ja132.system_bulk_density_kg_m3()
    assert abs(p_days - 515.0) < 10.0, "1999 JA132 orbital period mismatch"
    assert abs(rho_kg_m3 - 224.0) < 50.0, "1999 JA132 bulk density mismatch"


def test_fm185_binary():
    fm185 = FM185Binary()
    p_days = fm185.orbital_period_days()
    rho_kg_m3 = fm185.system_bulk_density_kg_m3()
    assert abs(p_days - 310.0) < 10.0, "2001 FM185 orbital period mismatch"
    assert abs(rho_kg_m3 - 395.0) < 50.0, "2001 FM185 bulk density mismatch"
