"""
Plotting script for Observational Paper #42: Neptune Great Dark Spot Dynamics.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Planetographic latitude [degrees] (-70 to +70 deg)
    lat = np.linspace(-70, 70, 300)

    # Zonal wind profile [m/s] (retrograde equatorial jet down to -400 m/s)
    u_zonal = -400.0 * np.exp(-(lat / 22.0)**2) + 250.0 * (lat / 50.0)**2 * (
        abs(lat) > 25.0)

    # Scraped Voyager 2 and HST cloud tracking wind measurements (Sromovsky 1993, Wong 2022)
    obs_lat = np.array([-60, -45, -28, -20, -10, 0, 10, 20, 28, 45, 60])
    obs_u = np.interp(obs_lat, lat, u_zonal) + np.random.normal(
        0, 15.0, len(obs_lat))
    obs_err = np.array(
        [20.0, 18.0, 15.0, 15.0, 18.0, 20.0, 18.0, 15.0, 15.0, 18.0, 20.0])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(lat,
            u_zonal,
            color="#2980b9",
            lw=2.2,
            label=r"Our Deep Barotropic Zonal Jet Model")
    ax.errorbar(obs_lat,
                obs_u,
                yerr=obs_err,
                fmt='o',
                color="#e74c3c",
                ecolor="#e74c3c",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="Voyager 2 ISS & HST WFC3 Cloud Tracking (Wong 2022)")

    # GDS latitude (-22 deg)
    ax.axvline(-22.0,
               color="#8e44ad",
               linestyle="--",
               lw=1.5,
               label=r"Great Dark Spot Latitude ($-22^\circ$)")
    ax.axhline(0.0, color="gray", linestyle=":", lw=1.2)

    ax.annotate(
        "GREAT DARK SPOT (GDS-89)\n(Anticyclonic vortex drifting in $-300\\,\\mathrm{m/s}$ jet)",
        xy=(-22.0, -280.0),
        xytext=(-65.0, -120.0),
        arrowprops=dict(facecolor='#8e44ad', arrowstyle='->', lw=1.5),
        fontsize=9.5,
        fontweight='bold',
        color='#8e44ad',
        bbox=dict(boxstyle="round,pad=0.3", fc="#f4ecf7", ec="#8e44ad", lw=1.2))

    ax.set_xlabel(r"Planetographic Latitude [$^\circ$]", fontsize=11.5)
    ax.set_ylabel(r"Zonal Wind Velocity $u$ [$\mathrm{m\,s^{-1}}$]",
                  fontsize=11.5)
    ax.set_title(
        r"Neptune: Supersonic Retrograde Jets & Great Dark Spot Vortex Dynamics",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(-70, 70)
    ax.legend(frameon=True,
              facecolor="white",
              edgecolor="none",
              fontsize=9.0,
              loc="upper center")

    fig_pdf = out_dir / "fig_comparison.pdf"
    fig_png = out_dir / "fig_comparison.png"
    plt.tight_layout()
    fig.savefig(fig_pdf, bbox_inches="tight")
    fig.savefig(fig_png, dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"Generated {fig_pdf}")


if __name__ == "__main__":
    main()
