"""
Visualization module for thermal evolution tracks and interior hydrostatic profiles.
"""

from typing import Optional
import numpy as np
import matplotlib.pyplot as plt

from thermal_evolution.constants import R_JUP, L_SUN, GYR
from thermal_evolution.structure import PlanetStructure
from thermal_evolution.evolution import EvolutionResult


def plot_evolution_track(
    result: EvolutionResult,
    title: str = "Giant Planet Thermal Evolution Track",
    savepath: Optional[str] = None,
) -> plt.Figure:
    """
    Plot 4-panel evolutionary track: Radius, Luminosity, Temperatures, Entropy over time.
    """
    fig, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)
    fig.suptitle(title, fontsize=14, fontweight="bold")

    t_gyr = result.t_gyr

    # Panel 1: Planet Radius vs Time
    ax1 = axes[0, 0]
    ax1.plot(t_gyr, result.R_p_jup, "b-", lw=2)
    ax1.set_ylabel(r"Radius $R_p$ [$R_{\mathrm{Jup}}$]")
    ax1.set_xscale("log")
    ax1.grid(True, alpha=0.3)

    # Panel 2: Intrinsic Luminosity vs Time
    ax2 = axes[0, 1]
    ax2.plot(t_gyr, result.L_int_sun, "r-", lw=2)
    ax2.set_ylabel(r"Intrinsic Luminosity $L_{\mathrm{int}}$ [$L_\odot$]")
    ax2.set_xscale("log")
    ax2.set_yscale("log")
    ax2.grid(True, alpha=0.3)

    # Panel 3: Effective Temperatures vs Time
    ax3 = axes[1, 0]
    ax3.plot(t_gyr, result.T_eff, "g-", lw=2, label=r"$T_{\mathrm{eff}}$ (total)")
    ax3.plot(t_gyr, result.T_int, "g--", lw=2, label=r"$T_{\mathrm{int}}$ (intrinsic)")
    ax3.set_xlabel("Age [Gyr]")
    ax3.set_ylabel("Temperature [K]")
    ax3.set_xscale("log")
    ax3.legend(loc="best")
    ax3.grid(True, alpha=0.3)

    # Panel 4: Specific Entropy vs Time
    ax4 = axes[1, 1]
    ax4.plot(t_gyr, result.S, "m-", lw=2)
    ax4.set_xlabel("Age [Gyr]")
    ax4.set_ylabel(r"Envelope Entropy $S$ [J kg$^{-1}$ K$^{-1}$]")
    ax4.set_xscale("log")
    ax4.grid(True, alpha=0.3)

    plt.tight_layout()
    if savepath:
        fig.savefig(savepath, dpi=300, bbox_inches="tight")
    return fig


def plot_internal_profile(
    struct: PlanetStructure,
    title: str = "1D Interior Profile",
    savepath: Optional[str] = None,
) -> plt.Figure:
    """
    Plot 4-panel 1D interior hydrostatic profile: Density, Pressure, Temperature, nabla_ad.
    """
    if struct.profile is None:
        raise ValueError("PlanetStructure does not contain an internal profile.")

    prof = struct.profile
    r_norm = prof.r / R_JUP

    fig, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)
    fig.suptitle(f"{title} ($M_p = {struct.M_p/1.898e27:.2f} M_J$, $R_p = {struct.R_p/7.149e7:.2f} R_J$)", fontsize=14, fontweight="bold")

    # Panel 1: Mass Density
    ax1 = axes[0, 0]
    ax1.plot(r_norm, prof.rho / 1000.0, "k-", lw=2)  # g/cm^3
    ax1.set_ylabel(r"Density $\rho$ [g/cm$^3$]")
    ax1.axvline(struct.R_c / R_JUP, color="red", linestyle="--", alpha=0.7, label="Core Boundary")
    ax1.grid(True, alpha=0.3)
    ax1.legend(loc="best")

    # Panel 2: Pressure
    ax2 = axes[0, 1]
    ax2.plot(r_norm, prof.P / 1e11, "r-", lw=2)  # Mbar
    ax2.set_ylabel(r"Pressure $P$ [Mbar]")
    ax2.grid(True, alpha=0.3)

    # Panel 3: Temperature
    ax3 = axes[1, 0]
    ax3.plot(r_norm, prof.T, "b-", lw=2)
    ax3.set_xlabel(r"Radius $r$ [$R_{\mathrm{Jup}}$]")
    ax3.set_ylabel(r"Temperature $T$ [K]")
    ax3.grid(True, alpha=0.3)

    # Panel 4: Adiabatic Gradient nabla_ad
    ax4 = axes[1, 1]
    ax4.plot(r_norm, prof.nabla_ad, "g-", lw=2)
    ax4.set_xlabel(r"Radius $r$ [$R_{\mathrm{Jup}}$]")
    ax4.set_ylabel(r"Adiabatic Gradient $\nabla_{\mathrm{ad}}$")
    ax4.grid(True, alpha=0.3)

    plt.tight_layout()
    if savepath:
        fig.savefig(savepath, dpi=300, bbox_inches="tight")
    return fig
