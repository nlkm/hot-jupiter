"""
Multi-Planet System Dynamics & Secular N-Body Perturbations.
Implements multi-planet systems with Laplace-Lagrange secular gravitational interactions,
tidal dissipation, and 1D thermal evolution for N planets simultaneously.
"""

from dataclasses import dataclass, field

import numpy as np

from hot_jupiter.constants import M_SUN, G
from hot_jupiter.orbit.orbital_elements import OrbitalState, SpinVectorState


@dataclass
class PlanetSystemMember:
    """
    Defines a single planet within a multi-planet system.
    """
    name: str
    M_p: float  # Planet mass [kg]
    M_c: float  # Core mass [kg]
    S_initial: float  # Initial specific entropy [J/(kg K)]
    orbital_state: OrbitalState
    spin_state: SpinVectorState
    k2_over_Q: float = 1.0e-5  # Tidal dissipation factor k2 / Q
    A_b: float = 0.34  # Bond albedo


@dataclass
class MultiPlanetSystem:
    """
    Container for a star with N orbiting planets.
    """
    name: str
    M_star: float = 1.0 * M_SUN
    Fe_H: float = 0.0
    planets: list[PlanetSystemMember] = field(default_factory=list)

    def add_planet(self, planet: PlanetSystemMember):
        """Add a planet to the system."""
        self.planets.append(planet)

    def laplace_coefficients(self, alpha: float) -> tuple[float, float]:
        """
        Compute Laplace coefficients b_{3/2}^{(1)}(alpha) and b_{3/2}^{(2)}(alpha)
        for secular planet-planet gravitational interactions (Murray & Dermott 1999).
        """
        alpha = float(np.clip(alpha, 1e-4, 0.99))
        # Series approximation for Laplace coefficients
        b1 = 3.0 * alpha * (1.0 + 0.75 * alpha**2 + 0.53 * alpha**4)
        b2 = 0.75 * (alpha**2) * (1.0 + 1.25 * alpha**2 + 1.17 * alpha**4)
        return b1, b2

    def secular_frequencies(self, a_vec: np.ndarray,
                            e_vec: np.ndarray) -> np.ndarray:
        """
        Compute Laplace-Lagrange secular eccentricity matrix A_ij for N planets.
        de_i/dt |_secular = sum_{j != i} A_ij e_j sin(varpi_j - varpi_i)
        """
        N = len(self.planets)
        A_matrix = np.zeros((N, N))

        for i in range(N):
            n_i = np.sqrt(G * self.M_star / (max(a_vec[i], 1e5)**3))

            for j in range(N):
                if i == j:
                    continue
                M_j = self.planets[j].M_p
                alpha = (a_vec[i] /
                         a_vec[j] if a_vec[i] < a_vec[j] else a_vec[j] /
                         a_vec[i])
                bar_alpha = alpha if a_vec[i] < a_vec[j] else 1.0

                _b1, b2 = self.laplace_coefficients(alpha)
                A_matrix[i, j] = -0.25 * n_i * (
                    M_j / self.M_star) * alpha * bar_alpha * b2

            # Diagonal term A_ii = - sum_{j != i} A_ij * (b1 / b2)
            A_matrix[i, i] = 0.25 * n_i * sum(
                (self.planets[j].M_p / self.M_star) *
                (min(a_vec[i], a_vec[j]) / max(a_vec[i], a_vec[j])) *
                self.laplace_coefficients(
                    min(a_vec[i], a_vec[j]) / max(a_vec[i], a_vec[j]))[0]
                for j in range(N)
                if j != i)

        return A_matrix


@dataclass
class MultiPlanetEvolutionResult:
    """Output container for N-planet system evolutionary trajectories."""
    t: np.ndarray  # Time array [s]
    t_gyr: np.ndarray  # Time array [Gyr]
    planet_names: list[str]
    S: dict[str, np.ndarray]  # Specific entropy [J/(kg K)]
    R_p_jup: dict[str, np.ndarray]  # Radius [R_Jup]
    a_au: dict[str, np.ndarray]  # Semi-major axis [AU]
    e: dict[str, np.ndarray]  # Eccentricity
    P_rot_hrs: dict[str, np.ndarray]  # Rotation period [hours]
    obliquity_deg: dict[str, np.ndarray]  # Obliquity [deg]
    T_eff: dict[str, np.ndarray]  # Effective temperature [K]
    P_tidal: dict[str, np.ndarray]  # Tidal power [W]
