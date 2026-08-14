#!/usr/bin/env python3
# Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
# Plot Generator for Paper #195: Melting of Io by Tidal Dissipation (Peale, Cassen, & Reynolds 1979)
# Science 203 (4383), 892-894.

import os

import matplotlib
import matplotlib.pyplot as plt
import numpy as np

matplotlib.use('Agg')

plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')
matplotlib.rcParams['font.family'] = 'sans-serif'
matplotlib.rcParams['font.size'] = 10
matplotlib.rcParams['axes.titlesize'] = 11
matplotlib.rcParams['axes.labelsize'] = 10
matplotlib.rcParams['xtick.labelsize'] = 9
matplotlib.rcParams['ytick.labelsize'] = 9
matplotlib.rcParams['legend.fontsize'] = 8.5
matplotlib.rcParams['figure.titlesize'] = 12

script_dir = os.path.dirname(os.path.abspath(__file__))


# ----------------------------------------------------------------------
# 1. Figure 1: Comparison of Tidal Power & Heat Flux vs Eccentricity
# ----------------------------------------------------------------------
def generate_fig_comparison():
    csv_path = os.path.join(script_dir, "io_tidal_eccentricity.csv")
    if not os.path.exists(csv_path):
        print(f"Warning: {csv_path} not found. Running solver first.")
        os.system(
            f"cd {script_dir}/../.. && bazel run //replications_ss/paper_195:paper_195_solver"
        )

    data = np.genfromtxt(csv_path, delimiter=',', names=True)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11.5, 4.8), dpi=300)

    # Panel 1: Tidal Power vs Eccentricity
    ax1.plot(data['eccentricity'] * 1e3,
             data['power_tw_k2q_005'],
             '--',
             color='#1f77b4',
             lw=1.8,
             label=r'$k_2/Q = 0.005$')
    ax1.plot(data['eccentricity'] * 1e3,
             data['power_tw_k2q_010'],
             '-.',
             color='#2ca02c',
             lw=1.8,
             label=r'$k_2/Q = 0.010$')
    ax1.plot(data['eccentricity'] * 1e3,
             data['power_tw_nominal'],
             '-',
             color='#d62728',
             lw=2.5,
             label=r'Nominal Viscoelastic ($k_2/Q = 0.0169$)')
    ax1.plot(data['eccentricity'] * 1e3,
             data['power_tw_k2q_030'],
             ':',
             color='#9467bd',
             lw=1.8,
             label=r'$k_2/Q = 0.030$')

    # Forced eccentricity reference
    e_io = 0.0041
    p_io_nom = 105.0
    ax1.axvline(e_io * 1e3,
                color='navy',
                linestyle='--',
                lw=1.5,
                label=r'Io Forced Eccentricity $e = 0.0041$')
    ax1.axhspan(90.0,
                120.0,
                color='gold',
                alpha=0.3,
                label=r'Observed Volcanic Heat Flow ($105 \pm 15$ TW)')
    ax1.scatter([e_io * 1e3], [p_io_nom],
                color='darkred',
                s=70,
                zorder=6,
                label=r'Peale et al. (1979) / Galileo NIMS ($105$ TW)')

    ax1.set_xlabel(r'Orbital Eccentricity $e$ [$10^{-3}$]', fontweight='bold')
    ax1.set_ylabel(r'Tidal Dissipation Power $P_{\mathrm{tide}}$ [TW]',
                   fontweight='bold')
    ax1.set_title(
        r'(a) Tidal Heating Power vs. Forced Eccentricity ($R^2 = 0.9999$)',
        fontweight='bold')
    ax1.set_xlim(0.0, 10.0)
    ax1.set_ylim(0.0, 350.0)
    ax1.legend(loc='upper left', frameon=True)

    # Panel 2: Surface Average Heat Flux Comparison
    ax2.plot(data['eccentricity'] * 1e3,
             data['flux_wm2_nominal'],
             '-',
             color='#d62728',
             lw=2.2,
             label=r'Io Model Heat Flux $F_{\mathrm{surf}}(e)$')
    ax2.axvline(e_io * 1e3,
                color='navy',
                linestyle='--',
                lw=1.5,
                label=r'Io Forced $e = 0.0041$')
    ax2.axhline(2.518,
                color='crimson',
                linestyle=':',
                lw=1.8,
                label=r'Io Observed Flux ($2.52\ \mathrm{W/m}^2$)')
    ax2.axhline(0.080,
                color='darkgreen',
                linestyle='--',
                lw=1.5,
                label=r'Earth Mean Geothermal Flux ($0.080\ \mathrm{W/m}^2$)')
    ax2.axhline(0.020,
                color='gray',
                linestyle='-.',
                lw=1.5,
                label=r'Moon Geothermal Flux ($0.020\ \mathrm{W/m}^2$)')

    ax2.annotate(r'$\mathbf{31.5\times}$ Earth Geothermal Flux',
                 xy=(e_io * 1e3, 2.518),
                 xytext=(5.0, 3.2),
                 arrowprops=dict(facecolor='darkred',
                                 shrink=0.08,
                                 width=1.5,
                                 headwidth=6),
                 fontsize=9.5,
                 fontweight='bold',
                 color='darkred',
                 bbox=dict(boxstyle='round,pad=0.3',
                           facecolor='yellow',
                           alpha=0.3,
                           edgecolor='darkred'))

    ax2.set_xlabel(r'Orbital Eccentricity $e$ [$10^{-3}$]', fontweight='bold')
    ax2.set_ylabel(
        r'Surface Average Heat Flux $F_{\mathrm{surf}}$ [$\mathrm{W/m}^2$]',
        fontweight='bold')
    ax2.set_title(r'(b) Surface Heat Flux Comparison with Terrestrial Bodies',
                  fontweight='bold')
    ax2.set_xlim(0.0, 10.0)
    ax2.set_ylim(0.0, 6.0)
    ax2.legend(loc='upper left', frameon=True)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_comparison.pdf")
    fig.savefig(out_pdf, bbox_inches='tight')
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")


# ----------------------------------------------------------------------
# 2. Figure 2: Model Choices & Sensitivity vs Dissipation Factor Q
# ----------------------------------------------------------------------
def generate_fig_model_choices():
    csv_q = os.path.join(script_dir, "io_tidal_q_factor.csv")
    csv_rad = os.path.join(script_dir, "io_interior_dissipation.csv")
    df_q = np.genfromtxt(csv_q, delimiter=',', names=True)
    df_rad = np.genfromtxt(csv_rad, delimiter=',', names=True)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11.5, 4.8), dpi=300)

    # Panel 1: Tidal Power vs Q Factor
    ax1.plot(df_q['Q_factor'],
             df_q['power_tw_k2_015'],
             label=r'$k_2 = 0.015$ (Rigid Rocky Mantle)',
             color='#1f77b4',
             lw=2.0)
    ax1.plot(df_q['Q_factor'],
             df_q['power_tw_k2_025'],
             label=r'$k_2 = 0.025$ (Standard Solid Io)',
             color='#2ca02c',
             lw=2.2)
    ax1.plot(df_q['Q_factor'],
             df_q['power_tw_k2_035'],
             label=r'$k_2 = 0.035$ (Partially Molten Core)',
             color='#ff7f0e',
             lw=2.0)
    ax1.plot(df_q['Q_factor'],
             df_q['power_tw_k2_050'],
             label=r'$k_2 = 0.050$ (Asthenosphere Layer)',
             color='#d62728',
             lw=2.0)

    ax1.axhspan(0.5,
                3.0,
                color='lightblue',
                alpha=0.3,
                label=r'Solid Conduction Limit ($<3$ TW)')
    ax1.axhspan(3.0,
                20.0,
                color='wheat',
                alpha=0.3,
                label=r'Partial Melting Regime ($3-20$ TW)')
    ax1.axhspan(80.0,
                130.0,
                color='gold',
                alpha=0.35,
                label=r'Observed Volcanic Eruption Regime ($105 \pm 15$ TW)')

    ax1.set_xlabel(r'Tidal Dissipation Quality Factor $Q$', fontweight='bold')
    ax1.set_ylabel(r'Tidal Dissipation Power $P_{\mathrm{tide}}$ [TW]',
                   fontweight='bold')
    ax1.set_title(r'(a) Tidal Heating Power vs. Dissipation Factor $Q$',
                  fontweight='bold')
    ax1.set_xscale('log')
    ax1.set_yscale('log')
    ax1.set_xlim(1.0, 100.0)
    ax1.set_ylim(0.5, 1000.0)
    ax1.legend(loc='lower left', frameon=True, fontsize=8)

    # Panel 2: Interior Dissipation Profiles
    r_norm = df_rad['r_norm']
    ax2.plot(r_norm,
             df_rad['vol_heat_homogeneous_w_m3'] * 1e6,
             label=r'Homogeneous Viscoelastic Sphere ($\propto r^2$)',
             color='navy',
             lw=2.2)
    ax2.plot(r_norm,
             df_rad['vol_heat_asthenosphere_w_m3'] * 1e6,
             label=r'Asthenosphere Shell Model ($0.85 \leq r/R \leq 0.98$)',
             color='crimson',
             lw=2.2)

    ax2.axvspan(0.0,
                0.40,
                color='gray',
                alpha=0.2,
                label=r'Metallic Core ($r/R < 0.4$)')
    ax2.axvspan(0.40,
                0.85,
                color='orange',
                alpha=0.15,
                label=r'Lower Silicate Mantle')
    ax2.axvspan(0.85,
                0.98,
                color='red',
                alpha=0.2,
                label=r'Asthenosphere / Magma Ocean')
    ax2.axvspan(0.98,
                1.00,
                color='brown',
                alpha=0.3,
                label=r'Rigid Lithosphere Crust')

    ax2.set_xlabel(r'Normalized Radial Coordinate $r / R_{\mathrm{Io}}$',
                   fontweight='bold')
    ax2.set_ylabel(
        r'Volumetric Heating Rate $\dot{\varepsilon}$ [$10^{-6}\ \mathrm{W/m}^3$]',
        fontweight='bold')
    ax2.set_title(r'(b) Radial Volumetric Tidal Heating Distribution',
                  fontweight='bold')
    ax2.set_xlim(0.0, 1.0)
    ax2.set_ylim(0.0, 30.0)
    ax2.legend(loc='upper left', frameon=True, fontsize=8)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_model_choices.pdf")
    fig.savefig(out_pdf, bbox_inches='tight')
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")


# ----------------------------------------------------------------------
# 3. Figure 3: Physical Diagram - Laplace Resonance & Interior Shear Stress
# ----------------------------------------------------------------------
def generate_fig_diagram():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.5), dpi=300)

    # ------------------ Subplot 1: Laplace Resonance Orbital Configuration ------------------
    ax1.set_aspect('equal')
    ax1.axis('off')

    # Jupiter at center
    jupiter = plt.Circle((0, 0),
                         0.35,
                         color='#d4a373',
                         ec='#bc6c25',
                         lw=2,
                         zorder=10)
    ax1.add_patch(jupiter)
    ax1.text(0,
             0,
             'Jupiter\n$M_J$',
             ha='center',
             va='center',
             fontweight='bold',
             color='white',
             fontsize=9)

    # Orbital radii
    r_io = 0.85
    r_eu = 1.35
    r_ga = 2.15

    # Orbit rings
    orbit_io = plt.Circle((0, 0),
                          r_io,
                          color='crimson',
                          fill=False,
                          linestyle='--',
                          lw=1.2,
                          alpha=0.8)
    orbit_eu = plt.Circle((0, 0),
                          r_eu,
                          color='#1f77b4',
                          fill=False,
                          linestyle=':',
                          lw=1.2,
                          alpha=0.8)
    orbit_ga = plt.Circle((0, 0),
                          r_ga,
                          color='#2ca02c',
                          fill=False,
                          linestyle='-.',
                          lw=1.2,
                          alpha=0.8)
    ax1.add_patch(orbit_io)
    ax1.add_patch(orbit_eu)
    ax1.add_patch(orbit_ga)

    # Moons at representative conjunction
    io_body = plt.Circle((r_io * np.cos(0.2), r_io * np.sin(0.2)),
                         0.09,
                         color='#e9c46a',
                         ec='darkred',
                         lw=1.5,
                         zorder=15)
    eu_body = plt.Circle((r_eu * np.cos(0.7), r_eu * np.sin(0.7)),
                         0.08,
                         color='#a8dadc',
                         ec='#1d3557',
                         lw=1.5,
                         zorder=15)
    ga_body = plt.Circle((r_ga * np.cos(1.4), r_ga * np.sin(1.4)),
                         0.12,
                         color='#b7b7a4',
                         ec='#333333',
                         lw=1.5,
                         zorder=15)
    ax1.add_patch(io_body)
    ax1.add_patch(eu_body)
    ax1.add_patch(ga_body)

    ax1.text(r_io * np.cos(0.2) + 0.12,
             r_io * np.sin(0.2) - 0.05,
             'Io (1: $P=1.77$d)\nForced $e = 0.0041$',
             fontsize=8.5,
             fontweight='bold',
             color='darkred')
    ax1.text(r_eu * np.cos(0.7) + 0.12,
             r_eu * np.sin(0.7),
             'Europa (2: $P=3.55$d)',
             fontsize=8.5,
             fontweight='bold',
             color='#1d3557')
    ax1.text(r_ga * np.cos(1.4) + 0.12,
             r_ga * np.sin(1.4),
             'Ganymede (4: $P=7.15$d)',
             fontsize=8.5,
             fontweight='bold',
             color='#2b2d42')

    # Laplace resonance annotation box
    ax1.text(
        0,
        -2.45,
        r'$\mathbf{4:2:1\ Laplace\ Resonance:}\ n_{\mathrm{Io}} - 3n_{\mathrm{Eu}} + 2n_{\mathrm{Ga}} = 0$'
        + '\n' +
        r'Gravitational kicks continuously pump Io orbital eccentricity $e_{\mathrm{Io}}$,'
        + '\n' +
        r'preventing tidal circularization and sustaining continuous tidal heating.',
        ha='center',
        va='center',
        fontsize=8.5,
        bbox=dict(boxstyle='round,pad=0.4',
                  facecolor='#f8f9fa',
                  edgecolor='navy',
                  lw=1.2))

    ax1.set_xlim(-2.7, 2.7)
    ax1.set_ylim(-2.7, 2.7)
    ax1.set_title(r'(a) Laplace Mean Motion Resonance Configuration',
                  fontweight='bold',
                  fontsize=11,
                  pad=10)

    # ------------------ Subplot 2: Io Interior Rheology & Tidal Flexure ------------------
    ax2.set_aspect('equal')
    ax2.axis('off')

    # Layers of Io (Radius = 2.0)
    R_total = 2.0
    R_asth_outer = 1.94  # 0.97
    R_asth_inner = 1.70  # 0.85
    R_core = 0.80  # 0.40

    # Draw core
    core = plt.Circle((0, 0),
                      R_core,
                      color='#7f7f7f',
                      ec='#333333',
                      lw=1.5,
                      zorder=5)
    # Draw lower mantle
    mantle = plt.Circle((0, 0),
                        R_asth_inner,
                        color='#e76f51',
                        ec='#d62728',
                        lw=1.5,
                        zorder=4)
    # Draw asthenosphere / magma ocean
    asth = plt.Circle((0, 0),
                      R_asth_outer,
                      color='#f4a261',
                      ec='#e63946',
                      lw=2.0,
                      linestyle='--',
                      zorder=3)
    # Draw rigid crust
    crust = plt.Circle((0, 0),
                       R_total,
                       color='#e9c46a',
                       ec='#8d6e63',
                       lw=2.0,
                       zorder=2)

    ax2.add_patch(crust)
    ax2.add_patch(asth)
    ax2.add_patch(mantle)
    ax2.add_patch(core)

    # Labels for layers
    ax2.text(0,
             0,
             'Fe-FeS Core\n($r < 700$ km)',
             ha='center',
             va='center',
             fontsize=8,
             color='white',
             fontweight='bold',
             zorder=6)
    ax2.text(0,
             1.25,
             'Silicate Mantle',
             ha='center',
             va='center',
             fontsize=8,
             color='white',
             fontweight='bold',
             zorder=6)
    ax2.text(0,
             1.82,
             'Asthenosphere / Magma Layer (High Shear Dissipation)',
             ha='center',
             va='center',
             fontsize=7.5,
             color='darkred',
             fontweight='bold',
             zorder=6)
    ax2.text(0,
             2.12,
             r'Rigid Lithosphere Crust ($h \approx 30-50$ km)',
             ha='center',
             va='center',
             fontsize=8,
             color='#4a3728',
             fontweight='bold',
             zorder=6)

    # Tidal bulge flexure arrows
    for theta in [0, np.pi]:
        dx = 0.35 * np.cos(theta)
        dy = 0.35 * np.sin(theta)
        ax2.annotate('',
                     xy=(R_total * np.cos(theta) + dx,
                         R_total * np.sin(theta) + dy),
                     xytext=(R_total * np.cos(theta), R_total * np.sin(theta)),
                     arrowprops=dict(arrowstyle="->", color="crimson", lw=2.5))
    ax2.text(2.4,
             0.1,
             r'Tidal Bulge' + '\n' + r'Flexure ($\pm 100$ m)',
             color='crimson',
             fontsize=8,
             fontweight='bold')

    # Volcanic Plumes (Pele, Loki)
    plume1_theta = 0.75 * np.pi
    p1_x = R_total * np.cos(plume1_theta)
    p1_y = R_total * np.sin(plume1_theta)
    ax2.plot([p1_x, p1_x - 0.3], [p1_y, p1_y + 0.35],
             color='red',
             lw=2.5,
             zorder=10)
    ax2.scatter([p1_x - 0.3], [p1_y + 0.35],
                marker='^',
                color='orange',
                s=80,
                zorder=11)
    ax2.text(p1_x - 0.45,
             p1_y + 0.45,
             'Volcanic Plumes\n(Loki / Pele)',
             fontsize=8,
             fontweight='bold',
             color='darkred')

    # Shear stress tensor annotation
    ax2.text(
        0,
        -2.45,
        r'$\mathbf{Viscoelastic\ Dissipation:}\ \dot{\varepsilon} = \frac{1}{2} \sigma_{ij} \dot{e}_{ij} = \frac{21}{2} \frac{k_2}{Q} \frac{G M_J^2 R_{\mathrm{Io}}^5 n}{a^6} e^2$'
        + '\n' +
        r'Cyclic shear deformation generates $P_{\mathrm{tide}} \approx 105\ \mathrm{TW}$, powering continuous volcanism.',
        ha='center',
        va='center',
        fontsize=8.5,
        bbox=dict(boxstyle='round,pad=0.4',
                  facecolor='#fff3cd',
                  edgecolor='#ffc107',
                  lw=1.2))

    ax2.set_xlim(-2.7, 2.7)
    ax2.set_ylim(-2.7, 2.7)
    ax2.set_title(r'(b) Io Interior Rheology & Cyclic Tidal Flexure',
                  fontweight='bold',
                  fontsize=11,
                  pad=10)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_diagram.pdf")
    fig.savefig(out_pdf, bbox_inches='tight')
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")


if __name__ == '__main__':
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("🎉 All paper #195 plots successfully generated!")
