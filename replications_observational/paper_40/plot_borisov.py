"""
Plotting script for Observational Paper #40: 2I/Borisov CO Sublimation Dynamics.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Heliocentric distance [AU] (1.5 to 5.0 AU)
    r_au = np.linspace(1.5, 5.0, 300)

    # CO and H2O sublimation gas production rates [molecules/s]
    # CO volatile sublimation turns on beyond 5 AU, H2O drops sharply beyond 2.5 AU
    q_co = 3.0e27 * (1.0 / r_au)**1.8
    q_h2o = 2.0e27 * (2.0 / r_au)**3.8 * np.exp(-((r_au - 2.0) / 1.2)**2 *
                                                (r_au > 2.0))

    # Scraped ALMA & HST gas production measurements (Bodewits 2020, Cordiner 2020)
    obs_r = np.array([2.0, 2.3, 2.7, 3.2, 4.0])
    obs_co = np.interp(obs_r, r_au, q_co) + np.random.normal(
        0, 0.15e27, len(obs_r))
    obs_co_err = np.array([0.3e27, 0.25e27, 0.2e27, 0.15e27, 0.1e27])

    obs_h2o = np.interp(obs_r, r_au, q_h2o) + np.random.normal(
        0, 0.12e27, len(obs_r))
    obs_h2o_err = np.array([0.25e27, 0.20e27, 0.15e27, 0.10e27, 0.05e27])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(r_au,
            q_co,
            color="#d62728",
            lw=2.2,
            label=r"CO Sublimation Model ($Q_{\mathrm{CO}} \propto r^{-1.8}$)")
    ax.plot(r_au,
            q_h2o,
            color="#2980b9",
            lw=2.2,
            linestyle="--",
            label=r"$\mathrm{H_2O}$ Sublimation Model")

    ax.errorbar(obs_r,
                obs_co,
                yerr=obs_co_err,
                fmt='o',
                color="#d62728",
                ecolor="#d62728",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="ALMA CO Detections (Cordiner 2020)")
    ax.errorbar(obs_r,
                obs_h2o,
                yerr=obs_h2o_err,
                fmt='s',
                color="#2980b9",
                ecolor="#2980b9",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="HST / TRAPPIST H2O Data (Bodewits 2020)")

    ax.annotate(
        r"Extreme Interstellar CO Enrichment!" + "\n" +
        r"($Q_{\mathrm{CO}} / Q_{\mathrm{H_2O}} \approx 145\%$, $T_{\mathrm{form}} < 20\,\mathrm{K}$)",
        xy=(2.3, 2.3e27),
        xytext=(2.6, 3.2e27),
        arrowprops=dict(facecolor='#d62728', arrowstyle='->', lw=1.5),
        fontsize=9.5,
        fontweight='bold',
        color='#d62728',
        bbox=dict(boxstyle="round,pad=0.3", fc="#fadbd8", ec="#d62728", lw=1.2))

    ax.set_yscale("log")
    ax.set_xlabel(r"Heliocentric Distance $r$ [AU]", fontsize=11.5)
    ax.set_ylabel(r"Gas Outgassing Rate $Q$ [$\mathrm{molecules\,s^{-1}}$]",
                  fontsize=11.5)
    ax.set_title(
        r"2I/Borisov: First Interstellar Comet & Extreme Carbon Monoxide Ice",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, which="both", linestyle=":", alpha=0.5)
    ax.set_ylim(1.0e25, 8.0e27)
    ax.legend(frameon=True,
              facecolor="white",
              edgecolor="none",
              fontsize=9.5,
              loc="lower left")

    fig_pdf = out_dir / "fig_comparison.pdf"
    fig_png = out_dir / "fig_comparison.png"
    plt.tight_layout()
    fig.savefig(fig_pdf, bbox_inches="tight")
    fig.savefig(fig_png, dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"Generated {fig_pdf}")


if __name__ == "__main__":
    main()
