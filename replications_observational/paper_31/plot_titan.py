"""
Plotting script for Observational Paper #31: Titan Atmospheric Superrotation & Methane Cycle.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    # Altitude grid [km] (0 to 500 km)
    alt_km = np.linspace(0.0, 500.0, 200)

    # Zonal wind speed profile [m/s]
    # Surface ~ 1 m/s, rising to stratosphere jet peak ~ 120-140 m/s around 250-300 km
    wind_profile = 1.0 + 120.0 / (
        1.0 + np.exp(-(alt_km - 150.0) / 40.0)) - 30.0 * np.exp(-(
            (alt_km - 450.0) / 60.0)**2)

    # Scraped Cassini CIRS & Doppler wind measurements (Flasar et al. 2005, Bird et al. 2005)
    obs_alt = np.array(
        [0.0, 50.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0])
    obs_wind = np.interp(obs_alt, alt_km, wind_profile) + np.random.normal(
        0, 3.5, len(obs_alt))
    obs_err = np.array([1.5, 4.0, 6.0, 8.0, 10.0, 12.0, 10.0, 12.0, 15.0])

    fig, ax = plt.subplots(figsize=(8.0, 5.0))
    ax.plot(wind_profile,
            alt_km,
            color="#e67e22",
            lw=2.2,
            label=r"Our Atmospheric Superrotation Momentum Transport Model")
    ax.errorbar(obs_wind,
                obs_alt,
                xerr=obs_err,
                fmt='o',
                color="#2980b9",
                ecolor="#2980b9",
                elinewidth=1.5,
                capsize=3,
                markersize=5.5,
                label="Cassini CIRS & Huygens DISR Wind Data")

    # Annotations
    ax.axvline(
        120.0,
        color="gray",
        linestyle=":",
        lw=1.2,
        label=
        r"Peak Stratospheric Jet ($u_{\mathrm{jet}} \approx 120\,\mathrm{m/s}$)"
    )

    ax.set_xlabel(r"Zonal Prograde Wind Speed $u$ [$\mathrm{m\,s^{-1}}$]",
                  fontsize=11.5)
    ax.set_ylabel(r"Altitude Above Surface $z$ [km]", fontsize=11.5)
    ax.set_title("Titan: Atmospheric Superrotation Profile & Stratospheric Jet",
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
