"""
Unit tests for population synthesis, core mass scaling, and selection effects.
"""

import pytest
import numpy as np

from hot_jupiter.constants import M_JUP, M_EARTH, R_JUP, R_SUN, AU
from hot_jupiter.population import (
    estimate_heavy_element_mass,
    transit_selection_weight,
    get_curated_hot_jupiter_catalog,
    PopulationSimulator,
    IncrementalPopulationResult,
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

    assert isinstance(res, IncrementalPopulationResult)
    assert len(res.catalog_names) == 3
    assert len(res.R_obs_jup) == 3
    assert "Stage 0: Non-irradiated Base" in res.stage_results
    assert 0.0 <= res.stage_results["Stage 0: Non-irradiated Base"].ks_stat <= 1.0

