"""
Publication plotting script for Frontier 6 Discovery:
Ocean-Freezing Pressurization & Viscoelastic Cryosphere Fracture Engine.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    charon_file = out_dir / "charon_freezing_track.csv"
    grid_file = out_dir / "fracture_phase_grid.csv"

    if not charon_file.exists() or not grid_file.exists():
        print("Error: CSV files not found. Run simulation driver first.")
        return

    # Parse Charon track
    t_ch, h_ice_ch, h_oc_ch, dp_ch, hoop_ch = [], [], [], [], []
    with open(charon_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            t_ch.append(float(row["time_myr"]))
            h_ice_ch.append(float(row["h_ice_km"]))
            h_oc_ch.append(float(row["h_ocean_km"]))
            dp_ch.append(float(row["delta_p_mpa"]))
            hoop_ch.append(float(row["hoop_stress_mpa"]))

    t_ch = np.array(t_ch)
    h_ice_ch = np.array(h_ice_ch)
    dp_ch = np.array(dp_ch)
    hoop_ch = np.array(hoop_ch)

    # -------------------------------------------------------------------------
    # FIGURE 1: OCEAN FREEZING & EXTENSIONAL HOOP STRESS EVOLUTION
    # -------------------------------------------------------------------------
    fig, (ax1, ax2) = plt.subplots(2,
                                   1,
                                   figsize=(10, 7.2),
                                   sharex=True,
                                   gridspec_kw={"height_ratios": [1.5, 1.5]})

    ax1.plot(t_ch,
             h_ice_ch,
             color="#2980b9",
             lw=2.8,
             label=r"Ice Shell Thickness $h_{\rm ice}(t)$ [km]")
    ax1.set_ylabel("Ice Shell Thickness [km]", fontweight="bold", fontsize=11.5)
    ax1.set_title(
        "Frontier 6 Discovery: Charon Cryosphere Freezing & Pressurization Evolution",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax1.grid(True, linestyle=":", alpha=0.6)
    ax1.legend(frameon=True, facecolor="white", fontsize=9.5, loc="upper left")

    # Hoop stress and tensile rupture
    ax2.plot(
        t_ch,
        hoop_ch,
        color="#e74c3c",
        lw=2.8,
        label=r"Surface Extensional Hoop Stress $\sigma_{\theta\theta}(t)$")
    ax2.plot(t_ch,
             dp_ch,
             color="#8e44ad",
             lw=2.0,
             linestyle="--",
             label=r"Internal Ocean Overpressure $\Delta P_{\rm ocean}(t)$")
    ax2.axhline(
        2.0,
        color="black",
        linestyle=":",
        lw=2.0,
        label=
        r"Tensile Strength Threshold $\sigma_{\rm tensile} = 2.0\,\mathrm{MPa}$"
    )

    # Shading for fracture regime
    mask_frac = hoop_ch >= 2.0
    if np.any(mask_frac):
        t_frac = t_ch[mask_frac][0]
        ax2.axvline(t_frac, color="darkred", linestyle="-.", lw=1.8)
        ax2.annotate(
            f"Catastrophic Tensile Failure\n(Serenity Chasma Rupture @ {t_frac:.0f} Myr)",
            xy=(t_frac, 2.5),
            xytext=(t_frac + 80, 4.0),
            arrowprops=dict(facecolor="black", shrink=0.08, width=1.5),
            fontsize=9.5,
            fontweight="bold",
            color="darkred")

    ax2.set_xlabel("Secular Freezing Time [Myr]",
                   fontweight="bold",
                   fontsize=11.5)
    ax2.set_ylabel("Mechanical Stress [MPa]", fontweight="bold", fontsize=11.5)
    ax2.grid(True, linestyle=":", alpha=0.6)
    ax2.set_ylim(0, 12.0)
    ax2.legend(frameon=True, facecolor="white", fontsize=9.5, loc="upper left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig1_ocean_freezing_stress_evolution.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig1_ocean_freezing_stress_evolution.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig1_ocean_freezing_stress_evolution.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 2: 2D VISCOELASTIC RELAXATION PHASE BOUNDARY
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8.5, 6.0))

    grid_t, grid_rate, grid_mode = [], [], []
    with open(grid_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            grid_t.append(float(row["lid_temp_k"]))
            grid_rate.append(float(row["freezing_rate_km_myr"]))
            grid_mode.append(row["failure_mode"])

    grid_t = np.array(grid_t)
    grid_rate = np.array(grid_rate)
    grid_mode = np.array(grid_mode)

    mask_rup = grid_mode == "brittle_rupture"
    mask_duc = grid_mode == "ductile_relaxation"

    ax.scatter(grid_t[mask_rup],
               grid_rate[mask_rup],
               color="#c0392b",
               marker="o",
               s=70,
               edgecolors="black",
               label="Brittle Tensile Rupture (Canyons / Chasmata Form)")
    ax.scatter(grid_t[mask_duc],
               grid_rate[mask_duc],
               color="#2980b9",
               marker="s",
               s=65,
               label="Ductile Viscous Relaxation (No Global Rifts)")

    ax.set_xlabel("Brittle Lid Temperature $T_{\\rm lid}$ [K]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel(r"Ocean Freezing Rate $\dot{h}_{\rm ice}$ [km / Myr]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Frontier 6 Phase Diagram: Brittle Cryosphere Rupture vs. Viscous Creep",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(frameon=True, facecolor="white", fontsize=9.5, loc="upper right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig2_viscoelastic_relaxation_boundary.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig2_viscoelastic_relaxation_boundary.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig2_viscoelastic_relaxation_boundary.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 3: CROSS-BODY TECTONIC CANYON DEPTHS VS SHELL THICKNESS
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(9.2, 5.5))

    bodies = [
        ("Charon (Serenity Chasma)", 606.0, 9.0, "#e74c3c", "Total Freezing"),
        ("Tethys (Ithaca Chasma)", 531.0, 5.0, "#e67e22", "Extensive Freezing"),
        ("Enceladus (Tiger Stripes)", 252.0, 0.5, "#3498db",
         "Active Tidal Ocean"),
        ("Europa (Astypalaea Linea)", 1560.0, 0.3, "#2ecc71",
         "Active Tidal Ocean"),
        ("Ganymede (Grooved Terrain)", 2634.0, 1.5, "#9b59b6",
         "High-Pressure Ice"),
        ("Ariel (Kachina Chasma)", 578.0, 6.0, "#34495e", "Partial Freezing"),
    ]

    for name, r_val, canyon_km, col, note in bodies:
        ax.scatter(r_val,
                   canyon_km,
                   color=col,
                   s=180,
                   edgecolors="black",
                   zorder=5,
                   label=f"{name}: {canyon_km} km ({note})")

    r_theory = np.linspace(200.0, 3000.0, 100)
    canyon_theory = 0.015 * r_theory  # Graben depth scaling ~ 1.5% of body radius
    ax.plot(
        r_theory,
        canyon_theory,
        color="black",
        linestyle="--",
        lw=2.0,
        label=
        r"Theoretical Freezing Graben Scaling $d_{\rm canyon} \sim 0.015 R_{\rm body}$"
    )

    ax.set_xlabel("Planetary / Moon Radius $R_{\\rm body}$ [km]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel("Observed Tectonic Canyon Depth [km]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Outer Solar System Cryosphere Tectonic Graben Depths vs. Body Radius",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(150, 3000)
    ax.set_ylim(0, 12.0)
    ax.legend(frameon=True, facecolor="white", fontsize=9.0, loc="upper left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig3_canyon_depth_cross_comparison.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig3_canyon_depth_cross_comparison.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig3_canyon_depth_cross_comparison.pdf")
    print("All 3 Frontier 6 discovery figures generated successfully!")


if __name__ == "__main__":
    main()
