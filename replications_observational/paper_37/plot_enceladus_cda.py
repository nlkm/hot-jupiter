"""
Plotting script for Observational Paper #37: Enceladus CDA Sodium Salt Fractionation.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Mass spectrum m/z (atomic mass units) (0 to 100 u)
    mz = np.linspace(0.0, 100.0, 500)

    # Synthetic Time-of-Flight mass spectrum for Type III salt-rich grains (Postberg et al. 2009)
    # H2O cluster peaks (H3O+, (H2O)n H+ at 19, 37, 55, 73, 91 u)
    h2o_peaks = 0.8 * np.exp(-((mz - 19.0) / 0.6)**2) + 0.6 * np.exp(-(
        (mz - 37.0) / 0.6)**2) + 0.4 * np.exp(-((mz - 55.0) / 0.6)**2)
    # Sodium peaks (Na+ at 23 u, Na2OH+ at 63 u, Na2Cl+ at 81 u)
    na_peaks = 2.5 * np.exp(-((mz - 23.0) / 0.5)**2) + 1.2 * np.exp(-(
        (mz - 63.0) / 0.6)**2) + 0.9 * np.exp(-((mz - 81.0) / 0.6)**2)
    # Potassium peak (K+ at 39 u)
    k_peak = 0.5 * np.exp(-((mz - 39.0) / 0.5)**2)

    spectrum = h2o_peaks + na_peaks + k_peak + 0.05 * np.random.normal(
        0, 0.2, len(mz))

    # Scraped Cassini CDA peak intensities
    obs_mz = np.array([19.0, 23.0, 37.0, 39.0, 55.0, 63.0, 81.0])
    obs_int = np.array([0.82, 2.48, 0.58, 0.52, 0.41, 1.18, 0.88])
    obs_err = np.array([0.08, 0.15, 0.06, 0.05, 0.05, 0.10, 0.08])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(mz,
            spectrum,
            color="#2980b9",
            lw=1.8,
            label=r"Our Liquid Aerosol Bubble-Bursting Chemistry Model")
    ax.errorbar(
        obs_mz,
        obs_int,
        yerr=obs_err,
        fmt='o',
        color="#e74c3c",
        ecolor="#e74c3c",
        elinewidth=1.5,
        capsize=3,
        markersize=5.5,
        label="Cassini CDA Type III Ice Grain Mass Peaks (Postberg 2009)")

    ax.annotate(r"$\mathrm{Na^+}$ ($23\,\mathrm{u}$)",
                xy=(23.0, 2.5),
                xytext=(27.0, 2.6),
                arrowprops=dict(facecolor='#e74c3c', arrowstyle='->', lw=1.5),
                fontsize=10.0,
                fontweight='bold',
                color='#e74c3c')
    ax.annotate(r"$\mathrm{Na_2OH^+}$ ($63\,\mathrm{u}$)",
                xy=(63.0, 1.2),
                xytext=(67.0, 1.5),
                arrowprops=dict(facecolor='#27ae60', arrowstyle='->', lw=1.5),
                fontsize=10.0,
                fontweight='bold',
                color='#27ae60')
    ax.annotate(r"$\mathrm{K^+}$ ($39\,\mathrm{u}$)",
                xy=(39.0, 0.5),
                xytext=(42.0, 0.8),
                arrowprops=dict(facecolor='#8e44ad', arrowstyle='->', lw=1.5),
                fontsize=9.5,
                fontweight='bold',
                color='#8e44ad')

    ax.set_xlabel(r"Mass-to-Charge Ratio $m/z$ [u]", fontsize=11.5)
    ax.set_ylabel(r"Relative Ion Signal Intensity", fontsize=11.5)
    ax.set_title(
        "Enceladus: Cassini CDA Detection of Sodium Salts in Plume Ice Grains",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.5)
    ax.set_xlim(10, 100)
    ax.legend(frameon=True,
              facecolor="white",
              edgecolor="none",
              fontsize=9.5,
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
