"""
Stellar Spin-Orbit Misalignment (Rossiter-McLaughlin Effect) Scenario Benchmark.
Demonstrates the coupled evolution of a planet in a misaligned / retrograde orbit
relative to its host star's spin axis (stellar obliquity psi_* = 80 deg and 135 deg).
"""

import os

import matplotlib.pyplot as plt
import numpy as np

from hot_jupiter.atmosphere import GuillotAtmosphere
from hot_jupiter.constants import (
    AU,
    GYR,
    M_EARTH,
    M_JUP,
    M_SUN,
    YEAR,
)
from hot_jupiter.eos import TabularEOS
from hot_jupiter.evolution import ThermalEvolutionIntegrator
from hot_jupiter.heating import TidalEccentricityHeating
from hot_jupiter.orbit import OrbitalState, SpinVectorState
from hot_jupiter.structure import InteriorSolver


def run_stellar_misaligned_scenario():
    print(
        "=========================================================================="
    )
    print(
        "   STELLAR SPIN-ORBIT MISALIGNMENT (ROSSITER-MCLAUGHLIN) BENCHMARK      "
    )
    print(
        "=========================================================================="
    )

    eos = TabularEOS.create_synthetic_grid(use_cache=False)
    solver = InteriorSolver(envelope_eos=eos)
    atmosphere = GuillotAtmosphere(envelope_eos=eos)
    tidal_heating = TidalEccentricityHeating(k2_over_Q=2.0e-5)

    integrator = ThermalEvolutionIntegrator(
        interior_solver=solver,
        atmosphere_model=atmosphere,
        heating_source=tidal_heating,
    )

    t_span = (1.0e6 * YEAR, 4.56e9 * YEAR)
    t_eval = np.geomspace(t_span[0], t_span[1], 100)
    t_gyr = t_eval / GYR

    # Case 1: Aligned Orbit (psi_* = 0 deg)
    print("Case 1: Aligned Orbit (psi_* = 0.0 deg)")
    orbit_1 = OrbitalState(a=0.04 * AU, e=0.10)
    spin_1 = SpinVectorState.from_period_hours(period_hrs=10.0,
                                               obliquity_deg=0.0)
    res_1 = integrator.evolve_coupled(
        S_initial=1.34e5,
        M_p=1.0 * M_JUP,
        M_c=12.0 * M_EARTH,
        M_star=1.0 * M_SUN,
        orbital_state_initial=orbit_1,
        spin_state_initial=spin_1,
        k2_over_Q=2.0e-5,
        t_span=t_span,
        num_eval=100,
    )

    # Case 2: Highly Misaligned Polar Orbit (psi_* = 80 deg)
    print("Case 2: Misaligned Polar Orbit (psi_* = 80.0 deg)")
    spin_2 = SpinVectorState.from_period_hours(period_hrs=10.0,
                                               obliquity_deg=80.0)
    res_2 = integrator.evolve_coupled(
        S_initial=1.34e5,
        M_p=1.0 * M_JUP,
        M_c=12.0 * M_EARTH,
        M_star=1.0 * M_SUN,
        orbital_state_initial=orbit_1,
        spin_state_initial=spin_2,
        k2_over_Q=2.0e-5,
        t_span=t_span,
        num_eval=100,
    )

    # Case 3: Retrograde Orbit (psi_* = 135 deg)
    print("Case 3: Retrograde Orbit (psi_* = 135.0 deg)")
    spin_3 = SpinVectorState.from_period_hours(period_hrs=10.0,
                                               obliquity_deg=135.0)
    res_3 = integrator.evolve_coupled(
        S_initial=1.34e5,
        M_p=1.0 * M_JUP,
        M_c=12.0 * M_EARTH,
        M_star=1.0 * M_SUN,
        orbital_state_initial=orbit_1,
        spin_state_initial=spin_3,
        k2_over_Q=2.0e-5,
        t_span=t_span,
        num_eval=100,
    )

    print(
        "\n--------------------------------------------------------------------------"
    )
    print("PRESENT-DAY METRICS AT 4.56 GYR:")
    print(
        "--------------------------------------------------------------------------"
    )
    print(
        f"Aligned Orbit (psi = 0 deg):    R_p = {res_1.R_p_jup[-1]:.3f} R_Jup, e = {res_1.e[-1]:.4f}, obl = {res_1.obliquity_deg[-1]:.1f} deg"
    )
    print(
        f"Polar Orbit (psi = 80 deg):     R_p = {res_2.R_p_jup[-1]:.3f} R_Jup, e = {res_2.e[-1]:.4f}, obl = {res_2.obliquity_deg[-1]:.1f} deg"
    )
    print(
        f"Retrograde Orbit (psi = 135 deg): R_p = {res_3.R_p_jup[-1]:.3f} R_Jup, e = {res_3.e[-1]:.4f}, obl = {res_3.obliquity_deg[-1]:.1f} deg"
    )
    print(
        "--------------------------------------------------------------------------"
    )

    # Render Comparison Plot
    fig, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)
    fig.suptitle(
        "Stellar Spin-Orbit Misalignment (Rossiter-McLaughlin Effect) Trajectories",
        fontsize=13,
        fontweight="bold")

    axes[0, 0].plot(t_gyr,
                    res_1.R_p_jup,
                    label=r"Aligned ($\psi_* = 0^\circ$)",
                    color="#1f77b4",
                    lw=2)
    axes[0, 0].plot(t_gyr,
                    res_2.R_p_jup,
                    label=r"Polar ($\psi_* = 80^\circ$)",
                    color="#ff7f0e",
                    lw=2)
    axes[0, 0].plot(t_gyr,
                    res_3.R_p_jup,
                    label=r"Retrograde ($\psi_* = 135^\circ$)",
                    color="#d62728",
                    lw=2)
    axes[0, 0].set_ylabel(r"Planet Radius $R_p$ [$R_{\mathrm{Jup}}$]")
    axes[0, 0].set_xscale("log")
    axes[0, 0].grid(True, alpha=0.3)
    axes[0, 0].legend(loc="best")

    axes[0, 1].plot(t_gyr, res_1.a_au, color="#1f77b4", lw=2)
    axes[0, 1].plot(t_gyr, res_2.a_au, color="#ff7f0e", lw=2)
    axes[0, 1].plot(t_gyr, res_3.a_au, color="#d62728", lw=2)
    axes[0, 1].set_ylabel(r"Semi-Major Axis $a$ [AU]")
    axes[0, 1].set_xscale("log")
    axes[0, 1].grid(True, alpha=0.3)

    axes[1, 0].plot(t_gyr, res_1.obliquity_deg, color="#1f77b4", lw=2)
    axes[1, 0].plot(t_gyr, res_2.obliquity_deg, color="#ff7f0e", lw=2)
    axes[1, 0].plot(t_gyr, res_3.obliquity_deg, color="#d62728", lw=2)
    axes[1, 0].set_xlabel("Age [Gyr]")
    axes[1, 0].set_ylabel(r"Stellar Obliquity $\psi_*$ [deg]")
    axes[1, 0].set_xscale("log")
    axes[1, 0].grid(True, alpha=0.3)

    axes[1, 1].plot(t_gyr, res_1.P_tidal, color="#1f77b4", lw=2)
    axes[1, 1].plot(t_gyr, res_2.P_tidal, color="#ff7f0e", lw=2)
    axes[1, 1].plot(t_gyr, res_3.P_tidal, color="#d62728", lw=2)
    axes[1, 1].set_xlabel("Age [Gyr]")
    axes[1, 1].set_ylabel(r"Tidal Power $P_{\mathrm{tidal}}$ [W]")
    axes[1, 1].set_xscale("log")
    axes[1, 1].set_yscale("log")
    axes[1, 1].grid(True, alpha=0.3, which="both")

    plt.tight_layout()
    os.makedirs("outputs", exist_ok=True)
    fig_path = "outputs/stellar_misaligned_orbit_evolution.pdf"
    fig.savefig(fig_path, bbox_inches="tight")
    fig.savefig(fig_path.replace(".pdf", ".png"), dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"\nVector PDF figure saved to {fig_path}.\n")


if __name__ == "__main__":
    run_stellar_misaligned_scenario()
