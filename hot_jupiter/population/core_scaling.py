"""
Core mass scaling relations as a function of planet mass and host star metallicity [Fe/H].
Ref: Thorngren et al. (2016), ApJ, 831, 64.
"""

import numpy as np
from hot_jupiter.constants import M_EARTH, M_JUP


def estimate_heavy_element_mass(
    M_p: float | np.ndarray,
    fe_h: float | np.ndarray = 0.0,
    base_core_mass: float = 15.0 * M_EARTH,
    alpha: float = 0.5,
    beta: float = 0.6,
) -> float | np.ndarray:
    """
    Estimate total heavy element core mass M_c [kg] from planet mass M_p and stellar metallicity [Fe/H].

    Parameters
    ----------
    M_p : float or array
        Planet mass [kg].
    fe_h : float or array
        Host star metallicity [Fe/H] (e.g. 0.0 for Solar, +0.3 for 2x Solar).
    base_core_mass : float
        Baseline core mass at solar metallicity [kg] (default 15 M_Earth).
    alpha : float
        Metallicity scaling exponent 10^(alpha * [Fe/H]).
    beta : float
        Mass scaling exponent (M_p / M_Jup)^beta.

    Returns
    -------
    M_c : float or array
        Core mass in kg.
    """
    M_p_jup = np.asarray(M_p) / M_JUP
    fe_h_arr = np.asarray(fe_h)

    # M_c = base_core_mass * (M_p / M_Jup)^beta * 10^(alpha * [Fe/H])
    M_c = base_core_mass * (M_p_jup**beta) * (10.0**(alpha * fe_h_arr))

    # Cap core mass to at most 50% of planet mass
    M_c = np.minimum(M_c, 0.5 * np.asarray(M_p))

    if np.isscalar(M_p) and np.isscalar(fe_h):
        return float(M_c)
    return M_c
