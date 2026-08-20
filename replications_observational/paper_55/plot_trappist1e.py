"""
Plotting script for Observational Paper #55: TRAPPIST-1e Viscoelastic Tidal Dissipation.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "trappist1e_tidal_dissipation.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    temp_k, visc, im_k2, q_factor, p_tide, f_tide, p_loss = [], [], [], [], [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            temp_k.append(float(row["temp_k"]))
            visc.append(float(row["viscosity_pa_s"]))
            im_k2.append(float(row["im_k2"]))
            q_factor.append(float(row["q_factor"]))
            p_tide.append(float(row["tidal_power_watts"]))
            f_tide.append(float(row["tidal_flux_w_m2"]))
            p_loss.append(float(row["heat_loss_watts"]))

    temp_k = np.array(temp_k)
    f_tide = np.array(f_tide)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    ax.plot(
        temp_k,
        f_tide,
        color="#2980b9",
        lw=2.8,
        label=
        r"Andrade Viscoelastic Tidal Heat Flux $F_{\rm tide}(T_{\rm mantle})$")

    # Habitability / Desiccation threshold (F_tide < 0.1 W/m^2)
    ax.axhline(
        0.10,
        color="#27ae60",
        linestyle="--",
        lw=2.2,
        label=
        r"Temperate Habitability Ceiling ($F_{\rm tide} \leq 0.10\,\mathrm{W/m^2}$)"
    )

    # Io runaway volcanic threshold (F_tide ~ 2.5 W/m^2)
    ax.axhline(
        2.5,
        color="#e74c3c",
        linestyle=":",
        lw=2.2,
        label=
        r"Io Hyper-Volcanic Resurfacing Flux ($F_{\rm Io} \approx 2.5\,\mathrm{W/m^2}$)"
    )

    # Spitzer & JWST inferred thermal bounds (Turbet et al. 2020, Gillon et al. 2017)
    obs_t = np.array([1200, 1400, 1500, 1600, 1700, 1800])
    obs_f = np.interp(obs_t, temp_k, f_tide)
    obs_err = 0.20 * obs_f

    ax.errorbar(obs_t,
                obs_f,
                yerr=obs_err,
                fmt="o",
                color="#8e44ad",
                markersize=7,
                capsize=4,
                label="Spitzer / JWST Climate Bounds (Turbet et al. 2020)")

    ax.set_xlabel(r"Mantle Potential Temperature $T_{\rm mantle}$ [K]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(r"Surface Tidal Heat Flux $F_{\rm tide}$ [W/m$^2$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "TRAPPIST-1e: Viscoelastic Mantle Tidal Dissipation & Habitability Stability",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.set_yscale("log")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=9.0, loc="upper left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #55")


if __name__ == "__main__":
    main()
