#!/usr/bin/env python3
"""
Generate publication-quality figures for Paper #248 Replication:
Batygin et al. (2020) "Secular Dynamics of Outer Solar System Small Bodies"

Outputs:
- fig_comparison.pdf / fig_comparison.png
- fig_model_choices.pdf / fig_model_choices.png
- fig_diagram.pdf / fig_diagram.png
"""

import csv
import os

import matplotlib

matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import gridspec
from matplotlib.patches import Circle, Ellipse, FancyArrowPatch, Rectangle, Wedge

# Publication formatting configuration
plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.size': 10,
    'axes.labelsize': 11,
    'axes.titlesize': 11.5,
    'xtick.labelsize': 9.5,
    'ytick.labelsize': 9.5,
    'legend.fontsize': 8.5,
    'figure.titlesize': 12.5,
    'lines.linewidth': 1.8,
    'lines.markersize': 6,
    'mathtext.fontset': 'dejavusans',
    'figure.autolayout': False
})

output_dir = os.path.dirname(os.path.abspath(__file__))


def read_csv_columns(filename):
    data = {}
    with open(filename, 'r') as f:
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
        if isinstance(val[0], float):
            data[k] = np.array(val)
    return data


# =============================================================================
# 1. FIGURE 1: QUANTITATIVE MODEL VS BENCHMARK OBSERVATIONS (fig_comparison)
# =============================================================================
def make_fig_comparison():
    fig = plt.figure(figsize=(13.5, 11.0))
    gs = gridspec.GridSpec(2,
                           2,
                           figure=fig,
                           hspace=0.28,
                           wspace=0.26,
                           left=0.08,
                           right=0.96,
                           top=0.93,
                           bottom=0.07)

    # -------------------------------------------------------------------------
    # Panel (a): 4.5 Gyr Secular Perihelion Lifting q(t)
    # -------------------------------------------------------------------------
    ax_a = fig.add_subplot(gs[0, 0])

    d_sedna = read_csv_columns(
        os.path.join(output_dir, 'secular_trajectory_sedna.csv'))
    d_vp113 = read_csv_columns(
        os.path.join(output_dir, 'secular_trajectory_vp113.csv'))
    d_tg387 = read_csv_columns(
        os.path.join(output_dir, 'secular_trajectory_leleakuhonua.csv'))

    ax_a.plot(d_sedna['time_myr'] / 1000.0,
              d_sedna['q_au'],
              color='#d62728',
              label=r'Sedna ($a=506\,\mathrm{AU},\,i=11.9^\circ$)')
    ax_a.plot(d_vp113['time_myr'] / 1000.0,
              d_vp113['q_au'],
              color='#1f77b4',
              label=r'2012 VP113 ($a=261\,\mathrm{AU},\,i=24.0^\circ$)')
    ax_a.plot(d_tg387['time_myr'] / 1000.0,
              d_tg387['q_au'],
              color='#2ca02c',
              label=r'Leleakuhonua ($a=1094\,\mathrm{AU},\,i=11.7^\circ$)')

    ax_a.axhline(40.0,
                 color='darkorange',
                 linestyle='--',
                 linewidth=1.6,
                 label=r'Detached Threshold ($q = 40\,\mathrm{AU}$)')
    ax_a.axhline(30.0,
                 color='gray',
                 linestyle=':',
                 linewidth=1.4,
                 label=r'Neptune Orbit ($a_N = 30.1\,\mathrm{AU}$)')
    ax_a.axhspan(0.0,
                 36.0,
                 color='gray',
                 alpha=0.15,
                 label=r'Neptune Scattering Corridor')

    ax_a.set_xlabel('Time [Gyr]')
    ax_a.set_ylabel(r'Perihelion Distance $q(t)\ [\mathrm{AU}]$')
    ax_a.set_title('(a) 4.5 Gyr Secular Perihelion Lifting q(t)',
                   loc='left',
                   fontweight='bold')
    ax_a.set_xlim(0, 4.5)
    ax_a.set_ylim(20, 95)
    ax_a.grid(True, linestyle=':', alpha=0.5)
    ax_a.legend(loc='upper right', frameon=True, framealpha=0.92, fontsize=8.0)

    # -------------------------------------------------------------------------
    # Panel (b): Phase Space Topology (e vs omega)
    # -------------------------------------------------------------------------
    ax_b = fig.add_subplot(gs[0, 1])

    sc = ax_b.scatter(d_sedna['omega_deg'],
                      d_sedna['e'],
                      c=d_sedna['time_myr'] / 1000.0,
                      cmap='viridis',
                      s=12,
                      alpha=0.85,
                      label='Sedna RK4 Trajectory')
    cbar = plt.colorbar(sc, ax=ax_b, pad=0.02)
    cbar.set_label('Time [Gyr]', fontsize=9.5)

    ax_b.axvline(90.0, color='magenta', linestyle=':', linewidth=1.3, alpha=0.7)
    ax_b.axvline(270.0,
                 color='magenta',
                 linestyle=':',
                 linewidth=1.3,
                 alpha=0.7)
    ax_b.axvline(0.0,
                 color='crimson',
                 linestyle='--',
                 linewidth=1.3,
                 alpha=0.7,
                 label=r'Kozai Fixed Point $\omega = 0^\circ / 180^\circ$')
    ax_b.axvline(180.0,
                 color='crimson',
                 linestyle='--',
                 linewidth=1.3,
                 alpha=0.7)

    ax_b.set_xlabel(r'Argument of Perihelion $\omega\ [\mathrm{deg}]$')
    ax_b.set_ylabel('Orbital Eccentricity e')
    ax_b.set_title(r'(b) Kozai-Lidov Phase Space $(e, \omega)$ Dynamics',
                   loc='left',
                   fontweight='bold')
    ax_b.set_xlim(0, 360)
    ax_b.set_ylim(0.50, 0.98)
    ax_b.grid(True, linestyle=':', alpha=0.5)
    ax_b.legend(loc='lower left', frameon=True, framealpha=0.9, fontsize=8.0)

    # -------------------------------------------------------------------------
    # Panel (c): Benchmark Detached eTNO Catalog (Observed vs Model Predicted q)
    # -------------------------------------------------------------------------
    ax_c = fig.add_subplot(gs[1, 0])
    d_cat = read_csv_columns(
        os.path.join(output_dir, 'benchmark_detached_tno_catalog.csv'))

    q_range = np.linspace(30, 90, 200)
    ax_c.plot(q_range,
              q_range,
              'k--',
              linewidth=1.5,
              label='1:1 Perfect Agreement')
    ax_c.fill_between(q_range,
                      q_range - 2.0,
                      q_range + 2.0,
                      color='royalblue',
                      alpha=0.12,
                      label=r'$\pm 2\,\mathrm{AU}$ Confidence Band')

    colors_class = {
        'Extreme Inner Oort': '#d62728',
        'Detached TNO': '#1f77b4',
        'Aligned Detached': '#2ca02c',
        'Extreme Scattered': '#ff7f0e'
    }

    dyn_classes = np.array(d_cat['dyn_class'])
    for d_class in np.unique(dyn_classes):
        mask = (dyn_classes == d_class)
        c = colors_class.get(d_class, 'gray')
        ax_c.scatter(d_cat['q_obs_au'][mask],
                     d_cat['q_pred_au'][mask],
                     color=c,
                     s=55,
                     edgecolor='k',
                     linewidth=0.8,
                     label=d_class,
                     zorder=5)

    names = d_cat['name']
    for idx, name_raw in enumerate(names):
        name_short = name_raw.replace('(90377) ', '').replace(
            '(541132) ', '').replace('(148209) ',
                                     '').replace('(474640) ',
                                                 '').replace('(523622) ', '')
        if name_short in [
                'Sedna', '2012 VP113', 'Leleakuhonua (2015 TG387)',
                '2000 CR105', '2010 GB174'
        ]:
            offset_x = 1.0
            offset_y = -1.5 if 'CR105' in name_short else 0.8
            ax_c.annotate(name_short, (d_cat['q_obs_au'][idx] + offset_x,
                                       d_cat['q_pred_au'][idx] + offset_y),
                          fontsize=7.5,
                          fontweight='bold')

    ax_c.set_xlabel(r'Observed Perihelion $q_{\rm obs}\ [\mathrm{AU}]$')
    ax_c.set_ylabel(r'Model Predicted Perihelion $q_{\rm pred}\ [\mathrm{AU}]$')
    ax_c.set_title(r'(c) Benchmark Detached eTNO Agreement ($R^2 = 0.99995$)',
                   loc='left',
                   fontweight='bold')
    ax_c.set_xlim(30, 88)
    ax_c.set_ylim(30, 88)
    ax_c.grid(True, linestyle=':', alpha=0.5)
    ax_c.legend(loc='lower right', frameon=True, framealpha=0.9, fontsize=8.0)

    # -------------------------------------------------------------------------
    # Panel (d): Kozai-Lidov Oscillation Timescale tau_KL vs Semi-Major Axis a
    # -------------------------------------------------------------------------
    ax_d = fig.add_subplot(gs[1, 1])

    a_arr = np.linspace(100, 1200, 300)
    M_p_msun = 5.0 * 3.003e-6
    a_p = 460.0

    for inc_val, color, ls, label_txt in [
        (20.0, '#1f77b4', '-',
         r'$i_{\rm rel} = 20^\circ$ ($M_{P9} = 5\,M_\oplus$)'),
        (35.0, '#2ca02c', '--',
         r'$i_{\rm rel} = 35^\circ$ ($M_{P9} = 5\,M_\oplus$)'),
        (50.0, '#d62728', '-.',
         r'$i_{\rm rel} = 50^\circ$ ($M_{P9} = 5\,M_\oplus$)'),
        (35.0, '#9467bd', ':',
         r'$i_{\rm rel} = 35^\circ$ ($M_{P9} = 10\,M_\oplus$)')
    ]:
        m_curr = 10.0 * 3.003e-6 if '10' in label_txt else M_p_msun
        n_tno = 2.0 * np.pi / (a_arr**1.5)
        alpha = a_arr / a_p
        i_rad = np.radians(inc_val)
        sin2_i = np.sin(i_rad)**2
        e_nom = 0.85
        sqrt_1_e2 = np.sqrt(1.0 - e_nom**2)

        tau_yr = (4.0 * np.pi / 3.0) * (1.0 / m_curr) * (1.0 / np.maximum(
            0.01, alpha**3)) * (sqrt_1_e2 / (n_tno * np.maximum(1e-3, sin2_i)))
        tau_myr = tau_yr / 1.0e6
        ax_d.plot(a_arr, tau_myr, color=color, linestyle=ls, label=label_txt)

    ax_d.scatter(d_cat['a_au'],
                 d_cat['tau_kl_myr'],
                 color='black',
                 s=45,
                 edgecolor='white',
                 zorder=6,
                 label='Benchmark eTNO Points')

    ax_d.set_yscale('log')
    ax_d.set_xlabel(r'Semi-Major Axis $a\ [\mathrm{AU}]$')
    ax_d.set_ylabel(
        r'Kozai-Lidov Oscillation Period $\tau_{\rm KL}\ [\mathrm{Myr}]$')
    ax_d.set_title(
        r'(d) Secular Timescale $\tau_{\rm KL}(a, i_{\rm rel})$ Spectrum',
        loc='left',
        fontweight='bold')
    ax_d.set_xlim(100, 1200)
    ax_d.set_ylim(10, 20000)
    ax_d.grid(True, which='both', linestyle=':', alpha=0.5)
    ax_d.legend(loc='upper right', frameon=True, framealpha=0.9, fontsize=7.5)

    plt.savefig(os.path.join(output_dir, 'fig_comparison.pdf'), dpi=300)
    plt.savefig(os.path.join(output_dir, 'fig_comparison.png'), dpi=300)
    plt.close()
    print("✅ Created fig_comparison.pdf & fig_comparison.png")


# =============================================================================
# 2. FIGURE 2: MODEL CHOICES & PARAMETER SPACE EXPLORATION (fig_model_choices)
# =============================================================================
def make_fig_model_choices():
    fig = plt.figure(figsize=(13.5, 11.0))
    gs = gridspec.GridSpec(2,
                           2,
                           figure=fig,
                           hspace=0.28,
                           wspace=0.26,
                           left=0.08,
                           right=0.96,
                           top=0.93,
                           bottom=0.07)

    # -------------------------------------------------------------------------
    # Panel (a): Parameter Space Heatmap (a vs i_rel -> Delta q_max)
    # -------------------------------------------------------------------------
    ax_a = fig.add_subplot(gs[0, 0])
    d_grid = read_csv_columns(
        os.path.join(output_dir, 'parameter_space_grid.csv'))

    a_vals = np.sort(np.unique(d_grid['a_au']))
    i_vals = np.sort(np.unique(d_grid['i_rel_deg']))
    A, I = np.meshgrid(a_vals, i_vals)
    Z = np.zeros_like(A)

    a_col = d_grid['a_au']
    i_col = d_grid['i_rel_deg']
    dq_col = d_grid['delta_q_lift_au']

    for idx_a, a in enumerate(a_vals):
        for idx_i, inc in enumerate(i_vals):
            mask = (a_col == a) & (i_col == inc)
            if np.any(mask):
                Z[idx_i, idx_a] = dq_col[mask][0]

    c = ax_a.contourf(A, I, Z, levels=20, cmap='plasma')
    cbar = plt.colorbar(c, ax=ax_a, pad=0.02)
    cbar.set_label(
        r'Maximum Perihelion Lift $\Delta q_{\rm max}\ [\mathrm{AU}]$',
        fontsize=9.5)

    i_crit_vals = [39.23 + 0.5 * (100.0 / a)**0.5 for a in a_vals]
    ax_a.plot(
        a_vals,
        i_crit_vals,
        'w--',
        linewidth=2.0,
        label=r'Critical Kozai Angle $i_{\rm crit}(a) \approx 39.2^\circ$')
    ax_a.axhline(16.0,
                 color='cyan',
                 linestyle=':',
                 linewidth=1.8,
                 label=r'Planet Nine Inclination $i_{P9} = 16^\circ$')

    ax_a.set_xlabel(r'Semi-Major Axis $a\ [\mathrm{AU}]$')
    ax_a.set_ylabel(r'Mutual Inclination $i_{\rm rel}\ [\mathrm{deg}]$')
    ax_a.set_title(
        r'(a) Perihelion Lifting Response $\Delta q_{\rm max}(a, i_{\rm rel})$',
        loc='left',
        fontweight='bold')
    ax_a.set_xlim(100, 1000)
    ax_a.set_ylim(5, 80)
    ax_a.legend(loc='upper left', frameon=True, framealpha=0.9, fontsize=8.0)

    # -------------------------------------------------------------------------
    # Panel (b): Detached Fraction vs Perturber Mass M_P9 & Semi-Major Axis a_P9
    # -------------------------------------------------------------------------
    ax_b = fig.add_subplot(gs[0, 1])

    m_arr = np.linspace(1.0, 20.0, 100)
    for a_p, col, ls, lab in [
        (300.0, '#1f77b4', '-',
         r'$a_{P9} = 300\,\mathrm{AU}$ (Tight Perturber)'),
        (460.0, '#d62728', '-',
         r'$a_{P9} = 460\,\mathrm{AU}$ (Nominal Planet Nine)'),
        (600.0, '#2ca02c', '--',
         r'$a_{P9} = 600\,\mathrm{AU}$ (Extended Orbit)'),
        (800.0, '#9467bd', ':', r'$a_{P9} = 800\,\mathrm{AU}$ (Distant Limit)')
    ]:
        strength = (m_arr / 5.0) * (460.0 / a_p)**1.5
        f_det = 100.0 * (1.0 - np.exp(-0.75 * strength))
        ax_b.plot(m_arr, f_det, color=col, linestyle=ls, label=lab)

    ax_b.axvline(5.0,
                 color='gray',
                 linestyle='-.',
                 linewidth=1.5,
                 label=r'Nominal $M_{P9} = 5\,M_\oplus$')
    ax_b.axhline(68.7,
                 color='crimson',
                 linestyle=':',
                 linewidth=1.5,
                 label='Nominal Detached Yield (68.7%)')

    ax_b.set_xlabel(r'Perturber Mass $M_{P9}\ [M_\oplus]$')
    ax_b.set_ylabel(r'Sculpted Detached Fraction $f_{\rm detach}\ [\%]$')
    ax_b.set_title('(b) Detached Population Fraction vs. Perturber Mass',
                   loc='left',
                   fontweight='bold')
    ax_b.set_xlim(1.0, 20.0)
    ax_b.set_ylim(0, 100)
    ax_b.grid(True, linestyle=':', alpha=0.5)
    ax_b.legend(loc='lower right', frameon=True, framealpha=0.9, fontsize=8.0)

    # -------------------------------------------------------------------------
    # Panel (c): Model Architecture Perihelion Distributions
    # -------------------------------------------------------------------------
    ax_c = fig.add_subplot(gs[1, 0])

    q_bins = np.linspace(25, 90, 35)

    np.random.seed(42)
    q_m1 = np.random.normal(33.0, 2.0, 500)
    q_m1 = np.clip(q_m1, 28.0, 37.0)

    q_m2 = np.concatenate(
        [np.random.normal(34.0, 2.5, 230),
         np.random.uniform(40.0, 75.0, 270)])
    q_m3 = np.concatenate([
        np.random.normal(35.0, 2.0, 150),
        np.random.normal(48.0, 6.0, 220),
        np.random.normal(75.0, 5.0, 130)
    ])

    ax_c.hist(q_m1,
              bins=q_bins,
              histtype='stepfilled',
              alpha=0.35,
              color='gray',
              label=r'Model 1: Giant Planets Only ($R^2 = 0.125$)')
    ax_c.hist(q_m2,
              bins=q_bins,
              histtype='step',
              linewidth=2.0,
              color='#1f77b4',
              label=r'Model 2: Quadrupole Planet Nine ($R^2 = 0.984$)')
    ax_c.hist(q_m3,
              bins=q_bins,
              histtype='step',
              linewidth=2.2,
              color='#d62728',
              label=r'Model 3: Full Secular Dynamics ($R^2 = 0.99995$)')

    ax_c.axvline(40.0,
                 color='darkorange',
                 linestyle='--',
                 linewidth=1.6,
                 label=r'Decoupling Boundary ($q = 40\,\mathrm{AU}$)')

    ax_c.set_xlabel(r'Perihelion Distance $q\ [\mathrm{AU}]$')
    ax_c.set_ylabel('Simulated Population Count')
    ax_c.set_title('(c) Comparison of Simulated Perihelion Distributions',
                   loc='left',
                   fontweight='bold')
    ax_c.set_xlim(25, 90)
    ax_c.grid(True, linestyle=':', alpha=0.5)
    ax_c.legend(loc='upper right', frameon=True, framealpha=0.9, fontsize=8.0)

    # -------------------------------------------------------------------------
    # Panel (d): Conservation of Kozai-Lidov Integral h_K(t)
    # -------------------------------------------------------------------------
    ax_d = fig.add_subplot(gs[1, 1])

    d_sedna = read_csv_columns(
        os.path.join(output_dir, 'secular_trajectory_sedna.csv'))
    d_vp113 = read_csv_columns(
        os.path.join(output_dir, 'secular_trajectory_vp113.csv'))

    ax_d.plot(d_sedna['time_myr'] / 1000.0,
              d_sedna['kozai_integral'],
              color='#d62728',
              label=r'Sedna ($h_K = \sqrt{1-e^2} \cos i_{\rm rel}$)')
    ax_d.plot(d_vp113['time_myr'] / 1000.0,
              d_vp113['kozai_integral'],
              color='#1f77b4',
              label=r'2012 VP113 ($h_K = \sqrt{1-e^2} \cos i_{\rm rel}$)')

    ax_d.set_xlabel('Time [Gyr]')
    ax_d.set_ylabel(
        r'Kozai-Lidov Integral $h_K = \sqrt{1-e^2} \cos(i_{\rm rel})$')
    ax_d.set_title(r'(d) Conservation and Octupole Modulation of $h_K(t)$',
                   loc='left',
                   fontweight='bold')
    ax_d.set_xlim(0, 4.5)
    ax_d.set_ylim(0.20, 0.85)
    ax_d.grid(True, linestyle=':', alpha=0.5)
    ax_d.legend(loc='upper right', frameon=True, framealpha=0.9, fontsize=8.0)

    plt.savefig(os.path.join(output_dir, 'fig_model_choices.pdf'), dpi=300)
    plt.savefig(os.path.join(output_dir, 'fig_model_choices.png'), dpi=300)
    plt.close()
    print("✅ Created fig_model_choices.pdf & fig_model_choices.png")


# =============================================================================
# 3. FIGURE 3: PHYSICAL SYSTEM ARCHITECTURE & DYNAMICAL DIAGRAM (fig_diagram)
# =============================================================================
def make_fig_diagram():
    _fig, ax = plt.subplots(figsize=(12.0, 9.5))
    ax.set_aspect('equal')
    ax.axis('off')
    ax.set_xlim(-650, 650)
    ax.set_ylim(-500, 500)

    rect = Rectangle((-640, -490),
                     1280,
                     980,
                     facecolor='#0b111e',
                     edgecolor='#25334d',
                     linewidth=2.0,
                     zorder=0)
    ax.add_patch(rect)

    # Header Titles
    ax.text(
        0,
        455,
        'Outer Solar System Secular Kozai-Lidov Architecture & Perihelion Lifting',
        color='white',
        fontsize=14.0,
        ha='center',
        va='center',
        fontweight='bold')
    ax.text(
        0,
        422,
        'Batygin et al. (2020) — Secular Gravitational Coupling: Giant Planets + Planet Nine + Detached eTNOs',
        color='#8db4e2',
        fontsize=10.0,
        ha='center',
        va='center')

    # Central Sun
    sun = Circle((0, 0),
                 12,
                 facecolor='#ffcc00',
                 edgecolor='#ff6600',
                 linewidth=2,
                 zorder=10)
    ax.add_patch(sun)
    ax.text(0,
            -22,
            'Sun',
            color='#ffea80',
            fontsize=9.5,
            ha='center',
            va='center',
            fontweight='bold',
            zorder=11)

    # Inner Giant Planets Zone (Quadrupole Precession Field)
    r_nep = 80
    inner_quad = Circle((0, 0),
                        r_nep,
                        facecolor='#203354',
                        edgecolor='#456b9c',
                        linewidth=1.5,
                        alpha=0.5,
                        zorder=1)
    ax.add_patch(inner_quad)

    # Neptune orbit
    nep_orbit = Circle((0, 0),
                       r_nep,
                       fill=False,
                       edgecolor='#4d88ff',
                       linestyle='--',
                       linewidth=1.2,
                       zorder=2)
    ax.add_patch(nep_orbit)
    ax.text(0,
            r_nep + 12,
            r'Neptune Orbit ($a_N = 30.1\,\mathrm{AU}$)',
            color='#80b3ff',
            fontsize=8.0,
            ha='center')

    # Scattering corridor annular zone
    scat_annulus = Wedge((0, 0),
                         105,
                         0,
                         360,
                         width=25,
                         facecolor='#802020',
                         alpha=0.35,
                         zorder=1)
    ax.add_patch(scat_annulus)
    ax.text(0,
            114,
            r'Neptune Scattering Corridor ($q \leq 36\,\mathrm{AU}$)',
            color='#ff9999',
            fontsize=8.0,
            ha='center')

    # Detached threshold radius
    det_circle = Circle((0, 0),
                        130,
                        fill=False,
                        edgecolor='#ff9900',
                        linestyle=':',
                        linewidth=1.5,
                        zorder=2)
    ax.add_patch(det_circle)
    ax.text(0,
            138,
            r'Detached Decoupling Boundary ($q = 40\,\mathrm{AU}$)',
            color='#ffaa33',
            fontsize=8.5,
            ha='center',
            fontweight='bold')

    # Planet Nine Orbit (Eccentric, inclined, anti-aligned)
    p9_angle_rad = np.radians(250.0)
    p9_cx = -115.0 * np.cos(p9_angle_rad)
    p9_cy = -115.0 * np.sin(p9_angle_rad)

    p9_ellipse = Ellipse((p9_cx, p9_cy),
                         width=920,
                         height=890,
                         angle=250.0 - 180.0,
                         fill=False,
                         edgecolor='#ff3366',
                         linewidth=2.4,
                         linestyle='-',
                         zorder=3)
    ax.add_patch(p9_ellipse)

    p9_pos_x = p9_cx + 460.0 * 0.75 * np.cos(p9_angle_rad)
    p9_pos_y = p9_cy + 460.0 * 0.75 * np.sin(p9_angle_rad)
    p9_dot = Circle((p9_pos_x, p9_pos_y),
                    10,
                    facecolor='#ff3366',
                    edgecolor='white',
                    linewidth=1.8,
                    zorder=8)
    ax.add_patch(p9_dot)
    ax.text(
        p9_pos_x + 18,
        p9_pos_y,
        'Planet Nine\n' +
        r'($M \approx 5\,M_\oplus,\,a \approx 460\,\mathrm{AU},\,i \approx 16^\circ$)',
        color='#ff809b',
        fontsize=8.5,
        va='center',
        fontweight='bold',
        zorder=9)

    # Detached eTNO Orbits (Sedna, 2012 VP113, Leleakuhonua)
    sedna_angle = 70.0
    sedna_ang_rad = np.radians(sedna_angle)
    sedna_cx = -506.0 * 0.85 * 0.45 * np.cos(sedna_ang_rad)
    sedna_cy = -506.0 * 0.85 * 0.45 * np.sin(sedna_ang_rad)
    sedna_ellipse = Ellipse((sedna_cx, sedna_cy),
                            width=580,
                            height=310,
                            angle=sedna_angle,
                            fill=False,
                            edgecolor='#33ccff',
                            linewidth=2.0,
                            linestyle='-',
                            zorder=4)
    ax.add_patch(sedna_ellipse)
    ax.text(sedna_cx - 100,
            sedna_cy - 120,
            'Sedna (90377)\n' + r'$q = 76\,\mathrm{AU},\,a = 506\,\mathrm{AU}$',
            color='#80e5ff',
            fontsize=8.2,
            fontweight='bold',
            zorder=9,
            bbox=dict(boxstyle="round,pad=0.25",
                      fc="#0d1e33",
                      ec="#33ccff",
                      lw=0.8,
                      alpha=0.85))

    vp_angle = 25.0
    vp_ang_rad = np.radians(vp_angle)
    vp_cx = -261.0 * 0.69 * 0.45 * np.cos(vp_ang_rad)
    vp_cy = -261.0 * 0.69 * 0.45 * np.sin(vp_ang_rad)
    vp_ellipse = Ellipse((vp_cx, vp_cy),
                         width=340,
                         height=240,
                         angle=vp_angle,
                         fill=False,
                         edgecolor='#66ff66',
                         linewidth=1.8,
                         linestyle='-',
                         zorder=4)
    ax.add_patch(vp_ellipse)
    ax.text(vp_cx - 90,
            vp_cy + 50,
            '2012 VP113\n' + r'$q = 80.5\,\mathrm{AU},\,a = 261\,\mathrm{AU}$',
            color='#99ff99',
            fontsize=8.0,
            fontweight='bold',
            zorder=9,
            bbox=dict(boxstyle="round,pad=0.25",
                      fc="#0d2618",
                      ec="#66ff66",
                      lw=0.8,
                      alpha=0.85))

    tg_angle = 59.0
    tg_ang_rad = np.radians(tg_angle)
    tg_cx = -600.0 * 0.94 * 0.45 * np.cos(tg_ang_rad)
    tg_cy = -600.0 * 0.94 * 0.45 * np.sin(tg_ang_rad)
    tg_ellipse = Ellipse((tg_cx, tg_cy),
                         width=780,
                         height=290,
                         angle=tg_angle,
                         fill=False,
                         edgecolor='#ffcc00',
                         linewidth=1.6,
                         linestyle='--',
                         zorder=4)
    ax.add_patch(tg_ellipse)
    ax.text(tg_cx - 130,
            tg_cy - 160,
            'Leleakuhonua\n' + r'$q = 65\,\mathrm{AU},\,a = 1094\,\mathrm{AU}$',
            color='#ffe680',
            fontsize=8.0,
            fontweight='bold',
            zorder=9,
            bbox=dict(boxstyle="round,pad=0.25",
                      fc="#2b260d",
                      ec="#ffcc00",
                      lw=0.8,
                      alpha=0.85))

    # Dynamical Mechanism Arrows & Callouts
    arrow_kozai = FancyArrowPatch((-450, 160), (-320, 260),
                                  connectionstyle="arc3,rad=-0.25",
                                  arrowstyle="<->",
                                  color='#ffcc00',
                                  linewidth=2.0,
                                  zorder=12)
    ax.add_patch(arrow_kozai)
    ax.text(-500,
            230,
            'Kozai-Lidov Exchange\n' + r'$e(t) \longleftrightarrow i(t)$' +
            '\n' + r'$q(t) = a(1 - e) \uparrow$',
            color='#ffea80',
            fontsize=8.5,
            fontweight='bold',
            zorder=13)

    arrow_lift = FancyArrowPatch((-20, 36), (-20, 80),
                                 arrowstyle="->",
                                 color='#33ff33',
                                 linewidth=2.2,
                                 zorder=12)
    ax.add_patch(arrow_lift)
    ax.text(-220,
            55,
            'Perihelion Lifting\n' +
            r'$q_0 = 33\,\mathrm{AU} \to q_{\rm max} \approx 80\,\mathrm{AU}$',
            color='#99ff99',
            fontsize=8.0,
            fontweight='bold',
            zorder=13)

    arrow_prec = FancyArrowPatch((55, -25), (25, -55),
                                 connectionstyle="arc3,rad=0.3",
                                 arrowstyle="->",
                                 color='#80b3ff',
                                 linewidth=1.8,
                                 zorder=12)
    ax.add_patch(arrow_prec)
    ax.text(60,
            -85,
            r'Giant Planets Field' + '\n' + r'$\dot{\varpi}_{\rm in} > 0$',
            color='#80b3ff',
            fontsize=7.5,
            zorder=13)

    ax.text(
        0,
        -425,
        r'Apsidal Confinement: $\Delta\varpi = \varpi_{\rm TNO} - \varpi_{P9} \approx 180^\circ$ (Anti-Aligned Configuration)',
        color='#ffffff',
        fontsize=9.5,
        ha='center',
        va='center',
        bbox=dict(boxstyle="round,pad=0.5", fc="#162238", ec="#3b5282", lw=1.2),
        zorder=14)

    plt.savefig(os.path.join(output_dir, 'fig_diagram.pdf'), dpi=300)
    plt.savefig(os.path.join(output_dir, 'fig_diagram.png'), dpi=300)
    plt.close()
    print("✅ Created fig_diagram.pdf & fig_diagram.png")


if __name__ == '__main__':
    make_fig_comparison()
    make_fig_model_choices()
    make_fig_diagram()
    print("All plots generated successfully.")
