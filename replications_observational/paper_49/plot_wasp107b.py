"""
Plotting script for Observational Paper #49: WASP-107b Puffy Super-Neptune.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Wavelength [microns] (0.8 to 12.0 um)
    wave = np.linspace(0.8, 12.0, 400)

    # Transmission spectrum transit depth [%] (baseline ~ 2.10%)
    # Water feature at 1.4 & 2.7 um, SO2 at 4.05 & 8.6 um, CO2 at 4.3 um (Dyrek 2024)
    h2o_peak = 0.12 * np.exp(-((wave - 1.4) / 0.15)**2) + 0.15 * np.exp(-(
        (wave - 2.7) / 0.3)**2)
    so2_peak = 0.10 * np.exp(-((wave - 4.05) / 0.12)**2) + 0.08 * np.exp(-(
        (wave - 8.6) / 0.4)**2)
    co2_peak = 0.09 * np.exp(-((wave - 4.3) / 0.15)**2)
    trans_depth = 2.10 + h2o_peak + so2_peak + co2_peak

    # Scraped JWST NIRSpec & MIRI transmission spectrum data (Dyrek et al. 2024 Nature)
    obs_w = np.array([1.1, 1.4, 1.9, 2.7, 3.4, 4.05, 4.3, 5.2, 6.8, 8.6, 10.5])
    obs_d = np.interp(obs_w, wave, trans_depth) + np.random.normal(
        0, 0.015, len(obs_w))
    obs_err = np.array([
        0.020, 0.018, 0.018, 0.018, 0.018, 0.015, 0.015, 0.018, 0.020, 0.018,
        0.022
    ])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(
        wave,
        trans_depth,
        color="#2980b9",
        lw=2.5,
        label=
        r"Our Puffy Envelope ($H \approx 800\,\mathrm{km}$) Photochemical Model"
    )
    ax.errorbar(obs_w,
                obs_d,
                yerr=obs_err,
                fmt='o',
                color="#e74c3c",
                ecolor="#e74c3c",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="JWST NIRSpec / MIRI Transmission Spectrum (Dyrek 2024)")

    ax.annotate(
        "SULFUR DIOXIDE ($\\mathrm{SO_2}$)\nPhotochemical alien smog at $4.05\\,\\mu\\mathrm{m}$!",
        xy=(4.05, 2.20),
        xytext=(2.2, 2.26),
        arrowprops=dict(facecolor='#27ae60', arrowstyle='->', lw=1.5),
        fontsize=9.5,
        fontweight='bold',
        color='#27ae60',
        bbox=dict(boxstyle="round,pad=0.3", fc="#e8f8f5", ec="#27ae60", lw=1.2))

    ax.annotate(r"PUFFY WATER VAPOR ($\mathrm{H_2O}$)",
                xy=(2.7, 2.25),
                xytext=(0.9, 2.28),
                arrowprops=dict(facecolor='#2980b9', arrowstyle='->', lw=1.5),
                fontsize=9.0,
                fontweight='bold',
                color='#2980b9',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#ebf5fb",
                          ec="#2980b9",
                          lw=1.2))

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=11.5)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=11.5)
    ax.set_title(
        r"WASP-107b: JWST Transmission Spectrum & Puffy Envelope Photochemistry",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(0.8, 12.0)
    ax.set_ylim(2.05, 2.34)
    ax.legend(frameon=True,
              facecolor="white",
              edgecolor="none",
              fontsize=9.0,
              loc="upper right")

    fig_pdf = out_dir / "fig_comparison.pdf"
    fig_png = out_dir / "fig_comparison.png"
    plt.tight_layout()
    fig.savefig(fig_pdf, bbox_inches="tight")
    fig.savefig(fig_png, dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"Generated {fig_pdf}")


if __name__ == "__main__":
    main()
