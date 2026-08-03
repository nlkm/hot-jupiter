"""
Stellar Rotation & Stellar Tidal Interaction Benchmark.
Demonstrates sub-synchronous inward orbital decay vs super-synchronous outward orbital expansion
driven by host star rotation frequency Omega_*.
"""

import os
import numpy as np
import matplotlib.pyplot as plt

from thermal_evolution.constants import M_JUP, M_EARTH, M_SUN, AU, YEAR, GYR, DAY, HOUR
from thermal_evolution.orbit import StellarTidalRates


def run_stellar_rotation_benchmark():
    print("==========================================================================")
    print("   STELLAR ROTATION & STELLAR TIDAL INTERACTION BENCHMARK VALIDATION    ")
    print("==========================================================================")

    stellar_evaluator = StellarTidalRates(k2_over_Q_star=1.0e-6)

    # System parameters: M_p = 1.0 M_Jup, M_star = 1.0 M_Sun
    M_p = 1.0 * M_JUP
    M_star = 1.0 * M_SUN

    # Time grid: 0 to 4.56 Gyr
    t_arr = np.linspace(0, 4.56 * GYR, 500)
    t_gyr = t_arr / GYR

    # Scenario 1: Sub-Synchronous Slow Rotating Star (P_star = 25 days, P_orb = 2.0 days at a = 0.03 AU)
    # n > Omega_* => Inward migration (da/dt < 0)
    a_init_1 = 0.030 * AU
    P_star_1_sec = 25.0 * DAY
    Omega_star_1 = 2.0 * np.pi / P_star_1_sec

    a_track_1 = np.zeros(len(t_arr))
    a_curr_1 = a_init_1
    dt = t_arr[1] - t_arr[0]

    for i in range(len(t_arr)):
        a_track_1[i] = a_curr_1 / AU
        da_dt, _ = stellar_evaluator.evaluate_stellar_rates(M_p, M_star, a_curr_1, Omega_star_1)
        a_curr_1 = max(0.005 * AU, a_curr_1 + da_dt * dt)

    # Scenario 2: Super-Synchronous Fast Rotating Star (P_star = 1.5 days, P_orb = 3.0 days at a = 0.04 AU)
    # n < Omega_* => Outward expansion (da/dt > 0)
    a_init_2 = 0.040 * AU
    P_star_2_sec = 1.5 * DAY
    Omega_star_2 = 2.0 * np.pi / P_star_2_sec

    a_track_2 = np.zeros(len(t_arr))
    a_curr_2 = a_init_2

    for i in range(len(t_arr)):
        a_track_2[i] = a_curr_2 / AU
        da_dt, _ = stellar_evaluator.evaluate_stellar_rates(M_p, M_star, a_curr_2, Omega_star_2)
        a_curr_2 = a_curr_2 + da_dt * dt

    print(f"Sub-synchronous Star (P_star = 25 days): Initial a = {a_init_1/AU:.3f} AU -> Final a = {a_track_1[-1]:.3f} AU (Inward Decay)")
    print(f"Super-synchronous Star (P_star = 1.5 days): Initial a = {a_init_2/AU:.3f} AU -> Final a = {a_track_2[-1]:.3f} AU (Outward Expansion)")

    # Render Plot
    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(t_gyr, a_track_1, label=r"Sub-synchronous Star ($P_* = 25\text{ days}$, Inward Decay)", color="#d62728", lw=2)
    ax.plot(t_gyr, a_track_2, label=r"Super-synchronous Star ($P_* = 1.5\text{ days}$, Outward Expansion)", color="#2ca02c", lw=2)
    ax.set_xlabel("Age [Gyr]", fontsize=11)
    ax.set_ylabel(r"Semi-Major Axis $a$ [AU]", fontsize=11)
    ax.set_title("Stellar Rotation Driven Tidal Orbital Migration", fontsize=12, fontweight="bold")
    ax.grid(True, alpha=0.3)
    ax.legend(loc="best")

    os.makedirs("outputs", exist_ok=True)
    fig_path = "outputs/stellar_rotation_tidal_migration.pdf"
    fig.savefig(fig_path, bbox_inches="tight")
    fig.savefig(fig_path.replace(".pdf", ".png"), dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"\nVector PDF figure saved to {fig_path}.\n")


if __name__ == "__main__":
    run_stellar_rotation_benchmark()
