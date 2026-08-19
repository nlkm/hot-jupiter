"""
Unit tests for atmosphere, heating, and thermal evolution time integrator.
"""

import pytest

from hot_jupiter.atmosphere import AtmosphereResult, GuillotAtmosphere
from hot_jupiter.constants import AU, BAR, M_EARTH, M_JUP, M_SUN, R_JUP, YEAR
from hot_jupiter.eos import AnalyticalHHeEOS
from hot_jupiter.evolution import EvolutionResult, ThermalEvolutionIntegrator
from hot_jupiter.heating import TidalEccentricityHeating, ZeroHeating
from hot_jupiter.structure import InteriorSolver


def test_guillot_atmosphere():
    eos = AnalyticalHHeEOS()
    atmos = GuillotAtmosphere(envelope_eos=eos)

    M_p = 1.0 * M_JUP
    R_p = 1.0 * R_JUP
    S_env = eos.specific_entropy(1.0 * BAR, 165.0)

    # Isolated planet (no stellar irradiation)
    res_iso = atmos.evaluate_atmosphere(M_p=M_p,
                                        R_p=R_p,
                                        S_env=S_env,
                                        F_inc=0.0)
    assert isinstance(res_iso, AtmosphereResult)
    assert res_iso.T_int > 0.0
    assert res_iso.L_int > 0.0
    assert res_iso.T_eff == res_iso.T_int

    # Irradiated planet (F_inc = 1e5 W/m^2 ~ Hot Jupiter at 0.05 AU)
    res_irr = atmos.evaluate_atmosphere(M_p=M_p,
                                        R_p=R_p,
                                        S_env=S_env,
                                        F_inc=1.0e5)
    assert res_irr.T_eff > res_irr.T_int


def test_tidal_heating():
    heating = TidalEccentricityHeating(M_star=1.0 * M_SUN,
                                       a=0.05 * AU,
                                       eccentricity=0.05)
    p_tidal = heating.evaluate_power(t=0.0,
                                     R_p=1.2 * R_JUP,
                                     M_p=1.0 * M_JUP,
                                     S_env=100000.0)
    assert p_tidal > 0.0


@pytest.mark.slow
def test_hot_jupiter_cooling_track():
    eos = AnalyticalHHeEOS()
    solver = InteriorSolver(envelope_eos=eos)
    atmos = GuillotAtmosphere(envelope_eos=eos)
    integrator = ThermalEvolutionIntegrator(
        interior_solver=solver,
        atmosphere_model=atmos,
        heating_source=ZeroHeating(),
    )

    M_p = 1.0 * M_JUP
    M_c = 10.0 * M_EARTH
    S_initial = eos.specific_entropy(1.0 * BAR, 2500.0)  # Hot young planet

    # Short 10 Myr evolution run
    t_span = (1.0e6 * YEAR, 1.0e7 * YEAR)
    result = integrator.evolve(
        M_p=M_p,
        M_c=M_c,
        S_initial=S_initial,
        t_span=t_span,
        num_eval=5,
    )

    assert isinstance(result, EvolutionResult)
    assert len(result.t) == 5
    assert result.S[0] > result.S[-1]  # Entropy decreases (cooling)
    assert result.R_p[0] >= result.R_p[-1]  # Planet contracts as it cools
    assert result.L_int[0] > result.L_int[-1]  # Intrinsic luminosity decreases


def test_ohmic_quenching_discovery():
    from hot_jupiter.evolution import OhmicQuenchingDiscovery
    oq = OhmicQuenchingDiscovery(b_field_gauss=5.0, planet_mass_mjup=1.0)

    # 1. Conductivity
    sig_1200 = oq.atmospheric_conductivity(1200.0)
    sig_2400 = oq.atmospheric_conductivity(2400.0)
    assert sig_2400 > sig_1200

    # 2. Wind speed braking
    v_1200 = oq.wind_speed(1200.0, sig_1200)
    v_2400 = oq.wind_speed(2400.0, sig_2400)
    assert v_2400 < v_1200  # Lorentz drag decelerates hot atmosphere

    # 3. Ohmic dissipation peak
    p_1200 = oq.ohmic_power(1200.0)
    p_1800 = oq.ohmic_power(1800.0)
    p_2600 = oq.ohmic_power(2600.0)
    assert p_1800 > p_1200
    assert p_1800 > p_2600  # Non-monotonic peak

    res = oq.evaluate(2200.0)
    assert res.is_quenched
    assert res.inflated_radius_rjup > 1.2


def test_usp_rlof_discovery():
    from hot_jupiter.evolution import USPRLOFDiscovery
    usp = USPRLOFDiscovery(star_mass_msun=0.4,
                           star_radius_rsun=0.4,
                           k2_q_star=1.0e-6)

    # 1. Roche radius
    a_roche = usp.roche_radius(5.0, 1.6)
    assert 0.001 < a_roche < 0.03

    # 2. Tidal decay
    da = usp.tidal_decay_rate(0.015, 5.0)
    assert da < 0.0

    # 3. Evolution run
    hist = usp.evolve(m_core_init_me=4.0,
                      m_mantle_init_me=6.0,
                      a_init_au=0.010,
                      t_max_myr=2000.0,
                      dt_myr=1.0)
    assert len(hist) > 0
    assert hist[0].planet_mass_mearth == 10.0
    assert hist[-1].planet_mass_mearth < 10.0
