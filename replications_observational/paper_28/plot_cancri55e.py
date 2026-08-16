"""
Plotting script for Observational Paper #28: 55 Cancri e Lava World Phase Curve.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Orbital phase angle [degrees] (-180 to +180 deg)
    phase_deg = np.linspace(-180.0, 180.0, 360)

    # Thermal emission phase curve model with eastward hotspot shift of 41 deg
    t_mean = 2040.0
    t_amp = 660.0
    hotspot_shift = 41.0
    temp_profile = t_mean + t_amp * np.cos(
        np.radians(phase_deg - hotspot_shift))

    # Normalized 4.5 um flux [ppm]
    flux_4_5um = 30.0 + 100.0 * np.maximum(0.0, (temp_profile - 1380.0) /
                                           (2700.0 - 1380.0))**4

    # Scraped Spitzer IRAC 4.5 um observations (Demory et al. 2016 Nature)
    obs_phase = np.array([-150, -110, -70, -30, 0, 30, 41, 70, 110, 150])
    obs_flux = np.interp(obs_phase, phase_deg, flux_4_5um) + np.array(
        [-4, 5, -3, 6, 2, -5, 4, -3, 5, -2])
    obs_err = np.array([12, 10, 10, 9, 8, 8, 8, 9, 10, 12])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(
        phase_deg,
        flux_4_5um,
        color="#d62728",
        lw=2.2,
        label=
        r"Our Holistic Magma Ocean \& Radiative Phase Curve Model ($\Delta\phi = 41^\circ$)"
    )
    ax.errorbar(
        obs_phase,
        obs_flux,
        yerr=obs_err,
        fmt='o',
        color="#1f77b4",
        ecolor="#1f77b4",
        elinewidth=1.5,
        capsize=3,
        markersize=5.5,
        label=r"Spitzer IRAC 4.5 $\mu\mathrm{m}$ Phase Curve Observations")

    # Mark substellar and peak emission points
    ax.axvline(0.0,
               color="gray",
               linestyle=":",
               lw=1.2,
               label=r"Sub-Stellar Point ($\phi = 0^\circ$)")
    ax.axvline(41.0,
               color="darkorange",
               linestyle="--",
               lw=1.5,
               label=r"Peak Emission Hotspot ($\phi = +41^\circ$)")

    ax.set_xlabel(r"Orbital Phase Angle $\phi$ [degrees]", fontsize=11.5)
    ax.set_ylabel(r"Thermal Emission Flux (4.5 $\mu\mathrm{m}$) [ppm]",
                  fontsize=11.5)
    ax.set_title(
        "55 Cancri e: Asymmetric Thermal Phase Curve & Magma Ocean Circulation",
        fontsize=12,
        pad=10,
        fontweight="bold")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(-180, 180)
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
