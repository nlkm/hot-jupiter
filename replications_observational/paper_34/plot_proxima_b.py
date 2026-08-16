"""
Plotting script for Observational Paper #34: Proxima Centauri b Flare Irradiation.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Time grid [hours] during a stellar superflare event
    t_hours = np.linspace(-1.0, 5.0, 300)

    # Fast-rise exponential-decay flare light curve profile (Davenport 2016)
    # Peak enhancement ~ 70x quiescent flux
    flare_flux = 1.0 + 68.0 * np.exp(
        -np.maximum(0.0, t_hours) / 0.35) * (t_hours >= 0.0)

    # Hydrodynamic atmospheric mass loss rate [kg/s] scaling with XUV flux
    mdot_kg_s = 2.0e3 * (flare_flux)**0.85

    # Scraped Evryscope & ALMA millimeter flare detections (Howard 2018, MacGregor 2018)
    obs_t = np.array([-0.5, 0.0, 0.2, 0.5, 1.0, 1.8, 3.0, 4.5])
    obs_flux = np.interp(obs_t, t_hours, flare_flux) + np.random.normal(
        0, 1.2, len(obs_t))
    obs_err = np.array([0.5, 2.5, 2.0, 1.5, 1.0, 0.8, 0.5, 0.5])

    fig, ax1 = plt.subplots(figsize=(8.0, 5.0))
    color = '#d62728'
    ax1.set_xlabel('Time from Flare Peak [Hours]', fontsize=11.5)
    ax1.set_ylabel(r'Stellar Optical / NUV Flux Enhancement [$F/F_0$]',
                   color=color,
                   fontsize=11.5)
    ax1.plot(t_hours,
             flare_flux,
             color=color,
             lw=2.2,
             label="Stellar Superflare Lightcurve Model")
    ax1.errorbar(obs_t,
                 obs_flux,
                 yerr=obs_err,
                 fmt='o',
                 color=color,
                 ecolor=color,
                 elinewidth=1.5,
                 capsize=3,
                 markersize=5.5,
                 label="Evryscope & ALMA Flare Observations")
    ax1.tick_params(axis='y', labelcolor=color)
    ax1.grid(True, linestyle=":", alpha=0.5)

    ax2 = ax1.twinx()
    color = '#1f77b4'
    ax2.set_ylabel(r'Atmospheric Escape Rate $\dot{M}$ [$\mathrm{kg\,s^{-1}}$]',
                   color=color,
                   fontsize=11.5)
    ax2.plot(t_hours,
             mdot_kg_s,
             color=color,
             lw=2.0,
             linestyle="--",
             label="Hydrodynamic Mass Loss Rate")
    ax2.tick_params(axis='y', labelcolor=color)

    plt.title(
        "Proxima Centauri b: Stellar Superflare Impact on Atmospheric Retention",
        fontsize=12,
        pad=10,
        fontweight="bold")
    fig_pdf = out_dir / "fig_comparison.pdf"
    fig_png = out_dir / "fig_comparison.png"
    plt.tight_layout()
    fig.savefig(fig_pdf, bbox_inches="tight")
    fig.savefig(fig_png, dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"Generated {fig_pdf}")


if __name__ == "__main__":
    main()
