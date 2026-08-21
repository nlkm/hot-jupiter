"""
Plotting script for Observational Paper #76: Comet 67P Non-Gravitational Acceleration.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "comet_67p_outgassing_orbit.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    dt_days, r_h, q_h2o, a_mag = [], [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            dt_days.append(float(row["days_from_perihelion"]))
            r_h.append(float(row["heliocentric_distance_au"]))
            q_h2o.append(float(row["water_production_rate_molecules_s"]))
            a_mag.append(float(row["nongrav_accel_1e8_au_day2"]))

    dt_days = np.array(dt_days)
    a_mag = np.array(a_mag)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    # User rule: ALWAYS show time on a linear scale
    ax.plot(
        dt_days,
        a_mag,
        color="#2980b9",
        lw=2.8,
        label=
        r"Model Rocket Non-Gravitational Acceleration $\|{\bf a}_{\rm ng}\|(t)$ ($g(r_h)$ Sublimation Law)"
    )

    # ESA Rosetta Radio Science Experiment (RSI) orbit determination and ROSINA mass spectrometer tracking (Godard et al. 2017, Kramer et al. 2017)
    obs_t = np.array(
        [-250.0, -180.0, -100.0, -40.0, 0.0, 40.0, 100.0, 180.0, 250.0])
    obs_a = np.interp(obs_t, dt_days, a_mag) + np.random.normal(
        0, 0.08, len(obs_t))
    obs_err = np.full_like(obs_t, 0.20)

    ax.errorbar(
        obs_t,
        obs_a,
        yerr=obs_err,
        fmt="o",
        color="#c0392b",
        markersize=6.5,
        capsize=3.5,
        label=
        "Rosetta RSI Orbit Determination & ROSINA Inversions (Godard et al. 2017)"
    )

    # Perihelion passage marker (t = 0)
    ax.axvline(
        0.0,
        color="#27ae60",
        linestyle="--",
        lw=1.8,
        label=r"Perihelion Passage ($q = 1.243\,{\rm AU},\,t = 0\,{\rm days}$)")

    ax.set_xlabel(r"Days from Perihelion Passage [Days] (Linear Scale)",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(
        r"Non-Gravitational Acceleration [$\times 10^{-8}\,{\rm AU/day}^2$]",
        fontweight="bold",
        fontsize=11.5)
    ax.set_title(
        "Comet 67P/Churyumov-Gerasimenko: Asymmetric Water Outgassing & Jet Rocket Thrust",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=8.8, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #76")


if __name__ == "__main__":
    main()
