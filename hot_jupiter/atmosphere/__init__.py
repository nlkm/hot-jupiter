"""
Atmosphere boundary sub-package.
"""

from hot_jupiter.atmosphere.base import AtmosphereResult, BaseAtmosphere
from hot_jupiter.atmosphere.guillot import GuillotAtmosphere
from hot_jupiter.atmosphere.models import (
    KomacekShowmanCirculation,
    MadhusudhanRetrieval,
    MHDDrag,
    MieClouds,
    NonLTEDissociation,
    ParmentierClouds,
    ShowmanCirculation3D,
    SingTransmission,
    SpiegelBurrowsInversion,
)
from hot_jupiter.atmosphere.terminator_aerosol import (
    JWSTTransmissionSpectrum,
    LimbMicrophysicsResult,
    TerminatorAerosolDiscovery,
)

__all__ = [
    "AtmosphereResult",
    "BaseAtmosphere",
    "GuillotAtmosphere",
    "JWSTTransmissionSpectrum",
    "KomacekShowmanCirculation",
    "LimbMicrophysicsResult",
    "MHDDrag",
    "MadhusudhanRetrieval",
    "MieClouds",
    "NonLTEDissociation",
    "ParmentierClouds",
    "ShowmanCirculation3D",
    "SingTransmission",
    "SpiegelBurrowsInversion",
    "TerminatorAerosolDiscovery",
]
