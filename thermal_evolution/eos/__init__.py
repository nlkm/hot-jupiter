"""
Equation of State sub-package.
"""

from thermal_evolution.eos.base import BaseEOS
from thermal_evolution.eos.analytical import AnalyticalHHeEOS
from thermal_evolution.eos.tabular import TabularEOS
from thermal_evolution.eos.core_eos import (
    BaseCoreEOS,
    ConstantDensityCoreEOS,
    BirchMurnaghanCoreEOS,
)

__all__ = [
    "BaseEOS",
    "AnalyticalHHeEOS",
    "TabularEOS",
    "BaseCoreEOS",
    "ConstantDensityCoreEOS",
    "BirchMurnaghanCoreEOS",
]
