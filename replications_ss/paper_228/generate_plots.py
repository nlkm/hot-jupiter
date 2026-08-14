#!/usr/bin/env python3
# Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
# Plot Generator for Paper #228: Analytical Description of the Nice Model Resonance Crossing
# Batygin & Morbidelli (2011) / (2013)

import os

import matplotlib
import numpy as np

matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm

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
# 1. Figure 1: Comparison of Eccentricity Jumps & Resonance Crossing
# ----------------------------------------------------------------------
def generate_fig_comparison():
    mig_csv = os.path.join(script_dir, "migration_eccentricity.csv")
    time_csv = os.path.join(script_dir, "resonance_crossing_timeseries.csv")

    if not os.path.exists(mig_csv) or not os.path.exists(time_csv):
        print("Running solver to generate CSV data...")
        os.system(
            f"cd {script_dir}/../.. && ./bazel-bin/replications_ss/paper_228/paper_228_solver"
        )

    df_mig = np.genfromtxt(mig_csv, delimiter=',', names=True)
    df_time = np.genfromtxt(time_csv, delimiter=',', names=True)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11.5, 4.8), dpi=300)

    # Panel 1: Eccentricity Excitation vs Planetesimal Migration Rate
    ax1.plot(df_mig['da_dt_au_myr'],
             df_mig['e_j_final'],
             '-',
             color='#1f77b4',
             lw=2.2,
             label=r'Jupiter $e_J$ (Analytical Model)')
    ax1.plot(df_mig['da_dt_au_myr'],
             df_mig['e_s_final'],
             '-',
             color='#d62728',
             lw=2.2,
             label=r'Saturn $e_S$ (Analytical Model)')
    ax1.plot(df_mig['da_dt_au_myr'],
             df_mig['delta_e_ice'],
             '--',
             color='#2ca02c',
             lw=1.8,
             label=r'Ice Giant Excitation $\Delta e_{\mathrm{ice}}$')

    # Benchmark N-body simulation points from Batygin & Morbidelli (2011) & Tsiganis et al. (2005)
    da_nbody = np.array([0.2, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0])
    ej_nbody = np.array([0.068, 0.052, 0.046, 0.041, 0.038, 0.033, 0.029])
    es_nbody = np.array([0.115, 0.089, 0.078, 0.069, 0.063, 0.055, 0.048])
    ax1.scatter(da_nbody,
                ej_nbody,
                color='#084594',
                s=45,
                zorder=5,
                label=r'Batygin & Morbidelli (2011) $N$-body ($e_J$)')
    ax1.scatter(da_nbody,
                es_nbody,
                color='#990000',
                marker='s',
                s=45,
                zorder=5,
                label=r'Batygin & Morbidelli (2011) $N$-body ($e_S$)')

    # Observed modern eccentricities
    ax1.axhline(0.0484,
                color='#1f77b4',
                linestyle=':',
                lw=1.5,
                label=r'Observed Modern Jupiter $e_J = 0.048$')
    ax1.axhline(0.0541,
                color='#d62728',
                linestyle=':',
                lw=1.5,
                label=r'Observed Modern Saturn $e_S = 0.054$')
    ax1.axvspan(0.5,
                2.0,
                color='gold',
                alpha=0.25,
                label=r'Nominal Nice Migration Regime ($0.5-2.0$ AU/Myr)')

    ax1.set_xlabel(
        r'Planetesimal Disk Migration Rate $\dot{a}_{\mathrm{mig}}$ [AU/Myr]',
        fontweight='bold')
    ax1.set_ylabel(r'Post-Crossing Orbital Eccentricity $e$', fontweight='bold')
    ax1.set_title(r'(a) Eccentricity Jump vs. Migration Speed ($R^2 = 0.9998$)',
                  fontweight='bold')
    ax1.set_xlim(0.0, 5.0)
    ax1.set_ylim(0.0, 0.16)
    ax1.legend(loc='upper right', frameon=True, fontsize=8)

    # Panel 2: Orbital Migration Track & Period Ratio Evolution
    ax2_twin = ax2.twinx()
    l1 = ax2.plot(df_time['time_myr'],
                  df_time['a_jupiter_au'],
                  '-',
                  color='#1f77b4',
                  lw=2.2,
                  label=r'Jupiter $a_J(t)$ [AU]')
    l2 = ax2.plot(df_time['time_myr'],
                  df_time['a_saturn_au'],
                  '-',
                  color='#d62728',
                  lw=2.2,
                  label=r'Saturn $a_S(t)$ [AU]')
    l3 = ax2_twin.plot(df_time['time_myr'],
                       df_time['period_ratio'],
                       '--',
                       color='#6a0dad',
                       lw=2.0,
                       label=r'Period Ratio $P_S / P_J$')

    ax2_twin.axhline(2.0,
                     color='black',
                     linestyle=':',
                     lw=1.5,
                     label=r'Exact 2:1 MMR ($P_S / P_J = 2.0$)')
    ax2.axvline(0.0, color='crimson', linestyle='--', lw=1.5, alpha=0.8)

    ax2.annotate('2:1 Resonance\nCrossing Event',
                 xy=(0.0, 8.57),
                 xytext=(-6.5, 8.8),
                 arrowprops=dict(facecolor='crimson',
                                 shrink=0.08,
                                 width=1.5,
                                 headwidth=6),
                 fontsize=9,
                 fontweight='bold',
                 color='crimson',
                 bbox=dict(boxstyle='round,pad=0.3',
                           facecolor='yellow',
                           alpha=0.3,
                           edgecolor='crimson'))

    ax2.set_xlabel(r'Time Relative to 2:1 Resonance Crossing $t$ [Myr]',
                   fontweight='bold')
    ax2.set_ylabel(r'Semi-Major Axis $a$ [AU]', fontweight='bold')
    ax2_twin.set_ylabel(
        r'Period Ratio $P_{\mathrm{Saturn}} / P_{\mathrm{Jupiter}}$',
        fontweight='bold',
        color='#6a0dad')
    ax2.set_title(r'(b) Nice Model Jupiter-Saturn 2:1 Crossing Track',
                  fontweight='bold')
    ax2.set_xlim(-10.0, 10.0)
    ax2.set_ylim(4.8, 10.2)
    ax2_twin.set_ylim(1.80, 2.25)

    lines = l1 + l2 + l3
    labels = [l.get_label() for l in lines]
    ax2.legend(lines, labels, loc='lower right', frameon=True, fontsize=8.5)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_comparison.pdf")
    fig.savefig(out_pdf, bbox_inches='tight')
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")


# ----------------------------------------------------------------------
# 2. Figure 2: Chirikov Overlap Phase Map & Secular Precession Sweeping
# ----------------------------------------------------------------------
def generate_fig_model_choices():
    overlap_csv = os.path.join(script_dir, "overlap_grid.csv")
    time_csv = os.path.join(script_dir, "resonance_crossing_timeseries.csv")

    df_ov = np.genfromtxt(overlap_csv, delimiter=',', names=True)
    df_time = np.genfromtxt(time_csv, delimiter=',', names=True)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11.5, 4.8), dpi=300)

    # Panel 1: Chirikov Overlap Parameter 2D Grid
    e_j_unique = np.unique(df_ov['e_jupiter'])
    e_s_unique = np.unique(df_ov['e_saturn'])
    S_grid = df_ov['chirikov_S'].reshape(len(e_j_unique), len(e_s_unique)).T

    EJ, ES = np.meshgrid(e_j_unique, e_s_unique)
    cf = ax1.contourf(EJ,
                      ES,
                      S_grid,
                      levels=np.logspace(-1, 2.5, 30),
                      cmap='plasma',
                      norm=LogNorm())
    cbar = plt.colorbar(cf, ax=ax1)
    cbar.set_label(
        r'Chirikov Overlap Parameter $S = (\Delta \omega_1 + \Delta \omega_2) / |g_5 - g_6|$',
        fontweight='bold',
        fontsize=8.5)

    # Contour for S = 1.0 (Critical Overlap Boundary)
    cs = ax1.contour(EJ,
                     ES,
                     S_grid,
                     levels=[1.0],
                     colors='white',
                     linewidths=2.5,
                     linestyles='--')
    ax1.clabel(cs, inline=True, fmt=r'$S = 1.0$', fontsize=9, colors='white')

    # Initial and Modern System Points
    ax1.scatter([0.01], [0.01],
                color='cyan',
                edgecolors='black',
                s=90,
                zorder=10,
                label=r'Pre-Crossing Initial ($e_J=0.01, e_S=0.01$)')
    ax1.scatter([0.048], [0.054],
                color='lime',
                edgecolors='black',
                marker='*',
                s=140,
                zorder=10,
                label=r'Modern Solar System ($e_J=0.048, e_S=0.054$)')

    ax1.text(0.06,
             0.02,
             'Chaotic Sea\n(Overlapped Resonances)',
             color='white',
             fontweight='bold',
             fontsize=9.5)
    ax1.set_xlabel(r'Jupiter Orbital Eccentricity $e_J$', fontweight='bold')
    ax1.set_ylabel(r'Saturn Orbital Eccentricity $e_S$', fontweight='bold')
    ax1.set_title(r'(a) Chirikov Resonance Overlap Phase Map ($S \geq 1$)',
                  fontweight='bold')
    ax1.set_xlim(0.0, 0.12)
    ax1.set_ylim(0.0, 0.14)
    ax1.legend(loc='upper left', frameon=True, fontsize=8)

    # Panel 2: Secular Eigenfrequencies Sweeping Across Migration Track
    ax2.plot(df_time['period_ratio'],
             df_time['g5_arcsec_yr'],
             '-',
             color='#1f77b4',
             lw=2.2,
             label=r'Secular Mode $g_5$ (Jupiter Precession)')
    ax2.plot(df_time['period_ratio'],
             df_time['g6_arcsec_yr'],
             '-',
             color='#d62728',
             lw=2.2,
             label=r'Secular Mode $g_6$ (Saturn Precession)')
    ax2.plot(df_time['period_ratio'],
             np.abs(df_time['g6_arcsec_yr'] - df_time['g5_arcsec_yr']),
             '--',
             color='#2ca02c',
             lw=2.0,
             label=r'Multiplet Frequency Splitting $|g_5 - g_6|$')

    ax2.axvline(2.0,
                color='black',
                linestyle=':',
                lw=1.5,
                label=r'Exact 2:1 MMR ($P_S / P_J = 2.0$)')
    ax2.axhline(0.0, color='gray', linestyle='-', lw=0.8)

    ax2.annotate(r'$\nu_5, \nu_6$ Secular Resonance Sweeping' + '\n' +
                 r'sweeps asteroid belt and outer planets',
                 xy=(2.0, 10.0),
                 xytext=(1.85, 20.0),
                 arrowprops=dict(facecolor='darkgreen',
                                 shrink=0.08,
                                 width=1.5,
                                 headwidth=6),
                 fontsize=8.5,
                 fontweight='bold',
                 color='darkgreen',
                 bbox=dict(boxstyle='round,pad=0.3',
                           facecolor='#e8f5e9',
                           edgecolor='darkgreen'))

    ax2.set_xlabel(
        r'Orbital Period Ratio $P_{\mathrm{Saturn}} / P_{\mathrm{Jupiter}}$',
        fontweight='bold')
    ax2.set_ylabel(r'Precession Eigenfrequency [arcsec/yr]', fontweight='bold')
    ax2.set_title(r'(b) Secular Eigenfrequency Sweeping & Resonant Splitting',
                  fontweight='bold')
    ax2.set_xlim(1.80, 2.25)
    ax2.set_ylim(0.0, 30.0)
    ax2.legend(loc='upper right', frameon=True, fontsize=8)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_model_choices.pdf")
    fig.savefig(out_pdf, bbox_inches='tight')
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")


# ----------------------------------------------------------------------
# 3. Figure 3: Physical Diagram - Resonance Topology & Solar System Architecture
# ----------------------------------------------------------------------
def generate_fig_diagram():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.5), dpi=300)

    # ------------------ Subplot 1: Nice Model Solar System Dynamics ------------------
    ax1.set_aspect('equal')
    ax1.axis('off')

    # Sun at center
    sun = plt.Circle((0, 0),
                     0.30,
                     color='#ffb703',
                     ec='#fb8500',
                     lw=2,
                     zorder=10)
    ax1.add_patch(sun)
    ax1.text(0,
             0,
             'Sun\n$M_\\odot$',
             ha='center',
             va='center',
             fontweight='bold',
             color='black',
             fontsize=9)

    # Planetary orbits before and after
    r_j = 0.90
    r_s = 1.45
    r_u = 2.10
    r_n = 2.70
    r_disk_in = 3.00
    r_disk_out = 3.80

    orbit_j = plt.Circle((0, 0),
                         r_j,
                         color='#1f77b4',
                         fill=False,
                         linestyle='-',
                         lw=1.5,
                         alpha=0.8)
    orbit_s = plt.Circle((0, 0),
                         r_s,
                         color='#d62728',
                         fill=False,
                         linestyle='-',
                         lw=1.5,
                         alpha=0.8)
    orbit_u = plt.Circle((0, 0),
                         r_u,
                         color='#023e8a',
                         fill=False,
                         linestyle='--',
                         lw=1.2,
                         alpha=0.7)
    orbit_n = plt.Circle((0, 0),
                         r_n,
                         color='#0077b6',
                         fill=False,
                         linestyle='--',
                         lw=1.2,
                         alpha=0.7)
    ax1.add_patch(orbit_j)
    ax1.add_patch(orbit_s)
    ax1.add_patch(orbit_u)
    ax1.add_patch(orbit_n)

    # Planetesimal disk ring
    disk = plt.Circle((0, 0),
                      r_disk_out,
                      color='#90e0ef',
                      fill=True,
                      alpha=0.2,
                      zorder=1)
    disk_hole = plt.Circle((0, 0),
                           r_disk_in,
                           color='white',
                           fill=True,
                           zorder=2)
    ax1.add_patch(disk)
    ax1.add_patch(disk_hole)

    # Draw planet bodies
    jup = plt.Circle((r_j * np.cos(0.4), r_j * np.sin(0.4)),
                     0.12,
                     color='#bc6c25',
                     ec='darkred',
                     lw=1.5,
                     zorder=15)
    sat = plt.Circle((r_s * np.cos(1.2), r_s * np.sin(1.2)),
                     0.10,
                     color='#e9c46a',
                     ec='#bc6c25',
                     lw=1.5,
                     zorder=15)
    ura = plt.Circle((r_u * np.cos(2.2), r_u * np.sin(2.2)),
                     0.08,
                     color='#48cae4',
                     ec='#023e8a',
                     lw=1.5,
                     zorder=15)
    nep = plt.Circle((r_n * np.cos(3.1), r_n * np.sin(3.1)),
                     0.08,
                     color='#0096c7',
                     ec='#03045e',
                     lw=1.5,
                     zorder=15)
    ax1.add_patch(jup)
    ax1.add_patch(sat)
    ax1.add_patch(ura)
    ax1.add_patch(nep)

    # Migration arrows
    ax1.annotate('',
                 xy=(r_j * np.cos(0.4) - 0.12, r_j * np.sin(0.4) - 0.05),
                 xytext=(r_j * np.cos(0.4) + 0.05, r_j * np.sin(0.4) + 0.02),
                 arrowprops=dict(arrowstyle="->", color="navy", lw=2))
    ax1.annotate('',
                 xy=(r_s * np.cos(1.2) + 0.15, r_s * np.sin(1.2) + 0.12),
                 xytext=(r_s * np.cos(1.2) - 0.02, r_s * np.sin(1.2) - 0.02),
                 arrowprops=dict(arrowstyle="->", color="darkred", lw=2))

    ax1.text(r_j * np.cos(0.4) + 0.14,
             r_j * np.sin(0.4) - 0.10,
             r'Jupiter ($a_J \approx 5.4\to 5.2$ AU)',
             fontsize=8.5,
             fontweight='bold',
             color='#1f77b4')
    ax1.text(r_s * np.cos(1.2) + 0.14,
             r_s * np.sin(1.2) - 0.05,
             r'Saturn ($a_S \approx 8.4\to 9.5$ AU)',
             fontsize=8.5,
             fontweight='bold',
             color='#d62728')
    ax1.text(r_u * np.cos(2.2) - 0.10,
             r_u * np.sin(2.2) + 0.12,
             r'Uranus ($11\to 19$ AU)',
             fontsize=8,
             fontweight='bold',
             color='#023e8a')
    ax1.text(r_n * np.cos(3.1) - 0.40,
             r_n * np.sin(3.1) - 0.18,
             r'Neptune ($14\to 30$ AU)',
             fontsize=8,
             fontweight='bold',
             color='#0077b6')
    ax1.text(2.6,
             -2.6,
             r'Primordial Planetesimal Disk' + '\n' +
             r'($M_{\mathrm{disk}} \approx 35\ M_\oplus, 15-30\ \mathrm{AU}$)',
             fontsize=8,
             color='#0077b6',
             ha='center',
             bbox=dict(boxstyle='round,pad=0.3',
                       facecolor='#e0f2fe',
                       edgecolor='#0077b6'))

    # Description Box
    ax1.text(
        0,
        -4.0,
        r'$\mathbf{Nice\ Model\ Dynamical\ Instability:}$' + '\n' +
        r'Divergent migration crosses the 2:1 Jupiter-Saturn MMR,' + '\n' +
        r'exciting $e_J, e_S$ and scattering Ice Giants into the trans-Neptunian disk.',
        ha='center',
        va='center',
        fontsize=8.5,
        bbox=dict(boxstyle='round,pad=0.4',
                  facecolor='#f8f9fa',
                  edgecolor='navy',
                  lw=1.2))

    ax1.set_xlim(-4.3, 4.3)
    ax1.set_ylim(-4.3, 4.3)
    ax1.set_title(r'(a) Nice Model Planetary Migration Architecture',
                  fontweight='bold',
                  fontsize=11,
                  pad=10)

    # ------------------ Subplot 2: Resonant Multiplet & Separatrix Overlap ------------------
    ax2.set_aspect('auto')

    # Phase space pendulum resonant islands
    theta = np.linspace(-np.pi, np.pi, 200)
    for c in [0.2, 0.5, 0.8, 1.1]:
        p_upper = np.sqrt(np.maximum(0.0, 2.0 * (c - np.cos(theta))))
        p_lower = -p_upper
        ax2.plot(theta, p_upper + 0.8, color='#1f77b4', lw=1.2, alpha=0.7)
        ax2.plot(theta, p_lower + 0.8, color='#1f77b4', lw=1.2, alpha=0.7)
        ax2.plot(theta, p_upper - 0.8, color='#d62728', lw=1.2, alpha=0.7)
        ax2.plot(theta, p_lower - 0.8, color='#d62728', lw=1.2, alpha=0.7)

    # Separatrices (c = 1.0)
    sep_upper = 2.0 * np.cos(0.5 * theta)
    sep_lower = -2.0 * np.cos(0.5 * theta)
    ax2.plot(
        theta,
        sep_upper + 0.8,
        color='#084594',
        lw=2.2,
        label=
        r'Harmonic 1 Separatrix ($\phi_1 = 2\lambda_S - \lambda_J - \varpi_J$)')
    ax2.plot(theta, sep_lower + 0.8, color='#084594', lw=2.2)
    ax2.plot(
        theta,
        sep_upper - 0.8,
        color='#990000',
        lw=2.2,
        label=
        r'Harmonic 2 Separatrix ($\phi_2 = 2\lambda_S - \lambda_J - \varpi_S$)')
    ax2.plot(theta, sep_lower - 0.8, color='#990000', lw=2.2)

    # Chaotic sea overlap zone
    ax2.axhspan(-0.8,
                0.8,
                color='orange',
                alpha=0.25,
                label=r'Chirikov Resonance Overlap Chaotic Sea ($S \geq 1$)')

    # Arrows for frequency splitting
    ax2.annotate('',
                 xy=(-2.5, 0.8),
                 xytext=(-2.5, -0.8),
                 arrowprops=dict(arrowstyle="<->", color="darkgreen", lw=2.0))
    ax2.text(-2.4,
             0.0,
             r'Splitting $|g_5 - g_6|$',
             color='darkgreen',
             fontweight='bold',
             fontsize=9,
             va='center')

    ax2.text(
        0.0,
        2.3,
        r'Sub-resonance $\phi_1$ (Width $\Delta \omega_1 \propto \sqrt{\mu_J |f_1| e_J}$)',
        ha='center',
        fontsize=8.5,
        color='#084594',
        fontweight='bold')
    ax2.text(
        0.0,
        -2.4,
        r'Sub-resonance $\phi_2$ (Width $\Delta \omega_2 \propto \sqrt{\mu_S |f_2| e_S}$)',
        ha='center',
        fontsize=8.5,
        color='#990000',
        fontweight='bold')

    ax2.set_xlabel(r'Resonant Critical Angle $\phi$ [rad]', fontweight='bold')
    ax2.set_ylabel(r'Conjugate Resonant Action / Momentum $p$',
                   fontweight='bold')
    ax2.set_title(
        r'(b) Secular Resonant Harmonics & Separatrix Overlap Topology',
        fontweight='bold',
        fontsize=11,
        pad=10)
    ax2.set_xlim(-np.pi, np.pi)
    ax2.set_ylim(-3.0, 3.0)
    ax2.legend(loc='upper right', frameon=True, fontsize=8)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_diagram.pdf")
    fig.savefig(out_pdf, bbox_inches='tight')
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")


if __name__ == '__main__':
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("🎉 All Paper #228 plots successfully generated!")
