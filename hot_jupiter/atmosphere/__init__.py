"""
Atmosphere boundary sub-package.
"""

from hot_jupiter.atmosphere.base import BaseAtmosphere, AtmosphereResult
from hot_jupiter.atmosphere.guillot import GuillotAtmosphere

__all__ = ["BaseAtmosphere", "AtmosphereResult", "GuillotAtmosphere"]
