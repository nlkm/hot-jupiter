"""
Atmosphere boundary sub-package.
"""

from hot_jupiter.atmosphere.base import AtmosphereResult, BaseAtmosphere
from hot_jupiter.atmosphere.guillot import GuillotAtmosphere

__all__ = ["AtmosphereResult", "BaseAtmosphere", "GuillotAtmosphere"]
