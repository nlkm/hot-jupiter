"""
Publication plotting script for Frontier 8 Discovery:
Frequency-Dependent Andrade Viscoelastic Tidal Dissipation & Thermal Equilibrium Engine.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    io_file = out_dir / "io_thermal_spectrum.csv"
    freq_file = out_dir / "frequency_response.csv"
    map_file = out_dir / "trappist1e_heating_map.csv"

    if not io_file.exists() or not freq_file.exists() or not map_file.exists():
        print("Error: CSV files not found. Run simulation driver first.")
        return

    # Parse Io thermal spectrum
    t_k, im_and, p_and, im_max, p_max, p_conv = [], [], [], [], [], []
    with open(io_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            t_k.append(float(row["temp_k"]))
            im_and.append(float(row["im_k2_andrade"]))
            p_and.append(float(row["p_tide_andrade_tw"]))
            im_max.append(float(row["im_k2_maxwell"]))
            p_max.append(float(row["p_tide_maxwell_tw"]))
            p_conv.append(float(row["p_conv_tw"]))

    t_k = np.array(t_k)
    im_and = np.array(im_and)
    p_and = np.array(p_and)
    p_max = np.array(p_max)
    p_conv = np.array(p_conv)

    # -------------------------------------------------------------------------
    # FIGURE 1: FREQUENCY RESPONSE ACROSS TIDAL PERIODS & TEMPERATURES
    # -------------------------------------------------------------------------
    p_days, im_1400, im_1600, im_1800 = [], [], [], []
    with open(freq_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            p_days.append(float(row["period_days"]))
            im_1400.append(float(row["im_k2_1400k"]))
            im_1600.append(float(row["im_k2_1600k"]))
            im_1800.append(float(row["im_k2_1800k"]))

    p_days = np.array(p_days)
    im_1400 = np.array(im_1400)
    im_1600 = np.array(im_1600)
    im_1800 = np.array(im_1800)

    fig, ax = plt.subplots(figsize=(8.8, 6.0))

    ax.plot(p_days,
            im_1400,
            color="#2980b9",
            lw=2.5,
            label=r"Andrade Rheology: $T_{\rm mantle} = 1400\,\mathrm{K}$")
    ax.plot(
        p_days,
        im_1600,
        color="#e67e22",
        lw=2.8,
        label=r"Andrade Rheology: $T_{\rm mantle} = 1600\,\mathrm{K}$ (Io Peak)"
    )
    ax.plot(
        p_days,
        im_1800,
        color="#e74c3c",
        lw=2.5,
        label=
        r"Andrade Rheology: $T_{\rm mantle} = 1800\,\mathrm{K}$ (Partial Melt)")

    # Mark Io's orbital period
    ax.axvline(
        1.769,
        color="purple",
        linestyle="--",
        lw=2.0,
        label=r"Io Orbital Forcing Period ($P_{\rm orb} = 1.769\,\mathrm{d}$)")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Tidal Forcing Period $P_{\\rm tide}$ [days]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(r"Tidal Dissipation Metric $\operatorname{Im}(k_2(\omega))$",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Frontier 8 Discovery: Frequency-Dependent Andrade Mantle Dissipation",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(0.1, 100.0)
    ax.set_ylim(1.0e-5, 0.5)
    ax.legend(frameon=True, facecolor="white", fontsize=9.2, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig1_andrade_frequency_response.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig1_andrade_frequency_response.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig1_andrade_frequency_response.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 2: THERMAL-ORBITAL EQUILIBRIUM BALANCE FOR IO (100 TW BENCHMARK)
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8.5, 5.8))

    ax.plot(t_k,
            p_and,
            color="#e74c3c",
            lw=2.8,
            label=r"Andrade Tidal Heating Power $E_{\rm tide}(T)$")
    ax.plot(t_k,
            p_max,
            color="#8e44ad",
            lw=2.0,
            linestyle="--",
            label=r"Classical Maxwell Heating (Narrow Resonant Peak)")
    ax.plot(t_k,
            p_conv,
            color="#2980b9",
            lw=2.5,
            label=r"Mantle Convective Heat Loss $F_{\rm conv}(T)$")

    # Observational benchmark box for Io
    ax.axhspan(
        90.0,
        115.0,
        color="#f1c40f",
        alpha=0.25,
        label=r"Io Observed Volcanic Heat Flux ($105 \pm 10\,\mathrm{TW}$)")

    # Find intersection equilibrium point
    diff = np.abs(p_and - p_conv)
    idx_eq = np.argmin(diff)
    t_eq = t_k[idx_eq]
    p_eq = p_and[idx_eq]

    ax.scatter(
        t_eq,
        p_eq,
        color="darkred",
        s=160,
        zorder=6,
        edgecolors="black",
        label=f"Stable Thermal Equilibrium @ {t_eq:.0f} K ({p_eq:.1f} TW)")

    ax.set_yscale("log")
    ax.set_xlabel("Silicate Mantle Temperature $T_{\\rm mantle}$ [K]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel("Power [TW]", fontweight="bold", fontsize=11.5)
    ax.set_title(
        "Frontier 8: Io Thermal-Orbital Equilibrium with Andrade Rheology",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(1000, 2000)
    ax.set_ylim(1.0, 1000.0)
    ax.legend(frameon=True, facecolor="white", fontsize=9.0, loc="lower left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig2_thermal_orbital_equilibrium_io.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig2_thermal_orbital_equilibrium_io.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig2_thermal_orbital_equilibrium_io.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 3: TRAPPIST-1e / SUPER-EARTH TIDAL HEATING MAP
    # -------------------------------------------------------------------------
    grid_a, grid_e, grid_flux, grid_run = [], [], [], []
    with open(map_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            grid_a.append(float(row["semi_major_axis_au"]))
            grid_e.append(float(row["eccentricity"]))
            grid_flux.append(float(row["heat_flux_w_m2"]))
            grid_run.append(int(row["is_runaway"]))

    grid_a = np.array(grid_a)
    grid_e = np.array(grid_e)
    grid_flux = np.array(grid_flux)

    fig, ax = plt.subplots(figsize=(9.2, 5.8))

    scatter = ax.scatter(grid_a,
                         grid_e,
                         c=np.log10(np.maximum(1.0e-3, grid_flux)),
                         cmap="inferno",
                         s=80,
                         edgecolors="none")
    cbar = plt.colorbar(scatter, ax=ax)
    cbar.set_label(
        r"$\log_{10}(\text{Surface Tidal Heat Flux } F_{\rm tide}\; [\mathrm{W/m^2}])$",
        fontweight="bold",
        fontsize=10.5)

    # Contour of runaway volcanism (F_tide = 10 W/m^2)
    ax.scatter(
        0.029,
        0.005,
        color="#2ecc71",
        marker="*",
        s=250,
        edgecolors="black",
        zorder=6,
        label=
        r"TRAPPIST-1e ($a = 0.029\,\mathrm{AU}, e \approx 0.005$, Temperate)")
    ax.scatter(
        0.015,
        0.08,
        color="#e74c3c",
        marker="o",
        s=160,
        edgecolors="black",
        zorder=6,
        label=r"Super-Io Extreme Volcanism Regime ($F > 10\,\mathrm{W/m^2}$)")

    ax.set_xlabel(r"Semi-Major Axis $a$ [AU]", fontweight="bold", fontsize=11.5)
    ax.set_ylabel(r"Orbital Eccentricity $e$", fontweight="bold", fontsize=11.5)
    ax.set_title(
        "TRAPPIST-1 / Super-Earth Tidal Heating & Habitability Phase Space",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=9.0, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig3_trappist1_super_earth_heating_map.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig3_trappist1_super_earth_heating_map.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig3_trappist1_super_earth_heating_map.pdf")
    print("All 3 Frontier 8 discovery figures generated successfully!")


if __name__ == "__main__":
    main()
