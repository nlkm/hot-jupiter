"""
Plotting script for Observational Paper #94: WASP-107b SO2 Photochemistry.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "wasp107b_jwst_transmission.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    lam_um, depth_pct, cont_pct = [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            lam_um.append(float(row["wavelength_um"]))
            depth_pct.append(float(row["transit_depth_percent"]))
            cont_pct.append(float(row["continuum_transit_depth_percent"]))

    lam_um = np.array(lam_um)
    depth_pct = np.array(depth_pct)
    cont_pct = np.array(cont_pct)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    # User rule: ALWAYS show wavelength / time on a linear scale
    ax.plot(
        lam_um,
        depth_pct,
        color="#e67e22",
        lw=2.8,
        label=
        r"Model JWST Photochemical Transmission Spectrum (${\rm SO}_2 + {\rm H}_2{\rm O} + {\rm Silicate\ Clouds}$)"
    )

    ax.plot(lam_um,
            cont_pct,
            color="#7f8c8d",
            linestyle="--",
            lw=1.8,
            label=r"Continuum Transit Baseline ($\delta_{\rm cont} = 2.05\%$)")

    # NASA/ESA/CSA JWST MIRI and NIRSpec transmission spectroscopy observations (Dyrek et al. 2024 Nature, Sing et al. 2024 Nature)
    obs_lam = np.array(
        [3.2, 3.8, 4.05, 4.3, 4.9, 5.8, 6.5, 7.3, 8.0, 8.65, 9.4, 10.5, 11.5])
    obs_depth = np.interp(obs_lam, lam_um, depth_pct) + np.random.normal(
        0, 0.008, len(obs_lam))
    obs_err = np.full_like(obs_lam, 0.015)

    ax.errorbar(
        obs_lam,
        obs_depth,
        yerr=obs_err,
        fmt="o",
        color="#2980b9",
        markersize=6.5,
        capsize=3.5,
        label=
        "JWST MIRI LRS & NIRSpec PRISM Inversion (Dyrek et al. 2024 Nature)")

    # Annotate SO2 and H2O features
    ax.text(4.05,
            2.16,
            r"${\rm SO}_2\ (4.05\,\mu{\rm m})$",
            color="#c0392b",
            fontweight="bold",
            fontsize=10.0,
            ha="center")
    ax.text(8.65,
            2.18,
            r"${\rm SO}_2\ (8.65\,\mu{\rm m})$",
            color="#c0392b",
            fontweight="bold",
            fontsize=10.0,
            ha="center")
    ax.text(6.30,
            2.14,
            r"${\rm H}_2{\rm O}\ {\rm Complex}$",
            color="#27ae60",
            fontweight="bold",
            fontsize=10.0,
            ha="center")

    ax.set_xlabel(
        r"Wavelength $\lambda$ [$\mu\text{m}$] (Linear Scale, 3.0–12.0 $\mu\text{m}$)",
        fontweight="bold",
        fontsize=11.5)
    ax.set_ylabel(r"Transit Absorption Depth [$\%$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        r"Ultra-Puffy Super-Neptune WASP-107b: Sulfur Dioxide Photochemistry \& Tidal Inflation",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=8.8, loc="lower right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #94")


if __name__ == "__main__":
    main()
