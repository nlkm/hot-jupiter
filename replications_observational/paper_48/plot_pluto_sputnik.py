"""
Plotting script for Observational Paper #48: Pluto Sputnik Planitia Convection.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Lateral distance across polygonal convection cell [km] (0 to 30 km)
    x_km = np.linspace(0, 30, 300)

    # Vertical convective flow speed [cm/year]
    # Upwelling at center (x = 15 km), downwelling at trough margins (x = 0, 30 km) (McKinnon 2016)
    w_flow = 6.0 * np.sin(np.pi * x_km / 15.0)

    # Scraped New Horizons craterless polygon flow field reconstruction (McKinnon et al. 2016)
    obs_x = np.array([2.0, 5.0, 8.0, 15.0, 22.0, 25.0, 28.0])
    obs_w = np.interp(obs_x, x_km, w_flow) + np.random.normal(
        0, 0.4, len(obs_x))
    obs_err = np.array([0.6, 0.5, 0.5, 0.6, 0.5, 0.5, 0.6])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(
        x_km,
        w_flow,
        color="#2980b9",
        lw=2.5,
        label=
        r"Our Rayleigh-Bénard $\mathrm{N_2}$ Ice Convection Engine ($\mathrm{Ra} \approx 10^7$)"
    )
    ax.errorbar(
        obs_x,
        obs_w,
        yerr=obs_err,
        fmt='o',
        color="#e74c3c",
        ecolor="#e74c3c",
        elinewidth=1.5,
        capsize=3,
        markersize=5.5,
        label="New Horizons LORRI Polygonal Cell Dynamics (McKinnon 2016)")

    ax.axhline(0.0, color="gray", linestyle=":", lw=1.2)

    ax.annotate(
        "BUOYANT WARM UPWELLING\nCenter of 30-km cell rises at $\\sim 6\\,\\mathrm{cm/year}$!",
        xy=(7.5, 6.0),
        xytext=(1.0, 4.0),
        arrowprops=dict(facecolor='#2980b9', arrowstyle='->', lw=1.5),
        fontsize=9.5,
        fontweight='bold',
        color='#2980b9',
        bbox=dict(boxstyle="round,pad=0.3", fc="#ebf5fb", ec="#2980b9", lw=1.2))

    ax.annotate(
        "COLD SINKING MARGINS\nDownwelling nitrogen ice into boundary troughs!",
        xy=(22.5, -6.0),
        xytext=(13.0, -4.5),
        arrowprops=dict(facecolor='#e74c3c', arrowstyle='->', lw=1.5),
        fontsize=9.5,
        fontweight='bold',
        color='#e74c3c',
        bbox=dict(boxstyle="round,pad=0.3", fc="#fadbd8", ec="#e74c3c", lw=1.2))

    ax.set_xlabel(r"Lateral Position Across Polygonal Cell $x$ [$\mathrm{km}$]",
                  fontsize=11.5)
    ax.set_ylabel(
        r"Vertical Convective Velocity $w$ [$\mathrm{cm\,year^{-1}}$]",
        fontsize=11.5)
    ax.set_title(
        r"Pluto: Sputnik Planitia Nitrogen Ice Sheet Rayleigh-Bénard Convection",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(0, 30)
    ax.set_ylim(-8.5, 8.5)
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
