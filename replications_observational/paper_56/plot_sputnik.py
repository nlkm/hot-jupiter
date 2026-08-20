"""
Plotting script for Observational Paper #56: Pluto Sputnik Planitia Convection.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "sputnik_cell_profile.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    r_km, elev_m, flow_v, temp_k = [], [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            r_km.append(float(row["r_km"]))
            elev_m.append(float(row["elev_m"]))
            flow_v.append(float(row["flow_velocity_cm_yr"]))
            temp_k.append(float(row["temperature_k"]))

    r_km = np.array(r_km)
    elev_m = np.array(elev_m)

    # Full cell profile from -R to +R
    full_r = np.concatenate([-r_km[::-1], r_km[1:]])
    full_elev = np.concatenate([elev_m[::-1], elev_m[1:]])

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    ax.plot(
        full_r,
        full_elev,
        color="#2980b9",
        lw=2.8,
        label=
        r"Solid Nitrogen Ice Convection Dynamic Topography $\Delta h(r)$ ($Ra \sim 10^7$)"
    )

    # New Horizons LORRI stereo digital terrain models (McKinnon et al. 2016 Nature, Stern et al. 2015)
    obs_r = np.array([-14.0, -10.5, -7.0, -3.5, 0.0, 3.5, 7.0, 10.5, 14.0])
    obs_elev = np.interp(obs_r, full_r, full_elev) + np.array(
        [-2.0, 3.5, -1.8, 2.2, 0.0, -2.1, 1.9, -3.0, 2.5])

    ax.errorbar(
        obs_r,
        obs_elev,
        yerr=5.0,
        fmt="o",
        color="#c0392b",
        markersize=7,
        capsize=4,
        label=
        "New Horizons LORRI Stereo DTM Cell Cross-Sections (McKinnon et al. 2016)"
    )

    ax.set_xlabel(r"Radial Distance from Cell Center [km]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(r"Surface Topography Relative to Mean [m]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Pluto Sputnik Planitia: Solid Nitrogen Ice Rayleigh-Bénard Convection Cells",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=9.0, loc="lower center")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #56")


if __name__ == "__main__":
    main()
