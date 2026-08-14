#!/usr/bin/env python3
"""
Generate publication-quality figures for Paper #238 Replication:
Goldreich, Lithwick, & Sari (2004) "Planet Formation by Coagulation: A Focus on Uranus & Neptune"
ARA&A 42:549-601 (2004); ApJ 614:497-507 (2004)

Outputs:
  - fig_comparison.pdf & fig_comparison.png
  - fig_model_choices.pdf & fig_model_choices.png
  - fig_diagram.pdf & fig_diagram.png
"""

import csv
import os

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import patches
from matplotlib.gridspec import GridSpec

# Set Matplotlib publication styling
plt.rcParams.update({
    'font.family': 'serif',
    'font.serif': ['DejaVu Serif', 'Times New Roman', 'Computer Modern Roman'],
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 14,
    'text.usetex': False,
    'mathtext.fontset': 'cm',
    'figure.autolayout': False
})

DIR = os.path.dirname(os.path.abspath(__file__))


def read_csv_dicts(filename):
    path = os.path.join(DIR, filename)
    with open(path, 'r', encoding='utf-8') as f:
        return list(csv.DictReader(f))


def plot_comparison():
    """Figure 1: Accretion Rate vs Velocity Dispersion theta = u / v_H across regimes."""
    rows = read_csv_dicts("growth_rate_vs_theta.csv")

    fig = plt.figure(figsize=(14, 6))
    gs = GridSpec(1, 2, width_ratios=[1.15, 1.0], wspace=0.28)

    # ---------------- Left Panel: dR/dt vs theta for Uranus (R = 5000 km) ----------------
    ax1 = fig.add_subplot(gs[0])

    u_5k = [
        r for r in rows
        if abs(float(r['a_au']) -
               19.2) < 0.1 and abs(float(r['R_km']) - 5000.0) < 1.0
    ]

    thetas = np.array([float(r['theta']) for r in u_5k])
    dr_dt = np.array([float(r['dr_dt_km_myr']) for r in u_5k])
    dr_2d = np.array([float(r['dr_dt_2d_km_myr']) for r in u_5k])
    dr_3d = np.array([float(r['dr_dt_3d_km_myr']) for r in u_5k])
    dr_disp = np.array([float(r['dr_dt_disp_km_myr']) for r in u_5k])
    dr_geom = np.array([float(r['dr_dt_geom_km_myr']) for r in u_5k])

    alpha_u = float(u_5k[0]['alpha'])
    sqrt_alpha = np.sqrt(alpha_u)
    inv_sqrt_alpha = 1.0 / sqrt_alpha

    # Plot regime background shading
    ax1.axvspan(1e-4,
                sqrt_alpha,
                color='#e0f2fe',
                alpha=0.6,
                label='2D Shear-Dominated')
    ax1.axvspan(sqrt_alpha,
                1.0,
                color='#dcfce7',
                alpha=0.6,
                label='3D Shear-Dominated')
    ax1.axvspan(1.0,
                inv_sqrt_alpha,
                color='#fef3c7',
                alpha=0.6,
                label='Dispersion-Dominated (Focused)')
    ax1.axvspan(inv_sqrt_alpha,
                500.0,
                color='#fee2e2',
                alpha=0.6,
                label='Geometric (Unfocused)')

    # Plot model curve and individual regime asymptotics
    ax1.plot(thetas,
             dr_dt,
             'b-',
             lw=2.8,
             label=r'GLS04 Unified $\mathrm{d}R/\mathrm{d}t$')
    ax1.plot(thetas,
             dr_2d,
             'k:',
             lw=1.5,
             alpha=0.7,
             label=r'2D Shear Asymptote ($\propto \alpha^{-3/2}$)')
    ax1.plot(thetas,
             dr_3d,
             'g--',
             lw=1.8,
             alpha=0.85,
             label=r'3D Shear Asymptote ($\propto \theta^{-1}$)')
    ax1.plot(thetas,
             dr_disp,
             'r-.',
             lw=1.8,
             alpha=0.85,
             label=r'Dispersion Asymptote ($\propto \theta^{-2}$)')
    ax1.plot(thetas,
             dr_geom,
             'm--',
             lw=1.5,
             alpha=0.7,
             label=r'Geometric Asymptote ($\propto \theta^0$)')

    # Add vertical transition lines
    ax1.axvline(sqrt_alpha, color='teal', linestyle=':', lw=1.5)
    ax1.axvline(1.0, color='darkgreen', linestyle=':', lw=1.5)
    ax1.axvline(inv_sqrt_alpha, color='darkred', linestyle=':', lw=1.5)

    ax1.text(sqrt_alpha * 0.45,
             1e7,
             r'$\theta = \alpha^{1/2}$',
             color='teal',
             rotation=90,
             fontsize=9.5,
             fontweight='bold')
    ax1.text(0.75,
             1e7,
             r'$\theta = 1$',
             color='darkgreen',
             rotation=90,
             fontsize=9.5,
             fontweight='bold')
    ax1.text(inv_sqrt_alpha * 1.08,
             1e7,
             r'$\theta = \alpha^{-1/2}$',
             color='darkred',
             rotation=90,
             fontsize=9.5,
             fontweight='bold')

    ax1.set_xscale('log')
    ax1.set_yscale('log')
    ax1.set_xlim(1e-4, 400.0)
    ax1.set_ylim(1e-1, 5e8)
    ax1.set_xlabel(r'Dimensionless Velocity Dispersion $\theta \equiv u / v_H$',
                   fontsize=12)
    ax1.set_ylabel(r'Radial Growth Rate $\mathrm{d}R/\mathrm{d}t$ [km / Myr]',
                   fontsize=12)
    ax1.set_title(
        r'\textbf{(a) Uranus ($a = 19.2$ AU, $R = 5,000$ km): Growth Regimes}',
        fontsize=12)
    ax1.legend(loc='upper right', fontsize=8.5, framealpha=0.92)
    ax1.grid(True, which='both', linestyle=':', alpha=0.45)

    # ---------------- Right Panel: Comparison Across Embryo Sizes & Planets ----------------
    ax2 = fig.add_subplot(gs[1])

    colors = ['#1f77b4', '#ff7f0e', '#2ca02c']
    radii = [500.0, 5000.0, 25000.0]

    for r_km, col in zip(radii, colors):
        sub_u = [
            r for r in rows
            if abs(float(r['a_au']) -
                   19.2) < 0.1 and abs(float(r['R_km']) - r_km) < 1.0
        ]
        sub_n = [
            r for r in rows
            if abs(float(r['a_au']) -
                   30.1) < 0.1 and abs(float(r['R_km']) - r_km) < 1.0
        ]
        th_u = np.array([float(r['theta']) for r in sub_u])
        dr_u = np.array([float(r['dr_dt_km_myr']) for r in sub_u])
        th_n = np.array([float(r['theta']) for r in sub_n])
        dr_n = np.array([float(r['dr_dt_km_myr']) for r in sub_n])

        ax2.plot(th_u,
                 dr_u,
                 color=col,
                 linestyle='-',
                 lw=2.2,
                 label=f'Uranus $R={int(r_km):,}$ km')
        ax2.plot(th_n,
                 dr_n,
                 color=col,
                 linestyle='--',
                 lw=1.8,
                 alpha=0.85,
                 label=f'Neptune $R={int(r_km):,}$ km')

    ax2.set_xscale('log')
    ax2.set_yscale('log')
    ax2.set_xlim(1e-4, 400.0)
    ax2.set_ylim(1e-1, 5e8)
    ax2.set_xlabel(r'Dimensionless Velocity Dispersion $\theta \equiv u / v_H$',
                   fontsize=12)
    ax2.set_ylabel(r'Radial Growth Rate $\mathrm{d}R/\mathrm{d}t$ [km / Myr]',
                   fontsize=12)
    ax2.set_title(r'\textbf{(b) Uranus vs. Neptune Accretion Scaling}',
                  fontsize=12)
    ax2.legend(loc='upper right', fontsize=8.5, ncol=2, framealpha=0.92)
    ax2.grid(True, which='both', linestyle=':', alpha=0.45)

    fig.suptitle(
        r'\textbf{Goldreich, Lithwick, \& Sari (2004) Planetesimal Accretion Scaling: Shear vs. Dispersion Regimes}',
        fontsize=13,
        y=0.98)

    out_pdf = os.path.join(DIR, "fig_comparison.pdf")
    out_png = os.path.join(DIR, "fig_comparison.png")
    plt.savefig(out_pdf, dpi=300, bbox_inches='tight')
    plt.savefig(out_png, dpi=300, bbox_inches='tight')
    plt.close()
    print(f"✅ Generated {out_pdf} and {out_png}")


def plot_model_choices():
    """Figure 2: Timescale Crisis Resolution and Dynamic Trajectories."""
    rows_time = read_csv_dicts("timescale_vs_distance.csv")
    rows_evo = read_csv_dicts("uranus_neptune_evolution.csv")

    fig = plt.figure(figsize=(14, 6))
    gs = GridSpec(1, 2, width_ratios=[1.05, 1.0], wspace=0.28)

    # ---------------- Left Panel: Timescale vs Distance ----------------
    ax1 = fig.add_subplot(gs[0])

    a_vals = np.array([float(r['a_au']) for r in rows_time])
    tau_cold = np.array([float(r['tau_shear_cold_myr']) for r in rows_time])
    tau_eq = np.array([float(r['tau_shear_eq_myr']) for r in rows_time])
    tau_saf = np.array([float(r['tau_safronov_myr']) for r in rows_time])

    ax1.plot(a_vals,
             tau_cold,
             'b-',
             lw=2.6,
             label=r'GLS04 Shear Cold ($\theta = 0.1$)')
    ax1.plot(a_vals,
             tau_eq,
             'g--',
             lw=2.2,
             label=r'GLS04 Equilibrium Stirred ($\theta = 0.25$)')
    ax1.plot(a_vals,
             tau_saf,
             'r-.',
             lw=2.4,
             label=r'Safronov Classical Dispersion ($\theta = 3.0$)')

    # Mark protoplanetary gas disk lifetime (~3-10 Myr) and solar system age (4500 Myr)
    ax1.axhspan(3.0,
                10.0,
                color='#bbf7d0',
                alpha=0.5,
                label='Protoplanetary Disk Lifetime (3--10 Myr)')
    ax1.axhline(4500.0,
                color='gray',
                linestyle=':',
                lw=1.5,
                label='Solar System Age (4.5 Gyr)')

    # Find Uranus (19.2 AU) and Neptune (30.1 AU) rows
    u_idx = int(np.argmin(np.abs(a_vals - 19.2)))
    n_idx = int(np.argmin(np.abs(a_vals - 30.1)))

    ax1.scatter([a_vals[u_idx]], [tau_cold[u_idx]],
                color='blue',
                s=60,
                zorder=5)
    ax1.scatter([a_vals[u_idx]], [tau_saf[u_idx]], color='red', s=60, zorder=5)
    ax1.scatter([a_vals[n_idx]], [tau_cold[n_idx]],
                color='blue',
                s=60,
                zorder=5)
    ax1.scatter([a_vals[n_idx]], [tau_saf[n_idx]], color='red', s=60, zorder=5)

    ax1.annotate(f"Uranus Shear\n{tau_cold[u_idx]:.1f} Myr",
                 xy=(19.2, tau_cold[u_idx]),
                 xytext=(13.0, 1.5),
                 arrowprops=dict(arrowstyle="->", color='blue', lw=1.2),
                 fontsize=9,
                 fontweight='bold')
    ax1.annotate(
        f"Uranus Safronov\n{tau_saf[u_idx]:.0f} Myr ({tau_saf[u_idx]/1000:.2f} Gyr)",
        xy=(19.2, tau_saf[u_idx]),
        xytext=(11.0, 1500.0),
        arrowprops=dict(arrowstyle="->", color='red', lw=1.2),
        fontsize=9,
        fontweight='bold')

    ax1.annotate(f"Neptune Shear\n{tau_cold[n_idx]:.1f} Myr",
                 xy=(30.1, tau_cold[n_idx]),
                 xytext=(26.0, 3.5),
                 arrowprops=dict(arrowstyle="->", color='blue', lw=1.2),
                 fontsize=9,
                 fontweight='bold')
    ax1.annotate(
        f"Neptune Safronov\n{tau_saf[n_idx]:.0f} Myr ({tau_saf[n_idx]/1000:.2f} Gyr)",
        xy=(30.1, tau_saf[n_idx]),
        xytext=(25.0, 4000.0),
        arrowprops=dict(arrowstyle="->", color='red', lw=1.2),
        fontsize=9,
        fontweight='bold')

    ax1.set_yscale('log')
    ax1.set_xlim(1.0, 45.0)
    ax1.set_ylim(0.01, 1e5)
    ax1.set_xlabel(r'Semimajor Axis $a$ [AU]', fontsize=12)
    ax1.set_ylabel(
        r'Growth Timescale $\tau_{\mathrm{growth}}$ to $25,000$ km [Myr]',
        fontsize=12)
    ax1.set_title(r'\textbf{(a) Growth Timescale Crisis vs. Distance $a$}',
                  fontsize=12)
    ax1.legend(loc='upper left', fontsize=8.5, framealpha=0.92)
    ax1.grid(True, which='both', linestyle=':', alpha=0.45)

    # ---------------- Right Panel: Dynamic Evolution R(t) ----------------
    ax2 = fig.add_subplot(gs[1])

    def extract_evo(planet_name, m_type):
        sub = [
            r for r in rows_evo
            if r['planet'] == planet_name and r['model_type'] == m_type
        ]
        t = np.array([float(r['time_myr']) for r in sub])
        rad = np.array([float(r['radius_km']) for r in sub])
        return t, rad

    t_uc, r_uc = extract_evo('Uranus', 'Shear_Dominated_Cold')
    t_ue, r_ue = extract_evo('Uranus', 'Equilibrium_Stirring')
    t_us, r_us = extract_evo('Uranus', 'Safronov_Dispersion')

    t_nc, r_nc = extract_evo('Neptune', 'Shear_Dominated_Cold')
    t_ne, r_ne = extract_evo('Neptune', 'Equilibrium_Stirring')
    t_ns, r_ns = extract_evo('Neptune', 'Safronov_Dispersion')

    ax2.plot(t_uc,
             r_uc,
             'b-',
             lw=2.4,
             label=r'Uranus (Shear Cold $\theta=0.1$)')
    ax2.plot(t_ue, r_ue, 'b--', lw=2.0, label=r'Uranus (Equilibrium Stirred)')
    ax2.plot(t_us,
             r_us,
             'b:',
             lw=1.8,
             label=r'Uranus (Safronov Hot $\theta=3.0$)')

    ax2.plot(t_nc,
             r_nc,
             'purple',
             linestyle='-',
             lw=2.4,
             label=r'Neptune (Shear Cold $\theta=0.1$)')
    ax2.plot(t_ne,
             r_ne,
             'purple',
             linestyle='--',
             lw=2.0,
             label=r'Neptune (Equilibrium Stirred)')
    ax2.plot(t_ns,
             r_ns,
             'purple',
             linestyle=':',
             lw=1.8,
             label=r'Neptune (Safronov Hot $\theta=3.0$)')

    ax2.axhline(25362.0,
                color='navy',
                linestyle='-.',
                lw=1.2,
                alpha=0.7,
                label='Uranus Radius (25,362 km)')
    ax2.axhline(24622.0,
                color='purple',
                linestyle='-.',
                lw=1.2,
                alpha=0.7,
                label='Neptune Radius (24,622 km)')

    ax2.set_xlim(0.0, 50.0)
    ax2.set_ylim(0.0, 30000.0)
    ax2.set_xlabel(r'Evolutionary Time $t$ [Myr]', fontsize=12)
    ax2.set_ylabel(r'Protoplanet Radius $R(t)$ [km]', fontsize=12)
    ax2.set_title(r'\textbf{(b) Time-Dependent Growth Trajectories $R(t)$}',
                  fontsize=12)
    ax2.legend(loc='lower right', fontsize=8.5, framealpha=0.92)
    ax2.grid(True, linestyle=':', alpha=0.5)

    fig.suptitle(
        r'\textbf{Resolution of the Outer Planet Formation Timescale Problem (Goldreich et al. 2004)}',
        fontsize=13,
        y=0.98)

    out_pdf = os.path.join(DIR, "fig_model_choices.pdf")
    out_png = os.path.join(DIR, "fig_model_choices.png")
    plt.savefig(out_pdf, dpi=300, bbox_inches='tight')
    plt.savefig(out_png, dpi=300, bbox_inches='tight')
    plt.close()
    print(f"✅ Generated {out_pdf} and {out_png}")


def plot_diagram():
    """Figure 3: Physics Schematic Diagram of Planetesimal Accretion Regimes & Stirring Equilibrium."""
    _fig, ax = plt.subplots(figsize=(12, 7))
    ax.axis('off')
    ax.set_xlim(0, 12)
    ax.set_ylim(0, 7)

    # 1. Main Header Box
    title_box = patches.FancyBboxPatch((0.4, 6.15),
                                       11.2,
                                       0.7,
                                       boxstyle="round,pad=0.08",
                                       ec="#1e3a8a",
                                       fc="#dbeafe",
                                       lw=1.8)
    ax.add_patch(title_box)
    ax.text(
        6.0,
        6.5,
        "Goldreich, Lithwick, & Sari (2004) Planetesimal Coagulation Physics Architecture",
        ha='center',
        va='center',
        fontsize=12.5,
        fontweight='bold',
        color="#1e3a8a")

    # 2. Left Box: Orbital Geometry & Hill Sphere
    box_left = patches.FancyBboxPatch((0.4, 0.5),
                                      3.5,
                                      5.4,
                                      boxstyle="round,pad=0.1",
                                      ec="#0369a1",
                                      fc="#f0f9ff",
                                      lw=1.5)
    ax.add_patch(box_left)
    ax.text(2.15,
            5.6,
            "1. Hill Sphere & Shear Flow",
            ha='center',
            va='center',
            fontsize=11,
            fontweight='bold',
            color="#0369a1")

    # Draw schematic planet and Hill sphere
    circle_hill = patches.Circle((2.15, 3.8),
                                 1.1,
                                 ec="#0284c7",
                                 fc="#e0f2fe",
                                 ls="--",
                                 lw=1.5)
    circle_planet = patches.Circle((2.15, 3.8),
                                   0.28,
                                   ec="#0369a1",
                                   fc="#0284c7")
    ax.add_patch(circle_hill)
    ax.add_patch(circle_planet)
    ax.text(2.15,
            3.8,
            "$M, R$",
            ha='center',
            va='center',
            color='white',
            fontsize=9,
            fontweight='bold')
    ax.text(2.15,
            5.05,
            r"Hill Radius: $R_H = a (M / 3 M_*)^{1/3}$",
            ha='center',
            va='center',
            fontsize=8.5)
    ax.text(2.15,
            4.75,
            r"Hill Speed: $v_H = \Omega R_H$",
            ha='center',
            va='center',
            fontsize=8.5)

    # Draw Keplerian shear flow arrows
    ax.annotate('',
                xy=(3.4, 4.4),
                xytext=(0.9, 4.4),
                arrowprops=dict(arrowstyle="->", color='#0369a1', lw=1.5))
    ax.annotate('',
                xy=(0.9, 3.2),
                xytext=(3.4, 3.2),
                arrowprops=dict(arrowstyle="->", color='#0369a1', lw=1.5))
    ax.text(2.15,
            2.45,
            r"Keplerian Shear: $\Delta v_K \sim \Omega x$",
            ha='center',
            va='center',
            fontsize=8.5,
            fontweight='bold')
    ax.text(2.15,
            2.05,
            r"Geometric Ratio: $\alpha \equiv R / R_H \ll 1$",
            ha='center',
            va='center',
            fontsize=8.5)
    ax.text(2.15,
            1.65,
            r"Velocity Dispersion: $\theta \equiv u / v_H$",
            ha='center',
            va='center',
            fontsize=8.5)
    ax.text(2.15,
            1.25,
            r"Scale Height: $H \sim u / \Omega \approx \theta R_H$",
            ha='center',
            va='center',
            fontsize=8.5)
    ax.text(
        2.15,
        0.85,
        r"Escape Speed: $v_{\mathrm{esc}} \approx \sqrt{6} \alpha^{-1/2} v_H$",
        ha='center',
        va='center',
        fontsize=8.5)

    # 3. Center Box: Four Dynamic Accretion Regimes
    box_center = patches.FancyBboxPatch((4.2, 0.5),
                                        4.2,
                                        5.4,
                                        boxstyle="round,pad=0.1",
                                        ec="#15803d",
                                        fc="#f0fdf4",
                                        lw=1.5)
    ax.add_patch(box_center)
    ax.text(6.3,
            5.6,
            "2. Coagulation Regimes & Growth",
            ha='center',
            va='center',
            fontsize=11,
            fontweight='bold',
            color="#15803d")

    # Sub-boxes for regimes
    # Regime 1
    r1 = patches.FancyBboxPatch((4.4, 4.25),
                                3.8,
                                1.05,
                                boxstyle="round,pad=0.06",
                                ec="#0284c7",
                                fc="#e0f2fe",
                                lw=1.2)
    ax.add_patch(r1)
    ax.text(6.3,
            5.05,
            r"2D Shear-Dominated ($\theta \leq \alpha^{1/2}$)",
            ha='center',
            va='center',
            fontsize=8.5,
            fontweight='bold',
            color="#0369a1")
    ax.text(
        6.3,
        4.65,
        r"$\mathrm{d}R/\mathrm{d}t \sim \frac{\Sigma \Omega}{\rho} \alpha^{-3/2}$ (Ultra-cold thin disk)",
        ha='center',
        va='center',
        fontsize=8.2)

    # Regime 2
    r2 = patches.FancyBboxPatch((4.4, 3.0),
                                3.8,
                                1.05,
                                boxstyle="round,pad=0.06",
                                ec="#16a34a",
                                fc="#dcfce7",
                                lw=1.2)
    ax.add_patch(r2)
    ax.text(6.3,
            3.8,
            r"3D Shear-Dominated ($\alpha^{1/2} < \theta \leq 1$)",
            ha='center',
            va='center',
            fontsize=8.5,
            fontweight='bold',
            color="#15803d")
    ax.text(
        6.3,
        3.4,
        r"$\mathrm{d}R/\mathrm{d}t \sim \frac{\Sigma \Omega}{\rho} \alpha^{-1} \theta^{-1}$ (3-body shear boost)",
        ha='center',
        va='center',
        fontsize=8.2)

    # Regime 3
    r3 = patches.FancyBboxPatch((4.4, 1.75),
                                3.8,
                                1.05,
                                boxstyle="round,pad=0.06",
                                ec="#d97706",
                                fc="#fef3c7",
                                lw=1.2)
    ax.add_patch(r3)
    ax.text(6.3,
            2.55,
            r"Dispersion Focused ($1 < \theta \leq \alpha^{-1/2}$)",
            ha='center',
            va='center',
            fontsize=8.5,
            fontweight='bold',
            color="#b45309")
    ax.text(
        6.3,
        2.15,
        r"$\mathrm{d}R/\mathrm{d}t \sim \frac{\Sigma \Omega}{\rho} \alpha^{-1} \theta^{-2}$ (2-body Safronov)",
        ha='center',
        va='center',
        fontsize=8.2)

    # Regime 4
    r4 = patches.FancyBboxPatch((4.4, 0.7),
                                3.8,
                                0.85,
                                boxstyle="round,pad=0.06",
                                ec="#dc2626",
                                fc="#fee2e2",
                                lw=1.2)
    ax.add_patch(r4)
    ax.text(6.3,
            1.35,
            r"\textbf{Geometric Unfocused} ($\theta > \alpha^{-1/2}$)",
            ha='center',
            va='center',
            fontsize=8.5,
            color="#b91c1c")
    ax.text(
        6.3,
        0.98,
        r"$\mathrm{d}R/\mathrm{d}t \sim \frac{\Sigma \Omega}{\rho}$ (Direct physical collisions)",
        ha='center',
        va='center',
        fontsize=8.2)

    # 4. Right Box: Stirring vs Damping Equilibrium & Uranus/Neptune
    box_right = patches.FancyBboxPatch((8.7, 0.5),
                                       2.9,
                                       5.4,
                                       boxstyle="round,pad=0.1",
                                       ec="#7c3aed",
                                       fc="#faf5ff",
                                       lw=1.5)
    ax.add_patch(box_right)
    ax.text(10.15,
            5.6,
            "3. Velocity Equilibrium",
            ha='center',
            va='center',
            fontsize=11,
            fontweight='bold',
            color="#7c3aed")

    ax.text(10.15,
            5.0,
            r"\textbf{Viscous Stirring}:",
            ha='center',
            va='center',
            fontsize=8.5,
            color="#6d28d9")
    ax.text(
        10.15,
        4.65,
        r"$\left(\frac{\mathrm{d}u^2}{\mathrm{d}t}\right)_{\mathrm{stir}} \sim \frac{\Sigma_{\mathrm{olig}}}{\Sigma} \Omega v_H^2$",
        ha='center',
        va='center',
        fontsize=8.2)

    ax.text(10.15,
            4.1,
            r"\textbf{Collisional Damping}:",
            ha='center',
            va='center',
            fontsize=8.5,
            color="#6d28d9")
    ax.text(
        10.15,
        3.75,
        r"$\left(\frac{\mathrm{d}u^2}{\mathrm{d}t}\right)_{\mathrm{damp}} \sim -\frac{\Sigma \Omega}{\rho r} u^2$",
        ha='center',
        va='center',
        fontsize=8.2)

    ax.text(10.15,
            3.2,
            r"\textbf{Equilibrium} $\theta_{\mathrm{eq}}$:",
            ha='center',
            va='center',
            fontsize=8.5,
            color="#6d28d9")
    ax.text(
        10.15,
        2.85,
        r"$\theta_{\mathrm{eq}} \sim \left(\frac{\Sigma_{\mathrm{olig}} r}{\Sigma R \alpha}\right)^{1/4} \sim 0.1-0.3$",
        ha='center',
        va='center',
        fontsize=8.0)

    # Uranus & Neptune summary box
    res_box = patches.FancyBboxPatch((8.85, 0.7),
                                     2.6,
                                     1.7,
                                     boxstyle="round,pad=0.06",
                                     ec="#4338ca",
                                     fc="#e0e7ff",
                                     lw=1.2)
    ax.add_patch(res_box)
    ax.text(10.15,
            2.15,
            r"\textbf{Outer Planet Timescales}",
            ha='center',
            va='center',
            fontsize=8.5,
            color="#3730a3")
    ax.text(10.15,
            1.75,
            r"$\bullet$ Uranus (19.2 AU): $\mathbf{9.8}$ Myr",
            ha='center',
            va='center',
            fontsize=8.0)
    ax.text(10.15,
            1.45,
            r"  (vs. Safronov: $587$ Myr)",
            ha='center',
            va='center',
            fontsize=7.5,
            color='#4b5563')
    ax.text(10.15,
            1.15,
            r"$\bullet$ Neptune (30.1 AU): $\mathbf{23.3}$ Myr",
            ha='center',
            va='center',
            fontsize=8.0)
    ax.text(10.15,
            0.85,
            r"  (vs. Safronov: $1.4$ Gyr)",
            ha='center',
            va='center',
            fontsize=7.5,
            color='#4b5563')

    out_pdf = os.path.join(DIR, "fig_diagram.pdf")
    out_png = os.path.join(DIR, "fig_diagram.png")
    plt.savefig(out_pdf, dpi=300, bbox_inches='tight')
    plt.savefig(out_png, dpi=300, bbox_inches='tight')
    plt.close()
    print(f"✅ Generated {out_pdf} and {out_png}")


if __name__ == "__main__":
    plot_comparison()
    plot_model_choices()
    plot_diagram()
    print("🎉 All 3 publication figures generated successfully!")
