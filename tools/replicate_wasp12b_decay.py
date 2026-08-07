"""
Replication Script for WASP-12b Orbital Decay Rate (arXiv 2026).
Compares model predictions for dP/dt against the reported value -29.4 ms/yr.
"""

import numpy as np

from hot_jupiter.constants import AU, DAY, M_JUP, M_SUN, R_JUP, R_SUN, YEAR, G
from hot_jupiter.orbit.orbital_elements import TidalOrbitalSpinRates


def main():
    print("=================================================================")
    print("Replicating WASP-12b Orbital Decay Rate (arXiv 2026 Benchmark)")
    print("=================================================================")

    # WASP-12 System Parameters
    M_star = 1.35 * M_SUN
    R_star = 1.60 * R_SUN
    M_p = 1.404 * M_JUP
    R_p = 1.90 * R_JUP
    a = 0.0229 * AU
    e = 0.005  # Near-circular
    P_orb_sec = 1.09142 * DAY

    # Reported observational decay rate: -29.4 +/- 4.0 ms/yr
    target_dP_dt_ms_yr = -29.4

    print(
        f"System: WASP-12b | M_p = {M_p/M_JUP:.2f} M_J, R_p = {R_p/R_JUP:.2f} R_J"
    )
    print(
        f"Semi-Major Axis a = {a/AU:.4f} AU | P_orb = {P_orb_sec/DAY:.5f} days")
    print(f"Target Observed dP/dt = {target_dP_dt_ms_yr:.1f} ms/yr\n")

    # 1. Stellar Tide Driven Decay (Stellar Dissipation Q_star_prime)
    # da/dt |_star = - 9/2 * sqrt(G / M_star) * (k2_star / Q_star) * M_p * R_star^5 * a^(-11/2)
    Q_star_prime_values = [1.0e5, 1.8e5, 2.5e5, 1.0e6]

    print("--- Scenario A: Stellar Tidal Dissipation (Q'_star) ---")
    for Q_star_prime in Q_star_prime_values:
        n_orb = np.sqrt(G * M_star / (a**3))
        da_dt_star = -(9.0 / 2.0) * np.sqrt(
            G / M_star) * (1.0 / Q_star_prime) * M_p * (R_star**5) * (a**(-5.5))

        # dP/dt = 1.5 * (P_orb / a) * da/dt
        dP_dt_sec_sec = 1.5 * (P_orb_sec / a) * da_dt_star
        dP_dt_ms_yr = dP_dt_sec_sec * (1.0e3) * YEAR  # s/s to ms/yr

        print(
            f"  Q'_star = {Q_star_prime:.1e} --> da/dt = {da_dt_star:.4e} m/s | dP/dt = {dP_dt_ms_yr:.2f} ms/yr"
        )

    # 2. Planetary Tide Driven Decay (Planetary Dissipation Q_p_prime via TidalOrbitalSpinRates)
    print(
        "\n--- Scenario B: Planetary Equilibrium Tidal Dissipation (Q'_p) ---")
    Q_p_values = [1.0e5, 5.0e5, 1.0e6]
    for Q_p in Q_p_values:
        k2_p = 0.38
        k2_over_Q = k2_p / Q_p
        rates = TidalOrbitalSpinRates(k2_over_Q=k2_over_Q)

        # Mean motion
        n_orb = np.sqrt(G * M_star / (a**3))
        da_dt_p, _de_dt_p, _, _ = rates.evaluate_rates(M_p=M_p,
                                                       R_p=R_p,
                                                       M_star=M_star,
                                                       a=a,
                                                       e=e,
                                                       Omega_rot=n_orb,
                                                       obliquity=0.0)

        dP_dt_sec_sec = 1.5 * (P_orb_sec / a) * da_dt_p
        dP_dt_ms_yr = dP_dt_sec_sec * (1.0e3) * YEAR

        print(
            f"  Q'_p = {Q_p:.1e} (k2/Q = {k2_over_Q:.2e}) --> da/dt = {da_dt_p:.4e} m/s | dP/dt = {dP_dt_ms_yr:.2f} ms/yr"
        )

    # 3. Best-Fit Inferred Quality Factor
    # Target dP/dt = -29.4 ms/yr -> da/dt_target = dP/dt / (1.5 * P_orb / a)
    dP_dt_target_sec_sec = (target_dP_dt_ms_yr / 1.0e3) / YEAR
    da_dt_target = dP_dt_target_sec_sec / (1.5 * P_orb_sec / a)

    # Inferred Q'_star
    Q_star_prime_inferred = -(9.0 / 2.0) * np.sqrt(
        G / M_star) * M_p * (R_star**5) * (a**(-5.5)) / da_dt_target

    print("\n=================================================================")
    print("Replication Summary:")
    print("  To match WASP-12b observed decay (dP/dt = -29.4 ms/yr):")
    print(
        f"  Inferred Modified Stellar Quality Factor Q'_star = {Q_star_prime_inferred:.2e}"
    )
    print("=================================================================")


if __name__ == "__main__":
    main()
