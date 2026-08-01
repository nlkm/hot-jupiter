"""
Curated Hot Jupiter Exoplanet Catalog and NASA Exoplanet Archive loader.
"""

from dataclasses import dataclass
from typing import List, Optional
import numpy as np

from thermal_evolution.constants import M_JUP, R_JUP, M_EARTH, R_EARTH, M_SUN, R_SUN, AU, GYR


@dataclass
class ExoplanetSystem:
    """Individual exoplanet system parameters."""
    name: str
    M_p: float           # Planet mass [kg]
    R_p_obs: float       # Observed planet radius [m]
    R_p_err: float       # Radius error [m]
    a: float             # Semi-major axis [m]
    P_orb_days: float    # Orbital period [days]
    eccentricity: float  # Orbital eccentricity
    M_star: float        # Host star mass [kg]
    R_star: float        # Host star radius [m]
    fe_h: float          # Host star metallicity [Fe/H]
    age_gyr: float       # Estimated system age [Gyr]


def get_curated_hot_jupiter_catalog() -> List[ExoplanetSystem]:
    """
    Return a curated dataset of well-characterized Hot Jupiters.
    """
    systems_data = [
        # Name, M_p [M_Jup], R_p [R_Jup], R_err, a [AU], P_orb [days], e, M_* [M_Sun], R_* [R_Sun], [Fe/H], Age [Gyr]
        ("WASP-12b",    1.47, 1.90, 0.09, 0.0229, 1.09, 0.04, 1.35, 1.63, +0.21, 2.0),
        ("WASP-17b",    0.48, 1.93, 0.08, 0.0515, 3.74, 0.02, 1.28, 1.58, -0.19, 3.0),
        ("WASP-19b",    1.14, 1.41, 0.04, 0.0165, 0.79, 0.00, 0.96, 1.00, +0.15, 5.0),
        ("HAT-P-1b",    0.52, 1.32, 0.05, 0.0556, 4.47, 0.00, 1.15, 1.17, +0.13, 3.6),
        ("HD 209458b",  0.69, 1.38, 0.02, 0.0475, 3.52, 0.00, 1.15, 1.19, +0.02, 4.0),
        ("HD 189733b",  1.13, 1.13, 0.03, 0.0310, 2.22, 0.00, 0.81, 0.76, -0.03, 5.0),
        ("Kepler-7b",   0.44, 1.61, 0.05, 0.0622, 4.89, 0.00, 1.35, 1.84, +0.11, 3.5),
        ("CoRoT-1b",    1.03, 1.49, 0.08, 0.0254, 1.51, 0.00, 0.95, 1.11, -0.30, 4.0),
        ("WASP-4b",     1.19, 1.32, 0.04, 0.0231, 1.34, 0.00, 0.89, 0.93, -0.03, 5.2),
        ("WASP-14b",    7.73, 1.28, 0.08, 0.0370, 2.24, 0.09, 1.32, 1.30, +0.09, 2.0),
        ("WASP-18b",    10.4, 1.17, 0.06, 0.0202, 0.94, 0.01, 1.25, 1.22, +0.10, 1.0),
        ("HAT-P-13b",   0.85, 1.28, 0.04, 0.0427, 2.92, 0.01, 1.22, 1.56, +0.41, 5.0),
        ("HAT-P-32b",   0.68, 1.98, 0.09, 0.0343, 2.15, 0.16, 1.16, 1.22, -0.04, 2.7),
        ("WASP-79b",    0.90, 1.70, 0.11, 0.0535, 3.66, 0.00, 1.56, 1.64, +0.03, 1.5),
        ("WASP-121b",   1.18, 1.87, 0.06, 0.0254, 1.27, 0.00, 1.35, 1.46, +0.13, 1.5),
        ("WASP-76b",    0.92, 1.83, 0.06, 0.0330, 1.81, 0.00, 1.46, 1.73, +0.19, 2.4),
        ("HAT-P-23b",   2.09, 1.37, 0.09, 0.0232, 1.21, 0.10, 1.13, 1.20, +0.15, 4.0),
        ("WASP-33b",    2.10, 1.60, 0.07, 0.0256, 1.22, 0.00, 1.50, 1.50, +0.10, 0.5),
        ("WASP-103b",   1.49, 1.53, 0.05, 0.0198, 0.93, 0.00, 1.22, 1.44, +0.06, 4.0),
        ("TrES-3b",     1.91, 1.34, 0.09, 0.0228, 1.31, 0.00, 0.93, 0.83, -0.19, 4.0),
    ]

    catalog = []
    for name, mp, rp, rerr, a_au, porb, ecc, mstar, rstar, feh, age in systems_data:
        sys = ExoplanetSystem(
            name=name,
            M_p=mp * M_JUP,
            R_p_obs=rp * R_JUP,
            R_p_err=rerr * R_JUP,
            a=a_au * AU,
            P_orb_days=porb,
            eccentricity=ecc,
            M_star=mstar * M_SUN,
            R_star=rstar * R_SUN,
            fe_h=feh,
            age_gyr=age,
        )
        catalog.append(sys)

    return catalog
