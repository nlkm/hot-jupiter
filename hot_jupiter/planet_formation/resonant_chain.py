"""
Python wrapper for Frontier 5: Resonant Chain Stability & Chaos in Compact Multi-Planet Systems.
"""

from dataclasses import dataclass

import numpy as np


@dataclass
class ResonantEvolutionStep:
    time_kyr: float
    semimajor_axis_1_au: float
    semimajor_axis_2_au: float
    eccentricity_1: float
    eccentricity_2: float
    period_ratio: float
    resonant_angle_deg: float
    is_librating: bool


class ResonantChainDiscovery:
    """Python interface to the Resonant Chain Migration & Chaos Discovery Engine."""

    def __init__(self,
                 star_mass_msun: float = 0.09,
                 m1_mearth: float = 1.0,
                 m2_mearth: float = 1.3,
                 m3_mearth: float = 0.9):
        self.star_mass_msun = star_mass_msun
        self.m1_mearth = m1_mearth
        self.m2_mearth = m2_mearth
        self.m3_mearth = m3_mearth

    def resonance_width(self, p_res: float, q_res: float, a_au: float,
                        m_planet_me: float) -> float:
        """First-order Mean Motion Resonance (MMR) width in semimajor axis [AU]."""
        mu = (m_planet_me * 5.972e24) / (self.star_mass_msun * 1.989e30)
        c_coeff = np.sqrt(1.5 * (p_res + 1.0) / q_res)
        return float(c_coeff * (mu**(2.0 / 3.0)) * a_au)

    def critical_overlap_separation(self, a_au: float, m1_me: float,
                                    m2_me: float) -> float:
        """Chirikov resonance overlap separation delta_a_crit [AU]."""
        mu_tot = ((m1_me + m2_me) * 5.972e24) / (self.star_mass_msun * 1.989e30)
        return float(1.40 * a_au * (mu_tot**(2.0 / 7.0)))

    def equilibrium_eccentricity(self, tau_mig_kyr: float,
                                 tau_e_kyr: float) -> float:
        """Equilibrium eccentricity under migration and damping."""
        if tau_mig_kyr <= 0.0:
            return 0.01
        ratio = tau_e_kyr / tau_mig_kyr
        return float(min(0.25, 0.40 * np.sqrt(ratio)))

    def evolve_chain(self,
                     a1_init_au: float,
                     a2_init_au: float,
                     tau_mig_kyr: float = 100.0,
                     k_damp: float = 100.0,
                     t_max_kyr: float = 200.0,
                     dt_kyr: float = 0.1) -> list[ResonantEvolutionStep]:
        """Simulate convergent migration, resonance capture, and libration."""
        history = []
        a1 = a1_init_au
        a2 = a2_init_au
        e1 = 0.01
        e2 = 0.01
        tau_e = tau_mig_kyr / k_damp
        target_ratio = 1.50

        lambda1 = 0.0
        lambda2 = 0.0
        pomega1 = 0.0

        for t in np.arange(0.0, t_max_kyr + dt_kyr, dt_kyr):
            p1 = 24.0 * np.sqrt((a1**3) / self.star_mass_msun) * 365.25
            p2 = 24.0 * np.sqrt((a2**3) / self.star_mass_msun) * 365.25
            pr = p2 / max(1.0e-4, p1)

            phi_rad = 3.0 * lambda2 - 2.0 * lambda1 - pomega1
            phi_deg = np.degrees(phi_rad) % 360.0

            in_res = abs(pr - target_ratio) < 0.03
            librating = in_res and (abs(phi_deg - 180.0) < 60.0 or
                                    abs(phi_deg - 0.0) < 60.0)

            history.append(
                ResonantEvolutionStep(
                    time_kyr=float(t),
                    semimajor_axis_1_au=float(a1),
                    semimajor_axis_2_au=float(a2),
                    eccentricity_1=float(e1),
                    eccentricity_2=float(e2),
                    period_ratio=float(pr),
                    resonant_angle_deg=float(phi_deg),
                    is_librating=bool(librating),
                ))

            if pr > target_ratio:
                a2 -= (a2 / tau_mig_kyr) * dt_kyr
            else:
                a1 -= (a1 / (3.0 * tau_mig_kyr)) * dt_kyr
                a2 -= (a2 / (3.0 * tau_mig_kyr)) * dt_kyr

                de_res = 0.05 / tau_mig_kyr
                de_damp = -e2 / tau_e
                e2 = max(0.001, e2 + (de_res + de_damp) * dt_kyr)
                e1 = max(0.001, e1 + (0.5 * de_res - e1 / tau_e) * dt_kyr)

            p1 = 24.0 * np.sqrt((a1**3) / self.star_mass_msun) * 365.25
            p2 = 24.0 * np.sqrt((a2**3) / self.star_mass_msun) * 365.25
            n1 = 2.0 * np.pi / (p1 / 24.0)
            n2 = 2.0 * np.pi / (p2 / 24.0)
            lambda1 += n1 * (dt_kyr * 365.25)
            lambda2 += n2 * (dt_kyr * 365.25)
            g_prec = 0.02 * (self.m1_mearth / self.star_mass_msun) * n1
            pomega1 += g_prec * (dt_kyr * 365.25)

        return history
