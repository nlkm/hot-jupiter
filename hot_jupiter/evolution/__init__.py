"""
Evolution sub-package.
"""

from hot_jupiter.evolution.integrator import (
    CoupledEvolutionResult,
    EvolutionResult,
    ThermalEvolutionIntegrator,
)
from hot_jupiter.evolution.ohmic_quenching import (
    OhmicQuenchingDiscovery,
    OhmicQuenchingResult,
)

__all__ = [
    "CoupledEvolutionResult",
    "EvolutionResult",
    "OhmicQuenchingDiscovery",
    "OhmicQuenchingResult",
    "ThermalEvolutionIntegrator",
]
