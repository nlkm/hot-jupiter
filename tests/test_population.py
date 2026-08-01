"""
Unit tests for population synthesis, core mass scaling, and selection effects.
"""

import pytest
import numpy as np

from thermal_evolution.constants import M_JUP, M_EARTH, R_JUP, R_SUN, AU
from thermal_evolution.population import (
    estimate_heavy_element_mass,
    transit_selection_weight,
    get_curated_hot_jupiter_catalog,
    PopulationSimulator,
    PopulationSimulationResult,
)


def test_core_mass_scaling():
    # Solar metallicity
    mc_solar = estimate_heavy_element_mass(M_p=1.0 * M_JUP, fe_h=0.0)
    assert mc_solar > 0.0

    # Metal-rich host star (+0.3 dex) should yield larger core mass
    mc_rich = estimate_heavy_element_mass(M_p=1.0 * M_JUP, fe_h=+0.3)
    assert mc_rich > mc_solar


def test_transit_selection_weight():
    w = transit_selection_weight(R_p=1.2 * R_JUP, R_star=1.0 * R_SUN, a=0.05 * AU)
    assert 0.0 < w <= 1.0

    # Larger radius planet should have higher detection weight
    w_small = transit_selection_weight(R_p=0.5 * R_JUP, R_star=1.0 * R_SUN, a=0.05 * AU)
    w_large = transit_selection_weight(R_p=1.5 * R_JUP, R_star=1.0 * R_SUN, a=0.05 * AU)
    assert w_large > w_small


def test_population_simulator():
    catalog = get_curated_hot_jupiter_catalog()[:3]  # Fast mini run over 3 systems
    sim = PopulationSimulator(catalog=catalog)
    res = sim.run_simulation()

    assert isinstance(res, PopulationSimulationResult)
    assert len(res.catalog_names) == 3
    assert len(res.R_obs_jup) == 3
    assert len(res.R_model_no_tidal_jup) == 3
    assert len(res.R_model_tidal_jup) == 3
    assert 0.0 <= res.ks_stat_no_tidal <= 1.0
    assert 0.0 <= res.p_val_no_tidal <= 1.0
