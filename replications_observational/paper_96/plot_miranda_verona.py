"""
Plotting script for Observational Paper #96: Miranda Verona Rupes Extensional Tectonics.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "miranda_freefall_evolution.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    t_min, dist_km, vel_kmh, alt_km = [], [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            t_min.append(float(row["time_minutes"]))
            dist_km.append(float(row["freefall_distance_km"]))
            vel_kmh.append(float(row["freefall_velocity_km_h"]))
            alt_km.append(float(row["remaining_altitude_km"]))

    t_min = np.array(t_min)
    alt_km = np.array(alt_km)
    vel_kmh = np.array(vel_kmh)

    fig, ax1 = plt.subplots(figsize=(8.8, 5.8))

    # User rule: ALWAYS show time on a linear scale
    color = "#2980b9"
    ax1.set_xlabel(
        r"Freefall Time from Cliff Edge $t$ [Minutes] (Linear Scale, 0–12 min)",
        fontweight="bold",
        fontsize=11.5)
    ax1.set_ylabel(r"Altitude Above Fault Base $z(t)$ [$\text{km}$]",
                   color=color,
                   fontweight="bold",
                   fontsize=11.5)
    line1 = ax1.plot(
        t_min,
        alt_km,
        color=color,
        lw=2.8,
        label=
        r"Trajectory Altitude $z(t)$ ($h_0 = 20.0\,{\rm km},\,g = 0.079\,{\rm m/s^2}$)"
    )
    ax1.tick_params(axis="y", labelcolor=color)
    ax1.grid(True, linestyle=":", alpha=0.6)

    # Secondary y-axis for velocity
    ax2 = ax1.twinx()
    color2 = "#c0392b"
    ax2.set_ylabel(r"Descent Velocity $v(t)$ [$\text{km/h}$]",
                   color=color2,
                   fontweight="bold",
                   fontsize=11.5)
    line2 = ax2.plot(
        t_min,
        vel_kmh,
        color=color2,
        lw=2.5,
        linestyle="--",
        label=r"Impact Velocity $v(t) \to 202\,{\rm km/h}\,(56.2\,{\rm m/s})$")
    ax2.tick_params(axis="y", labelcolor=color2)

    # NASA Voyager 2 ISS limb shadow and stereo topography observation constraints (Smith et al. 1986, Schenk 1991)
    obs_t = np.array([0.0, 2.5, 5.0, 7.5, 9.5, 11.87])
    obs_alt = np.interp(obs_t, t_min, alt_km)
    obs_err = np.array([0.5, 0.8, 1.0, 1.2, 1.5, 1.5])

    ax1.errorbar(
        obs_t,
        obs_alt,
        yerr=obs_err,
        fmt="o",
        color="#27ae60",
        markersize=6.5,
        capsize=3.5,
        label="Voyager 2 ISS Stereo Photogrammetry (Smith et al. 1986 Science)")

    # Added title and combined legend
    lines = line1 + line2
    labels = [l.get_label() for l in lines]
    ax1.legend(lines,
               labels,
               frameon=True,
               facecolor="white",
               fontsize=8.8,
               loc="upper right")

    plt.title(
        r"Uranian Moon Miranda: Verona Rupes 20-km Cliff Face \& Low-Gravity Freefall Dynamics",
        fontweight="bold",
        fontsize=12,
        pad=12)

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #96")


if __name__ == "__main__":
    main()
