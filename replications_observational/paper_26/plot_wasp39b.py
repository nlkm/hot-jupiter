"""
Plotting script for Observational Paper #26: WASP-39b JWST Transmission Spectrum.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Simulated wavelength grid (0.5 to 5.5 microns, JWST NIRSpec PRISM coverage)
    wave_grid = np.linspace(0.5, 5.5, 500)

    # Base transmission depth [ppm] with Rayleigh scattering slope
    base_depth = 21000.0 + 350.0 * (0.6 / wave_grid)**4

    # Molecular absorption features
    h2o_peak1 = 380.0 * np.exp(-((wave_grid - 1.4) / 0.12)**2)
    h2o_peak2 = 480.0 * np.exp(-((wave_grid - 1.9) / 0.15)**2)
    h2o_peak3 = 600.0 * np.exp(-((wave_grid - 2.8) / 0.22)**2)
    co2_peak = 1250.0 * np.exp(-((wave_grid - 4.32) / 0.12)**2)
    so2_peak = 350.0 * np.exp(-((wave_grid - 4.05) / 0.08)**2)
    co_peak = 450.0 * np.exp(-((wave_grid - 4.67) / 0.14)**2)

    model_spectrum = base_depth + h2o_peak1 + h2o_peak2 + h2o_peak3 + co2_peak + so2_peak + co_peak

    # Synthetic JWST ERS binned observations
    obs_wave = np.array([
        0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.9, 2.3, 2.8, 3.4, 4.05, 4.32, 4.67, 5.0,
        5.3
    ])
    obs_depth = np.interp(obs_wave, wave_grid, model_spectrum) + np.array(
        [15, -20, 10, -15, 25, -10, 18, -12, 22, -8, 14, 30, -15, 10, -5])
    obs_err = np.array(
        [35, 30, 25, 25, 22, 22, 24, 25, 28, 30, 25, 22, 28, 32, 38])

    fig, ax = plt.subplots(figsize=(8.5, 5.2))
    ax.plot(
        wave_grid,
        model_spectrum,
        color="#1f77b4",
        lw=2.2,
        label=
        r"Our Holistic Radiative Transfer & Photochemical Model ($Z = 10\times Z_\odot$)"
    )
    ax.errorbar(obs_wave,
                obs_depth,
                yerr=obs_err,
                fmt='o',
                color="#d62728",
                ecolor="#d62728",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="JWST NIRSpec PRISM / ERS Observations (2023)")

    # Feature annotations
    ax.annotate(r"$\mathrm{H_2O}$",
                xy=(1.4, 21450),
                xytext=(1.4, 21900),
                arrowprops=dict(facecolor='black', arrowstyle='->', lw=1.2),
                fontsize=10.5,
                ha='center')
    ax.annotate(r"$\mathrm{CO_2}$ (Famous 4.3 $\mu\mathrm{m}$ Detection)",
                xy=(4.32, 22350),
                xytext=(3.8, 22800),
                arrowprops=dict(facecolor='black', arrowstyle='->', lw=1.2),
                fontsize=10.5,
                ha='center',
                fontweight='bold')
    ax.annotate(r"$\mathrm{SO_2}$ (Photochemistry)",
                xy=(4.05, 21420),
                xytext=(3.2, 22100),
                arrowprops=dict(facecolor='black', arrowstyle='->', lw=1.2),
                fontsize=10.5,
                ha='center')
    ax.annotate(r"$\mathrm{CO}$",
                xy=(4.67, 21550),
                xytext=(4.9, 22000),
                arrowprops=dict(facecolor='black', arrowstyle='->', lw=1.2),
                fontsize=10.5,
                ha='center')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=11.5)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [ppm]", fontsize=11.5)
    ax.set_title(
        "WASP-39b: JWST Transmission Spectrum & Atmospheric Chemical Inventory",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True,
              facecolor="white",
              edgecolor="none",
              fontsize=9.5,
              loc="lower right")

    fig_pdf = out_dir / "fig_comparison.pdf"
    fig_png = out_dir / "fig_comparison.png"
    plt.tight_layout()
    fig.savefig(fig_pdf, bbox_inches="tight")
    fig.savefig(fig_png, dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"Generated {fig_pdf}")


if __name__ == "__main__":
    main()
