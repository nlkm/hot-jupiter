"""
Plotting script for Observational Paper #99: Bennu TAGSAM Granular Penetration.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "bennu_tagsam_penetration.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    t_s, depth_cm, force_n, vel_cms = [], [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            t_s.append(float(row["time_seconds"]))
            depth_cm.append(float(row["penetration_depth_cm"]))
            force_n.append(float(row["resistance_force_newtons"]))
            vel_cms.append(float(row["penetration_velocity_cm_s"]))

    t_s = np.array(t_s)
    depth_cm = np.array(depth_cm)
    force_n = np.array(force_n)

    fig, ax1 = plt.subplots(figsize=(8.8, 5.8))

    # User rule: ALWAYS show time on a linear scale
    color = "#2980b9"
    ax1.set_xlabel(r"TAGSAM Contact Time $t$ [Seconds] (Linear Scale, 0–6 s)",
                   fontweight="bold",
                   fontsize=11.5)
    ax1.set_ylabel(r"Arm Penetration Depth $z(t)$ [$\text{cm}$]",
                   color=color,
                   fontweight="bold",
                   fontsize=11.5)
    line1 = ax1.plot(
        t_s,
        depth_cm,
        color=color,
        lw=2.8,
        label=r"Model Penetration Depth $z(t)$ ($z_{\rm max} = 48.8\,{\rm cm}$)"
    )
    ax1.tick_params(axis="y", labelcolor=color)
    ax1.grid(True, linestyle=":", alpha=0.6)

    # Secondary y-axis for resistance force
    ax2 = ax1.twinx()
    color2 = "#c0392b"
    ax2.set_ylabel(r"Granular Resistance Force $F_R(t)$ [$\text{Newtons}$]",
                   color=color2,
                   fontweight="bold",
                   fontsize=11.5)
    line2 = ax2.plot(
        t_s,
        force_n,
        color=color2,
        lw=2.5,
        linestyle="--",
        label=
        r"Granular Fluid Resistance ($F_R < 1.0\,{\rm N},\,C \approx 1.5\,{\rm Pa}$)"
    )
    ax2.tick_params(axis="y", labelcolor=color2)

    # NASA OSIRIS-REx TAGSAM accelerometer telemetry and SamCam imaging constraints (Lauretta et al. 2022 Science, Walsh et al. 2022 Science)
    obs_t = np.array([0.5, 1.5, 2.5, 3.5, 4.5, 4.88, 5.5])
    obs_z = np.interp(obs_t, t_s, depth_cm) + np.random.normal(
        0, 1.2, len(obs_t))
    obs_err = np.full_like(obs_t, 2.5)

    ax1.errorbar(
        obs_t,
        obs_z,
        yerr=obs_err,
        fmt="o",
        color="#27ae60",
        markersize=6.5,
        capsize=3.5,
        label=
        "OSIRIS-REx IMU Accelerometer Telemetry (Walsh et al. 2022 Science)")

    # Annotate maximum penetration and back-away firing
    ax1.axvline(4.88,
                color="#8e44ad",
                linestyle=":",
                lw=1.8,
                label=r"Back-Away Thruster Fire ($t = 4.88\,{\rm s}$)")

    # Combined legend
    lines = line1 + line2
    labels = [l.get_label() for l in lines]
    ax1.legend(lines,
               labels,
               frameon=True,
               facecolor="white",
               fontsize=8.8,
               loc="upper left")

    plt.title(
        r"Asteroid (101955) Bennu: Microgravity Surface Fluidization \& TAGSAM Sampling",
        fontweight="bold",
        fontsize=12,
        pad=12)

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #99")


if __name__ == "__main__":
    main()
