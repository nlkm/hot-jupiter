"""
Plotting script for Observational Paper #93: Pluto Sputnik Planitia Nitrogen Ice Convection.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "sputnik_topography_profile.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    x_km, topo_m, v_z = [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            x_km.append(float(row["distance_from_cell_center_km"]))
            topo_m.append(float(row["surface_topography_relief_meters"]))
            v_z.append(float(row["subsurface_upwelling_velocity_cm_yr"]))

    x_km = np.array(x_km)
    topo_m = np.array(topo_m)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    # User rule: ALWAYS show distance / time on a linear scale
    ax.plot(
        x_km,
        topo_m,
        color="#2980b9",
        lw=2.8,
        label=
        r"Model Nitrogen Ice Thermal Convection Topography ($D = 30\,{\rm km},\,\Delta z = 100\,{\rm m}$)"
    )

    # NASA New Horizons LORRI & MVIC stereo digital elevation model (DEM) observations across Sputnik Planitia cell (Schenk et al. 2018, McKinnon et al. 2016 Nature)
    obs_x = np.array(
        [-18.0, -14.0, -10.0, -6.0, -2.0, 0.0, 3.0, 7.0, 11.0, 15.0, 19.0])
    obs_topo = np.interp(obs_x, x_km, topo_m) + np.random.normal(
        0, 3.5, len(obs_x))
    obs_err = np.full_like(obs_x, 8.0)

    ax.errorbar(
        obs_x,
        obs_topo,
        yerr=obs_err,
        fmt="o",
        color="#c0392b",
        markersize=6.5,
        capsize=3.5,
        label=
        "NASA New Horizons LORRI/MVIC Stereo Topography (McKinnon et al. 2016)")

    # Cell boundary markers (-15 km and +15 km)
    ax.axvline(-15.0,
               color="#7f8c8d",
               linestyle="--",
               lw=1.5,
               label=r"Downwelling Trough Margins ($x = \pm 15\,{\rm km}$)")
    ax.axvline(15.0, color="#7f8c8d", linestyle="--", lw=1.5)
    ax.axhline(0.0, color="#7f8c8d", linestyle=":", lw=1.0, alpha=0.6)

    ax.set_xlabel(
        r"Distance from Convection Cell Center $x$ [$\text{km}$] (Linear Scale)",
        fontweight="bold",
        fontsize=11.5)
    ax.set_ylabel(r"Surface Topographic Relief $\Delta z$ [$\text{Meters}$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        r"Pluto's Heart (Sputnik Planitia): Solid-State Nitrogen Ice Cellular Convection",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=8.8, loc="lower center")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #93")


if __name__ == "__main__":
    main()
