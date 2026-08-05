"""
Equation of State sub-package.
"""

from hot_jupiter.eos.base import BaseEOS
from hot_jupiter.eos.analytical import AnalyticalHHeEOS
from hot_jupiter.eos.tabular import TabularEOS
from hot_jupiter.eos.core_eos import (
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
