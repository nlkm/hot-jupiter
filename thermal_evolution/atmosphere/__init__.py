"""
Atmosphere boundary sub-package.
"""

from thermal_evolution.atmosphere.base import BaseAtmosphere, AtmosphereResult
from thermal_evolution.atmosphere.guillot import GuillotAtmosphere

__all__ = ["BaseAtmosphere", "AtmosphereResult", "GuillotAtmosphere"]
