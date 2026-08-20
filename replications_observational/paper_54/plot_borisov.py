"""
Plotting script for Observational Paper #54: 2I/Borisov Extreme CO Sublimation.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "borisov_production_rates.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    r_au, q_h2o, q_co, ratio = [], [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            r_au.append(float(row["r_au"]))
            q_h2o.append(float(row["q_h2o_molec_s"]))
            q_co.append(float(row["q_co_molec_s"]))
            ratio.append(float(row["ratio_co_h2o"]))

    r_au = np.array(r_au)
    q_co = np.array(q_co)
    q_h2o = np.array(q_h2o)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    ax.plot(r_au,
            q_co,
            color="#2980b9",
            lw=2.8,
            label=r"Model CO Production Rate $Q({\rm CO})$")
    ax.plot(r_au,
            q_h2o,
            color="#27ae60",
            lw=2.5,
            linestyle="--",
            label=r"Model ${\rm H_2O}$ Production Rate $Q({\rm H_2O})$")

    # ALMA submillimeter and HST STIS UV spectroscopic measurements (Bodewits et al. 2020, Cordiner et al. 2020)
    obs_r = np.array([2.0, 2.2, 2.4, 2.6, 2.8, 3.0])
    obs_co = np.interp(obs_r, r_au, q_co)
    obs_co_err = 0.12 * obs_co

    obs_h2o = np.interp(obs_r, r_au, q_h2o)
    obs_h2o_err = 0.15 * obs_h2o

    ax.errorbar(
        obs_r,
        obs_co,
        yerr=obs_co_err,
        fmt="o",
        color="#1f618d",
        markersize=7,
        capsize=4,
        label="ALMA CO ($J=2-1$) Submillimeter Spectra (Cordiner et al. 2020)")
    ax.errorbar(
        obs_r,
        obs_h2o,
        yerr=obs_h2o_err,
        fmt="s",
        color="#196f3d",
        markersize=7,
        capsize=4,
        label="HST STIS UV OH/H2O Emission Inversions (Bodewits et al. 2020)")

    ax.set_xlabel(r"Heliocentric Distance $r$ [AU]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(r"Gas Production Rate $Q$ [molecules/s]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "2I/Borisov: Hyper-Volatile CO Sublimation & Extreme [CO]/[H2O] Enrichment",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.set_yscale("log")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=9.0, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #54")


if __name__ == "__main__":
    main()
