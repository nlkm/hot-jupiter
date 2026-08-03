"""
Roche Lobe Overflow (RLOF) Mass-Loss Scenario Benchmark.
Demonstrates thermal radius inflation pushing an ultra-short-period Hot Jupiter (a = 0.018 AU)
to fill its Roche lobe (R_p / R_Roche >= 1.0), driving hydrodynamic mass stripping and orbital feedback.
"""

import os
import numpy as np
import matplotlib.pyplot as plt

from thermal_evolution.constants import M_JUP, M_EARTH, M_SUN, AU, YEAR, GYR, R_JUP
from thermal_evolution.mass_loss import RocheLobeMassLoss
from thermal_evolution.eos import TabularEOS
from thermal_evolution.structure import InteriorSolver
from thermal_evolution.atmosphere import GuillotAtmosphere
from thermal_evolution.heating import TidalEccentricityHeating
from thermal_evolution.orbit import OrbitalState, SpinVectorState
from thermal_evolution.evolution import ThermalEvolutionIntegrator


def run_rlof_scenario():
    print("==========================================================================")
    print("      ROCHE LOBE OVERFLOW (RLOF) MASS-LOSS SCENARIO BENCHMARK            ")
    print("==========================================================================")

    rlof_evaluator = RocheLobeMassLoss()

    # Planet & Star Parameters
    M_p_init = 1.0 * M_JUP
    M_star = 1.0 * M_SUN
    a_init = 0.018 * AU  # Ultra-close orbit (P_orb ~ 0.75 days)

    # Compute Roche Lobe Radius
    r_roche_m = rlof_evaluator.roche_lobe_radius(a_init, M_p_init, M_star)
    r_roche_jup = r_roche_m / R_JUP

    print(f"Initial Semi-Major Axis a:         {a_init/AU:.4f} AU")
    print(f"Volume-Equivalent Roche Lobe R:    {r_roche_jup:.3f} R_Jup ({r_roche_m/1e8:.2f} x 10^8 m)")

    # Time grid over 4.56 Gyr
    t_arr = np.linspace(1.0e6 * YEAR, 4.56 * GYR, 300)
    t_gyr = t_arr / GYR

    # Simulate Radius evolution & RLOF mass loss
    # Initial inflated radius R_p = 1.60 R_Jup
    R_p_track = np.zeros(len(t_arr))
    M_p_track = np.zeros(len(t_arr))
    filling_track = np.zeros(len(t_arr))
    dM_dt_track = np.zeros(len(t_arr))

    M_p_curr = M_p_init
    a_curr = a_init
    dt = t_arr[1] - t_arr[0]

    for i, t in enumerate(t_arr):
        # Thermal contraction with Ohmic/tidal inflation keeping R_p ~ 1.5 - 1.6 R_Jup
        R_p_curr = (1.65 - 0.25 * (t / (4.56 * GYR))**0.2) * R_JUP

        filling_factor = rlof_evaluator.roche_lobe_filling_factor(R_p_curr, a_curr, M_p_curr, M_star)
        dM_dt, da_dt_rlof = rlof_evaluator.evaluate_mass_loss_rate(R_p_curr, a_curr, M_p_curr, M_star)

        M_p_curr = max(0.1 * M_JUP, M_p_curr + dM_dt * dt)
        a_curr = max(0.01 * AU, a_curr + da_dt_rlof * dt)

        R_p_track[i] = R_p_curr / R_JUP
        M_p_track[i] = M_p_curr / M_JUP
        filling_track[i] = filling_factor
        dM_dt_track[i] = abs(dM_dt) / (M_EARTH / (1.0e9 * YEAR))  # M_earth / Gyr

    print(f"\nFinal State at 4.56 Gyr:")
    print(f"  Planet Mass M_p:                 {M_p_track[-1]:.3f} M_Jup (Lost {(1.0 - M_p_track[-1])*317.8:.1f} M_Earth)")
    print(f"  Final Filling Factor R_p/R_R:    {filling_track[-1]:.3f}")
    print(f"  Peak Mass-Loss Rate:             {np.max(dM_dt_track):.2f} M_Earth / Gyr")

    # Render Plot
    fig, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)
    fig.suptitle("Roche Lobe Overflow (RLOF) Mass-Loss & Atmospheric Stripping", fontsize=13, fontweight="bold")

    axes[0, 0].plot(t_gyr, R_p_track, color="#1f77b4", lw=2, label=r"$R_p$")
    axes[0, 0].axhline(r_roche_jup, color="#d62728", ls="--", lw=1.5, label=r"Roche Lobe $R_{\mathrm{Roche}}$")
    axes[0, 0].set_ylabel(r"Radius [$R_{\mathrm{Jup}}$]")
    axes[0, 0].grid(True, alpha=0.3)
    axes[0, 0].legend(loc="best")

    axes[0, 1].plot(t_gyr, filling_track, color="#ff7f0e", lw=2)
    axes[0, 1].axhline(1.0, color="#d62728", ls="--", lw=1.5, label="Overflow Threshold (1.0)")
    axes[0, 1].set_ylabel(r"Filling Factor $R_p / R_{\mathrm{Roche}}$")
    axes[0, 1].grid(True, alpha=0.3)
    axes[0, 1].legend(loc="best")

    axes[1, 0].plot(t_gyr, M_p_track, color="#2ca02c", lw=2)
    axes[1, 0].set_xlabel("Age [Gyr]")
    axes[1, 0].set_ylabel(r"Planet Mass $M_p$ [$M_{\mathrm{Jup}}$]")
    axes[1, 0].grid(True, alpha=0.3)

    axes[1, 1].plot(t_gyr, dM_dt_track, color="#9467bd", lw=2)
    axes[1, 1].set_xlabel("Age [Gyr]")
    axes[1, 1].set_ylabel(r"Mass-Loss Rate $\dot{M}_{\mathrm{RLOF}}$ [$M_\oplus/\mathrm{Gyr}$]")
    axes[1, 1].grid(True, alpha=0.3)

    plt.tight_layout()
    os.makedirs("outputs", exist_ok=True)
    fig_path = "outputs/roche_lobe_overflow_mass_loss.pdf"
    fig.savefig(fig_path, bbox_inches="tight")
    fig.savefig(fig_path.replace(".pdf", ".png"), dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"\nVector PDF figure saved to {fig_path}.\n")


if __name__ == "__main__":
    run_rlof_scenario()
