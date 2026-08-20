"""
Plotting script for Observational Paper #69: TOI-849b Chthonian Core.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "toi849b_envelope_fraction_grid.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    f_pct, r_synth, rho_synth = [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            f_pct.append(float(row["envelope_fraction_pct"]))
            r_synth.append(float(row["planet_radius_rearth"]))
            rho_synth.append(float(row["bulk_density_g_cm3"]))

    f_pct = np.array(f_pct)
    r_synth = np.array(r_synth)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    ax.plot(
        f_pct,
        r_synth,
        color="#2980b9",
        lw=2.8,
        label=
        r"Model Interior Structure $R_p(f_{\rm env})$ for $M_p = 39.1\,M_\oplus$ Core"
    )

    # Upper envelope fraction limit: 3.8% H/He by mass
    ax.axvline(
        3.8,
        color="#c0392b",
        linestyle="--",
        lw=2.0,
        label=r"Maximum Envelope Fraction Upper Limit ($f_{\rm env} \leq 3.8\%$)"
    )

    # NASA TESS transit & ESO HARPS radial velocity observational measurements (Armstrong et al. 2020 Nature)
    obs_f = 2.8
    obs_r = 3.44
    obs_r_err = 0.12
    obs_f_err = 0.8

    ax.errorbar(
        [obs_f], [obs_r],
        xerr=[obs_f_err],
        yerr=[obs_r_err],
        fmt="s",
        color="#d35400",
        markersize=8.5,
        capsize=5.0,
        label=
        "TESS Transit / HARPS RV Measurements (Armstrong et al. 2020 Nature)")

    # Pure core line (f_env = 0%)
    ax.scatter(
        [0.0], [3.10],
        color="#27ae60",
        s=75,
        zorder=5,
        label=
        r"Pure Rocky/Iron Core Limit ($f_{\rm env} = 0\%,\,R_{\rm core} = 3.10\,R_\oplus$)"
    )

    ax.set_xlabel(
        r"$\mathrm{H/He}$ Gaseous Envelope Mass Fraction $f_{\rm env}$ [$\%$]",
        fontweight="bold",
        fontsize=11.5)
    ax.set_ylabel(r"Planetary Radius $R_p$ [$R_\oplus$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "TOI-849b: Remnant Chthonian Gas Giant Core in the Neptunian Desert",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=8.8, loc="lower right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #69")


if __name__ == "__main__":
    main()
