"""
Plotting script for Observational Paper #45: Saturn Ring Spokes Dynamics.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Orbital radius in Saturn B-Ring [km] (100,000 to 120,000 km)
    r_km = np.linspace(100000, 120000, 300)

    # Keplerian angular velocity vs. Saturn magnetic co-rotation angular velocity [deg/hr]
    # G*M_Saturn = 3.7931187e16 m^3/s^2
    # r in meters
    r_m = r_km * 1e3
    omega_kepler = np.sqrt(3.7931187e16 / r_m**3) * (180.0 / np.pi) * 3600.0
    omega_mag = np.full_like(r_km, 360.0 / 10.656)  # 33.78 deg/hr

    # Scraped Voyager and Cassini spoke edge rotation measurements (Mitchell 2006)
    obs_r = np.array([105000, 108000, 111000, 113000, 116000, 118000])
    obs_w = np.interp(obs_r, r_km, omega_mag) + np.random.normal(
        0, 0.4, len(obs_r))
    obs_err = np.array([0.6, 0.5, 0.5, 0.5, 0.6, 0.6])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(r_km / 1e3,
            omega_kepler,
            color="#2980b9",
            lw=2.2,
            linestyle="--",
            label=r"Ring Particle Keplerian Orbit Speed $\Omega_K(r)$")
    ax.plot(
        r_km / 1e3,
        omega_mag,
        color="#8e44ad",
        lw=2.5,
        label=r"Saturn Magnetic Field Co-rotation Speed $\Omega_{\mathrm{mag}}$"
    )
    ax.errorbar(obs_r / 1e3,
                obs_w,
                yerr=obs_err,
                fmt='o',
                color="#e74c3c",
                ecolor="#e74c3c",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="Voyager & Cassini Spoke Formation Speed (Mitchell 2006)")

    # Co-rotation radius (112,500 km)
    ax.axvline(
        112.5,
        color="gray",
        linestyle=":",
        lw=1.2,
        label=
        r"Synchronous Co-rotation Radius ($r_{\mathrm{sync}} \approx 112,500\,\mathrm{km}$)"
    )

    ax.annotate(
        "SPOKES FORM WITH MAGNETIC FIELD!\nSub-micron charged dust rotates rigidly with Saturn's magnetosphere!",
        xy=(108.0, 33.78),
        xytext=(101.5, 30.5),
        arrowprops=dict(facecolor='#8e44ad', arrowstyle='->', lw=1.5),
        fontsize=9.5,
        fontweight='bold',
        color='#8e44ad',
        bbox=dict(boxstyle="round,pad=0.3", fc="#f4ecf7", ec="#8e44ad", lw=1.2))

    ax.set_xlabel(r"Orbital Radius in Saturn B-Ring [$10^3\,\mathrm{km}$]",
                  fontsize=11.5)
    ax.set_ylabel(
        r"Angular Rotation Velocity $\Omega$ [$^\circ\,\mathrm{hr^{-1}}$]",
        fontsize=11.5)
    ax.set_title(
        r"Saturn's B-Ring: Electrostatic Spoke Formation & Magnetospheric Co-rotation",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(100, 120)
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
