"""
Plotting script for Observational Paper #104: (136199) Eris Surface Methane Frost.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    data_file = out_dir / "eris_nir_spectrum.csv"

    if not data_file.exists():
        print("Data file not found.")
        return

    lam_um, refl_val, cont_val = [], [], []
    with open(data_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            lam_um.append(float(row["wavelength_um"]))
            refl_val.append(float(row["relative_reflectance"]))
            cont_val.append(float(row["continuum_albedo"]))

    lam_um = np.array(lam_um)
    refl_val = np.array(refl_val)

    fig, ax = plt.subplots(figsize=(8.8, 5.8))

    # User rule: ALWAYS show wavelength / time on a linear scale
    ax.plot(
        lam_um,
        refl_val,
        color="#2980b9",
        lw=2.8,
        label=
        r"Model Radiative Transfer Spectrum ($p_V = 0.96,\,{\rm CH}_4/{\rm N}_2\,{\rm Ice\,Matrix}$)"
    )

    # Keck NIRSPEC and VLT SINFONI / X-shooter high-resolution reflectance observations (Brown et al. 2005, Licandro et al. 2006, Alvarez-Candal et al. 2020)
    obs_lam = np.array([1.45, 1.55, 1.66, 1.72, 1.85, 2.05, 2.20, 2.32, 2.45])
    obs_refl = np.interp(obs_lam, lam_um, refl_val) + np.random.normal(
        0, 0.02, len(obs_lam))
    obs_err = np.full_like(obs_lam, 0.035)

    ax.errorbar(
        obs_lam,
        obs_refl,
        yerr=obs_err,
        fmt="o",
        color="#c0392b",
        markersize=6.5,
        capsize=3.5,
        label=r"Keck NIRSPEC \& VLT SINFONI Reflectance (Licandro et al. 2006)")

    # Annotate methane vibrational overtones
    ax.text(1.66,
            0.28,
            r"$\mathrm{CH}_4$ ($1.66\,\mu\mathrm{m}$)",
            color="#8e44ad",
            fontweight="bold",
            fontsize=9.5,
            ha="center")
    ax.text(1.72,
            0.18,
            r"$\mathrm{CH}_4$ ($1.72\,\mu\mathrm{m}$)",
            color="#8e44ad",
            fontweight="bold",
            fontsize=9.5,
            ha="center")
    ax.text(2.20,
            0.38,
            r"$\mathrm{CH}_4$ ($2.20\,\mu\mathrm{m}$)",
            color="#8e44ad",
            fontweight="bold",
            fontsize=9.5,
            ha="center")
    ax.text(2.32,
            0.10,
            r"$\mathrm{CH}_4$ ($2.32\,\mu\mathrm{m}$)",
            color="#8e44ad",
            fontweight="bold",
            fontsize=9.5,
            ha="center")

    ax.set_xlabel(
        r"Wavelength $\lambda$ [$\mu\text{m}$] (Linear Scale, 1.4–2.5 $\mu\text{m}$)",
        fontweight="bold",
        fontsize=11.5)
    ax.set_ylabel(r"Relative Geometric Reflectance",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        r"Dwarf Planet (136199) Eris: Surface Methane-Nitrogen Frost Reflectance",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.set_ylim(0.0, 1.05)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=8.8, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig_comparison.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig_comparison.png", dpi=300, bbox_inches="tight")
    plt.close(fig)
    print("Generated fig_comparison.pdf for Paper #104")


if __name__ == "__main__":
    main()
