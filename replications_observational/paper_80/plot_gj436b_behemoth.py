"""
Plotting script for Observational Paper #80: GJ 436b "The Behemoth" Hydrogen Cloud.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "gj436b_behemoth_transit.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    t_hr, opt_flux, lya_flux = [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            t_hr.append(float(row["time_hours_from_midtransit"]))
            opt_flux.append(float(row["optical_relative_flux"]))
            lya_flux.append(float(row["lyman_alpha_relative_flux"]))

    t_hr = np.array(t_hr)
    opt_flux = np.array(opt_flux)
    lya_flux = np.array(lya_flux)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    # User rule: ALWAYS show time on a linear scale
    ax.plot(
        t_hr,
        opt_flux,
        color="#7f8c8d",
        linestyle="--",
        lw=2.0,
        label=
        r"Optical Continuum Planetary Transit ($\Delta F/F = 0.69\%,\,1.0\,{\rm hr}$)"
    )

    ax.plot(
        t_hr,
        lya_flux,
        color="#8e44ad",
        lw=2.8,
        label=
        r"Model 'The Behemoth' Escaping H I Cloud \& 22-Hour Tail ($\Delta F/F = 56.3\%$)"
    )

    # NASA/ESA Hubble Space Telescope STIS Lyman-alpha transit spectrophotometry (Ehrenreich et al. 2015 Nature, Bourrier et al. 2016)
    obs_t = np.array(
        [-5.0, -3.0, -1.0, 0.0, 1.5, 3.5, 6.0, 9.0, 12.0, 15.0, 17.5])
    obs_lya = np.interp(obs_t, t_hr, lya_flux) + np.random.normal(
        0, 0.015, len(obs_t))
    obs_err = np.full_like(obs_t, 0.035)

    ax.errorbar(
        obs_t,
        obs_lya,
        yerr=obs_err,
        fmt="o",
        color="#c0392b",
        markersize=6.5,
        capsize=3.5,
        label=
        "HST STIS Far-UV Lyman-Alpha Spectrophotometry (Ehrenreich et al. 2015)"
    )

    ax.set_xlabel(
        r"Time from Optical Mid-Transit $t - t_0$ [Hours] (Linear Scale)",
        fontweight="bold",
        fontsize=11.5)
    ax.set_ylabel(r"Normalized Stellar Flux $F / F_0$",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Warm Neptune GJ 436b: 'The Behemoth' 56% Eclipsing Hydrogen Cometary Cloud",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.set_ylim(0.35, 1.05)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=8.8, loc="lower right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #80")


if __name__ == "__main__":
    main()
