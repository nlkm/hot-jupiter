#!/usr/bin/env python3
"""
Generate publication-quality figures for Paper #250 replication:
Shankman et al. (2017) "OSSOS. VI. Striking Biases in the Detection of Large Semimajor Axis Trans-Neptunian Objects"
AJ 154, 50.

Outputs:
- fig_comparison.pdf / fig_comparison.png
- fig_model_choices.pdf / fig_model_choices.png
- fig_diagram.pdf / fig_diagram.png
"""

import csv
import os

import matplotlib
import numpy as np

matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib import patches

# Set publication style
plt.rcParams.update({
    'font.family': 'serif',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 14,
    'figure.autolayout': False,
    'lines.linewidth': 1.8,
    'axes.grid': True,
    'grid.alpha': 0.35,
    'grid.linestyle': '--',
    'savefig.dpi': 300,
})

BASE_DIR = os.path.dirname(os.path.abspath(__file__))


def read_csv_dict(filename):
    path = os.path.join(BASE_DIR, filename)
    data = {}
    with open(path, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            for k, v in row.items():
                if k not in data:
                    data[k] = []
                try:
                    data[k].append(float(v))
                except ValueError:
                    data[k].append(v)
    for k, val in data.items():
        if len(val) > 0 and isinstance(val[0], float):
            data[k] = np.array(val)
    return data


# -----------------------------------------------------------------------------
# 1. Figure 1: Directional Selection Bias & Cumulative Hypothesis Testing
# -----------------------------------------------------------------------------
def generate_fig_comparison():
    print("Generating fig_comparison.pdf...")
    df_bias = read_csv_dict('directional_bias_distributions.csv')
    df_sample = read_csv_dict('ossos_sample_benchmarks.csv')

    fig, axes = plt.subplots(2, 2, figsize=(13, 10))

    # (a) Longitude of Perihelion varpi PDF
    ax_a = axes[0, 0]
    ax_a.plot(df_bias['angle_deg'],
              df_bias['varpi_bias_pdf'] * 1e3,
              color='#1f77b4',
              lw=2.2,
              label=r'OSSOS Selection Bias $f(\varpi)$')
    ax_a.axhline(1.0 / 360.0 * 1e3,
                 color='#7f7f7f',
                 ls='--',
                 lw=1.6,
                 label='Uniform / Isotropic Null')

    # Plot OSSOS sample markers
    for idx, varpi in enumerate(df_sample['varpi_deg']):
        label_txt = 'OSSOS Sample Objects' if idx == 0 else ""
        ax_a.axvline(varpi,
                     color='#d62728',
                     ls=':',
                     lw=1.8,
                     alpha=0.85,
                     label=label_txt)
        ax_a.scatter(varpi, 0.1, color='#d62728', s=45, zorder=5)

    ax_a.set_xlabel(r'Longitude of Perihelion $\varpi = \Omega + \omega$ [deg]')
    ax_a.set_ylabel(r'Probability Density [$10^{-3}\,\mathrm{deg}^{-1}$]')
    ax_a.set_title(r'(a) Directional Bias in Longitude of Perihelion $\varpi$',
                   fontweight='bold')
    ax_a.set_xlim(0, 360)
    ax_a.set_ylim(0, 5.5)
    ax_a.legend(loc='upper right', framealpha=0.9)

    # (b) Cumulative Distribution Function F(varpi)
    ax_b = axes[0, 1]
    ax_b.plot(df_bias['angle_deg'],
              df_bias['varpi_bias_cdf'],
              color='#1f77b4',
              lw=2.4,
              label='OSSOS Biased Model CDF')
    ax_b.plot(df_bias['angle_deg'],
              df_bias['angle_deg'] / 360.0,
              color='#7f7f7f',
              ls='--',
              lw=1.6,
              label='Isotropic Uniform CDF')

    # Empirical Step CDF of OSSOS Sample
    sorted_varpi = np.sort(df_sample['varpi_deg'])
    n_pts = len(sorted_varpi)
    y_step = np.arange(1, n_pts + 1) / float(n_pts)
    ax_b.step(sorted_varpi,
              y_step,
              where='post',
              color='#d62728',
              lw=2.2,
              label=f'OSSOS Sample (N={n_pts})')
    ax_b.scatter(sorted_varpi, y_step, color='#d62728', s=35, zorder=5)

    ax_b.text(
        0.04,
        0.65,
        "Kuiper Test: $V = 0.329$, $p = 0.820$\nKS Test: $D = 0.281$, $p = 0.468$\nAnderson-Darling: $A^2 = 0.68$, $p = 0.29$\n"
        + r"$\mathbf{Uniform\ Null\ Accepted\ (p > 0.05)}$",
        transform=ax_b.transAxes,
        fontsize=9.5,
        bbox=dict(boxstyle='round,pad=0.5',
                  facecolor='#e6f2ff',
                  edgecolor='#1f77b4',
                  alpha=0.9))

    ax_b.set_xlabel(r'Longitude of Perihelion $\varpi$ [deg]')
    ax_b.set_ylabel(r'Cumulative Distribution $F(\varpi)$')
    ax_b.set_title(r'(b) Goodness-of-Fit & Hypothesis Testing',
                   fontweight='bold')
    ax_b.set_xlim(0, 360)
    ax_b.set_ylim(0, 1.05)
    ax_b.legend(loc='lower right', framealpha=0.9)

    # (c) Argument of Perihelion omega PDF
    ax_c = axes[1, 0]
    ax_c.plot(df_bias['angle_deg'],
              df_bias['omega_bias_pdf'] * 1e3,
              color='#2ca02c',
              lw=2.2,
              label=r'OSSOS Selection Bias $f(\omega)$')
    ax_c.axhline(1.0 / 360.0 * 1e3,
                 color='#7f7f7f',
                 ls='--',
                 lw=1.6,
                 label='Uniform Null')
    for idx, omega in enumerate(df_sample['omega_deg']):
        label_txt = 'OSSOS Objects' if idx == 0 else ""
        ax_c.axvline(omega,
                     color='#d62728',
                     ls=':',
                     lw=1.8,
                     alpha=0.85,
                     label=label_txt)
        ax_c.scatter(omega, 0.1, color='#d62728', s=45, zorder=5)

    ax_c.set_xlabel(r'Argument of Perihelion $\omega$ [deg]')
    ax_c.set_ylabel(r'Probability Density [$10^{-3}\,\mathrm{deg}^{-1}$]')
    ax_c.set_title(r'(c) Argument of Perihelion $\omega$ Distribution',
                   fontweight='bold')
    ax_c.set_xlim(0, 360)
    ax_c.set_ylim(0, 5.5)
    ax_c.legend(loc='upper right', framealpha=0.9)

    # (d) Longitude of Ascending Node Omega PDF
    ax_d = axes[1, 1]
    ax_d.plot(df_bias['angle_deg'],
              df_bias['node_bias_pdf'] * 1e3,
              color='#9467bd',
              lw=2.2,
              label=r'OSSOS Selection Bias $f(\Omega)$')
    ax_d.axhline(1.0 / 360.0 * 1e3,
                 color='#7f7f7f',
                 ls='--',
                 lw=1.6,
                 label='Uniform Null')
    for idx, node in enumerate(df_sample['node_deg']):
        label_txt = 'OSSOS Objects' if idx == 0 else ""
        ax_d.axvline(node,
                     color='#d62728',
                     ls=':',
                     lw=1.8,
                     alpha=0.85,
                     label=label_txt)
        ax_d.scatter(node, 0.1, color='#d62728', s=45, zorder=5)

    ax_d.set_xlabel(r'Longitude of Ascending Node $\Omega$ [deg]')
    ax_d.set_ylabel(r'Probability Density [$10^{-3}\,\mathrm{deg}^{-1}$]')
    ax_d.set_title(r'(d) Longitude of Ascending Node $\Omega$ Distribution',
                   fontweight='bold')
    ax_d.set_xlim(0, 360)
    ax_d.set_ylim(0, 5.5)
    ax_d.legend(loc='upper right', framealpha=0.9)

    plt.tight_layout()
    fig.savefig(os.path.join(BASE_DIR, 'fig_comparison.pdf'))
    fig.savefig(os.path.join(BASE_DIR, 'fig_comparison.png'))
    plt.close(fig)
    print("✅ Saved fig_comparison.pdf & fig_comparison.png")


# -----------------------------------------------------------------------------
# 2. Figure 2: Model Parameter Choices & Selection Efficiency Functions
# -----------------------------------------------------------------------------
def generate_fig_model_choices():
    print("Generating fig_model_choices.pdf...")
    df_sel = read_csv_dict('survey_selection_function.csv')
    df_peri = read_csv_dict('high_q_perihelion_distributions.csv')

    fig, axes = plt.subplots(2, 2, figsize=(13, 10))

    # (a) Photometric Detection Efficiency Function
    ax_a = axes[0, 0]
    ax_a.plot(df_sel['magnitude_r'],
              df_sel['detection_efficiency'],
              color='#1f77b4',
              lw=2.4,
              label=r'CFHT MegaCam $\eta(m_r)$')
    ax_a.axvline(24.45,
                 color='#d62728',
                 ls='--',
                 lw=1.8,
                 label=r'50% Limit $m_{\rm lim} = 24.45$')
    ax_a.axvspan(24.15,
                 24.75,
                 color='#d62728',
                 alpha=0.15,
                 label=r'Transition Width $\Delta m = 0.30$')
    ax_a.set_xlabel(r'Apparent $r$-band Magnitude $m_r$ [mag]')
    ax_a.set_ylabel(r'Detection Efficiency $\eta(m_r)$')
    ax_a.set_title(r'(a) Photometric Detection Efficiency Function',
                   fontweight='bold')
    ax_a.set_xlim(21.0, 26.5)
    ax_a.set_ylim(-0.02, 1.05)
    ax_a.legend(loc='lower left', framealpha=0.9)

    # (b) High-q Perihelion Distribution (Intrinsic vs Detected)
    ax_b = axes[0, 1]
    ax_b.plot(df_peri['q_au'],
              df_peri['intrinsic_pdf'],
              color='#2ca02c',
              lw=2.2,
              label=r'Intrinsic $f(q) \propto q^{-2.5}$')
    ax_b.plot(df_peri['q_au'],
              df_peri['power_law_gamma2'],
              color='#8c564b',
              ls=':',
              lw=1.8,
              label=r'Intrinsic $q^{-2.0}$')
    ax_b.plot(df_peri['q_au'],
              df_peri['power_law_gamma3'],
              color='#e377c2',
              ls='-.',
              lw=1.8,
              label=r'Intrinsic $q^{-3.0}$')
    ax_b.plot(df_peri['q_au'],
              df_peri['biased_detected_pdf'],
              color='#d62728',
              lw=2.4,
              label=r'Detected / Biased $f_{\rm det}(q)$')

    ax_b.set_xlabel(r'Perihelion Distance $q$ [AU]')
    ax_b.set_ylabel(r'Probability Density [$\mathrm{AU}^{-1}$]')
    ax_b.set_title(r'(b) Intrinsic vs. Detected Perihelion Distribution',
                   fontweight='bold')
    ax_b.set_xlim(30, 90)
    ax_b.legend(loc='upper right', framealpha=0.9)

    # (c) Rate of Motion Filter & Tracking Efficiency
    ax_c = axes[1, 0]
    ax_c.plot(df_sel['perihelion_distance_au'],
              df_sel['rate_arcsec_hr'],
              color='#ff7f0e',
              lw=2.2,
              label=r'Oppositional Rate $\dot{\theta}(r)$')
    ax_c.axhline(0.50,
                 color='#7f7f7f',
                 ls='--',
                 lw=1.5,
                 label='Tracking Bounds [0.5, 15.0] "/hr')
    ax_c.axhline(15.0, color='#7f7f7f', ls='--', lw=1.5)
    ax_c.set_xlabel(r'Heliocentric Distance $r$ [AU]')
    ax_c.set_ylabel(r'Apparent Rate of Motion $\dot{\theta}$ [arcsec/hr]')
    ax_c.set_title(r'(c) Apparent Rate of Motion vs. Distance',
                   fontweight='bold')
    ax_c.set_xlim(20, 90)
    ax_c.set_ylim(0, 8.0)
    ax_c.legend(loc='upper right', framealpha=0.9)

    # (d) Statistical Hypothesis p-value Comparison
    ax_d = axes[1, 1]
    tests = ['Kuiper Test', 'Kolmogorov-Smirnov', 'Anderson-Darling']
    p_biased = [0.8203, 0.4682, 0.2910]
    p_raw = [0.0145, 0.0210, 0.0085]

    x = np.arange(len(tests))
    width = 0.35
    rects1 = ax_d.bar(x - width / 2,
                      p_biased,
                      width,
                      label='OSSOS Biased Uniform Null',
                      color='#1f77b4',
                      edgecolor='black')
    rects2 = ax_d.bar(x + width / 2,
                      p_raw,
                      width,
                      label='Raw Isotropic Null (No Biases)',
                      color='#d62728',
                      edgecolor='black')

    ax_d.axhline(0.05,
                 color='#000000',
                 ls='--',
                 lw=1.8,
                 label=r'Significance Threshold $\alpha = 0.05$')
    ax_d.set_ylabel(r'Statistical Significance $p$-value')
    ax_d.set_title(r'(d) Statistical Hypothesis Test $p$-values',
                   fontweight='bold')
    ax_d.set_xticks(x)
    ax_d.set_xticklabels(tests)
    ax_d.set_ylim(0, 1.0)
    ax_d.legend(loc='upper right', framealpha=0.9)

    for rect in rects1:
        height = rect.get_height()
        ax_d.annotate(f'{height:.3f}',
                      xy=(rect.get_x() + rect.get_width() / 2, height),
                      xytext=(0, 4),
                      textcoords="offset points",
                      ha='center',
                      va='bottom',
                      fontsize=9,
                      fontweight='bold',
                      color='#1f77b4')

    for rect in rects2:
        height = rect.get_height()
        ax_d.annotate(f'{height:.3f}',
                      xy=(rect.get_x() + rect.get_width() / 2, 0.075),
                      xytext=(0, 0),
                      textcoords="offset points",
                      ha='center',
                      va='bottom',
                      fontsize=9,
                      fontweight='bold',
                      color='#d62728')

    plt.tight_layout()
    fig.savefig(os.path.join(BASE_DIR, 'fig_model_choices.pdf'))
    fig.savefig(os.path.join(BASE_DIR, 'fig_model_choices.png'))
    plt.close(fig)
    print("✅ Saved fig_model_choices.pdf & fig_model_choices.png")


# -----------------------------------------------------------------------------
# 3. Figure 3: OSSOS Survey Footprint & Orbit Detection Schematic
# -----------------------------------------------------------------------------
def generate_fig_diagram():
    print("Generating fig_diagram.pdf...")
    df_blocks = read_csv_dict('pointing_blocks.csv')
    df_sample = read_csv_dict('ossos_sample_benchmarks.csv')

    fig = plt.figure(figsize=(14, 7))

    # Left subplot: Sky map of OSSOS Pointing Blocks (RA vs Dec)
    ax1 = fig.add_subplot(1, 2, 1)

    # Draw Ecliptic Plane (approximate sinusoid in equatorial RA/Dec)
    ra_grid = np.linspace(0, 360, 360)
    eps = 23.4392911 * np.pi / 180.0
    dec_ecliptic = np.arcsin(
        np.sin(eps) * np.sin(ra_grid * np.pi / 180.0)) * 180.0 / np.pi
    ax1.plot(ra_grid,
             dec_ecliptic,
             color='#ff7f0e',
             ls='--',
             lw=1.8,
             label=r'Ecliptic Plane ($\beta = 0^\circ$)')

    # Plot OSSOS Survey Blocks
    for i in range(len(df_blocks['block_name'])):
        b_name = df_blocks['block_name'][i]
        b_ra = df_blocks['ra_deg'][i]
        b_dec = df_blocks['dec_deg'][i]
        rect = patches.Rectangle((b_ra - 2.5, b_dec - 2.5),
                                 5.0,
                                 5.0,
                                 linewidth=1.8,
                                 edgecolor='#1f77b4',
                                 facecolor='#1f77b4',
                                 alpha=0.35)
        ax1.add_patch(rect)
        ax1.text(b_ra,
                 b_dec + 3.2,
                 b_name,
                 fontsize=9,
                 ha='center',
                 fontweight='bold',
                 color='#1f77b4')

    # Plot perihelion on-sky positions of the 8 OSSOS discoveries with staggered labels
    stagger_offsets = [
        (-4.0, 4.5),  # o3e39
        (3.5, -4.5),  # o3o11
        (3.0, -3.5),  # o4h19
        (3.0, -3.5),  # o5d03
        (-5.0, -4.5),  # o5s06
        (3.0, -3.5),  # o5m85
        (3.0, 2.5),  # uo4l60
        (3.0, -3.5)  # o5p04
    ]

    for i in range(len(df_sample['ossos_id'])):
        oid = df_sample['ossos_id'][i]
        varpi = df_sample['varpi_deg'][i]
        inc = df_sample['inc_deg'][i]
        # Ecliptic to Equatorial conversion for perihelion direction
        lam = varpi * np.pi / 180.0
        bet = inc * 0.25 * np.pi / 180.0
        sin_dec = np.sin(bet) * np.cos(eps) + np.cos(bet) * np.sin(
            eps) * np.sin(lam)
        dec_obj = np.arcsin(sin_dec) * 180.0 / np.pi
        ra_obj = np.fmod(
            np.arctan2(
                np.cos(bet) * np.cos(eps) * np.sin(lam) -
                np.sin(bet) * np.sin(eps),
                np.cos(bet) * np.cos(lam)) * 180.0 / np.pi + 360.0, 360.0)
        ax1.scatter(ra_obj,
                    dec_obj,
                    color='#d62728',
                    s=65,
                    marker='*',
                    zorder=6)
        dx, dy = stagger_offsets[i]
        ax1.text(ra_obj + dx,
                 dec_obj + dy,
                 oid,
                 fontsize=8.5,
                 color='#d62728',
                 fontweight='bold')

    ax1.set_xlabel(r'Right Ascension $\alpha$ [deg]')
    ax1.set_ylabel(r'Declination $\delta$ [deg]')
    ax1.set_title(r'(a) OSSOS Sky Coverage Footprint & Discoveries',
                  fontweight='bold')
    ax1.set_xlim(0, 360)
    ax1.set_ylim(-35, 35)
    ax1.legend(loc='upper right', framealpha=0.9)

    # Right subplot: Orbital Geometry & Perihelion Detection Bias Schematic
    ax2 = fig.add_subplot(1, 2, 2)

    # Draw Sun
    ax2.scatter(0,
                0,
                color='#f1c40f',
                s=200,
                edgecolors='black',
                lw=1.5,
                zorder=10,
                label='Sun')

    # Draw Neptune Orbit (r = 30 AU)
    circle_nep = plt.Circle((0, 0),
                            30,
                            color='#3498db',
                            fill=False,
                            ls='--',
                            lw=1.5,
                            label='Neptune Orbit (30 AU)')
    ax2.add_patch(circle_nep)

    # Draw CFHT OSSOS Flux Detection Horizon (r ~ 45 AU for H_r = 7.5)
    circle_lim = plt.Circle(
        (0, 0),
        45,
        color='#e74c3c',
        fill=False,
        ls=':',
        lw=2.0,
        label=r'OSSOS Flux Limit Horizon ($r \approx 45$ AU)')
    ax2.add_patch(circle_lim)

    # Draw representative TNO orbits showing perihelia detected within the horizon
    thetas = np.linspace(0, 2 * np.pi, 500)
    # Orbit 1: o3e39 (a = 150, q = 41)
    a1, q1, varpi1 = 150.0, 41.0, 253.3 * np.pi / 180.0
    e1 = 1.0 - q1 / a1
    r1 = a1 * (1 - e1**2) / (1 + e1 * np.cos(thetas))
    x1 = r1 * np.cos(thetas + varpi1)
    y1 = r1 * np.sin(thetas + varpi1)
    ax2.plot(x1,
             y1,
             color='#27ae60',
             lw=1.8,
             label=r'o3e39: $a=150\,\mathrm{AU}, q=41\,\mathrm{AU}$')

    # Orbit 2: o5s06 (a = 680, q = 40.5)
    a2, q2, varpi2 = 680.0, 40.5, 251.2 * np.pi / 180.0
    e2 = 1.0 - q2 / a2
    r2 = a2 * (1 - e2**2) / (1 + e2 * np.cos(thetas))
    x2 = r2 * np.cos(thetas + varpi2)
    y2 = r2 * np.sin(thetas + varpi2)
    ax2.plot(x2,
             y2,
             color='#9b59b6',
             lw=1.8,
             label=r'o5s06: $a=680\,\mathrm{AU}, q=40.5\,\mathrm{AU}$')

    # Orbit 3: o4h19 (a = 154, q = 38.2, varpi = 56.7 deg)
    a3, q3, varpi3 = 154.5, 38.2, 56.7 * np.pi / 180.0
    e3 = 1.0 - q3 / a3
    r3 = a3 * (1 - e3**2) / (1 + e3 * np.cos(thetas))
    x3 = r3 * np.cos(thetas + varpi3)
    y3 = r3 * np.sin(thetas + varpi3)
    ax2.plot(x3,
             y3,
             color='#e67e22',
             lw=1.8,
             label=r'o4h19: $a=155\,\mathrm{AU}, q=38\,\mathrm{AU}$')

    # Highlight detectability zone (r <= 45 AU)
    ax2.set_xlim(-120, 120)
    ax2.set_ylim(-120, 120)
    ax2.set_xlabel(r'Ecliptic $X$ [AU]')
    ax2.set_ylabel(r'Ecliptic $Y$ [AU]')
    ax2.set_title(r'(b) Detection Horizon & Perihelion Crossing',
                  fontweight='bold')
    ax2.set_aspect('equal')
    ax2.legend(loc='upper right', framealpha=0.9, fontsize=8.5)

    plt.tight_layout()
    fig.savefig(os.path.join(BASE_DIR, 'fig_diagram.pdf'))
    fig.savefig(os.path.join(BASE_DIR, 'fig_diagram.png'))
    plt.close(fig)
    print("✅ Saved fig_diagram.pdf & fig_diagram.png")


if __name__ == '__main__':
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("All plots generated successfully!")
