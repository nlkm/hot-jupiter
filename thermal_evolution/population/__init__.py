"""
Population synthesis sub-package.
"""

from thermal_evolution.population.catalog import ExoplanetSystem, get_curated_hot_jupiter_catalog
from thermal_evolution.population.core_scaling import estimate_heavy_element_mass
from thermal_evolution.population.selection_effects import (
    geometric_transit_probability,
    transit_detection_completeness,
    transit_selection_weight,
)
from thermal_evolution.population.simulator import (
    PopulationSimulationResult,
    PopulationSimulator,
)

__all__ = [
    "ExoplanetSystem",
    "get_curated_hot_jupiter_catalog",
    "estimate_heavy_element_mass",
    "geometric_transit_probability",
    "transit_detection_completeness",
    "transit_selection_weight",
    "PopulationSimulationResult",
    "PopulationSimulator",
]
