"""
Population synthesis sub-package.
"""

from hot_jupiter.population.catalog import ExoplanetSystem, get_curated_hot_jupiter_catalog
from hot_jupiter.population.core_scaling import estimate_heavy_element_mass
from hot_jupiter.population.selection_effects import (
    geometric_transit_probability,
    transit_detection_completeness,
    transit_selection_weight,
)
from hot_jupiter.population.simulator import (
    IncrementalModelStats,
    IncrementalPopulationResult,
    PopulationSimulator,
)

__all__ = [
    "ExoplanetSystem",
    "IncrementalModelStats",
    "IncrementalPopulationResult",
    "PopulationSimulator",
    "estimate_heavy_element_mass",
    "geometric_transit_probability",
    "get_curated_hot_jupiter_catalog",
    "transit_detection_completeness",
    "transit_selection_weight",
]
