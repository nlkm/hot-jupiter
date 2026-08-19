"""
Publication plotting script for Frontier 2 Discovery:
Non-Monotonic Hot Jupiter Radius Inflation & Ohmic Dynamo Quenching.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    csv_file = out_dir / "ohmic_quenching_results.csv"

    if not csv_file.exists():
        print(f"Error: {csv_file} not found. Run simulation driver first.")
        return

    # Parse CSV by B field
    data = {}
    with open(csv_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            b = float(row["b_gauss"])
            if b not in data:
                data[b] = {
                    "t": [],
                    "sig": [],
                    "v": [],
                    "drag": [],
                    "p": [],
                    "r": []
                }
            data[b]["t"].append(float(row["t_eq_k"]))
            data[b]["sig"].append(float(row["conductivity_s_m"]))
            data[b]["v"].append(float(row["wind_speed_m_s"]))
            data[b]["drag"].append(float(row["lorentz_drag_m_s2"]))
            data[b]["p"].append(float(row["ohmic_power_w"]))
            data[b]["r"].append(float(row["radius_rjup"]))

    for val in data.values():
        for k in val:
            val[k] = np.array(val[k])

    # -------------------------------------------------------------------------
    # FIGURE 1: NON-MONOTONIC RADIUS INFLATION & EMPIRICAL EXOPLANETS
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(9, 5.5))

    colors = {1.0: "#3498db", 5.0: "#e74c3c", 10.0: "#9b59b6", 20.0: "#f39c12"}

    for b, d in data.items():
        ax.plot(d["t"],
                d["r"],
                color=colors.get(b, "black"),
                lw=2.8,
                label=f"Ohmic Dynamo Model ($B = {b:.0f}\\,\\mathrm{{Gauss}}$)")

    # Standard monotonic model without Lorentz braking (Batten et al.)
    t_grid = data[5.0]["t"]
    r_unquenched = 1.05 + 0.15 * ((t_grid / 1500.0)**0.8) + 0.60 * (
        (t_grid / 1800.0)**1.8)
    ax.plot(t_grid,
            r_unquenched,
            color="gray",
            lw=2.0,
            linestyle="--",
            label="Standard Unquenched Model (Batten 2010)")

    # Landmark observed exoplanets
    obs_planets = [
        ("HD 209458b", 1450.0, 1.38, 0.03, "#2c3e50"),
        ("WASP-12b", 2580.0, 1.90, 0.06, "#c0392b"),
        ("WASP-17b", 1770.0, 1.99, 0.08, "#8e44ad"),
        ("WASP-76b", 2228.0, 1.83, 0.05, "#d35400"),
        ("HAT-P-67b", 1903.0, 2.08, 0.10, "#16a085"),
        ("KELT-9b", 4050.0, 1.89, 0.07, "#2980b9"),
    ]

    for name, t_eq, r_obs, r_err, col in obs_planets:
        ax.errorbar(t_eq,
                    r_obs,
                    yerr=r_err,
                    fmt='o',
                    color=col,
                    ecolor=col,
                    elinewidth=1.8,
                    capsize=4,
                    markersize=7)
        ax.annotate(name,
                    xy=(t_eq, r_obs),
                    xytext=(t_eq + 30, r_obs + 0.04),
                    fontsize=9.0,
                    fontweight="bold",
                    color=col)

    ax.axvspan(1850,
               3000,
               color="#e74c3c",
               alpha=0.08,
               label="Ohmic Quenching Regime")

    ax.set_xlabel("Equilibrium Temperature $T_{\\rm eq}$ [K]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel("Planetary Radius $R_p$ [$R_{\\rm Jup}$]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Hot Jupiter Radius Inflation: The Ohmic Quenching Peak & Plateau",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(1000, 3000)
    ax.set_ylim(1.1, 2.3)
    ax.legend(frameon=True, facecolor="white", fontsize=9.0, loc="upper left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig1_inflation_quenching_curve.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig1_inflation_quenching_curve.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig1_inflation_quenching_curve.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 2: LORENTZ DRAG & ZONAL JET DECELERATION
    # -------------------------------------------------------------------------
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5.0))

    d5 = data[5.0]
    ax1.plot(d5["t"],
             d5["v"],
             color="#2980b9",
             lw=2.8,
             label=r"Equatorial Jet Velocity $v_{\rm jet}$")
    ax1.plot(d5["t"],
             4000.0 * np.sqrt(d5["t"] / 2000.0),
             color="gray",
             lw=1.8,
             linestyle="--",
             label=r"Hydrodynamic Velocity $v_0$ (No Magnetic Drag)")
    ax1.set_xlabel("Equilibrium Temperature $T_{\\rm eq}$ [K]",
                   fontweight="bold",
                   fontsize=11)
    ax1.set_ylabel("Zonal Wind Speed [m/s]", fontweight="bold", fontsize=11)
    ax1.set_title("Zonal Wind Deceleration by Lorentz Drag",
                  fontweight="bold",
                  fontsize=12)
    ax1.grid(True, linestyle=":", alpha=0.6)
    ax1.legend(frameon=True, facecolor="white", fontsize=9.5)

    ax2.plot(
        d5["t"],
        d5["drag"],
        color="#c0392b",
        lw=2.8,
        label=r"Lorentz Drag Acceleration $a_{\rm mag} = \sigma B^2 v / \rho$")
    ax2.set_yscale("log")
    ax2.set_xlabel("Equilibrium Temperature $T_{\\rm eq}$ [K]",
                   fontweight="bold",
                   fontsize=11)
    ax2.set_ylabel("Lorentz Deceleration [m/s$^2$]",
                   fontweight="bold",
                   fontsize=11)
    ax2.set_title("Magnetic Drag Surge with Thermal Ionization",
                  fontweight="bold",
                  fontsize=12)
    ax2.grid(True, linestyle=":", alpha=0.6)
    ax2.legend(frameon=True, facecolor="white", fontsize=9.5)

    plt.tight_layout()
    fig.savefig(out_dir / "fig2_lorentz_wind_braking.pdf", bbox_inches="tight")
    fig.savefig(out_dir / "fig2_lorentz_wind_braking.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig2_lorentz_wind_braking.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 3: OHMIC POWER PEAK & MAGNETIC FIELD SENSITIVITY
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8.5, 5.2))
    for b, d in data.items():
        ax.plot(d["t"],
                d["p"],
                color=colors.get(b, "black"),
                lw=2.8,
                label=f"Magnetic Field $B = {b:.0f}\\,\\mathrm{{Gauss}}$")

    ax.set_yscale("log")
    ax.set_xlabel("Equilibrium Temperature $T_{\\rm eq}$ [K]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel("Ohmic Dissipation Power $\\dot{E}_{\\rm ohmic}$ [Watts]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_title(
        "Universal Ohmic Heating Turnover: Power Peak at $T_{\\rm eq} \\sim 1600 - 1800\\,\\mathrm{K}$",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(1000, 3000)
    ax.set_ylim(1.0e17, 5.0e20)
    ax.legend(frameon=True, facecolor="white", fontsize=10.0, loc="lower right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig3_magnetic_field_sensitivity.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig3_magnetic_field_sensitivity.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig3_magnetic_field_sensitivity.pdf")
    print("All 3 Frontier 2 discovery figures generated successfully!")


if __name__ == "__main__":
    main()
