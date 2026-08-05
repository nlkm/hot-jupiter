"""
Observational transit selection probability and survey detection completeness model.
"""

import numpy as np
from thermal_evolution.constants import R_SUN, AU, YEAR


def geometric_transit_probability(
    R_p: float | np.ndarray,
    R_star: float | np.ndarray,
    a: float | np.ndarray,
    e: float | np.ndarray = 0.0,
) -> float | np.ndarray:
    """
    Geometric probability of observing a transit: P_geo = (R_star + R_p) / (a * (1 - e^2)).
    """
    R_p_arr = np.asarray(R_p)
    R_star_arr = np.asarray(R_star)
    a_arr = np.asarray(a)
    e_arr = np.asarray(e)

    denom = a_arr * np.maximum(1e-3, 1.0 - e_arr**2)
    p_geo = (R_star_arr + R_p_arr) / denom
    return np.clip(p_geo, 0.0, 1.0)


def transit_detection_completeness(
    R_p: float | np.ndarray,
    R_star: float | np.ndarray,
    a: float | np.ndarray,
    P_orb_days: float | np.ndarray,
    sigma_phot: float = 500e-6,  # 500 ppm photometric noise per cadence
    T_obs_yr: float = 3.0,        # 3 year survey duration
    snr_threshold: float = 7.1,
) -> float | np.ndarray:
    """
    Transit survey detection efficiency P_det as a function of transit SNR.
    P_det(SNR) = 1 / (1 + exp(-(SNR - SNR_thresh) / 1.5))
    """
    R_p_arr = np.asarray(R_p)
    R_star_arr = np.asarray(R_star)
    P_days = np.asarray(P_orb_days)

    # Transit depth delta = (R_p / R_star)^2
    delta = (R_p_arr / R_star_arr)**2

    # Number of transits over survey duration
    N_trans = (T_obs_yr * 365.25) / np.maximum(P_days, 0.1)

    # Approximate SNR scaling
    snr = (delta / sigma_phot) * np.sqrt(N_trans)

    # Logistic completeness curve
    p_det = 1.0 / (1.0 + np.exp(-(snr - snr_threshold) / 1.5))
    return np.clip(p_det, 0.0, 1.0)


def transit_selection_weight(
    R_p: float | np.ndarray,
    R_star: float | np.ndarray = 1.0 * R_SUN,
    a: float | np.ndarray = 0.05 * AU,
    P_orb_days: float | np.ndarray = 3.5,
    e: float | np.ndarray = 0.0,
) -> float | np.ndarray:
    """
    Combined observational selection weight: W = P_geo * P_det.
    """
    p_geo = geometric_transit_probability(R_p, R_star, a, e)
    p_det = transit_detection_completeness(R_p, R_star, a, P_orb_days)
    return p_geo * p_det
