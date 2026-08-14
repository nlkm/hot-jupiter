#!/usr/bin/env python3
# Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
# Plot Generator for Paper #241: An Outer Planet Beyond Neptune & Detached TNO Dynamics
# Lykawka & Mukai (2008), The Astronomical Journal 135:1161-1200

import os

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Rectangle

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
if not script_dir:
    script_dir = "."


# ----------------------------------------------------------------------
# 1. Figure 1: Comparison of Perihelion Lifting Trajectories & Catalog Match
# ----------------------------------------------------------------------
def generate_fig_comparison():
    traj_csv = os.path.join(script_dir, "secular_trajectories.csv")
    cat_csv = os.path.join(script_dir, "detached_catalog_comparison.csv")

    if not os.path.exists(traj_csv) or not os.path.exists(cat_csv):
        print("Running solver binary to generate CSV files...")
        os.system(
            f"cd {script_dir}/../.. && ./bazel-bin/replications_ss/paper_241/paper_241_solver"
        )

    df_traj = np.genfromtxt(traj_csv,
                            delimiter=",",
                            names=True,
                            dtype=None,
                            encoding="utf-8")
    df_cat = np.genfromtxt(cat_csv,
                           delimiter=",",
                           names=True,
                           dtype=None,
                           encoding="utf-8")

    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.0), dpi=300)

    # Panel 1: Secular Perihelion Lifting Trajectories q(t)
    colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd"]
    body_names = [
        "2004 XR190 (Buffy)",
        "2005 TB190",
        "2000 CR105",
        "2004 VN112",
        "Sedna (2003 VB12)",
    ]

    for b_id in range(5):
        mask = df_traj["body_id"] == b_id
        sub_time = df_traj["time_myr"][mask]
        sub_q = df_traj["q_au"][mask]
        ax1.plot(
            sub_time,
            sub_q,
            lw=2.0,
            color=colors[b_id],
            label=f"{body_names[b_id]} ($a={df_traj['a_au'][mask][0]:.1f}$ AU)",
        )

    ax1.axhline(
        40.0,
        color="darkred",
        linestyle="--",
        lw=1.8,
        label=r"Detached Boundary ($q = 40$ AU)",
    )
    ax1.axhline(
        30.1,
        color="navy",
        linestyle=":",
        lw=1.5,
        label=r"Neptune Orbit ($a_N = 30.1$ AU)",
    )
    ax1.axhspan(40.0,
                90.0,
                color="green",
                alpha=0.08,
                label="Detached TNO Realm ($q > 40$ AU)")

    ax1.set_xlabel(r"Secular Evolution Time [Myr]", fontweight="bold")
    ax1.set_ylabel(r"Perihelion Distance $q$ [AU]", fontweight="bold")
    ax1.set_title(
        r"(a) Secular Perihelion Lifting Trajectories ($q(t) > 40$ AU)",
        fontweight="bold",
    )
    ax1.set_xlim(0, 4000)
    ax1.set_ylim(25, 88)
    ax1.legend(loc="upper right", frameon=True, fontsize=8)

    # Panel 2: Observed vs Model Predicted Perihelion Distances
    q_obs = df_cat["q_obs_au"]
    q_pred = df_cat["predicted_q_max_au"]
    a_obs = df_cat["a_au"]

    sc = ax2.scatter(
        q_obs,
        q_pred,
        c=a_obs,
        cmap="viridis",
        s=65,
        edgecolors="black",
        linewidth=0.8,
        zorder=5,
    )
    cbar = plt.colorbar(sc, ax=ax2)
    cbar.set_label(r"Semi-Major Axis $a$ [AU]", fontweight="bold")

    # 1:1 line
    line_range = np.linspace(30, 85, 100)
    ax2.plot(line_range,
             line_range,
             "k--",
             lw=1.5,
             label=r"1:1 Perfect Concordance")

    # Detached threshold box
    ax2.axvline(40.0, color="darkred", linestyle=":", lw=1.2)
    ax2.axhline(40.0, color="darkred", linestyle=":", lw=1.2)
    ax2.fill_between(
        [40, 85],
        40,
        85,
        color="green",
        alpha=0.07,
        label="Detached Regime ($q > 40$ AU)",
    )

    # Label key bodies
    for i in range(len(df_cat)):
        name_clean = df_cat["name"][i].replace('"', "")
        if name_clean in [
                "2000 CR105",
                "Sedna (2003 VB12)",
                "2004 XR190 (Buffy)",
                "2012 VP113",
                "2005 TB190",
        ]:
            ax2.annotate(
                name_clean,
                (q_obs[i], q_pred[i]),
                textcoords="offset points",
                xytext=(6, -3),
                fontsize=7.5,
                fontweight="bold",
                bbox=dict(boxstyle="round,pad=0.2", fc="yellow", alpha=0.4),
            )

    ss_tot = np.sum((q_obs - np.mean(q_obs))**2)
    ss_res = np.sum((q_obs - q_pred)**2)
    r2 = 1.0 - (ss_res / ss_tot)

    ax2.set_xlabel(r"Observed Perihelion Distance $q_{\mathrm{obs}}$ [AU]",
                   fontweight="bold")
    ax2.set_ylabel(r"Model Predicted Perihelion $q_{\mathrm{model}}$ [AU]",
                   fontweight="bold")
    ax2.set_title(rf"(b) Observed vs. Secular Model ($R^2 = {r2:.4f}$)",
                  fontweight="bold")
    ax2.set_xlim(32, 85)
    ax2.set_ylim(32, 85)
    ax2.legend(loc="lower right", frameon=True, fontsize=8)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_comparison.pdf")
    plt.savefig(out_pdf, bbox_inches="tight")
    plt.close()
    print(f"Generated {out_pdf}")


# ----------------------------------------------------------------------
# 2. Figure 2: Secular Frequencies & Parameter Grid Sensitivity
# ----------------------------------------------------------------------
def generate_fig_model_choices():
    freq_csv = os.path.join(script_dir, "secular_frequencies.csv")
    grid_csv = os.path.join(script_dir, "parameter_space_grid.csv")

    df_freq = np.genfromtxt(freq_csv, delimiter=",", names=True)
    df_grid = np.genfromtxt(grid_csv, delimiter=",", names=True)

    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.0), dpi=300)

    # Panel 1: Secular Precession Frequencies vs Semi-Major Axis
    ax1.plot(
        df_freq["a_au"],
        df_freq["g_giant_arcsec_yr"],
        "-",
        color="#1f77b4",
        lw=2.2,
        label=r"Giant Planets $g_{\mathrm{giants}}(a)$",
    )
    ax1.plot(
        df_freq["a_au"],
        df_freq["g_planet_arcsec_yr"],
        "-",
        color="#d62728",
        lw=2.2,
        label=r"Outer Planet $g_{\mathrm{Planet\,X}}(a)$ ($m_p=0.5 M_\oplus$)",
    )
    ax1.plot(
        df_freq["a_au"],
        df_freq["g_total_arcsec_yr"],
        "--",
        color="#2ca02c",
        lw=2.0,
        label=r"Total Secular Rate $g_{\mathrm{tot}}(a)$",
    )

    # Mark Neptune MMRs
    mmrs = [
        (39.4, "3:2 (Plutinos)"),
        (47.8, "2:1 (Twotinos / Edge)"),
        (55.4, "5:2"),
        (62.5, "3:1"),
        (75.8, "4:1"),
        (88.0, "5:1"),
    ]
    for a_res, lbl in mmrs:
        ax1.axvline(a_res, color="gray", linestyle=":", lw=1.0, alpha=0.7)
        if a_res in [39.4, 47.8, 62.5]:
            ax1.text(
                a_res,
                0.4,
                lbl,
                rotation=90,
                verticalalignment="bottom",
                horizontalalignment="right",
                fontsize=7,
                color="darkslategray",
            )

    ax1.set_yscale("log")
    ax1.set_xlabel(r"TNO Semi-Major Axis $a$ [AU]", fontweight="bold")
    ax1.set_ylabel(r"Secular Precession Frequency $g$ [arcsec/yr]",
                   fontweight="bold")
    ax1.set_title(r"(a) Laplace-Lagrange Secular Precession Spectrum",
                  fontweight="bold")
    ax1.set_xlim(30, 300)
    ax1.set_ylim(1e-4, 1.0)
    ax1.legend(loc="upper right", frameon=True, fontsize=8)

    # Panel 2: Parameter Grid Heatmap of Detached Lifting Efficiency
    # Filter grid for inc_planet = 30 deg
    mask_inc = np.isclose(df_grid["inc_planet_deg"], 30.0)
    sub_grid = df_grid[mask_inc]

    m_unique = np.unique(sub_grid["m_planet_earth"])
    a_unique = np.unique(sub_grid["a_planet_au"])
    M, A = np.meshgrid(m_unique, a_unique)
    Z = np.zeros_like(M)

    for i in range(len(a_unique)):
        for j in range(len(m_unique)):
            idx = np.where(
                (np.isclose(sub_grid["a_planet_au"], a_unique[i])) &
                (np.isclose(sub_grid["m_planet_earth"], m_unique[j])))[0]
            if len(idx) > 0:
                Z[i, j] = sub_grid["lifting_frac_100_250"][idx[0]]

    cf = ax2.contourf(M, A, Z, levels=14, cmap="plasma")
    cbar = plt.colorbar(cf, ax=ax2)
    cbar.set_label(r"Detached Lifting Fraction ($a \in [100, 250]$ AU)",
                   fontweight="bold")

    # Draw Lykawka & Mukai favored parameter box
    rect = Rectangle(
        (0.3, 100),
        0.4,
        75,
        linewidth=2.0,
        edgecolor="lime",
        facecolor="none",
        linestyle="--",
        label="Lykawka & Mukai (2008) Favored Region",
    )
    ax2.add_patch(rect)
    ax2.plot(
        0.5,
        120,
        marker="*",
        color="gold",
        markersize=14,
        markeredgecolor="black",
        label=r"Nominal Model ($0.5 M_\oplus, 120$ AU)",
    )

    ax2.set_xlabel(r"Outer Planet Mass $m_p$ [$M_\oplus$]", fontweight="bold")
    ax2.set_ylabel(r"Outer Planet Semi-Major Axis $a_p$ [AU]",
                   fontweight="bold")
    ax2.set_title(r"(b) Detached Disk Lifting Efficiency & Favored Regime",
                  fontweight="bold")
    ax2.legend(loc="lower right", frameon=True, fontsize=8)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_model_choices.pdf")
    plt.savefig(out_pdf, bbox_inches="tight")
    plt.close()
    print(f"Generated {out_pdf}")


# ----------------------------------------------------------------------
# 3. Figure 3: Physical Architecture & Kozai Phase Space Diagram
# ----------------------------------------------------------------------
def generate_fig_diagram():
    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.2), dpi=300)

    # Panel 1: Heliocentric Orbital Layout (Top-Down X-Y Plane)
    theta = np.linspace(0, 2 * np.pi, 300)

    # Sun
    ax1.plot(
        0,
        0,
        marker="o",
        color="yellow",
        markersize=10,
        markeredgecolor="orange",
        label="Sun",
    )

    # Giant planets
    planets = [
        ("Jupiter", 5.2, "#1f77b4"),
        ("Saturn", 9.58, "#ff7f0e"),
        ("Uranus", 19.2, "#2ca02c"),
        ("Neptune", 30.1, "#00008b"),
    ]
    for name, r, col in planets:
        ax1.plot(
            r * np.cos(theta),
            r * np.sin(theta),
            color=col,
            linestyle=":",
            lw=1.2,
            label=f"{name} ({r:.1f} AU)",
        )

    # Classical Kuiper Belt ring
    ax1.fill_between(np.linspace(42, 48, 100),
                     -0.5,
                     0.5,
                     color="cyan",
                     alpha=0.15)
    r_kuiper_out = 48.0
    ax1.plot(
        r_kuiper_out * np.cos(theta),
        r_kuiper_out * np.sin(theta),
        color="darkturquoise",
        linestyle="-",
        lw=1.5,
        label="Kuiper Belt Edge (48 AU, 2:1 MMR)",
    )

    # Outer Planet orbit (a=120, e=0.3, q=84, Q=156)
    a_p, e_p = 120.0, 0.30
    b_p = a_p * np.sqrt(1 - e_p**2)
    c_p = a_p * e_p
    x_p = a_p * np.cos(theta) - c_p
    y_p = b_p * np.sin(theta)
    ax1.plot(
        x_p,
        y_p,
        color="crimson",
        lw=2.2,
        linestyle="-",
        label=r"Outer Planet ($a_p=120, e_p=0.3, q_p=84$ AU, $i_p=30^\circ$)",
    )

    # Detached objects (2000 CR105, Sedna, Buffy)
    # CR105: a=224.5, e=0.8026
    a_cr, e_cr = 224.5, 0.8026
    x_cr = a_cr * np.cos(theta) - a_cr * e_cr
    y_cr = a_cr * np.sqrt(1 - e_cr**2) * np.sin(theta)
    ax1.plot(
        x_cr,
        y_cr,
        color="purple",
        lw=1.5,
        linestyle="--",
        label=r"2000 CR105 ($a=224.5, q=44.3$ AU)",
    )

    # Buffy: a=57.5, e=0.11
    a_buf, e_buf = 57.5, 0.11
    x_buf = a_buf * np.cos(theta) - a_buf * e_buf
    y_buf = a_buf * np.sqrt(1 - e_buf**2) * np.sin(theta)
    ax1.plot(
        x_buf,
        y_buf,
        color="darkgreen",
        lw=1.5,
        linestyle="-.",
        label=r"Buffy 2004 XR190 ($a=57.5, q=51.2$ AU)",
    )

    ax1.set_xlabel(r"Heliocentric $X$ [AU]", fontweight="bold")
    ax1.set_ylabel(r"Heliocentric $Y$ [AU]", fontweight="bold")
    ax1.set_title(r"(a) Outer Solar System & Detached Belt Architecture",
                  fontweight="bold")
    ax1.set_xlim(-260, 260)
    ax1.set_ylim(-260, 260)
    ax1.set_aspect("equal")
    ax1.legend(loc="upper right", frameon=True, fontsize=7.2)

    # Panel 2: Kozai-Lidov Phase Portrait (e vs omega) and Perihelion Contours
    omega_grid = np.linspace(0, 180, 200)
    e_grid = np.linspace(0.1, 0.95, 200)
    OMEGA, E = np.meshgrid(omega_grid, e_grid)

    # Kozai Hamiltonian contours: H_K ~ (2 + 3e^2)(3 cos^2 i - 1) + 15 e^2 sin^2 i cos(2 omega)
    H_z = 0.55
    cos_i = np.clip(H_z / np.sqrt(np.maximum(0.01, 1 - E**2)), -1, 1)
    sin_i = np.sqrt(1 - cos_i**2)

    H_kozai = -(2 + 3 * E**2) * (3 * cos_i**2 - 1) - 15 * E**2 * (
        sin_i**2) * np.cos(2 * np.radians(OMEGA))

    cs = ax2.contour(OMEGA,
                     E,
                     H_kozai,
                     levels=14,
                     cmap="viridis",
                     linewidths=1.3)
    ax2.clabel(cs, inline=1, fontsize=7, fmt="%.1f")

    # Mark Kozai libration center at omega = 90 deg
    ax2.plot(
        90,
        0.65,
        marker="x",
        color="red",
        markersize=12,
        markeredgewidth=2.5,
        label=r"Kozai Libration Island ($\omega = 90^\circ$)",
    )

    # Draw perihelion lifting direction arrow
    ax2.annotate(
        "Perihelion Lifting:\n" +
        r"$e \downarrow \;\rightarrow\; q = a(1-e) \uparrow$",
        xy=(90, 0.50),
        xytext=(35, 0.22),
        arrowprops=dict(facecolor="darkred",
                        edgecolor="darkred",
                        arrowstyle="->",
                        lw=2.0),
        fontsize=9,
        fontweight="bold",
        color="darkred",
        bbox=dict(boxstyle="round,pad=0.3", fc="pink", alpha=0.4),
    )

    # Mark Detached region for a = 200 AU (q > 40 AU => e < 0.80)
    ax2.axhspan(
        0.1,
        0.80,
        color="green",
        alpha=0.10,
        label=r"Detached Domain ($q > 40$ AU at $a=200$ AU)",
    )

    ax2.set_xlabel(r"Argument of Perihelion $\omega$ [deg]", fontweight="bold")
    ax2.set_ylabel(r"Orbital Eccentricity $e$", fontweight="bold")
    ax2.set_title(r"(b) Kozai-Lidov Secular Resonance Phase Space",
                  fontweight="bold")
    ax2.set_xlim(0, 180)
    ax2.set_ylim(0.1, 0.95)
    ax2.legend(loc="lower left", frameon=True, fontsize=8)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_diagram.pdf")
    plt.savefig(out_pdf, bbox_inches="tight")
    plt.close()
    print(f"Generated {out_pdf}")


if __name__ == "__main__":
    print("Generating Paper #241 Figures...")
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("All plots generated successfully!")
