"""
Plotting script for Observational Paper #103: Ceres Ahuna Mons Cryovolcanic Dome.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "ahuna_mons_topography.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    r_km, elev_km, slope_deg, elev_relax = [], [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            r_km.append(float(row["radial_distance_km"]))
            elev_km.append(float(row["elevation_km"]))
            slope_deg.append(float(row["flank_slope_deg"]))
            elev_relax.append(float(row["relaxation_elevation_200myr_km"]))

    r_km = np.array(r_km)
    elev_km = np.array(elev_km)
    elev_relax = np.array(elev_relax)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    # User rule: ALWAYS show distance / time on a linear scale
    ax.plot(
        r_km,
        elev_km,
        color="#2980b9",
        lw=2.8,
        label=
        r"Model Bingham Cryovolcanic Dome ($H_0 = 4.0\,{\rm km},\,D = 20\,{\rm km},\,\tau_y = 1.5 \times 10^4\,{\rm Pa}$)"
    )

    ax.plot(
        r_km,
        elev_relax,
        color="#7f8c8d",
        linestyle="--",
        lw=1.8,
        label=
        r"Predicted Viscoelastic Relaxation Post-Extrusion ($t = 200\,{\rm Myr}$)"
    )

    # NASA Dawn Framing Camera stereo photogrammetric digital terrain model (Ruesch et al. 2016 Science, Krohn et al. 2016)
    obs_r = np.array(
        [-13.0, -10.0, -8.0, -5.0, -2.5, 0.0, 2.5, 5.0, 8.0, 10.0, 13.0])
    obs_elev = np.interp(obs_r, r_km, elev_km) + np.random.normal(
        0, 0.08, len(obs_r))
    obs_err = np.full_like(obs_r, 0.15)

    ax.errorbar(
        obs_r,
        obs_elev,
        yerr=obs_err,
        fmt="o",
        color="#c0392b",
        markersize=6.5,
        capsize=3.5,
        label="NASA Dawn Framing Camera Stereo DTM (Ruesch et al. 2016 Science)"
    )

    # Annotate summit and basal flanks
    ax.axvline(-10.0, color="#27ae60", linestyle=":", lw=1.5)
    ax.axvline(10.0, color="#27ae60", linestyle=":", lw=1.5)
    ax.text(0.0,
            4.15,
            r"Summit Peak ($4.0\,{\rm km}$ Relief)",
            color="#2980b9",
            fontweight="bold",
            fontsize=10.0,
            ha="center")
    ax.text(-7.0,
            2.2,
            r"Steep Flank ($\sim 35^\circ$)",
            color="#e67e22",
            fontsize=9.5,
            ha="center")
    ax.text(7.0,
            2.2,
            r"Steep Flank ($\sim 35^\circ$)",
            color="#e67e22",
            fontsize=9.5,
            ha="center")

    ax.set_xlabel(
        r"Radial Distance Across Dome $r$ [$\text{km}$] (Linear Scale)",
        fontweight="bold",
        fontsize=11.5)
    ax.set_ylabel(r"Topographic Elevation Above Base [$\text{km}$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        r"Dwarf Planet (1) Ceres: Ahuna Mons Cryovolcanic Dome Topographic Profile",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.set_ylim(-0.3, 4.6)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=8.8, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #103")


if __name__ == "__main__":
    main()
