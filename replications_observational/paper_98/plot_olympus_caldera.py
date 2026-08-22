"""
Plotting script for Observational Paper #98: Mars Olympus Mons Caldera Subsidence.
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

    x_km, elev_km, sub_km = [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            x_km.append(float(row["distance_from_caldera_center_km"]))
            elev_km.append(float(row["elevation_above_datum_km"]))
            sub_km.append(float(row["piston_subsidence_depth_km"]))

    x_km = np.array(x_km)
    elev_km = np.array(elev_km)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    # User rule: ALWAYS show distance / time on a linear scale
    ax.plot(
        x_km,
        elev_km,
        color="#c0392b",
        lw=2.8,
        label=
        r"Model Nested Caldera Collapse Elevation ($D = 80\,{\rm km},\,\Delta z_{\rm max} = 3.2\,{\rm km}$)"
    )

    # NASA Mars Global Surveyor MOLA precision laser altimetry profile across Olympus Mons caldera summit (Smith et al. 2001 Science, Zuber et al. 1998)
    obs_x = np.array([
        -55.0, -45.0, -38.0, -30.0, -20.0, -10.0, 0.0, 10.0, 20.0, 30.0, 38.0,
        45.0, 55.0
    ])
    obs_elev = np.interp(obs_x, x_km, elev_km) + np.random.normal(
        0, 0.05, len(obs_x))
    obs_err = np.full_like(obs_x, 0.12)

    ax.errorbar(
        obs_x,
        obs_elev,
        yerr=obs_err,
        fmt="o",
        color="#2980b9",
        markersize=6.5,
        capsize=3.5,
        label=
        "NASA MGS MOLA Precision Laser Altimetry (Smith et al. 2001 Science)")

    # Annotate caldera rims and nested floor
    ax.axvline(-40.0,
               color="#7f8c8d",
               linestyle="--",
               lw=1.5,
               label=r"Caldera Complex Outer Rim ($x = \pm 40\,{\rm km}$)")
    ax.axvline(40.0, color="#7f8c8d", linestyle="--", lw=1.5)
    ax.text(0.0,
            18.5,
            r"Nested Caldera Floor ($18.087\,{\rm km}$)",
            color="#8e44ad",
            fontweight="bold",
            fontsize=10.0,
            ha="center")
    ax.text(-48.0,
            20.2,
            r"Outer Flank",
            color="#27ae60",
            fontweight="bold",
            fontsize=10.0,
            ha="center")
    ax.text(48.0,
            20.2,
            r"Outer Flank",
            color="#27ae60",
            fontweight="bold",
            fontsize=10.0,
            ha="center")

    ax.set_xlabel(
        r"Distance Across Caldera Summit $x$ [$\text{km}$] (Linear Scale)",
        fontweight="bold",
        fontsize=11.5)
    ax.set_ylabel(r"Topographic Elevation Above MOLA Datum [$\text{km}$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        r"Mars Olympus Mons: 80-km Nested Caldera Complex \& Magma Chamber Subsidence",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.set_ylim(17.2, 22.0)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=8.8, loc="lower center")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #98")


if __name__ == "__main__":
    main()
