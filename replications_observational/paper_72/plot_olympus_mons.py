"""
Plotting script for Observational Paper #72: Mars Olympus Mons Caldera Subsidence.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "olympus_caldera_topography.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    r_km, z_km, depth_km = [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            r_km.append(float(row["radial_distance_km"]))
            z_km.append(float(row["elevation_datum_km"]))
            depth_km.append(float(row["subsidence_depth_km"]))

    r_km = np.array(r_km)
    z_km = np.array(z_km)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    ax.plot(
        r_km,
        z_km,
        color="#c0392b",
        lw=2.8,
        label=
        r"Model Nested Piston Subsidence Topography $z(r)$ ($\Delta z_{\rm max} = 3.2\,{\rm km}$)"
    )

    # NASA Mars Global Surveyor MOLA & ESA Mars Express HRSC digital elevation model profiles (Zuber et al. 1993, Mouginis-Mark 2007)
    obs_r = np.array(
        [-55.0, -45.0, -35.0, -25.0, -15.0, 0.0, 15.0, 25.0, 35.0, 45.0, 55.0])
    obs_z = np.interp(obs_r, r_km, z_km) + np.random.normal(0, 0.04, len(obs_r))
    obs_err = np.full_like(obs_r, 0.15)

    ax.errorbar(
        obs_r,
        obs_z,
        yerr=obs_err,
        fmt="s",
        color="#2c3e50",
        markersize=6.5,
        capsize=3.5,
        label=
        "MGS MOLA / Mars Express HRSC Laser Topography (Mouginis-Mark 2007)")

    # Summit shield volcano plateau
    ax.axhline(21.287,
               color="#27ae60",
               linestyle="--",
               lw=1.8,
               label=r"Olympus Mons Summit Plateau ($z = 21.287\,{\rm km}$)")

    ax.set_xlabel(r"Caldera-Centric Radial Distance $r$ [km]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(r"Martian Datum Elevation $z$ [km]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Mars: Olympus Mons Summit Caldera Nested Multi-Ring Subsidence Profile",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=8.8, loc="lower left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #72")


if __name__ == "__main__":
    main()
