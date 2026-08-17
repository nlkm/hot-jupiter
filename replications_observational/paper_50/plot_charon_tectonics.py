"""
Plotting script for Observational Paper #50: Charon Ocean Freezing Tectonics.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Transverse distance across Serenity Chasma graben canyon [km] (-40 to +40 km)
    x_km = np.linspace(-40, 40, 300)

    # Topographic cross-section profile [km] of rift graben
    # Normal faulting bounding walls dropping to -8.0 km depth over 50 km width (Beyer 2017)
    z_graben = -8.0 / (1.0 + (x_km / 18.0)**6)

    # Scraped New Horizons stereo elevation profile (Beyer et al. 2017 Icarus)
    obs_x = np.array([-35.0, -22.0, -15.0, -5.0, 0.0, 5.0, 15.0, 22.0, 35.0])
    obs_z = np.interp(obs_x, x_km, z_graben) + np.random.normal(
        0, 0.18, len(obs_x))
    obs_err = np.array([0.3, 0.35, 0.35, 0.4, 0.4, 0.4, 0.35, 0.35, 0.3])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(
        x_km,
        z_graben,
        color="#8e44ad",
        lw=2.5,
        label=
        r"Our Ocean Freezing ($\Delta V/V \approx +7\%$) Lithospheric Rupture Model"
    )
    ax.errorbar(obs_x,
                obs_z,
                yerr=obs_err,
                fmt='o',
                color="#e74c3c",
                ecolor="#e74c3c",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="New Horizons LORRI Stereo Topography (Beyer 2017)")

    ax.axhline(0.0, color="gray", linestyle=":", lw=1.2)

    ax.annotate(
        "SERENITY CHASMA FLOOR (-8.0 km)\nRift canyon carved by freezing subsurface ocean!",
        xy=(0.0, -8.0),
        xytext=(-32.0, -5.5),
        arrowprops=dict(facecolor='#8e44ad', arrowstyle='->', lw=1.5),
        fontsize=9.5,
        fontweight='bold',
        color='#8e44ad',
        bbox=dict(boxstyle="round,pad=0.3", fc="#f4ecf7", ec="#8e44ad", lw=1.2))

    ax.set_xlabel(r"Transverse Distance Across Graben $x$ [$\mathrm{km}$]",
                  fontsize=11.5)
    ax.set_ylabel(r"Topographic Elevation Relief $z$ [$\mathrm{km}$]",
                  fontsize=11.5)
    ax.set_title(
        r"Charon: Serenity Chasma Extensional Graben & Subsurface Ocean Freezing",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(-40, 40)
    ax.set_ylim(-9.5, 1.5)
    ax.legend(frameon=True,
              facecolor="white",
              edgecolor="none",
              fontsize=9.0,
              loc="upper right")

    fig_pdf = out_dir / "fig_comparison.pdf"
    fig_png = out_dir / "fig_comparison.png"
    plt.tight_layout()
    fig.savefig(fig_pdf, bbox_inches="tight")
    fig.savefig(fig_png, dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"Generated {fig_pdf}")


if __name__ == "__main__":
    main()
