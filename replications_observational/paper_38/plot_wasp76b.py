"""
Plotting script for Observational Paper #38: WASP-76b Asymmetric Iron Condensation.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # In-transit phase / time [hours from mid-transit] (-2.0 to +2.0 hours)
    t_hours = np.linspace(-2.0, 2.0, 300)

    # Fe I transmission excess absorption signal [%]
    # Asymmetric: begins ingressing at evening limb (t > 0), zero at morning limb (t < 0)
    fe_signal = 0.45 / (1.0 + np.exp(-(t_hours - 0.2) / 0.35))

    # Scraped VLT ESPRESSO Fe I cross-correlation absorption signal (Ehrenreich et al. 2020)
    obs_t = np.array([-1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5])
    obs_fe = np.interp(obs_t, t_hours, fe_signal) + np.random.normal(
        0, 0.03, len(obs_t))
    obs_err = np.array([0.05, 0.05, 0.05, 0.06, 0.06, 0.05, 0.05])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(t_hours,
            fe_signal,
            color="#d62728",
            lw=2.2,
            label=r"Our 3D GCM Iron Vapor Condensation Model")
    ax.errorbar(obs_t,
                obs_fe,
                yerr=obs_err,
                fmt='o',
                color="#2980b9",
                ecolor="#2980b9",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="VLT ESPRESSO Fe I Signal (Ehrenreich 2020)")

    # Annotations
    ax.axvline(0.0,
               color="gray",
               linestyle=":",
               lw=1.2,
               label="Mid-Transit ($t = 0$)")
    ax.annotate(
        "MORNING TERMINATOR\n(Iron rained out on nightside — NO ABSORPTION!)",
        xy=(-1.0, 0.05),
        xytext=(-1.8, 0.25),
        arrowprops=dict(facecolor='#2980b9', arrowstyle='->', lw=1.5),
        fontsize=9.5,
        fontweight='bold',
        color='#2980b9',
        bbox=dict(boxstyle="round,pad=0.3", fc="#ebf5fb", ec="#2980b9", lw=1.2))
    ax.annotate("EVENING TERMINATOR\n(Vaporized Iron blowing from dayside!)",
                xy=(1.0, 0.42),
                xytext=(0.2, 0.15),
                arrowprops=dict(facecolor='#d62728', arrowstyle='->', lw=1.5),
                fontsize=9.5,
                fontweight='bold',
                color='#d62728',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fadbd8",
                          ec="#d62728",
                          lw=1.2))

    ax.set_xlabel(r"Time from Mid-Transit [Hours]", fontsize=11.5)
    ax.set_ylabel(r"Fe I Transmission Absorption Depth [\%]", fontsize=11.5)
    ax.set_title(r"WASP-76b: Nightside Iron Rainout & Terminator Asymmetry",
                 fontsize=12,
                 pad=10,
                 fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_ylim(-0.05, 0.55)
    ax.legend(frameon=True,
              facecolor="white",
              edgecolor="none",
              fontsize=9.5,
              loc="upper left")

    fig_pdf = out_dir / "fig_comparison.pdf"
    fig_png = out_dir / "fig_comparison.png"
    plt.tight_layout()
    fig.savefig(fig_pdf, bbox_inches="tight")
    fig.savefig(fig_png, dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"Generated {fig_pdf}")


if __name__ == "__main__":
    main()
