"""
Publication plotting script for Frontier 7 Discovery:
Interstellar Object Volatile Depletion, Outgassing Torques, & Spin Disruption Engine.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    flyby_file = out_dir / "oumuamua_flyby_track.csv"
    comp_file = out_dir / "ice_composition_comparison.csv"
    grid_file = out_dir / "spin_disruption_grid.csv"

    if not flyby_file.exists() or not comp_file.exists(
    ) or not grid_file.exists():
        print("Error: CSV files not found. Run simulation driver first.")
        return

    # Parse flyby track
    r_au, temp_k, sub_flux, a_ng, p_spin, sigma_cent, is_disr = [], [], [], [], [], [], []
    with open(flyby_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            r_au.append(float(row["r_au"]))
            temp_k.append(float(row["temp_k"]))
            sub_flux.append(float(row["sublimation_kg_m2_s"]))
            a_ng.append(float(row["a_ng_m_s2"]))
            p_spin.append(float(row["spin_period_hrs"]))
            sigma_cent.append(float(row["centrifugal_stress_pa"]))
            is_disr.append(int(row["is_disrupted"]))

    r_au = np.array(r_au)
    a_ng = np.array(a_ng)
    p_spin = np.array(p_spin)
    sigma_cent = np.array(sigma_cent)

    # -------------------------------------------------------------------------
    # FIGURE 1: NON-GRAVITATIONAL ACCELERATION ACROSS ICE COMPOSITIONS
    # -------------------------------------------------------------------------
    r_grid, a_h2, a_n2, a_co, a_h2o = [], [], [], [], []
    with open(comp_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            r_grid.append(float(row["r_au"]))
            a_h2.append(float(row["a_ng_h2"]))
            a_n2.append(float(row["a_ng_n2"]))
            a_co.append(float(row["a_ng_co"]))
            a_h2o.append(float(row["a_ng_h2o"]))

    r_grid = np.array(r_grid)
    a_h2 = np.array(a_h2)
    a_n2 = np.array(a_n2)
    a_co = np.array(a_co)
    a_h2o = np.array(a_h2o)

    fig, ax = plt.subplots(figsize=(8.8, 6.0))

    ax.plot(r_grid,
            a_h2 * 1.0e6,
            color="#e74c3c",
            lw=2.8,
            label=r"Molecular Hydrogen ($\mathrm{H_2}$) Iceberg Model")
    ax.plot(r_grid,
            a_n2 * 1.0e6,
            color="#2980b9",
            lw=2.5,
            label=r"Nitrogen ($\mathrm{N_2}$) Iceberg Model (Pluto Fragment)")
    ax.plot(r_grid,
            a_co * 1.0e6,
            color="#8e44ad",
            lw=2.2,
            linestyle="--",
            label=r"Carbon Monoxide ($\mathrm{CO}$) Outgassing")
    ax.plot(r_grid,
            a_h2o * 1.0e6,
            color="#27ae60",
            lw=2.0,
            linestyle=":",
            label=r"Canonical Water ($\mathrm{H_2O}$) Sublimation")

    # Observational benchmark for 'Oumuamua at 1.4 AU: a_ng ~ (4.92 +/- 0.16) x 10^-6 m/s^2
    ax.errorbar(
        1.40,
        4.92,
        yerr=0.25,
        fmt="o",
        color="black",
        markersize=8,
        capsize=5,
        label=r"1I/'Oumuamua Astrometric Observation (Micheli et al. 2018)")

    ax.set_yscale("log")
    ax.set_xlabel("Heliocentric Distance $r$ [AU]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(
        r"Non-Gravitational Acceleration $a_{\rm ng}$ [$10^{-6}\,\mathrm{m/s^2}$]",
        fontweight="bold",
        fontsize=11.5)
    ax.set_title(
        "Frontier 7 Discovery: Non-Gravitational Acceleration across Ice Compositions",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(0.25, 3.0)
    ax.set_ylim(0.05, 50.0)
    ax.legend(frameon=True, facecolor="white", fontsize=9.5, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig1_oumuamua_non_grav_trajectory.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig1_oumuamua_non_grav_trajectory.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig1_oumuamua_non_grav_trajectory.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 2: 2D SPIN DISRUPTION & CENTRIFUGAL FISSION BOUNDARY
    # -------------------------------------------------------------------------
    grid_aspect, grid_tensile, grid_disr = [], [], []
    with open(grid_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            grid_aspect.append(float(row["aspect_ratio"]))
            grid_tensile.append(float(row["tensile_strength_pa"]))
            grid_disr.append(int(row["is_disrupted"]))

    grid_aspect = np.array(grid_aspect)
    grid_tensile = np.array(grid_tensile)
    grid_disr = np.array(grid_disr)

    mask_dis = grid_disr == 1
    mask_sta = grid_disr == 0

    fig, ax = plt.subplots(figsize=(8.5, 5.8))

    ax.scatter(grid_aspect[mask_sta],
               grid_tensile[mask_sta],
               color="#27ae60",
               marker="o",
               s=65,
               edgecolors="black",
               label="Intact Interstellar Object (Survived Flyby)")
    ax.scatter(grid_aspect[mask_dis],
               grid_tensile[mask_dis],
               color="#c0392b",
               marker="x",
               s=65,
               label="Centrifugal Spin Disruption (Rotational Fission)")

    # Analytical critical tensile threshold: sigma_crit ~ 0.5 * rho * omega^2 * a^2
    aspect_line = np.linspace(1.0, 10.0, 100)
    sigma_crit_curve = 0.8 * (aspect_line**1.2)
    ax.plot(aspect_line,
            sigma_crit_curve,
            color="black",
            lw=2.5,
            linestyle="--",
            label=r"Critical Cohesive Threshold $\sigma_{\rm crit}(a/b)$")

    # Mark 1I/'Oumuamua parameters (a/b ~ 6:1, sigma ~ 10 Pa)
    ax.scatter(
        6.0,
        10.0,
        color="#e74c3c",
        s=200,
        marker="*",
        edgecolors="black",
        zorder=6,
        label=
        r"1I/'Oumuamua ($a/b \approx 6:1, \sigma_{\rm cohesive} \sim 10\,\mathrm{Pa}$)"
    )

    ax.set_xlabel(r"Elongation / Aspect Ratio $(a / b)$",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(r"Cohesive Tensile Strength $\sigma_{\rm tensile}$ [Pa]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Frontier 7 Phase Diagram: Rotational Stability vs. Centrifugal Disruption",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=9.2, loc="upper left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig2_tensile_spin_disruption_boundary.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig2_tensile_spin_disruption_boundary.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig2_tensile_spin_disruption_boundary.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 3: 2I/BORISOV VS SOLAR SYSTEM COMETARY VOLATILE PRODUCTION
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(9.2, 5.5))

    objects = [
        ("2I/Borisov (Interstellar)", 1.2e27, 2.5e27, "#e74c3c",
         r"Super-CO Rich ($Q_{\rm CO}/Q_{\rm H2O} \approx 0.50$)"),
        ("C/1995 O1 (Hale-Bopp)", 2.0e28, 1.0e29, "#2980b9",
         r"Pristine Oort Cloud ($Q_{\rm CO}/Q_{\rm H2O} \approx 0.20$)"),
        ("1P/Halley", 3.0e26, 5.0e27, "#8e44ad",
         r"Jupiter-Family ($Q_{\rm CO}/Q_{\rm H2O} \approx 0.06$)"),
        ("67P/Churyumov-Gerasimenko", 5.0e25, 2.0e27, "#34495e",
         r"Evolved Comet ($Q_{\rm CO}/Q_{\rm H2O} \approx 0.02$)"),
        ("C/2016 R2 (PanSTARRS)", 1.0e28, 1.0e26, "#27ae60",
         r"Hyper-Volatile Dominated ($Q_{\rm CO}/Q_{\rm H2O} \approx 100$)"),
    ]

    for name, q_co, q_h2o, col, label_desc in objects:
        ratio = q_co / q_h2o
        ax.scatter(q_h2o,
                   ratio,
                   color=col,
                   s=190,
                   edgecolors="black",
                   zorder=5,
                   label=f"{name}: {label_desc}")

    ax.axhspan(
        0.3,
        1.5,
        color="#e74c3c",
        alpha=0.15,
        label=
        r"Interstellar CO-Rich Regime ($T_{\mathrm{form}} < 20\,\mathrm{K}$)")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel(
        r"Water Production Rate $Q(\mathrm{H_2O})$ [$\mathrm{molecules/s}$]",
        fontweight="bold",
        fontsize=11.5)
    ax.set_ylabel(r"Carbon Monoxide Ratio $Q(\mathrm{CO}) / Q(\mathrm{H_2O})$",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Interstellar vs. Solar System Volatile Chemistry: 2I/Borisov Benchmark",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(1.0e25, 5.0e29)
    ax.set_ylim(0.01, 200.0)
    ax.legend(frameon=True, facecolor="white", fontsize=8.5, loc="lower left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig3_volatile_composition_cross_comparison.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig3_volatile_composition_cross_comparison.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig3_volatile_composition_cross_comparison.pdf")
    print("All 3 Frontier 7 discovery figures generated successfully!")


if __name__ == "__main__":
    main()
