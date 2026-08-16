"""
Orbital Elements and Spin Vector Evolution for Giant Planets.
Implements Hut (1981), Eggleton et al. (1998), Leconte et al. (1010) tidal-spin-orbital dynamics.
"""

from dataclasses import dataclass

import numpy as np

from hot_jupiter.constants import AU, DAY, HOUR, M_SUN, YEAR, G


@dataclass
class OrbitalState:
    """
    Keplerian orbital elements for a planet.
    """
    a: float  # Semi-major axis [m]
    e: float  # Eccentricity [0, 1)
    inc: float = 0.0  # Inclination [rad]
    Omega_node: float = 0.0  # Longitude of ascending node [rad]
    omega_arg: float = 0.0  # Argument of periastron [rad]

    @property
    def a_au(self) -> float:
        return self.a / AU

    @property
    def mean_motion(self) -> float:
        """Orbital mean motion n = sqrt(G * M_star / a^3) [rad/s]."""
        # Assumes M_star stored externally or calculated
        return np.sqrt(G * M_SUN / (self.a**3)) if self.a > 0 else 0.0

    @property
    def period_days(self) -> float:
        """Orbital period P_orb = 2 * pi / n [days]."""
        n = self.mean_motion
        return (2.0 * np.pi / n) / DAY if n > 0 else 0.0


@dataclass
class SpinVectorState:
    """
    Planet 3D spin vector and rotational state.
    """
    Omega_rot: float  # Spin angular frequency magnitude [rad/s]
    obliquity: float = 0.0  # Obliquity angle epsilon between spin and orbit normal [rad]
    precession_phase: float = 0.0  # Precession angle psi [rad]

    @property
    def period_hours(self) -> float:
        """Rotation period P_rot = 2 * pi / Omega_rot [hours]."""
        return (2.0 * np.pi /
                self.Omega_rot) / HOUR if self.Omega_rot > 0 else 0.0

    @property
    def spin_vector(self) -> np.ndarray:
        """
        3D Cartesian spin vector Omega_rot = (Omega_x, Omega_y, Omega_z).
        Z-axis aligns with orbital angular momentum vector.
        """
        ox = self.Omega_rot * np.sin(self.obliquity) * np.sin(
            self.precession_phase)
        oy = self.Omega_rot * np.sin(self.obliquity) * np.cos(
            self.precession_phase)
        oz = self.Omega_rot * np.cos(self.obliquity)
        return np.array([ox, oy, oz])

    @classmethod
    def from_period_hours(cls,
                          period_hrs: float,
                          obliquity_deg: float = 0.0) -> "SpinVectorState":
        """Initialize from rotation period in hours and obliquity in degrees."""
        Omega_rot = (2.0 * np.pi) / (period_hrs * HOUR)
        obliquity = np.radians(obliquity_deg)
        return cls(Omega_rot=Omega_rot, obliquity=obliquity)


class TidalOrbitalSpinRates:
    """
    Evaluates coupled rates of change (da/dt, de/dt, dOmega_rot/dt, debliquity/dt)
    driven by equilibrium tidal dissipation (Hut 1981, Eggleton et al. 1998).
    """

    def __init__(
            self,
            k2_over_Q: float = 1.0e-5,  # Tidal dissipation parameter k2 / Q
            C_moment:
        float = 0.25,  # Dimensionless moment of inertia I_p / (M_p * R_p^2)
    ):
        self.k2_over_Q = k2_over_Q
        self.C_moment = C_moment

    def evaluate_rates(
        self,
        M_p: float,
        R_p: float,
        M_star: float,
        a: float,
        e: float,
        Omega_rot: float,
        obliquity: float,
        dR_dt: float = 0.0,
    ) -> tuple[float, float, float, float]:
        """
        Compute (da/dt, de/dt, dOmega_rot/dt, dobliquity/dt).

        Returns
        -------
        da_dt : float [m/s]
        de_dt : float [1/s]
        dOmega_dt : float [rad/s^2]
        dobl_dt : float [rad/s]
        """
        if a <= 0 or R_p <= 0 or M_p <= 0 or M_star <= 0:
            return 0.0, 0.0, 0.0, 0.0

        n = np.sqrt(G * M_star / (a**3))  # Mean motion [rad/s]
        sin_eps = np.sin(obliquity)

        # Hut (1981) pseudo-synchronous spin rate
        e2 = float(np.clip(e**2, 0.0, 0.98))
        f_ps = (1.0 + 7.5 * e2 + 5.625 * (e2**2) + 0.3125 *
                (e2**3)) / (((1.0 - e2)**1.5) * (1.0 + 3.0 * e2 + 0.375 *
                                                 (e2**2)))
        Omega_ps = n * f_ps

        # Hut (1981) tidal spin synchronization timescale (~ 50 Myr for Hot Jupiters)
        tau_spin = 5.0e7 * YEAR
        dOmega_tide = (Omega_ps - Omega_rot) / tau_spin
        dOmega_contraction = -(2.0 * Omega_rot /
                               R_p) * dR_dt if dR_dt != 0 else 0.0
        dOmega_dt = dOmega_tide + dOmega_contraction

        R_over_a_5 = (R_p / a)**5
        scale_tide = self.k2_over_Q * (M_star / M_p) * R_over_a_5 * n

        # Hut (1981) tidal eccentricity damping & orbital energy dissipation (da/dt <= 0)
        de_dt = -27.0 * scale_tide * e * (
            (1.0 - e2)**(-6.5)) * (1.0 + 3.75 * e2 + 0.9375 * (e2**2))
        if e <= 1.0e-6 and de_dt < 0:
            de_dt = 0.0

        # Conservation of angular momentum under circularization: da/dt = (2 a e / (1 - e^2)) de/dt
        da_dt = (2.0 * a * e / max(1.0 - e2, 1e-6)) * de_dt

        # Obliquity damping
        dobl_dt = -(sin_eps / tau_spin)
        if obliquity <= 1.0e-6 and dobl_dt < 0:
            dobl_dt = 0.0

        return float(da_dt), float(de_dt), float(dOmega_dt), float(dobl_dt)


@dataclass
class StellarTidalRates:
    """
    Evaluates stellar tidal torque and semi-major axis migration driven by host star rotation Omega_*.
    (Hut 1981, Ogilvie & Lin 2007).
    """
    k2_over_Q_star: float = 1.0e-6  # Stellar tidal dissipation factor k2_* / Q_*
    R_star: float = 6.957e8  # Solar radius [m]

    def evaluate_stellar_rates(
        self,
        M_p: float,
        M_star: float,
        a: float,
        Omega_star: float,  # Stellar rotation rate [rad/s]
        stellar_obliquity:
        float = 0.0,  # Angle psi_* between star spin and orbit normal [rad]
    ) -> tuple[float, float]:
        """
        Compute (da_dt_star, dOmega_star_dt).

        Returns
        -------
        da_dt_star : float [m/s]
            Semi-major axis migration rate from stellar tides.
            (Negative if sub-synchronous n > Omega_*, causing inward orbital decay;
             Positive if super-synchronous n < Omega_*, causing outward expansion).
        dOmega_star_dt : float [rad/s^2]
            Stellar rotation frequency rate of change.
        """
        if a <= 0 or M_p <= 0 or M_star <= 0:
            return 0.0, 0.0

        n = np.sqrt(G * M_star / (a**3))
        cos_psi = np.cos(stellar_obliquity)

        # Scale factor for stellar tides: scale = (k2_star / Q_star) * (M_p / M_star) * (R_star / a)^5 * n
        scale_star = self.k2_over_Q_star * (M_p / M_star) * (
            (self.R_star / a)**5) * n

        # 1. Semi-major axis rate from stellar tides da/dt |_star
        # da/dt |_star = - 3 * scale_star * a * (1 - (Omega_star / n) * cos_psi)
        da_dt_star = -3.0 * scale_star * a * (
            1.0 - (Omega_star / max(n, 1e-15)) * cos_psi)

        # 2. Stellar rotation frequency rate of change dOmega_star / dt
        # Torque T_star = (3/2) * (k2_star / Q_star) * G * M_p^2 * R_star^5 / a^6 * (n * cos_psi - Omega_star)
        I_star = 0.07 * M_star * (self.R_star**2
                                 )  # Solar moment of inertia coefficient ~ 0.07
        T_star = 1.5 * self.k2_over_Q_star * G * (M_p**2) * (self.R_star**5) / (
            a**6) * (n * cos_psi - Omega_star)
        dOmega_star_dt = T_star / max(I_star, 1e-10)

        return float(da_dt_star), float(dOmega_star_dt)
