#!/usr/bin/env python3
"""Plot generation script for Paper #208 Replication: Nimmo & Spencer (2006) / Nimmo et al. (2007)

Powering the South Polar Plumes of Enceladus.

Generates:
  1. fig_comparison.pdf: Fault heat generation vs displacement amplitude
  2. fig_model_choices.pdf: Plume power output vs friction coefficient & pore pressure
  3. fig_diagram.pdf: Enceladus south polar tiger stripe fault schematic
"""

import csv
import os

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

OUTPUT_DIR = "replications_ss/paper_208"

# Set publication style formatting
plt.rcParams.update({
    "font.family": "serif",
    "font.size": 11,
    "axes.labelsize": 12,
    "axes.titlesize": 13,
    "xtick.labelsize": 10,
    "ytick.labelsize": 10,
    "legend.fontsize": 10,
    "figure.titlesize": 14,
    "lines.linewidth": 2.0,
    "axes.grid": True,
    "grid.alpha": 0.35,
    "grid.linestyle": "--",
})


# ==============================================================================
# 1. Figure 1: Fault Heat Generation vs Displacement Amplitude (fig_comparison.pdf)
# ==============================================================================
def plot_comparison():
    csv_path = os.path.join(OUTPUT_DIR, "enceladus_fault_shear_heating.csv")
    ds_list, p_3k, p_5k, p_7k, p_10k = [], [], [], [], []

    with open(csv_path, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            ds_list.append(float(row["displacement_m"]))
            p_3k.append(float(row["power_gw_d3km"]))
            p_5k.append(float(row["power_gw_d5km"]))
            p_7k.append(float(row["power_gw_d7km"]))
            p_10k.append(float(row["power_gw_d10km"]))

    ds = np.array(ds_list)
    p3 = np.array(p_3k)
    p5 = np.array(p_5k)
    p7 = np.array(p_7k)
    p10 = np.array(p_10k)

    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    # Left: Total Fault Heat Generation vs Slip Displacement
    ax1.plot(
        ds,
        p3,
        label=r"Fault Depth $d = 3\ \mathrm{km}$",
        color="#2b5c8f",
        linestyle=":",
    )
    ax1.plot(
        ds,
        p5,
        label=r"Fault Depth $d = 5\ \mathrm{km}$ (Nominal)",
        color="#1b7837",
        linewidth=2.5,
    )
    ax1.plot(
        ds,
        p7,
        label=r"Fault Depth $d = 7\ \mathrm{km}$",
        color="#e66101",
        linestyle="--",
    )
    ax1.plot(
        ds,
        p10,
        label=r"Fault Depth $d = 10\ \mathrm{km}$",
        color="#99000d",
        linestyle="-.",
    )

    # Cassini CIRS Observation Bands
    # Spencer et al. (2006): 5.8 +/- 1.4 GW
    ax1.axhspan(
        5.8 - 1.4,
        5.8 + 1.4,
        color="#7fbf7b",
        alpha=0.35,
        label=r"Spencer et al. (2006) CIRS ($5.8 \pm 1.4\ \mathrm{GW}$)",
    )
    ax1.axhline(5.8, color="#1b7837", linestyle="--", alpha=0.7, linewidth=1.2)

    # Howett et al. (2011): 15.8 +/- 3.1 GW
    ax1.axhspan(
        15.8 - 3.1,
        15.8 + 3.1,
        color="#fdc086",
        alpha=0.35,
        label=r"Howett et al. (2011) CIRS ($15.8 \pm 3.1\ \mathrm{GW}$)",
    )
    ax1.axhline(15.8, color="#b2182b", linestyle="--", alpha=0.7, linewidth=1.2)

    # Nominal point marker
    ax1.scatter([0.50], [5.486], color="darkred", s=70, zorder=5)
    ax1.annotate(
        r"Nominal: $d_s = 0.50\ \mathrm{m},\ d=5\ \mathrm{km}$" + "\n" +
        r"$P_{\mathrm{shear}} = 5.49\ \mathrm{GW}$ ($R^2 = 0.99999$)",
        xy=(0.50, 5.486),
        xytext=(0.55, 2.2),
        arrowprops=dict(facecolor="black", arrowstyle="->", lw=1.2),
        fontsize=9.5,
        bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.9),
    )

    ax1.set_xlabel(
        r"Cyclic Strike-Slip Displacement Amplitude $d_s\ \mathrm{[m]}$")
    ax1.set_ylabel(
        r"Total Fault Shear Heating Power $P_{\mathrm{shear}}\ \mathrm{[GW]}$")
    ax1.set_title(
        r"(a) Fault Heat Generation vs. Slip Amplitude",
        fontsize=12,
        weight="bold",
    )
    ax1.set_xlim(0, 1.5)
    ax1.set_ylim(0, 30)
    ax1.legend(loc="upper left", framealpha=0.92, fontsize=9.0)

    # Right: Heat Generation Rate per Unit Fault Length vs Depth
    depth_sweep = np.linspace(1.0, 15.0, 100)  # km
    rho = 917.0
    g = 0.1134
    mu = 0.50
    P_orb = 1.184785e5
    dq_dx_03m = (4.0 * 0.30 * 0.5 * mu * rho * g *
                 (depth_sweep * 1000.0)**2) / P_orb  # W/m
    dq_dx_05m = (4.0 * 0.50 * 0.5 * mu * rho * g *
                 (depth_sweep * 1000.0)**2) / P_orb
    dq_dx_10m = (4.0 * 1.00 * 0.5 * mu * rho * g *
                 (depth_sweep * 1000.0)**2) / P_orb

    ax2.plot(
        depth_sweep,
        dq_dx_03m / 1000.0,
        label=r"$d_s = 0.30\ \mathrm{m}$",
        color="#4575b4",
        linestyle=":",
    )
    ax2.plot(
        depth_sweep,
        dq_dx_05m / 1000.0,
        label=r"$d_s = 0.50\ \mathrm{m}$ (Nominal)",
        color="#1b7837",
        linewidth=2.5,
    )
    ax2.plot(
        depth_sweep,
        dq_dx_10m / 1000.0,
        label=r"$d_s = 1.00\ \mathrm{m}$",
        color="#d73027",
        linestyle="--",
    )

    # Add observed average per-meter power band (5.8 GW / 500 km = 11.6 kW/m, 15.8 GW / 500 km = 31.6 kW/m)
    ax2.axhspan(
        11.6 - 2.8,
        11.6 + 2.8,
        color="#7fbf7b",
        alpha=0.35,
        label=r"Spencer 2006: $11.6 \pm 2.8\ \mathrm{kW/m}$",
    )
    ax2.axhspan(
        31.6 - 6.2,
        31.6 + 6.2,
        color="#fdc086",
        alpha=0.35,
        label=r"Howett 2011: $31.6 \pm 6.2\ \mathrm{kW/m}$",
    )

    ax2.set_xlabel(r"Fault Penetration Depth $d\ \mathrm{[km]}$")
    ax2.set_ylabel(r"Linear Heat Dissipation Rate $dQ/dx\ \mathrm{[kW/m]}$")
    ax2.set_title(r"(b) Linear Heat Dissipation Rate vs. Depth",
                  fontsize=12,
                  weight="bold")
    ax2.set_xlim(1.0, 15.0)
    ax2.set_ylim(0, 60)
    ax2.legend(loc="upper left", framealpha=0.92, fontsize=9.0)

    plt.tight_layout()
    pdf_path = os.path.join(OUTPUT_DIR, "fig_comparison.pdf")
    plt.savefig(pdf_path, dpi=300)
    plt.close()
    print(f"✅ Created {pdf_path}")


# ==============================================================================
# 2. Figure 2: Plume Power Output vs Friction Coefficient (fig_model_choices.pdf)
# ==============================================================================
def plot_model_choices():
    csv_path = os.path.join(OUTPUT_DIR, "enceladus_plume_friction_sweep.csv")
    mu_list, p00, p20, p40, p60, ptot = [], [], [], [], [], []

    with open(csv_path, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            mu_list.append(float(row["friction_coeff"]))
            p00.append(float(row["power_gw_pore00"]))
            p20.append(float(row["power_gw_pore20"]))
            p40.append(float(row["power_gw_pore40"]))
            p60.append(float(row["power_gw_pore60"]))
            ptot.append(float(row["total_power_gw_pore00"]))

    mu = np.array(mu_list)
    p00 = np.array(p00)
    p20 = np.array(p20)
    p40 = np.array(p40)
    p60 = np.array(p60)
    ptot = np.array(ptot)

    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    # Left: Shear Power vs Friction Coefficient for various pore fluid pressures
    ax1.plot(
        mu,
        p00,
        label=r"Hydrostatic Lithostatic ($\lambda = 0.0$)",
        color="#1b7837",
        linewidth=2.2,
    )
    ax1.plot(
        mu,
        p20,
        label=r"Pore Fluid Ratio $\lambda = 0.2$",
        color="#3182bd",
        linestyle="--",
    )
    ax1.plot(
        mu,
        p40,
        label=r"Pore Fluid Ratio $\lambda = 0.4$",
        color="#fe9929",
        linestyle="-.",
    )
    ax1.plot(
        mu,
        p60,
        label=r"Pore Fluid Ratio $\lambda = 0.6$",
        color="#de2d26",
        linestyle=":",
    )

    # CIRS observations
    ax1.axhspan(
        5.8 - 1.4,
        5.8 + 1.4,
        color="#7fbf7b",
        alpha=0.35,
        label=r"Spencer et al. (2006) CIRS",
    )
    ax1.axhspan(
        15.8 - 3.1,
        15.8 + 3.1,
        color="#fdc086",
        alpha=0.35,
        label=r"Howett et al. (2011) CIRS",
    )

    ax1.set_xlabel(r"Ice Friction Coefficient $\mu$ (Coulomb)")
    ax1.set_ylabel(
        r"Shear Dissipation Power $P_{\mathrm{shear}}\ \mathrm{[GW]}$")
    ax1.set_title(
        r"(a) Shear Power vs. Friction & Pore Pressure",
        fontsize=12,
        weight="bold",
    )
    ax1.set_xlim(0.05, 0.85)
    ax1.set_ylim(0, 18)
    ax1.legend(loc="upper left", framealpha=0.92, fontsize=8.8)

    # Right: Total Energy Budget Breakdown (Shear + Plume Latent + Kinetic)
    p_latent = 0.566  # GW (200 kg/s * 2.83 MJ/kg)
    p_kinetic = 0.016  # GW (0.5 * 200 * 400^2)

    ax2.plot(
        mu,
        ptot,
        label=r"Total Endogenic Power $P_{\mathrm{total}}$",
        color="#111111",
        linewidth=2.5,
    )
    ax2.plot(
        mu,
        p00,
        label=r"Frictional Shear Heat $P_{\mathrm{shear}}$",
        color="#1b7837",
        linewidth=1.8,
        linestyle="-",
    )
    ax2.axhline(
        p_latent,
        color="#2b83ba",
        linestyle="--",
        linewidth=1.8,
        label=r"Plume Latent Heat $P_{\mathrm{latent}} = 0.57\ \mathrm{GW}$",
    )
    ax2.axhline(
        p_kinetic,
        color="#d7191c",
        linestyle=":",
        linewidth=1.8,
        label=r"Plume Kinetic Power $P_{\mathrm{kin}} = 0.016\ \mathrm{GW}$",
    )

    # Observed benchmark
    ax2.axhspan(5.8 - 1.4, 5.8 + 1.4, color="#7fbf7b", alpha=0.30)
    ax2.annotate(
        "Spencer et al. (2006)\nObservation Target",
        xy=(0.35, 5.8),
        xytext=(0.48, 8.5),
        arrowprops=dict(facecolor="black", arrowstyle="->", lw=1.0),
        fontsize=9.0,
        bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="green", alpha=0.9),
    )

    ax2.set_xlabel(r"Ice Friction Coefficient $\mu$ (Coulomb)")
    ax2.set_ylabel(r"Power Output Component $\mathrm{[GW]}$")
    ax2.set_title(r"(b) South Polar Power Budget Breakdown",
                  fontsize=12,
                  weight="bold")
    ax2.set_xlim(0.05, 0.85)
    ax2.set_ylim(0, 12)
    ax2.legend(loc="upper left", framealpha=0.92, fontsize=8.8)

    plt.tight_layout()
    pdf_path = os.path.join(OUTPUT_DIR, "fig_model_choices.pdf")
    plt.savefig(pdf_path, dpi=300)
    plt.close()
    print(f"✅ Created {pdf_path}")


# ==============================================================================
# 3. Figure 3: Enceladus South Polar Tiger Stripe Fault Schematic (fig_diagram.pdf)
# ==============================================================================
def plot_diagram():
    fig = plt.figure(figsize=(13, 6.0))

    # Panel 1: Map View of South Polar Terrain (Tiger Stripes)
    ax1 = fig.add_subplot(1, 2, 1)

    # South Polar Circle
    theta = np.linspace(0, 2 * np.pi, 200)
    r_spt = 1.0
    ax1.plot(
        r_spt * np.cos(theta),
        r_spt * np.sin(theta),
        color="#555555",
        linestyle="--",
        linewidth=1.5,
    )
    ax1.fill(r_spt * np.cos(theta),
             r_spt * np.sin(theta),
             color="#e0f3f8",
             alpha=0.4)

    # 4 Tiger Stripes: Alexandria, Cairo, Baghdad, Damascus
    # Baghdad Sulcus (Central)
    x_bag = np.linspace(-0.65, 0.65, 50)
    y_bag = 0.05 * np.sin(3 * x_bag)
    ax1.plot(x_bag,
             y_bag,
             color="#b2182b",
             linewidth=3.5,
             label="Baghdad Sulcus")

    # Cairo Sulcus
    x_cai = np.linspace(-0.55, 0.60, 50)
    y_cai = y_bag + 0.28 + 0.03 * np.cos(3 * x_cai)
    ax1.plot(x_cai, y_cai, color="#d6604d", linewidth=3.0, label="Cairo Sulcus")

    # Alexandria Sulcus
    x_ale = np.linspace(-0.45, 0.50, 50)
    y_ale = y_bag + 0.54 + 0.02 * np.sin(2 * x_ale)
    ax1.plot(x_ale,
             y_ale,
             color="#f4a582",
             linewidth=2.8,
             label="Alexandria Sulcus")

    # Damascus Sulcus
    x_dam = np.linspace(-0.55, 0.55, 50)
    y_dam = y_bag - 0.32 - 0.03 * np.sin(3 * x_dam)
    ax1.plot(x_dam,
             y_dam,
             color="#92c5de",
             linewidth=3.0,
             label="Damascus Sulcus")

    # Active Vent hot spots along tiger stripes
    vent_x = [-0.4, -0.1, 0.2, 0.45, -0.3, 0.1, 0.35, -0.2, 0.0, 0.3]
    vent_y = [0.03, 0.01, -0.02, 0.04, 0.30, 0.27, 0.32, -0.30, -0.34, -0.31]
    ax1.scatter(
        vent_x,
        vent_y,
        color="#ff7f00",
        s=60,
        edgecolors="black",
        zorder=5,
        label="Active Hydrothermal Vents",
    )

    # Shear stress arrows indicating cyclic strike-slip motion
    ax1.annotate(
        "",
        xy=(-0.2, 0.10),
        xytext=(0.2, 0.10),
        arrowprops=dict(arrowstyle="<->", color="navy", lw=2.0),
    )
    ax1.text(
        0.0,
        0.13,
        r"Tidal Strike-Slip Shear $\tau_{\mathrm{fric}}$",
        ha="center",
        fontsize=9.5,
        color="navy",
        weight="bold",
    )

    # Pole Marker
    ax1.scatter([0], [0],
                color="black",
                marker="+",
                s=100,
                linewidth=2,
                zorder=6)
    ax1.text(0.02, -0.08, "South Pole (-90°)", fontsize=9.0, weight="bold")

    ax1.set_xlim(-1.15, 1.15)
    ax1.set_ylim(-1.15, 1.15)
    ax1.set_aspect("equal")
    ax1.set_title(
        r"(a) South Polar Terrain (SPT) Tiger Stripes",
        fontsize=12,
        weight="bold",
    )
    ax1.legend(loc="lower right", framealpha=0.92, fontsize=8.2)
    ax1.axis("off")

    # Panel 2: Cross-Section Schematic of Fault & Plume Eruption
    ax2 = fig.add_subplot(1, 2, 2)

    # Coordinates: x in km, y in km
    ax2.fill_between(
        [-5, 5],
        0,
        -10,
        color="#d1e5f0",
        alpha=0.8,
        label="Brittle Ice Shell (0 - 10 km)",
    )
    ax2.fill_between(
        [-5, 5],
        -10,
        -25,
        color="#92c5de",
        alpha=0.8,
        label="Ductile Convective Ice (10 - 25 km)",
    )
    ax2.fill_between(
        [-5, 5],
        -25,
        -35,
        color="#2166ac",
        alpha=0.9,
        label="Global Subsurface Ocean",
    )

    # Fault Fracture (Tiger Stripe)
    fault_x = [-0.15, -0.08, -0.02, 0.0]
    fault_y = [0.0, -4.0, -10.0, -25.0]
    ax2.plot(fault_x, fault_y, color="#b2182b", linewidth=4.0)
    ax2.plot([-x for x in fault_x], fault_y, color="#b2182b", linewidth=4.0)
    ax2.fill_betweenx(fault_y,
                      fault_x, [-x for x in fault_x],
                      color="#fddbc7",
                      alpha=0.9)

    # Strike-Slip motion arrows on fault walls
    ax2.annotate(
        "",
        xy=(-0.8, -4.0),
        xytext=(-0.8, -2.0),
        arrowprops=dict(arrowstyle="->", color="darkred", lw=2.5),
    )
    ax2.annotate(
        "",
        xy=(0.8, -2.0),
        xytext=(0.8, -4.0),
        arrowprops=dict(arrowstyle="->", color="darkred", lw=2.5),
    )
    ax2.text(-1.1,
             -3.0,
             r"$v_{\mathrm{slip}}$",
             color="darkred",
             weight="bold",
             fontsize=11)
    ax2.text(1.1,
             -3.0,
             r"$v_{\mathrm{slip}}$",
             color="darkred",
             weight="bold",
             fontsize=11)

    # Erupting Plume into Space
    plume_x = np.array([-2.5, -1.2, -0.3, 0.0, 0.3, 1.2, 2.5])
    plume_y = np.array([10.0, 6.0, 1.5, 0.0, 1.5, 6.0, 10.0])
    ax2.fill_between(
        plume_x,
        0,
        plume_y,
        color="#ffffbf",
        alpha=0.7,
        label=
        r"Plume Vapor + Ice Jets ($v_{\mathrm{jet}} \approx 400\ \mathrm{m/s}$)",
    )
    ax2.plot(plume_x, plume_y, color="#fee08b", linestyle="--", linewidth=1.5)

    # Annotations
    ax2.text(
        0.0,
        4.5,
        "Supersonic Plume\n" + r"$\dot{M} \approx 200\ \mathrm{kg/s}$",
        ha="center",
        fontsize=9.5,
        bbox=dict(boxstyle="round,pad=0.2", fc="white", ec="orange",
                  alpha=0.85),
    )

    ax2.text(
        2.2,
        -5.0,
        "Shear Heating Zone\n" + r"$\tau(z) = \mu \rho g z$" + "\n" +
        r"$dQ/dx \approx 11\ \mathrm{kW/m}$",
        fontsize=8.5,
        bbox=dict(boxstyle="round,pad=0.2",
                  fc="white",
                  ec="darkred",
                  alpha=0.85),
    )

    ax2.text(
        0.0,
        -29.0,
        "Subsurface Ocean Decoupling\n" +
        r"($h_2 \geq 0.01 \rightarrow d_s \approx 0.5\ \mathrm{m}$)",
        ha="center",
        color="white",
        weight="bold",
        fontsize=9.5,
    )

    ax2.axhline(0, color="black", linewidth=1.5)
    ax2.set_xlim(-4.5, 4.5)
    ax2.set_ylim(-35, 11)
    ax2.set_ylabel(r"Depth / Altitude $z\ \mathrm{[km]}$")
    ax2.set_xlabel(r"Horizontal Distance from Fault Axis $\mathrm{[km]}$")
    ax2.set_title(
        r"(b) Tiger Stripe Cross-Section & Plume Venting",
        fontsize=12,
        weight="bold",
    )
    ax2.legend(loc="lower left", framealpha=0.92, fontsize=8.0)

    plt.tight_layout()
    pdf_path = os.path.join(OUTPUT_DIR, "fig_diagram.pdf")
    plt.savefig(pdf_path, dpi=300)
    plt.close()
    print(f"✅ Created {pdf_path}")


if __name__ == "__main__":
    plot_comparison()
    plot_model_choices()
    plot_diagram()
    print("🎯 All Paper #208 replication plots generated successfully!")
