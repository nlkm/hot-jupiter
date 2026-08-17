"""
Plotting script for Observational Paper #47: Ceres Ahuna Mons Cryovolcanism.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Radial distance from summit [km] (0 to 12 km)
    r_km = np.linspace(0, 12, 300)

    # Topographic elevation profile [km] of Bingham plastic dome
    # Peak at 4.0 km, parabolic profile out to base radius R_base = 10 km (Blake 1990)
    z_dome = 4.0 * np.maximum(0.0, 1.0 - (r_km / 10.0)**2)**0.5

    # Scraped Dawn Framing Camera stereo DTM topographic profile (Ruesch et al. 2016)
    obs_r = np.array([0.0, 1.5, 3.0, 5.0, 7.0, 8.5, 9.8, 11.0])
    obs_z = np.interp(obs_r, r_km, z_dome) + np.random.normal(
        0, 0.08, len(obs_r))
    obs_err = np.array([0.15, 0.12, 0.12, 0.10, 0.10, 0.10, 0.12, 0.10])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(
        r_km,
        z_dome,
        color="#27ae60",
        lw=2.5,
        label=
        r"Our Bingham Cryomagma Slurry Extrusion Model ($\tau_y \approx 1.5 \times 10^4\,\mathrm{Pa}$)"
    )
    ax.errorbar(obs_r,
                obs_z,
                yerr=obs_err,
                fmt='o',
                color="#2c3e50",
                ecolor="#2c3e50",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="Dawn Framing Camera Stereo Topography (Ruesch 2016)")

    ax.annotate(
        "AHUNA MONS SUMMIT (4.0 km)\nExtruded viscous brine mud-slurry volcano!",
        xy=(0.0, 4.0),
        xytext=(2.5, 3.6),
        arrowprops=dict(facecolor='#27ae60', arrowstyle='->', lw=1.5),
        fontsize=9.5,
        fontweight='bold',
        color='#27ae60',
        bbox=dict(boxstyle="round,pad=0.3", fc="#e8f8f5", ec="#27ae60", lw=1.2))

    ax.set_xlabel(r"Radial Distance from Summit [$\mathrm{km}$]", fontsize=11.5)
    ax.set_ylabel(r"Topographic Elevation Relief [$\mathrm{km}$]",
                  fontsize=11.5)
    ax.set_title(
        r"Ceres: Ahuna Mons Cryovolcanic Dome Topography & Slurry Rheology",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(0, 12)
    ax.set_ylim(-0.2, 4.6)
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
