"""
Plotting script for Observational Paper #33: TOI-849b Mass-Radius & Interior Composition.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Mass grid [M_Earth] (1 to 100 M_Earth)
    m_grid = np.logspace(0, 2, 200)

    # Theoretical mass-radius relations (Zeng et al. 2019)
    # 100% Iron core
    r_iron = 0.78 * (m_grid)**0.27
    # Earth-like (32.5% Fe + 67.5% MgSiO3)
    r_earth_like = 1.00 * (m_grid)**0.274
    # Pure Water World (100% H2O)
    r_water = 1.45 * (m_grid)**0.265
    # Core + 2% H/He envelope
    r_hhe_2pct = 2.15 * (m_grid)**0.22
    # Core + 10% H/He envelope
    r_hhe_10pct = 3.60 * (m_grid)**0.18

    fig, ax = plt.subplots(figsize=(8.0, 5.2))
    ax.plot(m_grid,
            r_iron,
            color="#7f8c8d",
            lw=1.8,
            linestyle=":",
            label="100% Iron Core")
    ax.plot(m_grid,
            r_earth_like,
            color="#27ae60",
            lw=2.2,
            label="Earth-like Rocky Composition (33% Fe + 67% Silicate)")
    ax.plot(m_grid,
            r_water,
            color="#2980b9",
            lw=2.0,
            linestyle="--",
            label="100% Water / Ice World")
    ax.plot(m_grid,
            r_hhe_2pct,
            color="#e67e22",
            lw=2.0,
            label="Rocky Core + 2% H/He Envelope")
    ax.plot(m_grid,
            r_hhe_10pct,
            color="#8e44ad",
            lw=1.8,
            linestyle="-.",
            label="Sub-Neptune (10% H/He)")

    # TOI-849b observed measurement (Armstrong et al. 2020 Nature)
    ax.errorbar(
        [39.1], [3.44],
        xerr=[2.5],
        yerr=[0.12],
        fmt='*',
        color="#d62728",
        ecolor="#d62728",
        elinewidth=2.0,
        capsize=4,
        markersize=14,
        label=
        r"TOI-849b (TESS / HARPS: $M = 39.1\,M_\oplus, R = 3.44\,R_\oplus$)")

    # Solar System planets for reference
    ax.scatter([1.0], [1.0],
               color="green",
               s=50,
               marker='o',
               label="Earth (1.0, 1.0)")
    ax.scatter([17.1], [3.88],
               color="navy",
               s=60,
               marker='s',
               label="Neptune (17.1, 3.88)")
    ax.scatter([95.2], [9.45],
               color="gold",
               s=80,
               marker='D',
               label="Saturn (95.2, 9.45)")

    ax.annotate(
        "TOI-849b: Stripped Remnant Giant Core!\nEnvelope Mass Fraction < 3.8%",
        xy=(39.1, 3.44),
        xytext=(12.0, 5.5),
        arrowprops=dict(facecolor='#d62728', arrowstyle='->', lw=1.8),
        fontsize=10.0,
        fontweight='bold',
        color='#d62728',
        bbox=dict(boxstyle="round,pad=0.3", fc="#fadbd8", ec="#d62728", lw=1.5))

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlim(0.8, 120)
    ax.set_ylim(0.7, 12)
    ax.set_xlabel(r"Planetary Mass $M_p$ [$M_\oplus$]", fontsize=11.5)
    ax.set_ylabel(r"Planetary Radius $R_p$ [$R_\oplus$]", fontsize=11.5)
    ax.set_title("TOI-849b: Mass-Radius Diagram & Chthonian Core Structure",
                 fontsize=12,
                 pad=10,
                 fontweight="bold")
    ax.grid(True, which="both", linestyle=":", alpha=0.5)
    ax.legend(frameon=True,
              facecolor="white",
              edgecolor="none",
              fontsize=8.5,
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
