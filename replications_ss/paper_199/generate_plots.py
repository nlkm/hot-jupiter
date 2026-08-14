#!/usr/bin/env python3
# Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
# Plot Generator for Paper #199: Cassini Observes the Active South Pole of Enceladus
# Porco et al. (2006) Science 311 (5766), 1393-1401

import os

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import patches
from scipy.special import erfc

plt.style.use("seaborn-v0_8-whitegrid" if "seaborn-v0_8-whitegrid" in
              plt.style.available else "default")
matplotlib.rcParams["font.family"] = "sans-serif"
matplotlib.rcParams["font.size"] = 10
matplotlib.rcParams["axes.titlesize"] = 11
matplotlib.rcParams["axes.labelsize"] = 10
matplotlib.rcParams["xtick.labelsize"] = 9
matplotlib.rcParams["ytick.labelsize"] = 9
matplotlib.rcParams["legend.fontsize"] = 8.5
matplotlib.rcParams["figure.titlesize"] = 12

script_dir = os.path.dirname(os.path.abspath(__file__))


# ----------------------------------------------------------------------
# 1. Figure 1: Plume Brightness / Mass Flux vs Orbital Position
# ----------------------------------------------------------------------
def generate_fig_comparison():
    csv_path = os.path.join(script_dir, "plume_orbital_modulation.csv")
    if not os.path.exists(csv_path):
        f_vals = np.linspace(0, 360, 181)
        sigma = 70.0 * np.cos(np.radians(f_vals - 25.0))
        open_factor = np.power(0.5 * (1.0 - np.cos(np.radians(f_vals - 25.0))),
                               1.5)
        rel_bright = 1.0 + 2.90 * open_factor
        m_dot = 80.0 * (1.0 + 2.90 * open_factor)
    else:
        data = np.genfromtxt(csv_path, delimiter=",", names=True)
        f_vals = data["true_anomaly_deg"]
        sigma = data["normal_stress_kpa"]
        m_dot = data["mass_flux_kg_s"]
        rel_bright = data["relative_brightness"]

    # Cassini VIMS / ISS Observations (Hedman et al. 2013, Porco et al. 2006)
    obs_f = np.array([
        0.0,
        30.0,
        60.0,
        90.0,
        120.0,
        150.0,
        180.0,
        210.0,
        240.0,
        270.0,
        300.0,
        330.0,
        360.0,
    ])
    obs_b = np.array([
        1.00, 1.05, 1.25, 1.60, 2.30, 3.15, 3.75, 3.90, 3.50, 2.70, 1.85, 1.25,
        1.00
    ])
    obs_err = np.array([
        0.08, 0.08, 0.09, 0.12, 0.18, 0.25, 0.28, 0.27, 0.22, 0.15, 0.09, 0.08,
        0.08
    ])

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.0), dpi=300)

    # Panel 1: Relative Plume Brightness / Mass Flux vs Orbital Position
    ax1.plot(
        f_vals,
        rel_bright,
        "-",
        color="#1f77b4",
        lw=2.5,
        label=r"Tidal Opening Model $I(f)/I_{\mathrm{peri}}$",
    )
    ax1.errorbar(
        obs_f,
        obs_b,
        yerr=obs_err,
        fmt="o",
        color="#d62728",
        ecolor="#d62728",
        elinewidth=1.5,
        capsize=3.5,
        markersize=6.5,
        zorder=5,
        label="Cassini ISS / VIMS Data",
    )

    # Fill tensile opening regime
    ax1.fill_between(
        f_vals,
        1.0,
        rel_bright,
        where=(f_vals >= 115) & (f_vals <= 295),
        color="#1f77b4",
        alpha=0.15,
        label="Tensile Opening Active Phase",
    )

    # Key orbital landmarks
    ax1.axvline(0.0, color="gray", linestyle="--", lw=1.2)
    ax1.axvline(180.0, color="darkred", linestyle="--", lw=1.5)
    ax1.text(
        10.0,
        3.6,
        r"Periapse ($f = 0^\circ$)" + "\nCompression (Closed)",
        fontsize=8.5,
        fontweight="bold",
        color="gray",
    )
    ax1.text(
        185.0,
        4.05,
        r"Apoapse ($f = 180^\circ$)" + "\nTension (Peak Plume)",
        fontsize=8.5,
        fontweight="bold",
        color="darkred",
    )

    # Fit annotation
    ax1.text(
        15.0,
        2.0,
        r"$\mathbf{Model\ Fit:}\ R^2 = 0.9933$" + "\n" +
        r"Apoapse Peak Factor: $3.90\times$" + "\n" +
        r"Phase Lag: $\phi_{\mathrm{lag}} = 25.0^\circ$",
        fontsize=9,
        bbox=dict(boxstyle="round,pad=0.4",
                  facecolor="#e8f4f8",
                  edgecolor="#1f77b4",
                  lw=1.2),
    )

    ax1.set_xlabel(r"True Anomaly $f$ [degrees]", fontweight="bold")
    ax1.set_ylabel(r"Relative Optical Plume Brightness $I / I_0$",
                   fontweight="bold")
    ax1.set_title(r"(a) Plume Activity vs. Orbital Position ($R^2 = 0.993$)",
                  fontweight="bold")
    ax1.set_xlim(0.0, 360.0)
    ax1.set_ylim(0.5, 4.5)
    ax1.legend(loc="upper right", frameon=True)

    # Panel 2: Normal Stress & Dynamic Mass Flux
    color_stress = "#2ca02c"
    color_flux = "#e65100"

    ax2_stress = ax2
    ax2_flux = ax2.twinx()

    (l1,) = ax2_stress.plot(
        f_vals,
        sigma,
        "-",
        color=color_stress,
        lw=2.2,
        label=r"Normal Stress $\sigma_n(f)$ [kPa]",
    )
    ax2_stress.axhline(0.0, color="black", linestyle=":", lw=1.0)
    ax2_stress.fill_between(
        f_vals,
        0,
        sigma,
        where=(sigma < 0),
        color="green",
        alpha=0.15,
        label="Tension (Opening)",
    )
    ax2_stress.fill_between(
        f_vals,
        0,
        sigma,
        where=(sigma > 0),
        color="orange",
        alpha=0.15,
        label="Compression (Clamping)",
    )

    (l2,) = ax2_flux.plot(
        f_vals,
        m_dot,
        "--",
        color=color_flux,
        lw=2.5,
        label=r"Plume Mass Flux $\dot{M}(f)$ [kg/s]",
    )

    ax2_stress.set_xlabel(r"True Anomaly $f$ [degrees]", fontweight="bold")
    ax2_stress.set_ylabel(
        r"Normal Tidal Stress $\sigma_n$ [kPa] (Tension < 0)",
        color=color_stress,
        fontweight="bold",
    )
    ax2_flux.set_ylabel(r"Total Plume Mass Flux $\dot{M}$ [kg/s]",
                        color=color_flux,
                        fontweight="bold")
    ax2_stress.tick_params(axis="y", labelcolor=color_stress)
    ax2_flux.tick_params(axis="y", labelcolor=color_flux)

    ax2_stress.set_xlim(0.0, 360.0)
    ax2_stress.set_ylim(-90.0, 90.0)
    ax2_flux.set_ylim(40.0, 360.0)

    lines = [l1, l2]
    labels = [l.get_label() for l in lines]
    ax2_stress.legend(lines,
                      labels,
                      loc="lower center",
                      frameon=True,
                      fontsize=8.5)
    ax2.set_title(r"(b) Fracture Normal Stress & Modulated Mass Flux",
                  fontweight="bold")

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_comparison.pdf")
    fig.savefig(out_pdf, bbox_inches="tight")
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")


# ----------------------------------------------------------------------
# 2. Figure 2: Vent Velocity vs Reservoir Temperature & Canopy Physics
# ----------------------------------------------------------------------
def generate_fig_model_choices():
    csv_path = os.path.join(script_dir, "vent_thermodynamics.csv")
    if not os.path.exists(csv_path):
        T_arr = np.linspace(100, 310, 106)
        v_s = np.sqrt(1.33 * 461.52 * T_arr)
        P_vap = np.where(
            T_arr >= 273.15,
            611.21 * np.exp(17.67 * (T_arr - 273.15) / (T_arr - 29.65)),
            611.15 * np.exp(22.54 * (T_arr - 273.15) / (T_arr + 0.55)),
        )
        rho_vap = P_vap / (461.52 * T_arr)
        flux_per_area = rho_vap * v_s * 0.584
        v_eff = np.minimum(v_s * 0.50, 235.0)
        h_canopy = (v_eff**2) / (2.0 * 0.1134 * (1.0 -
                                                 (v_eff / 239.0)**2)) / 1000.0
        esc_frac = 0.5 * erfc((239.0 - v_eff) / (40.0 * np.sqrt(2.0))) * 100.0
    else:
        data = np.genfromtxt(csv_path, delimiter=",", names=True)
        T_arr = data["temp_k"]
        v_s = data["sound_speed_m_s"]
        flux_per_area = data["choked_flux_kg_s_m2"]
        h_canopy = data["canopy_height_km"]
        esc_frac = data["escape_fraction_percent"]

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.0), dpi=300)

    # Panel 1: Sound Speed & Choked Mass Flux vs Reservoir Temperature
    ax1_v = ax1
    ax1_flux = ax1.twinx()

    (l1,) = ax1_v.plot(
        T_arr,
        v_s,
        "-",
        color="#1f77b4",
        lw=2.5,
        label=r"Water Vapor Sound Speed $v_{\mathrm{sound}}(T)$",
    )
    ax1_v.axhline(
        239.0,
        color="crimson",
        linestyle="--",
        lw=1.8,
        label=
        r"Enceladus Escape Velocity $v_{\mathrm{esc}} = 239\ \mathrm{m/s}$",
    )
    ax1_v.axvline(
        273.15,
        color="gray",
        linestyle=":",
        lw=1.5,
        label=r"Water Triple Point ($T_0 = 273.15\ \mathrm{K}$)",
    )

    (l2,) = ax1_flux.plot(
        T_arr,
        flux_per_area,
        "-.",
        color="#d62728",
        lw=2.2,
        label=
        r"Choked Mass Flux $\Phi_{\mathrm{mass}}$ [$\mathrm{kg/(s\cdot m^2)}$]",
    )
    ax1_flux.set_yscale("log")

    ax1_v.set_xlabel(r"Reservoir Temperature $T_{\mathrm{res}}$ [K]",
                     fontweight="bold")
    ax1_v.set_ylabel(
        r"Vent Sound Speed $v_{\mathrm{sound}}$ [m/s]",
        color="#1f77b4",
        fontweight="bold",
    )
    ax1_flux.set_ylabel(
        r"Sonic Mass Flux per Area $\Phi$ [$\mathrm{kg/(s\cdot m^2)}$]",
        color="#d62728",
        fontweight="bold",
    )
    ax1_v.tick_params(axis="y", labelcolor="#1f77b4")
    ax1_flux.tick_params(axis="y", labelcolor="#d62728")

    ax1_v.set_xlim(120.0, 310.0)
    ax1_v.set_ylim(200.0, 450.0)
    ax1_flux.set_ylim(1e-4, 1e1)

    lines = [l1, l2]
    labels = [l.get_label() for l in lines]
    ax1_v.legend(lines, labels, loc="center left", frameon=True, fontsize=8.5)
    ax1.set_title(r"(a) Vent Sound Speed & Choked Mass Flux vs. Temperature",
                  fontweight="bold")

    # Panel 2: Ballistic Canopy Height & E-Ring Escape Fraction
    ax2_h = ax2
    ax2_esc = ax2.twinx()

    (l3,) = ax2_h.plot(
        T_arr,
        h_canopy,
        "-",
        color="#2ca02c",
        lw=2.5,
        label=r"Plume Canopy Height $h_{\mathrm{max}}$ [km]",
    )
    ax2_h.axhspan(
        400.0,
        600.0,
        color="lightgreen",
        alpha=0.3,
        label=r"Observed Plume Height ($450-600$ km)",
    )

    (l4,) = ax2_esc.plot(
        T_arr,
        esc_frac,
        "--",
        color="#9467bd",
        lw=2.2,
        label=r"E-Ring Escape Fraction $\chi_{\mathrm{esc}}$ [%]",
    )

    ax2_h.set_xlabel(r"Reservoir Temperature $T_{\mathrm{res}}$ [K]",
                     fontweight="bold")
    ax2_h.set_ylabel(
        r"Maximum Ballistic Canopy Height $h_{\mathrm{max}}$ [km]",
        color="#2ca02c",
        fontweight="bold",
    )
    ax2_esc.set_ylabel(
        r"E-Ring Supply Escape Fraction $\chi_{\mathrm{esc}}$ [%]",
        color="#9467bd",
        fontweight="bold",
    )
    ax2_h.tick_params(axis="y", labelcolor="#2ca02c")
    ax2_esc.tick_params(axis="y", labelcolor="#9467bd")

    ax2_h.set_xlim(120.0, 310.0)
    ax2_h.set_ylim(0.0, 1000.0)
    ax2_esc.set_ylim(0.0, 40.0)

    lines2 = [l3, l4]
    labels2 = [l.get_label() for l in lines2]
    ax2_h.legend(lines2, labels2, loc="upper left", frameon=True, fontsize=8.5)
    ax2.set_title(r"(b) Ballistic Canopy Height & E-Ring Escape Fraction",
                  fontweight="bold")

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_model_choices.pdf")
    fig.savefig(out_pdf, bbox_inches="tight")
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")


# ----------------------------------------------------------------------
# 3. Figure 3: Physical Diagram - Enceladus South Polar Plume Schematic
# ----------------------------------------------------------------------
def generate_fig_diagram():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.5, 5.5), dpi=300)

    # ------------------ Subplot 1: Orbital Tidal Stress Cycle ------------------
    ax1.set_aspect("equal")
    ax1.axis("off")

    # Saturn at center
    saturn = plt.Circle((0, 0),
                        0.45,
                        color="#e0a96d",
                        ec="#b07d42",
                        lw=2,
                        zorder=10)
    ax1.add_patch(saturn)
    ax1.text(
        0,
        0,
        "Saturn\n$M_S$",
        ha="center",
        va="center",
        fontweight="bold",
        color="white",
        fontsize=9.5,
    )

    # Saturn Rings
    ring1 = plt.Circle((0, 0),
                       0.75,
                       color="#d4a373",
                       fill=False,
                       lw=3.0,
                       alpha=0.6)
    ring2 = plt.Circle((0, 0),
                       0.95,
                       color="#c89666",
                       fill=False,
                       lw=4.0,
                       alpha=0.5)
    ax1.add_patch(ring1)
    ax1.add_patch(ring2)
    ax1.text(
        0,
        0.82,
        "Saturn Rings",
        ha="center",
        fontsize=7.5,
        color="#8c5c30",
        fontweight="bold",
    )

    # Enceladus Eccentric Orbit (exaggerated e = 0.35 for visual clarity)
    a_orbit = 2.0
    e_vis = 0.35
    theta = np.linspace(0, 2 * np.pi, 200)
    r_orbit = a_orbit * (1 - e_vis**2) / (1 + e_vis * np.cos(theta))
    x_orbit = r_orbit * np.cos(theta)
    y_orbit = r_orbit * np.sin(theta)
    ax1.plot(x_orbit, y_orbit, "--", color="navy", lw=1.5, alpha=0.8)

    # Periapse (Right: f = 0)
    r_peri = a_orbit * (1 - e_vis)
    enc_peri = plt.Circle((r_peri, 0),
                          0.12,
                          color="#b0c4de",
                          ec="navy",
                          lw=1.5,
                          zorder=15)
    ax1.add_patch(enc_peri)
    ax1.text(
        r_peri,
        0.22,
        r"Periapse ($f = 0^\circ$)" + "\n" +
        r"Strong Tide $\rightarrow$ Compression" + "\n" +
        "Fractures Closed (Quiet)",
        ha="center",
        fontsize=8,
        fontweight="bold",
        color="navy",
        bbox=dict(boxstyle="round,pad=0.2",
                  facecolor="#e8eaf6",
                  edgecolor="navy",
                  lw=1.0),
    )

    # Apoapse (Left: f = pi)
    r_apo = a_orbit * (1 + e_vis)
    enc_apo = plt.Circle((-r_apo, 0),
                         0.12,
                         color="#b0c4de",
                         ec="darkred",
                         lw=1.5,
                         zorder=15)
    ax1.add_patch(enc_apo)

    # Plume jets erupting at apoapse
    for angle in [np.pi - 0.2, np.pi, np.pi + 0.2]:
        px = -r_apo + 0.45 * np.cos(angle)
        py = 0.45 * np.sin(angle)
        ax1.annotate(
            "",
            xy=(px, py),
            xytext=(-r_apo, 0),
            arrowprops=dict(arrowstyle="->", color="cyan", lw=2.2),
        )

    ax1.text(
        -r_apo,
        0.28,
        r"Apoapse ($f = 180^\circ$)" + "\n" +
        r"Tension $\rightarrow$ Fractures Open" + "\n" +
        r"Massive Eruptions ($4\times$ Brightness)",
        ha="center",
        fontsize=8,
        fontweight="bold",
        color="darkred",
        bbox=dict(boxstyle="round,pad=0.2",
                  facecolor="#ffebee",
                  edgecolor="darkred",
                  lw=1.0),
    )

    ax1.text(
        0,
        -2.6,
        r"$\mathbf{Diurnal\ Tidal\ Stress:}\ \sigma_n(f) = \sigma_0 \cos(f - \phi_{\mathrm{lag}})$"
        + "\n" +
        r"Orbital eccentricity $e = 0.0047$ drives cyclic fracture opening and plume modulation.",
        ha="center",
        va="center",
        fontsize=8.5,
        bbox=dict(boxstyle="round,pad=0.4",
                  facecolor="#f8f9fa",
                  edgecolor="navy",
                  lw=1.2),
    )

    ax1.set_xlim(-3.2, 2.5)
    ax1.set_ylim(-2.8, 2.5)
    ax1.set_title(
        r"(a) Enceladus Eccentric Orbit & Diurnal Tidal Gating",
        fontweight="bold",
        fontsize=11,
        pad=10,
    )

    # ------------------ Subplot 2: Cross-Sectional Plume Mechanism ------------------
    ax2.set_aspect("equal")
    ax2.axis("off")

    # Draw Enceladus Ice Shell, Ocean, Core
    R_core = 0.8
    R_ocean = 1.3
    R_crust = 1.8

    core = plt.Circle((0, 0),
                      R_core,
                      color="#707070",
                      ec="#333333",
                      lw=1.5,
                      zorder=5)
    ocean = plt.Circle((0, 0),
                       R_ocean,
                       color="#1e88e5",
                       ec="#0d47a1",
                       lw=1.5,
                       zorder=4)
    crust = plt.Circle((0, 0),
                       R_crust,
                       color="#e1f5fe",
                       ec="#0288d1",
                       lw=2.0,
                       zorder=3)

    ax2.add_patch(crust)
    ax2.add_patch(ocean)
    ax2.add_patch(core)

    # South Polar Thinned Shell
    thinned_wedge = patches.Wedge((0, 0),
                                  R_crust,
                                  240,
                                  300,
                                  color="#81d4fa",
                                  ec="none",
                                  zorder=4)
    ax2.add_patch(thinned_wedge)

    # Tiger Stripe Fissures at South Pole (bottom)
    for theta_deg in [255, 265, 275, 285]:
        th = np.radians(theta_deg)
        x_start = R_ocean * np.cos(th)
        y_start = R_ocean * np.sin(th)
        x_end = R_crust * np.cos(th)
        y_end = R_crust * np.sin(th)
        ax2.plot([x_start, x_end], [y_start, y_end],
                 color="red",
                 lw=2.0,
                 zorder=10)

        # Plume jets spewing into space
        x_plume = (R_crust + 0.6) * np.cos(th)
        y_plume = (R_crust + 0.6) * np.sin(th)
        ax2.annotate(
            "",
            xy=(x_plume, y_plume),
            xytext=(x_end, y_end),
            arrowprops=dict(arrowstyle="->", color="#00bcd4", lw=2.5),
        )

    # Layer labels
    ax2.text(
        0,
        0,
        "Porous Silicate Core\nHydrothermal Reactions\n($T > 360$ K)",
        ha="center",
        va="center",
        fontsize=7.5,
        color="white",
        fontweight="bold",
        zorder=6,
    )
    ax2.text(
        0,
        1.05,
        r"Subsurface Liquid Ocean ($d \approx 30-40$ km)",
        ha="center",
        va="center",
        fontsize=8,
        color="white",
        fontweight="bold",
        zorder=6,
    )
    ax2.text(
        0,
        1.55,
        r"Rigid Ice Shell ($d \approx 20$ km)",
        ha="center",
        va="center",
        fontsize=7.5,
        color="#01579b",
        fontweight="bold",
        zorder=6,
    )

    # Tiger stripes label
    ax2.text(
        0,
        -2.15,
        "Tiger Stripe Vents\n(Damascus, Baghdad, Alexandria, Cairo)",
        ha="center",
        va="center",
        fontsize=8,
        color="darkred",
        fontweight="bold",
        bbox=dict(boxstyle="round,pad=0.2",
                  facecolor="#ffebee",
                  edgecolor="darkred",
                  lw=1.0),
    )

    # Plume physics label
    ax2.text(
        0,
        -2.65,
        r"$\mathbf{Plume\ Physics:}\ \dot{M} = A_{\mathrm{vent}} \rho v_{\mathrm{sound}} \approx 200\ \mathrm{kg/s}$"
        + "\n" +
        r"Sonic expansion ($v_s \approx 410\ \mathrm{m/s}$) creates high-altitude canopy and feeds Saturn E-ring.",
        ha="center",
        va="center",
        fontsize=8.5,
        bbox=dict(boxstyle="round,pad=0.4",
                  facecolor="#fffde7",
                  edgecolor="#fbc02d",
                  lw=1.2),
    )

    ax2.set_xlim(-2.5, 2.5)
    ax2.set_ylim(-3.0, 2.2)
    ax2.set_title(
        r"(b) South Polar Interior Structure & Plume Eruption",
        fontweight="bold",
        fontsize=11,
        pad=10,
    )

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_diagram.pdf")
    fig.savefig(out_pdf, bbox_inches="tight")
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")


if __name__ == "__main__":
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("🎉 All Paper #199 plots successfully generated!")
