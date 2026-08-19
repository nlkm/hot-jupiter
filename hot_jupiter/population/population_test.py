"""
Unit tests for population synthesis, core mass scaling, and selection effects.
"""

import pytest

from hot_jupiter.constants import AU, M_JUP, R_JUP, R_SUN
from hot_jupiter.population import (
    IncrementalPopulationResult,
    PopulationSimulator,
    estimate_heavy_element_mass,
    get_curated_hot_jupiter_catalog,
    transit_selection_weight,
)


def test_core_mass_scaling():
    # Solar metallicity
    mc_solar = estimate_heavy_element_mass(M_p=1.0 * M_JUP, fe_h=0.0)
    assert mc_solar > 0.0

    # Metal-rich host star (+0.3 dex) should yield larger core mass
    mc_rich = estimate_heavy_element_mass(M_p=1.0 * M_JUP, fe_h=+0.3)
    assert mc_rich > mc_solar


def test_transit_selection_weight():
    w = transit_selection_weight(R_p=1.2 * R_JUP,
                                 R_star=1.0 * R_SUN,
                                 a=0.05 * AU)
    assert 0.0 < w <= 1.0

    # Larger radius planet should have higher detection weight
    w_small = transit_selection_weight(R_p=0.5 * R_JUP,
                                       R_star=1.0 * R_SUN,
                                       a=0.05 * AU)
    w_large = transit_selection_weight(R_p=1.5 * R_JUP,
                                       R_star=1.0 * R_SUN,
                                       a=0.05 * AU)
    assert w_large > w_small


@pytest.mark.slow
def test_population_simulator():
    catalog = get_curated_hot_jupiter_catalog(
    )[:3]  # Fast mini run over 3 systems
    sim = PopulationSimulator(catalog=catalog)
    res = sim.run_incremental_simulation()

    assert isinstance(res, IncrementalPopulationResult)
    assert 0.0 <= res.stage_results[
        "Stage 0: Non-irradiated Base"].ks_stat <= 1.0


def test_radius_valley_discovery():
    from hot_jupiter.population import RadiusValleyDiscovery
    rv = RadiusValleyDiscovery(seed=42)

    # 1. Mass loss rates
    mdot_photo = rv.photoevaporative_mass_loss_rate(m_core_me=5.0,
                                                    f_env=0.03,
                                                    a_au=0.05,
                                                    m_star_msun=1.0,
                                                    age_gyr=0.05)
    assert mdot_photo > 0.0

    mdot_core = rv.core_powered_mass_loss_rate(m_core_me=5.0,
                                               f_env=0.03,
                                               a_au=0.05,
                                               m_star_msun=1.0,
                                               age_gyr=1.0)
    assert mdot_core > 0.0

    # 2. Planet radii
    r_rock = rv.compute_planet_radius(5.0, 0.0, 0.0, 0.05, 1.0, 5.0)
    r_water = rv.compute_planet_radius(5.0, 0.0, 0.50, 0.05, 1.0, 5.0)
    r_gas = rv.compute_planet_radius(5.0, 0.03, 0.0, 0.05, 1.0, 5.0)
    assert r_gas > r_water > r_rock

    # 3. Valley slopes
    assert abs(rv.valley_slope_dlogr_dlogp("photoevaporation") - (-0.11)) < 0.01
    assert abs(rv.valley_slope_dlogr_dlogp("core_powered") - (-0.06)) < 0.01
    assert abs(rv.valley_slope_dlogr_dlogp("water_worlds") - 0.00) < 0.01
    assert abs(rv.valley_slope_dlogr_dlogmstar("photoevaporation") -
               0.25) < 0.01
    assert abs(rv.valley_slope_dlogr_dlogmstar("core_powered") - 0.35) < 0.01
