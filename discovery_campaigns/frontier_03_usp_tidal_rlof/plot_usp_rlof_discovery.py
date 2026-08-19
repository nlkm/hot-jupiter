"""
Publication plotting script for Frontier 3 Discovery:
USP Tidal Decaying Planet Stripping vs. Catastrophic Disruption.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    track_file = out_dir / "usp_evolution_track.csv"
    grid_file = out_dir / "usp_fate_grid.csv"

    if not track_file.exists() or not grid_file.exists():
        print("Error: CSV files not found. Run simulation driver first.")
        return

    # Parse track
    t, a, p, m, mc, mm, r = [], [], [], [], [], [], []
    with open(track_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            t.append(float(row["time_myr"]))
            a.append(float(row["a_au"]))
            p.append(float(row["period_hr"]))
            m.append(float(row["mass_me"]))
            mc.append(float(row["core_me"]))
            mm.append(float(row["mantle_me"]))
            r.append(float(row["radius_re"]))

    t = np.array(t)
    a = np.array(a)
    p = np.array(p)
    m = np.array(m)
    mc = np.array(mc)
    mm = np.array(mm)
    r = np.array(r)

    # -------------------------------------------------------------------------
    # FIGURE 1: EVOLUTION TRACK OF SUPER-MERCURY FORMATION (TOI-849b ANALOG)
    # -------------------------------------------------------------------------
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5.2))

    ax1.plot(t, m, color="#2c3e50", lw=2.8, label=r"Total Planetary Mass $M_p$")
    ax1.plot(t,
             mm,
             color="#e67e22",
             lw=2.2,
             linestyle="--",
             label=r"Silicate Mantle Mass $M_{\rm mantle}$ (Stripped)")
    ax1.plot(t,
             mc,
             color="#c0392b",
             lw=2.2,
             linestyle="-.",
             label=r"Iron Core Mass $M_{\rm core}$ (Preserved)")

    ax1.axvspan(220,
                3000,
                color="#f39c12",
                alpha=0.12,
                label="Stable Roche Lobe Overflow (RLOF) Phase")

    ax1.set_xlabel("Evolutionary Time [Myr]", fontweight="bold", fontsize=11.5)
    ax1.set_ylabel(r"Planetary Mass [$M_\oplus$]",
                   fontweight="bold",
                   fontsize=11.5)
    ax1.set_title("Mantle Evaporation & Core Stripping (TOI-849b Analog)",
                  fontweight="bold",
                  fontsize=12,
                  pad=8)
    ax1.grid(True, linestyle=":", alpha=0.6)
    ax1.legend(frameon=True,
               facecolor="white",
               fontsize=9.5,
               loc="center right")

    # Orbit & Period
    ax2.plot(t,
             p,
             color="#2980b9",
             lw=2.8,
             label=r"Orbital Period $P_{\rm orb}$ [Hours]")
    ax2.set_xlabel("Evolutionary Time [Myr]", fontweight="bold", fontsize=11.5)
    ax2.set_ylabel("Orbital Period [Hours]", fontweight="bold", fontsize=11.5)
    ax2.set_title(
        "Tidal Inward Plunge Halted by Mass Loss: Orbital Parking at $P \\approx 6.2\\,\\mathrm{h}$",
        fontweight="bold",
        fontsize=12,
        pad=8)
    ax2.grid(True, linestyle=":", alpha=0.6)
    ax2.legend(frameon=True,
               facecolor="white",
               fontsize=10.0,
               loc="lower right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig1_mass_radius_trajectory.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig1_mass_radius_trajectory.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig1_mass_radius_trajectory.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 2: PHASE BOUNDARY (COLLISION VS. STRIPPED SUPER-MERCURY)
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8.5, 5.5))

    grid_data = {"m_core": [], "m_mantle": [], "fate": [], "fe_frac": []}
    with open(grid_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            grid_data["m_core"].append(float(row["m_core_init"]))
            grid_data["m_mantle"].append(float(row["m_mantle_init"]))
            grid_data["fate"].append(row["fate"])
            grid_data["fe_frac"].append(float(row["final_iron_frac"]))

    for k, val in grid_data.items():
        grid_data[k] = np.array(val)

    # Scatter of fates
    mask_coll = grid_data["fate"] == "collision"
    mask_strip = grid_data["fate"] == "stripped_remnant"
    mask_park = grid_data["fate"] == "parked"

    ax.scatter(grid_data["m_core"][mask_coll],
               grid_data["m_mantle"][mask_coll],
               color="#c0392b",
               marker="x",
               s=60,
               label="Catastrophic Plunge (Engulfed by Host Star)")
    ax.scatter(grid_data["m_core"][mask_strip],
               grid_data["m_mantle"][mask_strip],
               color="#27ae60",
               marker="o",
               s=70,
               edgecolors="black",
               label="Stable Roche Stripped Core (Super-Mercury)")
    ax.scatter(grid_data["m_core"][mask_park],
               grid_data["m_mantle"][mask_park],
               color="#2980b9",
               marker="^",
               s=65,
               edgecolors="black",
               label="Parked Rocky Super-Earth")

    ax.set_xlabel(r"Initial Iron Core Mass $M_{\rm core,0}$ [$M_\oplus$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(
        r"Initial Silicate Mantle Mass $M_{\rm mantle,0}$ [$M_\oplus$]",
        fontweight="bold",
        fontsize=11.5)
    ax.set_title(
        "Frontier 3 Phase Diagram: Tidal Fate of Ultra-Short-Period Planets",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=9.5, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig2_usp_phase_boundary.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig2_usp_phase_boundary.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig2_usp_phase_boundary.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 3: POPULATION COMPARISON (CONFIRMED SUPER-MERCURIES)
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8.5, 5.4))

    m_grid = np.linspace(0.5, 30.0, 200)
    # Mass-Radius relations for pure iron, Earth-like (33% Fe, 67% MgSiO3), and pure water
    r_fe = 0.78 * (m_grid**0.30)
    r_earth = 1.00 * (m_grid**0.27)
    r_water = 1.35 * (m_grid**0.29)

    ax.plot(m_grid,
            r_fe,
            color="#c0392b",
            lw=2.5,
            linestyle="-.",
            label="100% Iron Core Model")
    ax.plot(m_grid,
            r_earth,
            color="#2c3e50",
            lw=2.5,
            label="Earth-like Composition (33% Fe, 67% Silicate)")
    ax.plot(m_grid,
            r_water,
            color="#2980b9",
            lw=2.0,
            linestyle=":",
            label="50% Water Ice Model")

    # Landmark Super-Mercuries and bare cores
    usps = [
        ("TOI-849b", 39.1, 1.8, 3.43, 0.12, "#c0392b"),
        ("Kepler-10b", 4.6, 0.4, 1.47, 0.03, "#d35400"),
        ("K2-141b", 5.1, 0.6, 1.51, 0.05, "#e67e22"),
        ("K2-229b", 2.6, 0.4, 1.16, 0.07, "#8e44ad"),
        ("CoRoT-7b", 4.7, 0.8, 1.58, 0.10, "#16a085"),
        ("55 Cnc e", 8.0, 0.3, 1.88, 0.03, "#27ae60"),
    ]

    for name, mp, me, rp, re, col in usps:
        ax.errorbar(mp,
                    rp,
                    xerr=me,
                    yerr=re,
                    fmt='o',
                    color=col,
                    ecolor=col,
                    elinewidth=1.8,
                    capsize=4,
                    markersize=7.5)
        ax.annotate(name,
                    xy=(mp, rp),
                    xytext=(mp * 1.12, rp - 0.08),
                    fontsize=9.5,
                    fontweight="bold",
                    color=col)

    ax.set_xscale("log")
    ax.set_xlabel(r"Planetary Mass $M_p$ [$M_\oplus$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(r"Planetary Radius $R_p$ [$R_\oplus$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Super-Mercury Remnants: Comparison of RLOF Stripping Model to Transiting USPs",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(0.5, 50.0)
    ax.set_ylim(0.5, 3.2)
    ax.legend(frameon=True, facecolor="white", fontsize=10.0, loc="upper left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig3_population_super_mercuries.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig3_population_super_mercuries.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig3_population_super_mercuries.pdf")
    print("All 3 Frontier 3 discovery figures generated successfully!")


if __name__ == "__main__":
    main()
