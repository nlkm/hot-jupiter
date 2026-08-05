"""
Giant Planet Thermal Evolution Model.

A modular framework for modeling interior structure, atmospheric boundary conditions,
tidal dissipation, coupled orbital-spin dynamics, and Roche lobe overflow mass-loss histories.
"""

from hot_jupiter.mass_loss import RocheLobeMassLoss

__version__ = "0.1.0"
__all__ = ["RocheLobeMassLoss"]
