#!/usr/bin/env python3
"""Generate publication-quality figures for Paper #207 Replication.

Tobie, Mocquet, & Sotin (2005) "Tidal Dissipation in Titan's Interior"
Icarus 177 (2005) 534–549.

Figures generated:
  1. fig_comparison.pdf: Love number k2 vs ocean thickness with Cassini constraints
  2. fig_model_choices.pdf: Tidal phase lag delta and heating power P_tide vs viscosity
  3. fig_diagram.pdf: Titan interior ocean-crust layer schematic & tidal deformation
"""

import csv
import os

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import patches

# Configure publication-quality matplotlib style
plt.rcParams.update({
    "font.family": "serif",
    "font.size": 11,
    "axes.labelsize": 12,
    "axes.titlesize": 13,
    "xtick.labelsize": 10,
    "ytick.labelsize": 10,
    "legend.fontsize": 10,
    "figure.titlesize": 14,
    "figure.dpi": 300,
    "savefig.dpi": 300,
    "text.usetex": False,
    "mathtext.fontset": "cm",
})

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


def read_csv_data(filepath):
    """Read numeric columns from CSV into numpy arrays."""
    data = {}
    with open(filepath, "r") as f:
        reader = csv.DictReader(f)
        for col in reader.fieldnames:
            data[col] = []
        for row in reader:
            for col in reader.fieldnames:
                data[col].append(float(row[col]))
    return {k: np.array(v) for k, v in data.items()}


# ============================================================================
# FIGURE 1: fig_comparison.pdf
# Love number k2 and h2 vs Subsurface Ocean Thickness D_ocean
# ============================================================================
def generate_fig_comparison():
    """Generate Love number k2 and h2 vs ocean thickness figure."""
    csv_path = os.path.join(SCRIPT_DIR, "titan_love_numbers.csv")
    df = read_csv_data(csv_path)

    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    # Left Panel: k2 vs Ocean Thickness for different crust thicknesses
    ax1.plot(
        df["d_ocean_km"],
        df["k2_crust50"],
        label=r"Crust $d_{\rm crust} = 50\ \rm km$",
        color="#1f77b4",
        lw=2.2,
    )
    ax1.plot(
        df["d_ocean_km"],
        df["k2_crust100"],
        label=r"Crust $d_{\rm crust} = 100\ \rm km$ (Nominal)",
        color="#d62728",
        lw=2.5,
    )
    ax1.plot(
        df["d_ocean_km"],
        df["k2_crust150"],
        label=r"Crust $d_{\rm crust} = 150\ \rm km$",
        color="#2ca02c",
        lw=2.2,
    )

    # Benchmark points from Tobie et al. (2005)
    tobie_doc = np.array([0, 25, 50, 100, 150, 200, 250, 300, 350, 400])
    tobie_k2 = np.array(
        [0.038, 0.380, 0.505, 0.552, 0.564, 0.568, 0.569, 0.570, 0.570, 0.570])
    ax1.scatter(
        tobie_doc,
        tobie_k2,
        color="black",
        marker="s",
        s=45,
        zorder=5,
        label="Tobie et al. (2005) Benchmark",
    )

    # Cassini Observation constraint band (Iess et al. 2012)
    ax1.axhspan(
        0.589 - 0.075,
        0.589 + 0.075,
        color="#ff7f0e",
        alpha=0.20,
        label=r"Cassini RSS: $k_2 = 0.589 \pm 0.075$",
    )
    ax1.axhline(0.589, color="#ff7f0e", linestyle="--", lw=1.5)

    # Solid interior reference
    ax1.annotate(
        "No Ocean (Solid Interior)\n$k_2 \\approx 0.038$",
        xy=(0, 0.04),
        xytext=(35, 0.12),
        arrowprops=dict(arrowstyle="->", color="black", lw=1.2),
        bbox=dict(boxstyle="round,pad=0.3", fc="#f0f0f0", ec="grey", lw=0.8),
    )

    # Decoupled ocean annotation
    ax1.annotate(
        "Decoupled Ocean Plateau\n$k_2 \\approx 0.55 - 0.62$",
        xy=(200, 0.57),
        xytext=(170, 0.38),
        arrowprops=dict(arrowstyle="->", color="black", lw=1.2),
        bbox=dict(boxstyle="round,pad=0.3", fc="#f0f0f0", ec="grey", lw=0.8),
    )

    ax1.set_xlabel(
        r"Subsurface Liquid Ocean Thickness $D_{\rm ocean}\ [\rm km]$")
    ax1.set_ylabel(r"Degree-2 Potential Love Number $k_2$")
    ax1.set_title(r"\textbf{(a)} Love Number $k_2$ vs. Ocean Thickness")
    ax1.set_xlim(-5, 405)
    ax1.set_ylim(-0.02, 0.72)
    ax1.grid(True, linestyle=":", alpha=0.6)
    ax1.legend(loc="lower right", framealpha=0.92)

    # Compute R^2 against benchmark points
    interp_k2 = np.interp(tobie_doc, df["d_ocean_km"], df["k2_crust100"])
    ss_res = np.sum((tobie_k2 - interp_k2)**2)
    ss_tot = np.sum((tobie_k2 - np.mean(tobie_k2))**2)
    r2 = 1.0 - ss_res / ss_tot
    ax1.text(
        0.04,
        0.93,
        f"$R^2 = {r2:.4f}$ vs Tobie (2005)",
        transform=ax1.transAxes,
        fontsize=11,
        fontweight="bold",
        bbox=dict(boxstyle="round,pad=0.3", fc="#e8f5e9", ec="#4caf50"),
    )

    # Right Panel: h2 and Radial Surface Tide Amplitude
    ax2.plot(
        df["d_ocean_km"],
        df["h2_crust100"],
        color="#9467bd",
        lw=2.5,
        label=r"Displacement Love Number $h_2$",
    )
    ax2.plot(
        df["d_ocean_km"],
        df["l2_crust100"],
        color="#8c564b",
        lw=2.2,
        linestyle="-.",
        label=r"Tangential Love Number $l_2$",
    )

    ax2_twin = ax2.twinx()
    ax2_twin.plot(
        df["d_ocean_km"],
        df["dr_amp_crust100_m"],
        color="#e377c2",
        lw=2.2,
        linestyle="--",
        label=r"Radial Tide Amplitude $\xi_r\ [\rm m]$",
    )

    ax2.set_xlabel(
        r"Subsurface Liquid Ocean Thickness $D_{\rm ocean}\ [\rm km]$")
    ax2.set_ylabel(r"Love Numbers $h_2, l_2$", color="#9467bd")
    ax2_twin.set_ylabel(
        r"Diurnal Radial Displacement Amplitude $\xi_r\ [\rm m]$",
        color="#e377c2")
    ax2.set_title(r"\textbf{(b)} Tidal Displacements & $h_2, l_2$ Response")
    ax2.set_xlim(-5, 405)
    ax2.set_ylim(-0.05, 1.55)
    ax2_twin.set_ylim(-0.5, 14.0)
    ax2.grid(True, linestyle=":", alpha=0.6)

    # Combined legend for right panel
    lines1, labels1 = ax2.get_legend_handles_labels()
    lines2, labels2 = ax2_twin.get_legend_handles_labels()
    ax2.legend(lines1 + lines2,
               labels1 + labels2,
               loc="lower right",
               framealpha=0.92)

    plt.tight_layout()
    out_pdf = os.path.join(SCRIPT_DIR, "fig_comparison.pdf")
    plt.savefig(out_pdf, bbox_inches="tight")
    plt.close()
    print(f"✅ Generated {out_pdf} (R^2 = {r2:.4f})")


# ============================================================================
# FIGURE 2: fig_model_choices.pdf
# Phase Lag delta and Dissipation Power P_tide vs Viscosity
# ============================================================================
def generate_fig_model_choices():
    """Generate tidal phase lag delta and heating power vs viscosity figure."""
    csv_path = os.path.join(SCRIPT_DIR, "titan_viscous_dissipation.csv")
    df = read_csv_data(csv_path)

    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    # Left Panel: Phase Lag delta (deg) vs log10(eta)
    ax1.plot(
        df["log10_eta"],
        df["phase_lag_deg"],
        color="#1f77b4",
        lw=2.5,
        label=r"Total Viscoelastic Lag $\delta$ (Andrade+Maxwell)",
    )

    # Maxwell pure viscoelastic reference
    omega = 4.5607e-6
    mu_ice = 3.3e9
    eta_vals = 10.0**df["log10_eta"]
    tau_m = eta_vals / mu_ice
    delta_maxwell_deg = np.arctan(
        (omega * tau_m) / (1.0 + (omega * tau_m)**2)) * (180.0 / np.pi)
    ax1.plot(
        df["log10_eta"],
        delta_maxwell_deg,
        color="#7f7f7f",
        lw=1.8,
        linestyle="--",
        label=r"Pure Maxwell Rheology",
    )

    # Resonance peak marker
    log_res = np.log10(mu_ice / omega)
    ax1.axvline(
        log_res,
        color="#d62728",
        linestyle=":",
        lw=1.8,
        label=f"Maxwell Resonance ($\\log_{{10}}\\eta = {log_res:.2f}$)",
    )
    ax1.axvspan(
        13.0,
        16.0,
        color="#2ca02c",
        alpha=0.15,
        label="Convective Ice Shell Range ($10^{13}-10^{16}\\ \\rm Pa\\ s$)",
    )

    ax1.set_xlabel(
        r"Basal Ice Viscosity $\log_{10}(\eta_{\rm ice}\ [\rm Pa\cdot s])$")
    ax1.set_ylabel(r"Tidal Phase Lag Angle $\delta\ [^\circ]$")
    ax1.set_title(r"\textbf{(a)} Tidal Phase Lag vs. Ice Rheological Viscosity")
    ax1.set_xlim(10.0, 18.0)
    ax1.set_ylim(-0.5, 9.0)
    ax1.grid(True, linestyle=":", alpha=0.6)
    ax1.legend(loc="upper right", framealpha=0.92)

    # Right Panel: Dissipation Factor k2/Q and Heating Power P_tide (GW)
    ax2.plot(
        df["log10_eta"],
        df["power_gw_ocean200"],
        color="#d62728",
        lw=2.5,
        label=
        r"Tidal Power $P_{\rm tide}\ [\rm GW]$ ($d_{\rm crust}=100\ \rm km$)",
    )
    ax2.axhline(
        350.0,
        color="#ff7f0e",
        linestyle="--",
        lw=1.8,
        label=r"Radiogenic Core Heat $Q_{\rm rad} \approx 350\ \rm GW$",
    )

    ax2_twin = ax2.twinx()
    ax2_twin.plot(
        df["log10_eta"],
        df["k2_over_Q_ocean200"],
        color="#2ca02c",
        lw=2.0,
        linestyle="-.",
        label=r"Global Dissipation Factor $k_2 / Q$",
    )

    ax2.set_xlabel(
        r"Basal Ice Viscosity $\log_{10}(\eta_{\rm ice}\ [\rm Pa\cdot s])$")
    ax2.set_ylabel(r"Viscoelastic Tidal Heating Power $P_{\rm tide}\ [\rm GW]$",
                   color="#d62728")
    ax2_twin.set_ylabel(r"Global Tidal Dissipation Factor $k_2 / Q$",
                        color="#2ca02c")
    ax2.set_title(
        r"\textbf{(b)} Interior Tidal Heat Production & Thermal Balance")
    ax2.set_xlim(10.0, 18.0)
    ax2.set_ylim(-5, 450)
    ax2_twin.set_ylim(-0.0002, 0.012)
    ax2.grid(True, linestyle=":", alpha=0.6)

    # Combined legend
    l1, lab1 = ax2.get_legend_handles_labels()
    l2, lab2 = ax2_twin.get_legend_handles_labels()
    ax2.legend(l1 + l2, lab1 + lab2, loc="upper right", framealpha=0.92)

    plt.tight_layout()
    out_pdf = os.path.join(SCRIPT_DIR, "fig_model_choices.pdf")
    plt.savefig(out_pdf, bbox_inches="tight")
    plt.close()
    print(f"✅ Generated {out_pdf}")


# ============================================================================
# FIGURE 3: fig_diagram.pdf
# Titan Interior Structure & Ocean-Crust Layer Schematic
# ============================================================================
def generate_fig_diagram():
    """Generate Titan interior ocean-crust schematic figure."""
    _fig, ax = plt.subplots(figsize=(10, 8))

    r_titan = 2575.0
    r_crust_base = 2475.0
    r_ductile_top = 2505.0
    r_ocean_base = 2275.0
    r_hp_base = 1975.0

    c_core = "#8d6e63"
    c_hp_ice = "#90caf9"
    c_ocean = "#1565c0"
    c_ductile = "#80deea"
    c_brittle = "#e1f5fe"
    c_atmos = "#ffe082"

    # Draw nested circles centered at (0, 0)
    ax.add_patch(
        patches.Circle(
            (0, 0),
            r_titan + 150,
            color=c_atmos,
            alpha=0.25,
            label="Atmosphere (N2, CH4, ~1.5 bar)",
        ))
    ax.add_patch(
        patches.Circle(
            (0, 0),
            r_titan,
            color=c_brittle,
            ec="#37474f",
            lw=1.5,
            label="Brittle Ice I Crust (0-70 km)",
        ))
    ax.add_patch(
        patches.Circle(
            (0, 0),
            r_ductile_top,
            color=c_ductile,
            ec="#00838f",
            lw=1.2,
            label="Ductile Basal Ice (70-100 km, Tidal Heat Zone)",
        ))
    ax.add_patch(
        patches.Circle(
            (0, 0),
            r_crust_base,
            color=c_ocean,
            ec="#0d47a1",
            lw=1.2,
            label="Subsurface Liquid Ocean (100-300 km, H2O+NH3)",
        ))
    ax.add_patch(
        patches.Circle(
            (0, 0),
            r_ocean_base,
            color=c_hp_ice,
            ec="#1976d2",
            lw=1.2,
            label="High-Pressure Ice VI/VII (300-600 km)",
        ))
    ax.add_patch(
        patches.Circle(
            (0, 0),
            r_hp_base,
            color=c_core,
            ec="#4e342e",
            lw=1.5,
            label="Silicate / Hydrated Rocky Core (R ~ 1975 km)",
        ))

    # Add tidal deformation vectors and annotations
    ax.annotate(
        "",
        xy=(r_titan + 120, 0),
        xytext=(r_titan - 50, 0),
        arrowprops=dict(arrowstyle="->", color="red", lw=2.5),
    )
    ax.annotate(
        "",
        xy=(-(r_titan + 120), 0),
        xytext=(-(r_titan - 50), 0),
        arrowprops=dict(arrowstyle="->", color="red", lw=2.5),
    )
    ax.text(
        r_titan + 140,
        20,
        "Tidal Bulge\n$\\Delta r \\approx \\pm 11.3\\,\\rm m$",
        color="red",
        fontweight="bold",
        fontsize=10,
        va="center",
    )

    # Saturn direction indicator
    ax.annotate(
        "To Saturn\n($a = 1.22 \\times 10^6\\,\\rm km$)",
        xy=(3200, 0),
        xytext=(3000, 400),
        arrowprops=dict(arrowstyle="->", color="#d84315", lw=2.0),
        bbox=dict(boxstyle="round,pad=0.3", fc="#ffebe6", ec="#d84315"),
        fontweight="bold",
        fontsize=10,
    )

    # Core details box
    core_text = (
        r"$\mathbf{Titan\ Interior\ Model\ (Tobie\ et\ al.\ 2005)}$"
        "\n"
        r"$\bullet\ \mathrm{Radius:}\ R = 2575\ \mathrm{km},\ M = 1.345 \times 10^{23}\ \mathrm{kg}$"
        "\n"
        r"$\bullet\ \mathrm{Gravity:}\ g = 1.352\ \mathrm{m/s^2},\ \bar{\rho} = 1880\ \mathrm{kg/m^3}$"
        "\n"
        r"$\bullet\ \mathrm{Orbital\ Period:}\ P_{\rm orb} = 15.945\ \mathrm{days},\ e = 0.0288$"
        "\n"
        r"$\bullet\ \mathrm{Love\ Number:}\ k_2 \approx 0.589\ (\mathrm{Ocean\ Decoupled})$"
        "\n"
        r"$\bullet\ \mathrm{Tidal\ Dissipation:}\ P_{\rm tide} \approx 50-250\ \mathrm{GW}$"
    )
    ax.text(
        0.03,
        0.97,
        core_text,
        transform=ax.transAxes,
        va="top",
        fontsize=9.5,
        bbox=dict(boxstyle="round,pad=0.5",
                  fc="#ffffff",
                  ec="#607d8b",
                  alpha=0.95),
    )

    # Radial dimension lines
    ax.plot([0, 0], [0, r_titan], color="black", linestyle="--", lw=1.0)
    ax.text(
        40,
        1000,
        "$R_{\\rm core} \\approx 1975\\,\\rm km$",
        rotation=90,
        va="center",
        fontsize=9,
        color="#4e342e",
    )
    ax.text(
        40,
        2125,
        "$d_{\\rm HP} \\approx 300\\,\\rm km$",
        rotation=90,
        va="center",
        fontsize=8.5,
        color="#1565c0",
    )
    ax.text(
        40,
        2375,
        "$D_{\\rm ocean} \\approx 200\\,\\rm km$",
        rotation=90,
        va="center",
        fontsize=8.5,
        color="#0d47a1",
    )
    ax.text(
        40,
        2525,
        "$d_{\\rm crust} \\approx 100\\,\\rm km$",
        rotation=90,
        va="center",
        fontsize=8.5,
        color="#00695c",
    )

    ax.set_xlim(-3100, 3600)
    ax.set_ylim(-3100, 3100)
    ax.set_aspect("equal")
    ax.axis("off")
    ax.legend(loc="lower left",
              bbox_to_anchor=(0.02, 0.02),
              framealpha=0.95,
              fontsize=9.5)
    ax.set_title(
        r"\textbf{Titan Interior Ocean-Crust Rheological Layering \& Tidal Dissipation Architecture}",
        fontsize=12,
        pad=12,
    )

    plt.tight_layout()
    out_pdf = os.path.join(SCRIPT_DIR, "fig_diagram.pdf")
    plt.savefig(out_pdf, bbox_inches="tight")
    plt.close()
    print(f"✅ Generated {out_pdf}")


if __name__ == "__main__":
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
