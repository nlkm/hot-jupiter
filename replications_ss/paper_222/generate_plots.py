#!/usr/bin/env python3
"""generate_plots.py - Publication Figures for Paper #222 Replication.

Mitri & Showman (2005) | Icarus 177 (2), 447-460
"Thermal Evolution and State of Europa's Ice Shell:
 Convective-Conductive Transitions, Stagnant-Lid Regimes, and Sensitivity to Basal Heat Flux"

Generates:
1. fig_comparison.pdf - Convective scaling Nu(Ra_b), Equilibrium Thickness D_eq(F_b) hysteresis,
                        and Temperature/Viscosity Depth Profiles.
2. fig_model_choices.pdf - Dynamic thermal evolution D(t), lid/sublayer response to step perturbations,
                           heat flux balance, and convective plume velocities.
3. fig_diagram.pdf - Comprehensive schematic of Europa's icy shell interior structure,
                     thermal regimes, and convective-conductive switching mechanism.
"""

import os

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Rectangle, Wedge

# Publication style
plt.rcParams.update({
    "font.family": "serif",
    "font.size": 10,
    "axes.labelsize": 11,
    "axes.titlesize": 12,
    "xtick.labelsize": 9,
    "ytick.labelsize": 9,
    "legend.fontsize": 8.5,
    "figure.titlesize": 13,
    "text.usetex": False,
    "mathtext.fontset": "cm",
    "lines.linewidth": 1.8,
    "axes.grid": True,
    "grid.alpha": 0.30,
    "grid.linestyle": "--",
})

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


def load_datasets():
    csv_nu = os.path.join(SCRIPT_DIR, "nu_vs_ra_regimes.csv")
    csv_eq = os.path.join(SCRIPT_DIR, "equilibrium_branches_vs_flux.csv")
    csv_evol = os.path.join(SCRIPT_DIR, "thermal_evolution_perturbation.csv")
    csv_prof = os.path.join(SCRIPT_DIR, "temperature_viscosity_profiles.csv")

    data_nu = np.genfromtxt(csv_nu, delimiter=",", names=True)
    data_eq = np.genfromtxt(csv_eq, delimiter=",", names=True)
    data_evol = np.genfromtxt(csv_evol, delimiter=",", names=True)
    data_prof = np.genfromtxt(csv_prof, delimiter=",", names=True)

    return data_nu, data_eq, data_evol, data_prof


def plot_fig_comparison(data_nu, data_eq, data_prof):
    _fig, axs = plt.subplots(2, 2, figsize=(13, 10.5))

    # (a) Nu vs Ra_b for different convective regimes
    ax = axs[0, 0]
    ax.plot(data_nu["log10_Ra_b"],
            data_nu["Nu_stagnant"],
            "b-",
            lw=2.4,
            label=r"Stagnant Lid ($\theta = 14.02, \beta = 0.22$)")
    ax.plot(data_nu["log10_Ra_b"],
            data_nu["Nu_mobile"],
            "r--",
            lw=2.0,
            label=r"Mobile Lid ($Nu \propto Ra_b^{0.28}$)")
    ax.plot(data_nu["log10_Ra_b"],
            data_nu["Nu_isoviscous"],
            "g-.",
            lw=1.8,
            label=r"Isoviscous Bénard ($Nu \propto Ra_b^{1/3}$)")
    ax.axhline(1.0,
               color="gray",
               ls=":",
               lw=1.5,
               label="Pure Conduction ($Nu = 1$)")

    # Critical onset
    log_ra_cr = np.log10(7.735e5)
    ax.axvline(log_ra_cr,
               color="purple",
               ls="--",
               alpha=0.7,
               label=r"Critical $Ra_{\rm cr} = 7.74 \times 10^5$")
    ax.scatter([log_ra_cr], [1.0], color="purple", s=60, zorder=5)
    ax.annotate(r"Convection Onset",
                xy=(log_ra_cr, 1.0),
                xytext=(log_ra_cr - 1.8, 4.0),
                arrowprops=dict(arrowstyle="->", color="purple", lw=1.5),
                fontsize=9,
                fontweight="bold",
                color="purple")

    ax.set_xlabel(r"$\log_{10}(\mathrm{Basal\ Rayleigh\ Number}\ Ra_b)$")
    ax.set_ylabel(r"Nusselt Number $Nu = F / F_{\rm cond}$")
    ax.set_title(
        r"\textbf{(a) Heat Transfer Scaling: Stagnant Lid vs. Mobile Lid}",
        fontsize=11)
    ax.set_xlim(3.0, 9.0)
    ax.set_ylim(0.5, 30.0)
    ax.set_yscale("log")
    ax.legend(loc="upper left", framealpha=0.92)

    # (b) Equilibrium Ice Shell Thickness Branches & Hysteresis
    ax = axs[0, 1]
    F_b = data_eq["F_basal_mW_m2"]
    D_cond = data_eq["D_conductive_km"]
    D_conv_13 = data_eq["D_convective_eta13_km"]
    D_conv_14 = data_eq["D_convective_eta14_km"]
    D_conv_15 = data_eq["D_convective_eta15_km"]
    D_cr_14 = data_eq["D_crit_eta14_km"][0]

    # Conductive branch
    ax.plot(F_b,
            D_cond,
            "k-",
            lw=2.2,
            label=r"Conductive Branch ($D \propto 1/F_b$)")
    # Convective branches
    ax.plot(F_b,
            D_conv_13,
            "g--",
            lw=1.8,
            label=r"Convective ($\eta_b = 10^{13}\ \mathrm{Pa\cdot s}$)")
    ax.plot(
        F_b,
        D_conv_14,
        "b-",
        lw=2.4,
        label=r"Convective ($\eta_b = 10^{14}\ \mathrm{Pa\cdot s}$, Nominal)")
    ax.plot(F_b,
            D_conv_15,
            "m-.",
            lw=1.8,
            label=r"Convective ($\eta_b = 10^{15}\ \mathrm{Pa\cdot s}$)")

    # Critical threshold
    ax.axhline(D_cr_14,
               color="darkred",
               ls=":",
               lw=1.6,
               label=rf"Critical $D_{{\rm cr}} = {D_cr_14:.1f}\ \mathrm{{km}}$")

    # Shade Hysteresis / Bistability window for eta_14 (approx 15 to 45 mW/m2)
    ax.axvspan(15.0,
               42.0,
               color="orange",
               alpha=0.15,
               label=r"Bistability / Hysteresis Zone")
    ax.text(28.0,
            65.0,
            r"\textbf{Bistable Regime}" + "\n" + r"(Conductive or Convective)",
            ha="center",
            va="center",
            fontsize=9,
            color="darkorange",
            bbox=dict(boxstyle="round,pad=0.3",
                      fc="white",
                      ec="darkorange",
                      alpha=0.9))

    ax.set_xlabel(r"Basal Heat Flux $F_{\rm basal}\ [\mathrm{mW/m^2}]$")
    ax.set_ylabel(r"Equilibrium Ice Thickness $D_{\rm eq}\ [\mathrm{km}]$")
    ax.set_title(r"\textbf{(b) Equilibrium Thickness Branches \& Hysteresis}",
                 fontsize=11)
    ax.set_xlim(5.0, 80.0)
    ax.set_ylim(0.0, 85.0)
    ax.legend(loc="upper right", framealpha=0.92, fontsize=8)

    # (c) Temperature Profile Depth Comparison
    ax = axs[1, 0]
    z_km = data_prof["z_km"]
    T_cond = data_prof["T_cond_K"]
    T_conv = data_prof["T_conv_K"]

    ax.plot(T_cond,
            z_km,
            "k--",
            lw=2.0,
            label="Pure Conduction (Linear Geotherm)")
    ax.plot(T_conv,
            z_km,
            "b-",
            lw=2.4,
            label=r"Stagnant Lid Convection ($D = 25\ \mathrm{km}$)")

    # Shade stagnant lid vs convective sublayer
    d_lid_nom = 12.8
    ax.axhspan(
        0.0,
        d_lid_nom,
        color="lightblue",
        alpha=0.25,
        label=
        rf"Stagnant Lid ($\delta_{{\rm lid}} \approx {d_lid_nom:.1f}\ \mathrm{{km}}$)"
    )
    ax.axhspan(
        d_lid_nom,
        25.0,
        color="lightcoral",
        alpha=0.25,
        label=
        r"Convective Sublayer ($\delta_{\rm conv} \approx 12.2\ \mathrm{km}$)")

    ax.axvline(257.88, color="red", ls=":", alpha=0.7)
    ax.text(255.0,
            18.0,
            r"$T_{\rm conv} \approx 257.9\ \mathrm{K}$",
            color="darkred",
            rotation=90,
            va="center",
            fontsize=8.5)

    ax.set_xlabel(r"Temperature $T\ [\mathrm{K}]$")
    ax.set_ylabel(r"Depth Below Surface $z\ [\mathrm{km}]$")
    ax.set_title(r"\textbf{(c) Ice Shell Thermal Geotherms}", fontsize=11)
    ax.set_xlim(90.0, 280.0)
    ax.set_ylim(25.0, 0.0)  # Inverted depth
    ax.legend(loc="lower left", framealpha=0.92)

    # (d) Viscosity & Volumetric Tidal Dissipation Depth Profiles
    ax = axs[1, 1]
    visc_cond = data_prof["visc_cond_Pa_s"]
    visc_conv = data_prof["visc_conv_Pa_s"]
    q_tide_conv = data_prof["q_tide_conv_W_m3"] * 1.0e6  # uW/m3

    color_visc = "darkblue"
    ax.plot(visc_cond,
            z_km,
            color="gray",
            ls="--",
            lw=1.8,
            label=r"$\eta(z)$ Pure Conduction")
    ax.plot(visc_conv,
            z_km,
            color=color_visc,
            lw=2.4,
            label=r"$\eta(z)$ Stagnant Lid Convection")
    ax.set_xlabel(r"Ice Dynamic Viscosity $\eta\ [\mathrm{Pa\cdot s}]$",
                  color=color_visc)
    ax.set_xscale("log")
    ax.set_xlim(1.0e13, 1.0e26)
    ax.set_ylabel(r"Depth Below Surface $z\ [\mathrm{km}]$")
    ax.set_ylim(25.0, 0.0)  # Inverted depth
    ax.tick_params(axis="x", labelcolor=color_visc)

    # Twin axis for tidal volumetric heating
    ax2 = ax.twiny()
    color_tide = "crimson"
    ax2.plot(q_tide_conv,
             z_km,
             color=color_tide,
             lw=2.2,
             ls="-",
             label=r"Tidal Dissipation $\dot{e}_{\rm tide}(z)$")
    ax2.set_xlabel(
        r"Volumetric Tidal Heating $\dot{e}_{\rm tide}\ [\mu\mathrm{W/m^3}]$",
        color=color_tide)
    ax2.tick_params(axis="x", labelcolor=color_tide)
    ax2.set_xlim(0.0, 2.5)

    ax.set_title(r"\textbf{(d) Rheology \& Volumetric Tidal Heating}",
                 fontsize=11,
                 pad=20)

    # Combined legend
    lines1, labels1 = ax.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax.legend(lines1 + lines2,
              labels1 + labels2,
              loc="lower left",
              framealpha=0.92,
              fontsize=8)

    plt.tight_layout()
    out_pdf = os.path.join(SCRIPT_DIR, "fig_comparison.pdf")
    plt.savefig(out_pdf, dpi=300)
    plt.close()
    print(f"✅ Generated {out_pdf}")


def plot_fig_model_choices(data_evol):
    _fig, axs = plt.subplots(2, 2, figsize=(13, 9.5))

    time_kyr = data_evol["time_kyr"]
    D_km = data_evol["D_km"]
    d_lid_km = data_evol["delta_lid_km"]
    d_conv_km = data_evol["delta_conv_km"]
    F_surf = data_evol["F_surf_mW_m2"]
    F_basal = data_evol["F_basal_mW_m2"]
    F_tide = data_evol["F_tide_mW_m2"]
    Nu = data_evol["Nu"]
    u_conv = data_evol["u_conv_m_yr"]

    # (a) Time Evolution of Ice Shell Thickness D(t)
    ax = axs[0, 0]
    ax.plot(time_kyr, D_km, "b-", lw=2.4, label=r"Total Shell Thickness $D(t)$")
    ax.axhline(
        14.32,
        color="darkred",
        ls=":",
        lw=1.6,
        label=r"Critical Convection Limit $D_{\rm cr} \approx 14.3\ \mathrm{km}$"
    )

    # Highlight perturbation window
    ax.axvspan(500.0,
               1500.0,
               color="salmon",
               alpha=0.20,
               label=r"Basal Flux Pulse ($F_b: 30 \to 50\ \mathrm{mW/m^2}$)")

    ax.annotate("Rapid Convective\nThinning (~100 kyr)",
                xy=(650.0, 22.0),
                xytext=(750.0, 25.0),
                arrowprops=dict(arrowstyle="->", color="darkred", lw=1.5),
                fontsize=8.5,
                fontweight="bold",
                color="darkred")

    ax.annotate("Sluggish Freezing\nRe-thickening (~1 Myr)",
                xy=(1800.0, 22.0),
                xytext=(1900.0, 26.0),
                arrowprops=dict(arrowstyle="->", color="darkblue", lw=1.5),
                fontsize=8.5,
                fontweight="bold",
                color="darkblue")

    ax.set_xlabel(r"Time $t\ [\mathrm{kyr}]$")
    ax.set_ylabel(r"Ice Shell Thickness $D\ [\mathrm{km}]$")
    ax.set_title(r"\textbf{(a) Transient Response to Basal Heat Flux Step}",
                 fontsize=11)
    ax.set_xlim(0.0, 3000.0)
    ax.set_ylim(10.0, 32.0)
    ax.legend(loc="upper right", framealpha=0.92)

    # (b) Stagnant Lid vs Convective Sublayer Thickness
    ax = axs[0, 1]
    ax.plot(time_kyr,
            d_lid_km,
            "navy",
            lw=2.2,
            label=r"Stagnant Lid $\delta_{\rm lid}(t)$")
    ax.plot(time_kyr,
            d_conv_km,
            "firebrick",
            lw=2.2,
            label=r"Convective Sublayer $\delta_{\rm conv}(t)$")
    ax.plot(time_kyr,
            D_km,
            "gray",
            ls="--",
            lw=1.5,
            label=r"Total $D(t) = \delta_{\rm lid} + \delta_{\rm conv}$")
    ax.axvspan(500.0, 1500.0, color="salmon", alpha=0.20)

    ax.set_xlabel(r"Time $t\ [\mathrm{kyr}]$")
    ax.set_ylabel(r"Layer Thickness $[\mathrm{km}]$")
    ax.set_title(r"\textbf{(b) Layer Partitioning: Lid vs. Sublayer}",
                 fontsize=11)
    ax.set_xlim(0.0, 3000.0)
    ax.set_ylim(0.0, 32.0)
    ax.legend(loc="center right", framealpha=0.92)

    # (c) Surface Heat Flux vs Internal Heating
    ax = axs[1, 0]
    ax.plot(time_kyr,
            F_surf,
            "darkgreen",
            lw=2.4,
            label=r"Surface Heat Flux $F_{\rm surf}(t)$")
    ax.plot(time_kyr,
            F_basal,
            "crimson",
            ls="--",
            lw=2.0,
            label=r"Basal Input $F_{\rm basal}(t)$")
    ax.plot(time_kyr,
            F_tide,
            "orange",
            ls="-.",
            lw=1.8,
            label=r"Tidal Dissipation $F_{\rm tide}(t)$")
    ax.plot(time_kyr,
            F_basal + F_tide,
            "black",
            ls=":",
            lw=2.0,
            label=r"Total Input $F_{\rm in} = F_b + F_{\rm tide}$")
    ax.axvspan(500.0, 1500.0, color="salmon", alpha=0.20)

    ax.set_xlabel(r"Time $t\ [\mathrm{kyr}]$")
    ax.set_ylabel(r"Heat Flux $[\mathrm{mW/m^2}]$")
    ax.set_title(r"\textbf{(c) Dynamic Energy Balance at Ice-Ocean Boundary}",
                 fontsize=11)
    ax.set_xlim(0.0, 3000.0)
    ax.set_ylim(0.0, 70.0)
    ax.legend(loc="upper right", framealpha=0.92, fontsize=8)

    # (d) Nusselt Number & Convective Plume Velocity
    ax = axs[1, 1]
    color_nu = "purple"
    ax.plot(time_kyr,
            Nu,
            color=color_nu,
            lw=2.4,
            label=r"Nusselt Number $Nu(t)$")
    ax.set_xlabel(r"Time $t\ [\mathrm{kyr}]$")
    ax.set_ylabel(r"Nusselt Number $Nu$", color=color_nu)
    ax.tick_params(axis="y", labelcolor=color_nu)
    ax.set_xlim(0.0, 3000.0)
    ax.set_ylim(1.0, 4.0)

    ax2 = ax.twiny()
    color_u = "teal"
    ax2.plot(time_kyr,
             u_conv * 100.0,
             color=color_u,
             lw=2.0,
             ls="--",
             label=r"Plume Velocity $u_{\rm conv}(t)\ [\mathrm{cm/yr}]$")
    ax2.set_xlabel(
        r"Convective Plume Velocity $u_{\rm conv}\ [\mathrm{cm/yr}]$",
        color=color_u)
    ax2.tick_params(axis="x", labelcolor=color_u)
    ax2.set_xlim(0.0, 3000.0)

    ax.set_title(r"\textbf{(d) Convective Vigour & Upwelling Plume Dynamics}",
                 fontsize=11,
                 pad=20)
    ax.axvspan(500.0, 1500.0, color="salmon", alpha=0.20)

    lines1, labels1 = ax.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax.legend(lines1 + lines2,
              labels1 + labels2,
              loc="upper right",
              framealpha=0.92)

    plt.tight_layout()
    out_pdf = os.path.join(SCRIPT_DIR, "fig_model_choices.pdf")
    plt.savefig(out_pdf, dpi=300)
    plt.close()
    print(f"✅ Generated {out_pdf}")


def plot_fig_diagram():
    _fig, ax = plt.subplots(figsize=(12, 8.5))

    ax.set_xlim(0.0, 100.0)
    ax.set_ylim(0.0, 100.0)
    ax.axis("off")

    # Outer Frame
    frame = Rectangle((2.0, 2.0),
                      96.0,
                      96.0,
                      fc="#FAFBFC",
                      ec="#2C3E50",
                      lw=2.0,
                      zorder=0)
    ax.add_patch(frame)

    # Title Header
    ax.text(
        50.0,
        95.0,
        r"\textbf{Europa Ice Shell Thermal Convection \& Bistability Architecture}",
        fontsize=13,
        fontweight="bold",
        ha="center",
        va="center",
        color="#1A252F")
    ax.text(
        50.0,
        92.0,
        r"Mitri \& Showman (2005) \textit{Icarus} 177, 447--460 | First-Principles Geophysical Cross-Section",
        fontsize=9.5,
        style="italic",
        ha="center",
        va="center",
        color="#566573")

    # Left Section: Cross-Section schematic of the Ice Shell
    left_x = 5.0
    left_w = 42.0

    # 1. Vacuum / Surface Space (z = 90 to 82)
    ax.add_patch(
        Rectangle((left_x, 80.0),
                  left_w,
                  8.0,
                  fc="#1C2833",
                  ec="none",
                  zorder=1))
    ax.text(left_x + 21.0,
            84.0,
            r"Space Vacuum / Europa Surface ($T_s = 100\ \mathrm{K}$)",
            color="white",
            fontsize=8.5,
            fontweight="bold",
            ha="center",
            va="center",
            zorder=2)

    # Radiation arrows
    for dx in [8.0, 18.0, 28.0, 38.0]:
        ax.annotate("",
                    xy=(left_x + dx, 87.0),
                    xytext=(left_x + dx, 81.0),
                    arrowprops=dict(arrowstyle="->", color="#F39C12", lw=1.8),
                    zorder=3)
    ax.text(left_x + 40.0,
            87.0,
            r"$F_{\rm rad}$",
            color="#F39C12",
            fontsize=8,
            fontweight="bold")

    # 2. Brittle Elastic Lid & Stagnant Conductive Lid (z = 80 to 52)
    # Upper elastic part (z = 80 to 74)
    ax.add_patch(
        Rectangle((left_x, 72.0),
                  left_w,
                  8.0,
                  fc="#85C1E9",
                  ec="white",
                  lw=1.0,
                  zorder=1))
    ax.text(
        left_x + 21.0,
        76.0,
        r"Brittle Elastic Lithosphere ($T_e \sim 1-3\ \mathrm{km},\ T < 190\ \mathrm{K}$)",
        color="#1B4F72",
        fontsize=8,
        fontweight="bold",
        ha="center",
        va="center",
        zorder=2)

    # Stagnant Conductive Lid lower part (z = 72 to 54)
    ax.add_patch(
        Rectangle((left_x, 54.0),
                  left_w,
                  18.0,
                  fc="#AED6F1",
                  ec="white",
                  lw=1.0,
                  zorder=1))
    ax.text(
        left_x + 21.0,
        63.0,
        r"\textbf{Stagnant Conductive Lid} ($\delta_{\rm lid} \approx 12-15\ \mathrm{km}$)"
        + "\n" +
        r"$\eta(T) > 10^{16}\ \mathrm{Pa\cdot s}$, Immobile Conductive Diffusion"
        + "\n" +
        r"$F_{\rm cond} = k \Delta T / \delta_{\rm lid} \approx 25-35\ \mathrm{mW/m^2}$",
        color="#154360",
        fontsize=8,
        ha="center",
        va="center",
        zorder=2)

    # 3. Convective Warm Ductile Sublayer (z = 54 to 28)
    ax.add_patch(
        Rectangle((left_x, 28.0),
                  left_w,
                  26.0,
                  fc="#FADBD8",
                  ec="white",
                  lw=1.0,
                  zorder=1))
    ax.text(
        left_x + 21.0,
        50.0,
        r"\textbf{Warm Ductile Convective Sublayer} ($\delta_{\rm conv} \approx 10-15\ \mathrm{km}$)"
        + "\n" +
        r"Well-Mixed Core $T_{\rm conv} \approx 258\ \mathrm{K},\ \eta \approx 10^{14}\ \mathrm{Pa\cdot s}$",
        color="#78281F",
        fontsize=8,
        ha="center",
        va="center",
        zorder=2)

    # Convective Plumes (thermal diapirs)
    for px, color_p in [(left_x + 10.0, "#E74C3C"), (left_x + 32.0, "#E74C3C")]:
        ax.add_patch(
            Wedge((px, 38.0),
                  4.5,
                  0,
                  180,
                  fc=color_p,
                  ec="darkred",
                  lw=1.2,
                  alpha=0.85,
                  zorder=3))
        ax.annotate("",
                    xy=(px, 49.0),
                    xytext=(px, 32.0),
                    arrowprops=dict(arrowstyle="->", color="darkred", lw=2.2),
                    zorder=4)
        ax.text(px,
                35.0,
                r"$\uparrow$ Plume" + "\n" +
                r"$u \sim 10\ \frac{\rm cm}{\rm yr}$",
                color="white",
                fontsize=6.5,
                fontweight="bold",
                ha="center",
                va="center",
                zorder=5)

    # Downwelling cold sheets
    for px in [left_x + 21.0]:
        ax.annotate("",
                    xy=(px, 31.0),
                    xytext=(px, 48.0),
                    arrowprops=dict(arrowstyle="->", color="#2980B9", lw=2.0),
                    zorder=4)
        ax.text(px,
                39.0,
                r"$\downarrow$ Cold" + "\n" + r"Drip",
                color="#1B4F72",
                fontsize=6.5,
                fontweight="bold",
                ha="center",
                va="center",
                zorder=5)

    # 4. Basal Boundary & Subsurface Ocean (z = 28 to 8)
    ax.add_patch(
        Rectangle((left_x, 8.0),
                  left_w,
                  20.0,
                  fc="#3498DB",
                  ec="white",
                  lw=1.0,
                  zorder=1))
    ax.text(
        left_x + 21.0,
        22.0,
        r"\textbf{Global Subsurface Ocean} ($T_b = 270\ \mathrm{K},\ \rho = 1000\ \mathrm{kg/m^3}$)",
        color="white",
        fontsize=8.5,
        fontweight="bold",
        ha="center",
        va="center",
        zorder=2)
    ax.text(
        left_x + 21.0,
        14.0,
        r"Basal Heat Flux $F_{\rm basal} = F_{\rm radio} + F_{\rm tidal,rock} \approx 20-50\ \mathrm{mW/m^2}$"
        + "\n" + r"Decoupled Hydrothermal Exchange with Silicate Core",
        color="#EBF5FB",
        fontsize=7.5,
        ha="center",
        va="center",
        zorder=2)

    # Basal heat arrows
    for dx in [10.0, 21.0, 32.0]:
        ax.annotate("",
                    xy=(left_x + dx, 27.5),
                    xytext=(left_x + dx, 18.0),
                    arrowprops=dict(arrowstyle="->", color="#F39C12", lw=2.0),
                    zorder=3)

    # Right Section: Convective-Conductive Transition & Bistability Mechanisms
    right_x = 52.0
    right_w = 44.0

    # Panel A: Regime Map Box
    ax.add_patch(
        Rectangle((right_x, 52.0),
                  right_w,
                  36.0,
                  fc="#EAEDED",
                  ec="#BDC3C7",
                  lw=1.2,
                  zorder=1))
    ax.text(right_x + right_w / 2.0,
            85.0,
            r"\textbf{I. Convective--Conductive Regime Criteria}",
            color="#17202A",
            fontsize=9.5,
            fontweight="bold",
            ha="center",
            va="center",
            zorder=2)

    crit_text = (
        r"$\bullet$ \textbf{Basal Rayleigh Number}: $Ra_b = \frac{\rho g \alpha \Delta T D^3}{\kappa \eta_{\rm base}}$"
        + "\n\n" +
        r"$\bullet$ \textbf{Frank-Kamenetskii Rheology}: $\theta = \frac{E^* \Delta T}{R_g T_b^2} \approx 14.02$"
        + "\n\n" +
        r"$\bullet$ \textbf{Critical Rayleigh Threshold}: $Ra_{\rm cr} \approx 20.0\,\theta^4 \approx 7.74 \times 10^5$"
        + "\n\n" +
        r"$\bullet$ \textbf{Critical Shell Thickness}: $D_{\rm cr} = \left(\frac{Ra_{\rm cr} \kappa \eta_{\rm base}}{\rho g \alpha \Delta T}\right)^{1/3} \approx 14.3\ \mathrm{km}$"
        + "\n\n" +
        r"$\bullet$ \textbf{Nusselt Scaling}: $Nu = 0.95\,\theta^{-1.22}\,Ra_b^{0.22} \approx 2-5$ in stagnant lid"
    )
    ax.text(right_x + 2.0,
            68.0,
            crit_text,
            color="#2C3E50",
            fontsize=7.8,
            va="center",
            zorder=2)

    # Panel B: Hysteresis & Bistability Switching Box
    ax.add_patch(
        Rectangle((right_x, 8.0),
                  right_w,
                  40.0,
                  fc="#FEF9E7",
                  ec="#F39C12",
                  lw=1.4,
                  zorder=1))
    ax.text(right_x + right_w / 2.0,
            45.0,
            r"\textbf{II. Bistability \& Dynamic Switching Cycle}",
            color="#7D6608",
            fontsize=9.5,
            fontweight="bold",
            ha="center",
            va="center",
            zorder=2)

    bistable_text = (
        r"\textbf{Conductive-Convective Hysteresis Window} ($F_b \in [15, 42]\ \mathrm{mW/m^2}$):"
        + "\n\n" +
        r"1. \textbf{Thick Convective Branch} ($D \approx 20-35\ \mathrm{km}$):"
        + "\n" +
        r"   High heat transport ($Nu > 2.5$) removes basal flux + tidal heat."
        + "\n" + r"   Sustained by vigorous ductile sublayer convection." +
        "\n\n" +
        r"2. \textbf{Thin Conductive Branch} ($D \approx 8-14\ \mathrm{km} < D_{\rm cr}$):"
        + "\n" + r"   Subcritical Rayleigh number suppresses convection." +
        "\n" + r"   Steep thermal gradient conducts heat without flow." +
        "\n\n" + r"3. \textbf{Perturbation Transitions}:" + "\n" +
        r"   $\bullet$ Heat surge triggers rapid catastrophic melting ($10^5\ \mathrm{yr}$)."
        + "\n" +
        r"   $\bullet$ Heat deficit causes slow conductive freeze-out ($10^6\ \mathrm{yr}$)."
    )
    ax.text(right_x + 2.0,
            26.0,
            bistable_text,
            color="#4D5656",
            fontsize=7.6,
            va="center",
            zorder=2)

    plt.tight_layout()
    out_pdf = os.path.join(SCRIPT_DIR, "fig_diagram.pdf")
    plt.savefig(out_pdf, dpi=300)
    plt.close()
    print(f"✅ Generated {out_pdf}")


def main():
    print("Loading simulation datasets...")
    data_nu, data_eq, data_evol, data_prof = load_datasets()

    print("Generating Figure 1: fig_comparison.pdf...")
    plot_fig_comparison(data_nu, data_eq, data_prof)

    print("Generating Figure 2: fig_model_choices.pdf...")
    plot_fig_model_choices(data_evol)

    print("Generating Figure 3: fig_diagram.pdf...")
    plot_fig_diagram()

    print("All 3 publication figures generated successfully.")


if __name__ == "__main__":
    main()
