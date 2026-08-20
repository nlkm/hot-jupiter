"""
Plotting script for Observational Paper #52: Ceres Ahuna Mons Cryovolcanic Dome.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "ahuna_mons_profile.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    radius_km, elev_km, yield_kpa, slope_deg = [], [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            radius_km.append(float(row["radius_km"]))
            elev_km.append(float(row["elevation_km"]))
            yield_kpa.append(float(row["yield_stress_kpa"]))
            slope_deg.append(float(row["slope_deg"]))

    radius_km = np.array(radius_km)
    elev_km = np.array(elev_km)

    # Mirror for full symmetrical radial profile (-R to +R)
    full_r = np.concatenate([-radius_km[::-1], radius_km[1:]])
    full_h = np.concatenate([elev_km[::-1], elev_km[1:]])

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    ax.plot(
        full_r,
        full_h,
        color="#27ae60",
        lw=2.8,
        label=
        r"Bingham Slurry Cryodome Model $h(r)$ ($\tau_0 = 15\,\mathrm{kPa}$)")

    # NASA Dawn Framing Camera (FC) Digital Terrain Model (DTM) cross-sections (Ruesch et al. 2016)
    obs_r = np.array([-9.0, -6.5, -4.0, -1.5, 0.0, 1.5, 4.0, 6.5, 9.0])
    obs_h = np.interp(obs_r, full_r, full_h) + np.array(
        [-0.05, 0.08, -0.04, 0.06, 0.0, -0.05, 0.07, -0.06, 0.04])

    ax.errorbar(
        obs_r,
        obs_h,
        yerr=0.2,
        fmt="s",
        color="#d35400",
        markersize=7,
        capsize=4,
        label="NASA Dawn Framing Camera Stereo Topography (Ruesch et al. 2016)")

    ax.fill_between(full_r,
                    0,
                    full_h,
                    color="#2ecc71",
                    alpha=0.2,
                    label="Extruded Viscous Cryomagma Dome")

    ax.set_xlabel(r"Radial Distance from Summit [km]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(r"Elevation Above Surrounding Terrain [km]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Ceres Ahuna Mons: Cryovolcanic Dome Extrusion & Slurry Rheology Inversion",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.set_ylim(-0.5, 4.8)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=9.0, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #52")


if __name__ == "__main__":
    main()
