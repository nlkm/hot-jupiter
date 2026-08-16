"""
Publication-Grade Visualization Engine & Design System for LaTeX / Astronomy Papers.
Implements unified rcParams, curated color palettes, panel labeling, and vector figure exports.
"""

from typing import ClassVar

import matplotlib.pyplot as plt
import numpy as np

from hot_jupiter.constants import R_JUP
from hot_jupiter.evolution import EvolutionResult
from hot_jupiter.structure import PlanetStructure


class PaperStyle:
    """
    Unified Design System and Theme Manager for Publication-Grade Figures.
    """

    # Curated Harmonious HSL Color Palette
    COLORS: ClassVar[dict[str, str]] = {
        'ZONE_I': '#d95f02',  # Vibrant Dark Orange (Disruption Zone)
        'ZONE_II': '#7570b3',  # Rich Deep Violet (Stagnation Zone)
        'ZONE_III': '#1b9e77',  # Teal Green (Cooling / Survival Zone)
        'GAS_GIANT': '#2b5c8f',  # Sleek Royal Blue
        'ROCKY_CORE': '#e66101',  # Rich Ochre / Terrestrial
        'FORBIDDEN': '#b2182b',  # Crimson Red
        'REFERENCE': '#666666',  # Dark Slate Grey
        'HIGHLIGHT': '#e7298a',  # Hot Pink / Callout
    }

    @classmethod
    def apply(cls):
        """Configure matplotlib global rcParams for publication-quality graphics."""
        plt.rcParams.update({
            'font.family': 'sans-serif',
            'font.size': 10,
            'axes.titlesize': 11.5,
            'axes.titleweight': 'bold',
            'axes.labelsize': 10.5,
            'xtick.labelsize': 9.5,
            'ytick.labelsize': 9.5,
            'legend.fontsize': 9.0,
            'figure.titlesize': 12.5,
            'figure.titleweight': 'bold',
            'figure.dpi': 300,
            'savefig.dpi': 300,
            'savefig.bbox': 'tight',
            'axes.grid': True,
            'grid.linestyle': '--',
            'grid.alpha': 0.35,
            'legend.frameon': True,
            'legend.facecolor': 'white',
            'legend.framealpha': 0.9,
            'lines.linewidth': 2.0,
        })

    @staticmethod
    def add_panel_label(ax: plt.Axes, label: str, loc: str = 'top left'):
        """
        Add a bold panel identification tag (e.g. '(a)', '(b)') to a subplot axis.
        """
        label_text = f'({label.lower()})'
        if loc == 'top left':
            ax.text(0.03,
                    0.94,
                    label_text,
                    transform=ax.transAxes,
                    fontsize=11,
                    fontweight='bold',
                    va='top',
                    ha='left',
                    bbox=dict(boxstyle='round,pad=0.2',
                              facecolor='white',
                              alpha=0.85,
                              edgecolor='none'))
        elif loc == 'top right':
            ax.text(0.97,
                    0.94,
                    label_text,
                    transform=ax.transAxes,
                    fontsize=11,
                    fontweight='bold',
                    va='top',
                    ha='right',
                    bbox=dict(boxstyle='round,pad=0.2',
                              facecolor='white',
                              alpha=0.85,
                              edgecolor='none'))

    @staticmethod
    def save_figure(fig: plt.Figure, filepath: str):
        """Save publication figure as PNG (300 DPI) and vector PDF."""
        fig.savefig(filepath, dpi=300, bbox_inches='tight')
        if filepath.endswith('.png'):
            pdf_path = filepath[:-4] + '.pdf'
            fig.savefig(pdf_path, bbox_inches='tight')


def plot_evolution_track(
    result: EvolutionResult,
    title: str = "Giant Planet Thermal Evolution Track",
    savepath: str | None = None,
) -> plt.Figure:
    """Plot 4-panel evolutionary track: Radius, Luminosity, Temperatures, Entropy over time."""
    PaperStyle.apply()
    fig, axes = plt.subplots(2, 2, figsize=(9.5, 6.5), sharex=True)
    fig.suptitle(title, fontsize=12.5, fontweight="bold")

    t_gyr = result.t_gyr

    # Panel 1: Planet Radius vs Time
    ax1 = axes[0, 0]
    ax1.plot(t_gyr, result.R_p_jup, color=PaperStyle.COLORS['GAS_GIANT'], lw=2)
    ax1.set_ylabel(r"Radius $R_p$ [$R_{\mathrm{Jup}}$]")
    ax1.set_xscale("log")
    PaperStyle.add_panel_label(ax1, "a")

    # Panel 2: Intrinsic Luminosity vs Time
    ax2 = axes[0, 1]
    ax2.plot(t_gyr, result.L_int_sun, color=PaperStyle.COLORS['ZONE_I'], lw=2)
    ax2.set_ylabel(r"Luminosity $L_{\mathrm{int}}$ [$L_\odot$]")
    ax2.set_xscale("log")
    ax2.set_yscale("log")
    PaperStyle.add_panel_label(ax2, "b")

    # Panel 3: Effective Temperatures vs Time
    ax3 = axes[1, 0]
    ax3.plot(t_gyr,
             result.T_eff,
             color=PaperStyle.COLORS['ZONE_III'],
             lw=2,
             label=r"$T_{\mathrm{eff}}$")
    ax3.plot(t_gyr,
             result.T_int,
             color=PaperStyle.COLORS['ZONE_III'],
             lw=2,
             ls='--',
             label=r"$T_{\mathrm{int}}$")
    ax3.set_xlabel("Age [Gyr]")
    ax3.set_ylabel("Temperature [K]")
    ax3.set_xscale("log")
    ax3.legend(loc="best")
    PaperStyle.add_panel_label(ax3, "c")

    # Panel 4: Specific Entropy vs Time
    ax4 = axes[1, 1]
    ax4.plot(t_gyr, result.S, color=PaperStyle.COLORS['ZONE_II'], lw=2)
    ax4.set_xlabel("Age [Gyr]")
    ax4.set_ylabel(r"Entropy $S$ [J kg$^{-1}$ K$^{-1}$]")
    ax4.set_xscale("log")
    PaperStyle.add_panel_label(ax4, "d")

    plt.tight_layout()
    if savepath:
        PaperStyle.save_figure(fig, savepath)
    return fig


def plot_internal_profile(
    struct: PlanetStructure,
    title: str = "1D Interior Hydrostatic Profile",
    savepath: str | None = None,
) -> plt.Figure:
    """Plot 4-panel 1D interior hydrostatic profile."""
    if struct.profile is None:
        raise ValueError(
            "PlanetStructure does not contain an internal profile.")

    PaperStyle.apply()
    prof = struct.profile
    r_norm = prof.r / R_JUP

    fig, axes = plt.subplots(2, 2, figsize=(9.5, 6.5), sharex=True)
    fig.suptitle(
        f"{title} ($M_p = {struct.M_p/1.898e27:.2f} M_J$, $R_p = {struct.R_p/7.149e7:.2f} R_J$)",
        fontsize=12.5,
        fontweight="bold",
    )

    # Panel 1: Density
    ax1 = axes[0, 0]
    ax1.plot(r_norm, prof.rho, color=PaperStyle.COLORS['GAS_GIANT'], lw=2)
    ax1.set_ylabel(r"Density $\rho$ [kg m$^{-3}$]")
    ax1.set_yscale("log")
    PaperStyle.add_panel_label(ax1, "a")

    # Panel 2: Pressure
    ax2 = axes[0, 1]
    ax2.plot(r_norm, prof.P / 1e9, color=PaperStyle.COLORS['ZONE_I'], lw=2)
    ax2.set_ylabel(r"Pressure $P$ [GPa]")
    ax2.set_yscale("log")
    PaperStyle.add_panel_label(ax2, "b")

    # Panel 3: Temperature
    ax3 = axes[1, 0]
    ax3.plot(r_norm, prof.T, color=PaperStyle.COLORS['ZONE_III'], lw=2)
    ax3.set_xlabel(r"Radius $r$ [$R_{\mathrm{Jup}}$]")
    ax3.set_ylabel("Temperature $T$ [K]")
    PaperStyle.add_panel_label(ax3, "c")

    # Panel 4: Adiabatic Gradient
    ax4 = axes[1, 1]
    ax4.plot(r_norm, prof.nabla_ad, color=PaperStyle.COLORS['ZONE_II'], lw=2)
    ax4.set_xlabel(r"Radius $r$ [$R_{\mathrm{Jup}}$]")
    ax4.set_ylabel(
        r"$\nabla_{\mathrm{ad}} \equiv (\partial \ln T / \partial \ln P)_s$")
    PaperStyle.add_panel_label(ax4, "d")

    plt.tight_layout()
    if savepath:
        PaperStyle.save_figure(fig, savepath)
    return fig


def plot_coupled_orbital_spin_evolution(
    result,
    title: str = "Coupled Thermal, Orbital Element & Spin Vector Evolution",
    savepath: str | None = None,
) -> plt.Figure:
    """Plot 4-panel coupled dynamical evolution track."""
    PaperStyle.apply()
    fig, axes = plt.subplots(2, 2, figsize=(9.5, 6.5), sharex=True)
    fig.suptitle(title, fontsize=12.5, fontweight="bold")

    t_gyr = result.t_gyr

    # Panel 1: Radius
    ax1 = axes[0, 0]
    ax1.plot(t_gyr, result.R_p_jup, color=PaperStyle.COLORS['GAS_GIANT'], lw=2)
    ax1.set_ylabel(r"Radius $R_p$ [$R_{\mathrm{Jup}}$]")
    ax1.set_xscale("log")
    PaperStyle.add_panel_label(ax1, "a")

    # Panel 2: Semi-major axis & Eccentricity
    ax2 = axes[0, 1]
    ax2.plot(t_gyr,
             result.a_au,
             color=PaperStyle.COLORS['ZONE_I'],
             lw=2,
             label="a [AU]")
    ax2_twin = ax2.twinx()
    ax2_twin.plot(t_gyr,
                  result.e,
                  color=PaperStyle.COLORS['ZONE_III'],
                  lw=1.5,
                  ls="--",
                  label="e")
    ax2.set_ylabel("Semi-major Axis a [AU]")
    ax2_twin.set_ylabel("Eccentricity e")
    ax2.set_xscale("log")
    PaperStyle.add_panel_label(ax2, "b")

    # Panel 3: Rotation Period & Obliquity
    ax3 = axes[1, 0]
    ax3.plot(t_gyr,
             result.P_rot_hrs,
             color=PaperStyle.COLORS['ZONE_II'],
             lw=2,
             label=r"$P_{\mathrm{rot}}$ [hrs]")
    ax3.set_xlabel("Age [Gyr]")
    ax3.set_ylabel(r"Rotation Period $P_{\mathrm{rot}}$ [hrs]")
    ax3.set_xscale("log")
    PaperStyle.add_panel_label(ax3, "c")

    # Panel 4: Tidal Power
    ax4 = axes[1, 1]
    ax4.plot(t_gyr,
             np.maximum(result.P_tidal, 1.0),
             color=PaperStyle.COLORS['HIGHLIGHT'],
             lw=2)
    ax4.set_xlabel("Age [Gyr]")
    ax4.set_ylabel(r"Tidal Power $P_{\mathrm{tidal}}$ [W]")
    ax4.set_xscale("log")
    ax4.set_yscale("log")
    PaperStyle.add_panel_label(ax4, "d")

    plt.tight_layout()
    if savepath:
        PaperStyle.save_figure(fig, savepath)
    return fig


def plot_multi_planet_system_evolution(
    result,
    title: str = "Coupled Multi-Planet System Evolution",
    savepath: str | None = None,
) -> plt.Figure:
    """Plot 4-panel multi-planet system evolution track."""
    PaperStyle.apply()
    fig, axes = plt.subplots(2, 2, figsize=(9.5, 6.5), sharex=True)
    fig.suptitle(title, fontsize=12.5, fontweight="bold")

    t_gyr = result.t_gyr
    colors = [
        PaperStyle.COLORS['GAS_GIANT'], PaperStyle.COLORS['ZONE_I'],
        PaperStyle.COLORS['ZONE_III']
    ]

    # Panel 1: Semi-major axes
    ax1 = axes[0, 0]
    for i, name in enumerate(result.planet_names):
        c = colors[i % len(colors)]
        ax1.plot(t_gyr,
                 result.a_au[name],
                 color=c,
                 lw=2,
                 label=f"Planet {name}")
    ax1.set_ylabel("Semi-major Axis a [AU]")
    ax1.set_xscale("log")
    ax1.legend(loc="best")
    PaperStyle.add_panel_label(ax1, "a")

    # Panel 2: Eccentricities
    ax2 = axes[0, 1]
    for i, name in enumerate(result.planet_names):
        c = colors[i % len(colors)]
        ax2.plot(t_gyr, result.e[name], color=c, lw=2, label=f"Planet {name}")
    ax2.set_ylabel("Eccentricity e")
    ax2.set_xscale("log")
    PaperStyle.add_panel_label(ax2, "b")

    # Panel 3: Radii
    ax3 = axes[1, 0]
    for i, name in enumerate(result.planet_names):
        c = colors[i % len(colors)]
        ax3.plot(t_gyr,
                 result.R_p_jup[name],
                 color=c,
                 lw=2,
                 label=f"Planet {name}")
    ax3.set_xlabel("Age [Gyr]")
    ax3.set_ylabel(r"Radius $R_p$ [$R_{\mathrm{Jup}}$]")
    ax3.set_xscale("log")
    PaperStyle.add_panel_label(ax3, "c")

    # Panel 4: Effective Temperature
    ax4 = axes[1, 1]
    for i, name in enumerate(result.planet_names):
        c = colors[i % len(colors)]
        ax4.plot(t_gyr,
                 result.T_eff[name],
                 color=c,
                 lw=2,
                 label=f"Planet {name}")
    ax4.set_xlabel("Age [Gyr]")
    ax4.set_ylabel(r"Effective Temp $T_{\mathrm{eff}}$ [K]")
    ax4.set_xscale("log")
    PaperStyle.add_panel_label(ax4, "d")

    plt.tight_layout()
    if savepath:
        PaperStyle.save_figure(fig, savepath)
    return fig
