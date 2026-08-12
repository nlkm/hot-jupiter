"""
Unit tests for hot_jupiter.solar_system subpackage.
"""

from hot_jupiter.solar_system import (
    AltjiraBinary,
    AsteroidDynamics,
    AZ84Binary,
    BennuYarkovsky,
    CA101Binary,
    CetoPhorcysBinary,
    Comet67POutgassing,
    CometDynamics,
    EG138Binary,
    EnceladusTidalAnalysis,
    EnceladusTidalOcean,
    ErisDysnomia,
    FB128Binary,
    FM185Binary,
    GJ436bHydrogenCloud,
    GZ31Binary,
    HATP11bHeliumEscape,
    HaumeaEllipsoidRing,
    HD189733bMassLoss,
    HD209458bPhotoevaporation,
    IoLaplaceTidalAnalysis,
    JA132Binary,
    JupiterJunoGravityAnalysis,
    KELT9bUltraHotThermosphere,
    Kepler223ResonantChain,
    KP76Binary,
    KS38Binary,
    LaplaceLagrangeSecular,
    MercuryRelativisticPrecession,
    MoonTidalDynamics,
    NiceModelResonanceCrossing,
    OJ67Binary,
    OJ67TNOBinary,
    PD149Binary,
    PlanetaryRings,
    PlanetNineSecular,
    PlutoCharonMutual,
    QuaoarWeywotBinary,
    QY90Binary,
    QY297Binary,
    RelativisticPrecession,
    RN43Binary,
    RyuguYarkovsky,
    SaturnCassiniGravityAnalysis,
    SaturnRingLindbladResonance,
    SaturnRingResonances,
    SeasonalYarkovsky,
    SilaNunamBinary,
    TeharonhiawakoBinary,
    TOI560bSubNeptuneEscape,
    TRAPPIST1ResonantChain,
    UQ18Binary,
    UX10Binary,
    VT130Binary,
    WASP12bTidalDecay,
    WASP43bTidalCircularization,
    WASP121bDeformabilityRLOF,
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
    assert prec > 0, "Planet Nine secular precession should be positive"
    angle = p9.secular_perihelion_clustering_deg()
    assert abs(
        angle -
        180.0) < 5.0, "Planet Nine secular perihelion clustering mismatch"


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


def test_oj67_tno_binary():
    oj67_tno = OJ67TNOBinary()
    p_days = oj67_tno.orbital_period_days()
    rho_kg_m3 = oj67_tno.system_bulk_density_kg_m3()
    assert abs(p_days - 1005.0) < 10.0, "2000 OJ67 TNO orbital period mismatch"
    assert abs(rho_kg_m3 - 566.0) < 30.0, "2000 OJ67 TNO bulk density mismatch"


def test_quaoar_weywot_binary():
    quaoar = QuaoarWeywotBinary()
    p_days = quaoar.orbital_period_days()
    rho_kg_m3 = quaoar.system_bulk_density_kg_m3()
    assert abs(p_days - 12.438) < 0.5, "Quaoar / Weywot orbital period mismatch"
    assert abs(rho_kg_m3 -
               1640.0) < 50.0, "Quaoar / Weywot bulk density mismatch"


def test_ux10_binary():
    ux10 = UX10Binary()
    p_days = ux10.orbital_period_days()
    rho_kg_m3 = ux10.system_bulk_density_kg_m3()
    assert abs(p_days - 122.0) < 5.0, "2004 UX10 orbital period mismatch"
    assert abs(rho_kg_m3 - 1164.0) < 50.0, "2004 UX10 bulk density mismatch"


def test_qy297_binary():
    qy297 = QY297Binary()
    p_days = qy297.orbital_period_days()
    rho_kg_m3 = qy297.system_bulk_density_kg_m3()
    assert abs(p_days - 138.1) < 5.0, "2001 QY297 orbital period mismatch"
    assert abs(rho_kg_m3 - 471.0) < 30.0, "2001 QY297 bulk density mismatch"


def test_ca101_binary():
    ca101 = CA101Binary()
    p_days = ca101.orbital_period_days()
    rho_kg_m3 = ca101.system_bulk_density_kg_m3()
    assert abs(p_days - 345.0) < 10.0, "2000 CA101 orbital period mismatch"
    assert abs(rho_kg_m3 - 613.0) < 40.0, "2000 CA101 bulk density mismatch"


def test_uq18_binary():
    uq18 = UQ18Binary()
    p_days = uq18.orbital_period_days()
    rho_kg_m3 = uq18.system_bulk_density_kg_m3()
    assert abs(p_days - 165.0) < 5.0, "2001 UQ18 orbital period mismatch"
    assert abs(rho_kg_m3 - 398.0) < 30.0, "2001 UQ18 bulk density mismatch"


def test_saturn_ring_resonances():
    saturn_ring = SaturnRingResonances()
    r_mimas21 = saturn_ring.inner_lindblad_resonance_km(185539.0, 2, 1)
    r_janus76 = saturn_ring.inner_lindblad_resonance_km(151460.0, 7, 6)
    r_fring = saturn_ring.shepherd_torque_balance_km()
    assert abs(r_mimas21 - 117580.0) < 1000.0, "Mimas 2:1 ILR mismatch"
    assert abs(r_janus76 - 136770.0) < 500.0, "Janus 7:6 ILR mismatch"
    assert abs(r_fring -
               140220.0) < 500.0, "F-ring shepherd torque balance mismatch"


def test_enceladus_tidal_analysis():
    enceladus = EnceladusTidalAnalysis()
    p_diss_gw = enceladus.tidal_dissipation_power_gw()
    q_cond_gw = enceladus.conductive_heat_flux_gw(20.0)
    assert abs(p_diss_gw -
               15.8) < 1.0, "Enceladus tidal dissipation power mismatch"
    assert abs(q_cond_gw -
               29.3) < 2.0, "Enceladus conductive heat flux mismatch"


def test_io_laplace_tidal_analysis():
    io = IoLaplaceTidalAnalysis()
    p_tw = io.io_tidal_power_tw()
    f_w_m2 = io.surface_heat_flux_w_m2(p_tw)
    assert abs(p_tw - 105.0) < 1.0, "Io tidal power mismatch"
    assert abs(f_w_m2 - 2.52) < 0.1, "Io surface heat flux mismatch"


def test_jupiter_juno_gravity_analysis():
    jg = JupiterJunoGravityAnalysis()
    j2 = jg.j2_harmonic_1e6()
    j4 = jg.j4_harmonic_1e6()
    j6 = jg.j6_harmonic_1e6()
    assert abs(j2 - 14696.57) < 50.0, "Jupiter J2 mismatch"
    assert abs(j4 - (-586.61)) < 5.0, "Jupiter J4 mismatch"
    assert abs(j6 - 34.20) < 1.0, "Jupiter J6 mismatch"


def test_saturn_cassini_gravity_analysis():
    sg = SaturnCassiniGravityAnalysis()
    j2 = sg.j2_harmonic_1e6()
    j4 = sg.j4_harmonic_1e6()
    j6 = sg.j6_harmonic_1e6()
    assert abs(j2 - 16290.71) < 50.0, "Saturn J2 mismatch"
    assert abs(j4 - (-935.83)) < 5.0, "Saturn J4 mismatch"
    assert abs(j6 - 86.14) < 1.0, "Saturn J6 mismatch"


def test_mercury_relativistic_precession():
    mp = MercuryRelativisticPrecession()
    gr_rate = mp.gr_precession_arcsec_century()
    j2_rate = mp.j2_sun_precession_arcsec_century()
    assert abs(gr_rate - 42.982) < 0.1, "Mercury GR rate mismatch"
    assert abs(j2_rate - 0.0286) < 0.01, "Mercury Solar J2 rate mismatch"


def test_bennu_yarkovsky():
    by = BennuYarkovsky()
    drift = by.yarkovsky_drift_m_yr()
    assert abs(drift - (-284.0)) < 5.0, "Bennu Yarkovsky drift rate mismatch"


def test_ryugu_yarkovsky():
    ry = RyuguYarkovsky()
    drift = ry.yarkovsky_drift_m_yr()
    assert abs(drift - (-215.0)) < 5.0, "Ryugu Yarkovsky drift rate mismatch"


def test_comet67p_outgassing():
    co = Comet67POutgassing()
    a1_val = co.radial_acceleration_au_day2(1.0)
    assert abs(a1_val -
               3.25e-8) < 1.0e-9, "Comet 67P outgassing acceleration mismatch"


def test_pluto_charon_mutual():
    pc = PlutoCharonMutual()
    period = pc.orbital_period_days()
    assert abs(period - 6.38723) < 0.001, "Pluto-Charon period mismatch"


def test_eris_dysnomia():
    ed = ErisDysnomia()
    period = ed.orbital_period_days()
    assert abs(period - 15.7232) < 0.01, "Eris-Dysnomia period mismatch"


def test_haumea_ellipsoid_ring():
    h = HaumeaEllipsoidRing()
    r_ring = h.ring_3to1_resonance_radius_km()
    assert abs(r_ring - 2287.3) < 15.0, "Haumea ring radius mismatch"


def test_hd209458b_photoevaporation():
    photo = HD209458bPhotoevaporation()
    mdot = photo.mass_loss_rate_g_s()
    assert abs(mdot - 5.0e10) < 1.0e10, "HD 209458b mass loss rate mismatch"


def test_hd189733b_mass_loss():
    hd189 = HD189733bMassLoss()
    mdot_flare = hd189.flare_mass_loss_rate_g_s()
    assert abs(mdot_flare -
               4.5e11) < 1.0e11, "HD 189733b flare mass loss rate mismatch"


def test_gj436b_hydrogen_cloud():
    gj = GJ436bHydrogenCloud()
    mdot = gj.mass_loss_rate_g_s()
    assert abs(mdot - 2.2e10) < 5.0e9, "GJ 436b cloud mass loss rate mismatch"


def test_wasp12b_tidal_decay():
    w = WASP12bTidalDecay()
    pdot = w.period_decay_rate_ms_yr()
    assert abs(pdot - (-29.0)) < 2.0, "WASP-12b period decay rate mismatch"


def test_wasp43b_tidal_circularization():
    w = WASP43bTidalCircularization()
    tau_e = w.circularization_timescale_myr()
    assert abs(tau_e - 7.5) < 1.0, "WASP-43b circularization timescale mismatch"


def test_trappist1_resonant_chain():
    t = TRAPPIST1ResonantChain()
    ttv = t.ttv_chopping_amplitude_minutes()
    assert abs(ttv - 38.5) < 2.0, "TRAPPIST-1 TTV chopping amplitude mismatch"


def test_kepler223_resonant_chain():
    k = Kepler223ResonantChain()
    ttv = k.ttv_chopping_amplitude_minutes()
    assert abs(ttv - 14.2) < 1.0, "Kepler-223 TTV chopping amplitude mismatch"


def test_kelt9b_ultra_hot_thermosphere():
    k = KELT9bUltraHotThermosphere()
    depth = k.halpha_excess_depth_percent()
    assert abs(depth - 1.15) < 0.2, "KELT-9b H-alpha excess depth mismatch"


def test_hatp11b_helium_escape():
    h = HATP11bHeliumEscape()
    depth = h.hei_10830_excess_depth_percent()
    assert abs(depth - 1.08) < 0.2, "HAT-P-11b helium absorption depth mismatch"


def test_toi560b_sub_neptune_escape():
    t = TOI560bSubNeptuneEscape()
    depth = t.hei_10830_excess_depth_percent()
    assert abs(depth - 0.68) < 0.2, "TOI-560b helium absorption depth mismatch"


def test_wasp121b_deformability_rlof():
    w = WASP121bDeformabilityRLOF()
    depth = w.nuv_fe_ii_excess_depth_percent()
    assert abs(depth - 0.85) < 0.2, "WASP-121b Fe II NUV depth mismatch"
