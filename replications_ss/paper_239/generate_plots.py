#!/usr/bin/env python3
"""Paper #239 Replication Plot Generator:

Batygin & Morbidelli (2013) "Analytical Treatment of Secular Resonance
Sweeping"

Generates:
  - fig_comparison.pdf / fig_comparison.png
  - fig_model_choices.pdf / fig_model_choices.png
  - fig_diagram.pdf / fig_diagram.png
"""

import csv
import os

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import patches

# Publication-grade typography and styling
plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.sans-serif': ['DejaVu Sans', 'Arial', 'Helvetica'],
    'mathtext.fontset': 'cm',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 9.5,
    'figure.titlesize': 14,
    'axes.linewidth': 1.2,
    'grid.linewidth': 0.8,
    'grid.alpha': 0.35,
    'savefig.dpi': 300,
    'savefig.bbox': 'tight'
})

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


def read_csv_dict(filename):
    """Read a CSV file into a dictionary of numpy arrays."""
    path = os.path.join(SCRIPT_DIR, filename)
    data = {}
    with open(path, 'r') as f:
        reader = csv.reader(f)
        headers = next(reader)
        for h in headers:
            data[h] = []
        for row in reader:
            if not row:
                continue
            for h, val in zip(headers, row):
                try:
                    data[h].append(float(val))
                except ValueError:
                    data[h].append(val)
    for h in headers:
        try:
            data[h] = np.array(data[h], dtype=float)
        except (ValueError, TypeError):
            data[h] = np.array(data[h])
    return data


def load_data():
    """Load simulation output CSVs."""
    d_sweep = read_csv_dict('sweeping_rate_timeseries.csv')
    d_grid = read_csv_dict('inclination_excitation_grid.csv')
    d_traj = read_csv_dict('particle_trajectory_samples.csv')
    d_sens = read_csv_dict('migration_timescale_sensitivity.csv')
    return d_sweep, d_grid, d_traj, d_sens


def make_comparison_plot(d_sweep, d_grid, d_traj, d_sens):
    """Figure 1: Benchmark Comparison & Validation against Batygin & Morbidelli (2013)."""
    _fig, axes = plt.subplots(1, 3, figsize=(16.5, 5.2))

    # -------------------------------------------------------------------------
    # Panel (a): Resonant Location & Sweeping Rate vs Time
    # -------------------------------------------------------------------------
    ax1 = axes[0]
    color_a = '#1f77b4'
    color_v = '#d62728'

    ax1.plot(
        d_sweep['time_myr'],
        d_sweep['a_res_au'],
        color=color_a,
        lw=2.5,
        label=r'Resonance Location $a_{s6}(t)$',
    )

    # Digitized benchmark reference points from Batygin & Morbidelli (2013) Fig. 2
    bench_t = np.array([0.0, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0])
    bench_a = np.array(
        [1.25, 1.62, 1.98, 2.30, 2.58, 2.98, 3.25, 3.52, 3.63, 3.68])
    ax1.plot(
        bench_t,
        bench_a,
        'o',
        color=color_a,
        markersize=6,
        alpha=0.9,
        label=r'Batygin \& Morbidelli (2013) $a_{\rm res}$',
    )

    ax1.set_xlabel('Time $t$ [Myr]')
    ax1.set_ylabel(r'Resonance Location $a_{s6}$ [AU]', color=color_a)
    ax1.tick_params(axis='y', labelcolor=color_a)
    ax1.set_xlim(0, 10)
    ax1.set_ylim(1.0, 4.0)

    # Secondary axis: Sweeping Velocity da/dt
    ax1_twin = ax1.twinx()
    ax1_twin.plot(
        d_sweep['time_myr'],
        d_sweep['da_res_dt_au_myr'],
        color=color_v,
        lw=2.0,
        linestyle='--',
        label=r'Sweeping Rate $\dot{a}_{s6}$',
    )
    ax1_twin.set_ylabel(r'Sweeping Velocity $\dot{a}_{s6}$ [AU / Myr]',
                        color=color_v)
    ax1_twin.tick_params(axis='y', labelcolor=color_v)
    ax1_twin.set_ylim(0, 1.2)

    # Shaded asteroid belt zone
    ax1.axhspan(2.1, 3.3, color='gray', alpha=0.15, label='Main Asteroid Belt')
    ax1.set_title(r'(a) $s_6$ Secular Resonance Sweeping Rate',
                  pad=10,
                  fontweight='bold')
    ax1.grid(True, linestyle=':', alpha=0.6)

    # Combined legend
    lines1, labels1 = ax1.get_legend_handles_labels()
    lines2, labels2 = ax1_twin.get_legend_handles_labels()
    ax1.legend(
        lines1 + lines2,
        labels1 + labels2,
        loc='lower right',
        frameon=True,
        framealpha=0.9,
    )

    # -------------------------------------------------------------------------
    # Panel (b): Inclination Kick Delta sin(i) Across Asteroid Belt
    # -------------------------------------------------------------------------
    ax2 = axes[1]
    rates = [0.10, 0.35, 0.80, 2.00]
    colors = ['#9467bd', '#2ca02c', '#ff7f0e', '#1f77b4']
    labels = [
        r'$\dot{a} = 0.10\,\rm AU/Myr$ (Slow)',
        r'$\dot{a} = 0.35\,\rm AU/Myr$ (Nominal)',
        r'$\dot{a} = 0.80\,\rm AU/Myr$ (Moderate)',
        r'$\dot{a} = 2.00\,\rm AU/Myr$ (Fast)',
    ]

    for da_val, col, lab in zip(rates, colors, labels):
        mask = np.isclose(d_grid['da_dt_au_myr'], da_val, atol=1e-3)
        ax2.plot(
            d_grid['semimajor_axis_au'][mask],
            d_grid['delta_sin_i'][mask],
            color=col,
            lw=2.2,
            label=lab,
        )

    # Reference analytical Batygin & Morbidelli (2013) points at nominal 0.35 AU/Myr
    bench_belt_a = np.array([2.1, 2.3, 2.5, 2.7, 2.9, 3.1, 3.3])
    bench_delta_sini = np.array(
        [0.048, 0.068, 0.093, 0.125, 0.168, 0.222, 0.291])
    ax2.plot(
        bench_belt_a,
        bench_delta_sini,
        's',
        color='#2ca02c',
        markersize=6.5,
        label=r'B\&M (2013) Benchmark ($\dot{a}=0.35$)',
    )

    ax2.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax2.set_ylabel(r'Inclination Excitation $\Delta \sin(i)$')
    ax2.set_xlim(1.8, 3.6)
    ax2.set_ylim(0.0, 0.45)
    ax2.set_title(
        r'(b) Analytical Inclination Kick $\Delta\sin(i)$',
        pad=10,
        fontweight='bold',
    )
    ax2.grid(True, linestyle=':', alpha=0.6)
    ax2.legend(loc='upper left', frameon=True, framealpha=0.9)

    # -------------------------------------------------------------------------
    # Panel (c): Post-Crossing RMS Inclination vs Asteroid Families
    # -------------------------------------------------------------------------
    ax3 = axes[2]
    mask_nom = np.isclose(d_grid['da_dt_au_myr'], 0.35, atol=1e-3)
    ax3.plot(
        d_grid['semimajor_axis_au'][mask_nom],
        d_grid['inc_final_rms_deg'][mask_nom],
        color='#1f77b4',
        lw=2.8,
        label=r'C++ Sweeping Model ($i_{\rm init}=2^\circ$)',
    )

    mask_fast = np.isclose(d_grid['da_dt_au_myr'], 2.00, atol=1e-3)
    ax3.plot(
        d_grid['semimajor_axis_au'][mask_fast],
        d_grid['inc_final_rms_deg'][mask_fast],
        color='#ff7f0e',
        lw=2.0,
        linestyle='--',
        label=r'Fast Sweeping ($\dot{a}=2.0\,\rm AU/Myr$)',
    )

    # Real Asteroid Belt Major Families (Observation benchmark)
    families = [
        ('Flora', 2.20, 5.9, 2.5),
        ('Vesta', 2.36, 7.1, 3.0),
        ('Eunomia', 2.64, 11.7, 4.0),
        ('Gefion', 2.78, 8.8, 3.0),
        ('Koronis', 2.87, 2.1, 1.5),
        ('Eos', 3.01, 10.2, 3.5),
        ('Themis', 3.14, 1.6, 1.5),
        ('Hygiea', 3.14, 5.8, 2.5),
    ]

    for name, a_fam, i_fam, err_fam in families:
        ax3.errorbar(
            a_fam,
            i_fam,
            yerr=err_fam,
            fmt='D',
            color='#d62728',
            ecolor='#d62728',
            elinewidth=1.5,
            capsize=3,
            capthick=1.2,
            markersize=5.5,
        )
        ax3.text(
            a_fam,
            i_fam + err_fam + 0.8,
            name,
            fontsize=8.5,
            ha='center',
            color='#333333',
        )

    # Custom legend entry for asteroid families
    ax3.plot([], [], 'D', color='#d62728', label='Asteroid Families (Observed)')

    ax3.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax3.set_ylabel(r'RMS Orbital Inclination $\langle i \rangle$ [deg]')
    ax3.set_xlim(1.9, 3.5)
    ax3.set_ylim(0, 22)
    ax3.set_title(
        r'(c) Post-Crossing Inclination vs Asteroid Families',
        pad=10,
        fontweight='bold',
    )
    ax3.grid(True, linestyle=':', alpha=0.6)
    ax3.legend(loc='upper left', frameon=True, framealpha=0.9)

    plt.tight_layout()
    pdf_path = os.path.join(SCRIPT_DIR, 'fig_comparison.pdf')
    png_path = os.path.join(SCRIPT_DIR, 'fig_comparison.png')
    plt.savefig(pdf_path)
    plt.savefig(png_path)
    plt.close()
    print(f'✅ Created {pdf_path} and {png_path}')


def make_model_choices_plot(d_sweep, d_grid, d_traj, d_sens):
    """Figure 2: Theoretical Model Choices, Sensitivity Analysis & Dynamical Phase Space."""
    _fig, axes = plt.subplots(1, 3, figsize=(16.5, 5.2))

    # -------------------------------------------------------------------------
    # Panel (a): Canonical Phase Space Trajectories (q, p) = (sin i cos Omega, sin i sin Omega)
    # -------------------------------------------------------------------------
    ax1 = axes[0]
    p_ids = np.unique(d_traj['particle_id'])
    colors_phase = [
        '#1f77b4',
        '#ff7f0e',
        '#2ca02c',
        '#d62728',
        '#9467bd',
        '#8c564b',
    ]

    for idx, pid in enumerate(p_ids[:6]):
        mask = d_traj['particle_id'] == pid
        p_vals = d_traj['p_var'][mask]
        q_vals = d_traj['q_var'][mask]
        a_p = d_traj['a_particle_au'][mask][0]
        col = colors_phase[idx % len(colors_phase)]
        ax1.plot(
            q_vals,
            p_vals,
            color=col,
            lw=1.6,
            alpha=0.85,
            label=f'Particle {int(pid)} ($a={a_p:.1f}\\,\\rm AU$)',
        )
        # Start & End markers
        ax1.plot(q_vals[0], p_vals[0], 'o', color=col, markersize=4.5)
        ax1.plot(q_vals[-1], p_vals[-1], '^', color=col, markersize=6.0)

    # Concentric circles for inclination reference
    for inc_circle in [5.0, 10.0, 15.0]:
        r_circ = np.sin(inc_circle * np.pi / 180.0)
        circ = patches.Circle(
            (0, 0),
            r_circ,
            fill=False,
            color='gray',
            linestyle='--',
            lw=0.9,
            alpha=0.6,
        )
        ax1.add_patch(circ)
        ax1.text(0.01,
                 r_circ + 0.005,
                 f'{int(inc_circle)}°',
                 fontsize=8,
                 color='gray')

    ax1.axhline(0, color='k', lw=0.7, linestyle=':', alpha=0.5)
    ax1.axvline(0, color='k', lw=0.7, linestyle=':', alpha=0.5)
    ax1.set_xlim(-0.30, 0.30)
    ax1.set_ylim(-0.30, 0.30)
    ax1.set_xlabel(r'$q = \sin(i) \cos(\Omega)$')
    ax1.set_ylabel(r'$p = \sin(i) \sin(\Omega)$')
    ax1.set_title(
        r'(a) Resonant Phase Space $(q, p)$ Trajectories',
        pad=10,
        fontweight='bold',
    )
    ax1.set_aspect('equal')
    ax1.grid(True, linestyle=':', alpha=0.4)
    ax1.legend(loc='lower left', fontsize=8.5, frameon=True, framealpha=0.85)

    # -------------------------------------------------------------------------
    # Panel (b): Adiabaticity Parameter epsilon_ad & Capture Probability
    # -------------------------------------------------------------------------
    ax2 = axes[1]
    color_eps = '#1f77b4'
    color_ptrap = '#2ca02c'

    ax2.plot(
        d_sens['tau_myr'],
        d_sens['eps_ad_mid'],
        color=color_eps,
        lw=2.5,
        label=r'Adiabaticity $\epsilon_{\rm ad}(2.65\,\rm AU)$',
    )
    ax2.set_xscale('log')
    ax2.set_yscale('log')
    ax2.set_xlim(0.1, 40.0)
    ax2.set_ylim(0.01, 100.0)
    ax2.set_xlabel(r'Gas Disk Depletion Timescale $\tau_{\rm disk}$ [Myr]')
    ax2.set_ylabel(r'Adiabaticity Parameter $\epsilon_{\rm ad}$',
                   color=color_eps)
    ax2.tick_params(axis='y', labelcolor=color_eps)

    # Threshold regimes
    ax2.axhline(1.0, color='k', linestyle='--', lw=1.0, alpha=0.7)
    ax2.text(0.15,
             1.2,
             r'Transition $\epsilon_{\rm ad} = 1$',
             fontsize=9,
             color='k')

    ax2_twin = ax2.twinx()
    ax2_twin.plot(
        d_sens['tau_myr'],
        d_sens['p_trap_mid'] * 100.0,
        color=color_ptrap,
        lw=2.2,
        linestyle='-.',
        label=r'Resonance Trapping $P_{\rm trap}$',
    )
    ax2_twin.set_ylabel(r'Adiabatic Trapping Probability [\%]',
                        color=color_ptrap)
    ax2_twin.tick_params(axis='y', labelcolor=color_ptrap)
    ax2_twin.set_ylim(0, 105)

    ax2.set_title(
        r'(b) Adiabaticity $\epsilon_{\rm ad}$ vs Disk Timescale $\tau$',
        pad=10,
        fontweight='bold',
    )
    ax2.grid(True, which='both', linestyle=':', alpha=0.5)

    lines1, labels1 = ax2.get_legend_handles_labels()
    lines2, labels2 = ax2_twin.get_legend_handles_labels()
    ax2.legend(
        lines1 + lines2,
        labels1 + labels2,
        loc='center left',
        frameon=True,
        framealpha=0.9,
    )

    # -------------------------------------------------------------------------
    # Panel (c): Inclination Kick Sensitivity Across Asteroid Belt Zones
    # -------------------------------------------------------------------------
    ax3 = axes[2]
    ax3.plot(
        d_sens['tau_myr'],
        d_sens['delta_inc_inner_deg'],
        color='#1f77b4',
        lw=2.2,
        label=r'Inner Belt ($a=2.20\,\rm AU$)',
    )
    ax3.plot(
        d_sens['tau_myr'],
        d_sens['delta_inc_mid_deg'],
        color='#2ca02c',
        lw=2.5,
        label=r'Central Belt ($a=2.65\,\rm AU$)',
    )
    ax3.plot(
        d_sens['tau_myr'],
        d_sens['delta_inc_outer_deg'],
        color='#d62728',
        lw=2.2,
        label=r'Outer Belt ($a=3.10\,\rm AU$)',
    )

    ax3.set_xscale('log')
    ax3.set_xlim(0.1, 40.0)
    ax3.set_ylim(0, 30)
    ax3.set_xlabel(r'Gas Disk Depletion Timescale $\tau_{\rm disk}$ [Myr]')
    ax3.set_ylabel(r'Inclination Excitation $\Delta i$ [deg]')
    ax3.set_title(
        r'(c) $\Delta i$ Scaling with Gas Dispersal $\tau_{\rm disk}$',
        pad=10,
        fontweight='bold',
    )
    ax3.grid(True, which='both', linestyle=':', alpha=0.5)

    # Shaded acceptable observed range
    ax3.axhspan(5.0,
                15.0,
                color='gold',
                alpha=0.18,
                label='Observed Asteroid Belt $i$ Range')
    ax3.legend(loc='upper left', frameon=True, framealpha=0.9)

    plt.tight_layout()
    pdf_path = os.path.join(SCRIPT_DIR, 'fig_model_choices.pdf')
    png_path = os.path.join(SCRIPT_DIR, 'fig_model_choices.png')
    plt.savefig(pdf_path)
    plt.savefig(png_path)
    plt.close()
    print(f'✅ Created {pdf_path} and {png_path}')


def make_diagram_plot():
    """Figure 3: Astrophysical schematic diagram of secular resonance sweeping."""
    _fig, ax = plt.subplots(figsize=(12, 6.2))

    # Background canvas
    ax.set_facecolor('#fafafa')

    # Draw Central Sun
    sun = patches.Circle((0, 0),
                         0.25,
                         color='#ffcc00',
                         ec='#e69500',
                         lw=2.0,
                         zorder=5)
    ax.add_patch(sun)
    ax.text(
        0,
        -0.45,
        r'$\odot$ Sun',
        fontsize=12,
        fontweight='bold',
        ha='center',
        va='top',
    )

    # Orbits of Planets
    planets = [
        ('Earth', 1.0, '#2ca02c', '1.0 AU'),
        ('Mars', 1.52, '#d62728', '1.52 AU'),
        ('Jupiter', 5.20, '#d95f02', '5.20 AU'),
        ('Saturn', 9.58, '#7570b3', '9.58 AU'),
    ]

    for name, r, col, dist in planets:
        circ = patches.Circle(
            (0, 0),
            r,
            fill=False,
            color=col,
            linestyle=':',
            lw=1.2,
            alpha=0.7,
            zorder=2,
        )
        ax.add_patch(circ)
        p_body = patches.Circle(
            (r, 0),
            0.12 if r > 4 else 0.08,
            color=col,
            ec='black',
            lw=1.0,
            zorder=6,
        )
        ax.add_patch(p_body)
        ax.text(
            r,
            0.22,
            f'{name}\n({dist})',
            fontsize=9.5,
            fontweight='bold',
            ha='center',
            color=col,
        )

    # Asteroid Belt Ring
    belt_inner = 2.1
    belt_outer = 3.3
    belt_wedge = patches.Wedge(
        (0, 0),
        belt_outer,
        0,
        360,
        width=(belt_outer - belt_inner),
        color='#cccccc',
        alpha=0.35,
        zorder=1,
    )
    ax.add_patch(belt_wedge)
    ax.text(
        2.7,
        -2.9,
        'Main Asteroid Belt\n(2.1 - 3.3 AU)',
        fontsize=10.5,
        ha='center',
        fontweight='bold',
        color='#555555',
        bbox=dict(
            boxstyle='round,pad=0.3',
            facecolor='white',
            alpha=0.8,
            ec='gray',
        ),
    )

    # Secular Resonance Sweeping Path (s6 and nu6)
    r_sweep = np.linspace(1.8, 3.6, 200)
    theta_sweep = np.linspace(np.pi / 4, 3 * np.pi / 4, 200)
    x_sweep = r_sweep * np.cos(theta_sweep)
    y_sweep = r_sweep * np.sin(theta_sweep)

    ax.plot(
        x_sweep,
        y_sweep,
        color='#9467bd',
        lw=3.0,
        linestyle='--',
        zorder=4,
        label=r'Sweeping $s_6$ Secular Resonance ($B(a, t) = s_6(t)$)',
    )

    # Sweep direction arrows
    ax.annotate(
        '',
        xy=(-1.8, 2.5),
        xytext=(-1.0, 1.8),
        arrowprops=dict(arrowstyle='->',
                        color='#9467bd',
                        lw=2.5,
                        mutation_scale=20),
    )
    ax.text(
        -1.7,
        2.0,
        r'Resonance Sweeping Direction $\dot{a}_{s6}$' + '\n' +
        r'as Gas Disk Disperses ($\Sigma_{\rm gas} \to 0$)',
        fontsize=10,
        fontweight='bold',
        color='#9467bd',
        bbox=dict(boxstyle='round,pad=0.4', facecolor='#f4effa', ec='#9467bd'),
    )

    # Analytical Physics Annotations Box
    box_lines = [
        'Batygin & Morbidelli (2013) Theory:',
        r'$\bullet$ Resonance Condition: $B(a, t) = s_6(t)$',
        (r'$\bullet$ Sweeping Velocity: $\dot{a}_{s6} = (\dot{s}_6 - \partial'
         r' B/\partial t) / (\partial B/\partial a)$'),
        r'$\bullet$ Adiabaticity: $\epsilon_{\rm ad} = |\dot{B} - \dot{s}_6| / \nu^2(a)$',
        r'$\bullet$ Impulsive Kick: $\Delta\sin(i) = \sqrt{2\pi / \epsilon_{\rm ad}}$',
        (r'$\bullet$ Post-Crossing: $\langle\sin^2(i_{\rm final})\rangle ='
         r' \sin^2(i_0) + 2\pi / \epsilon_{\rm ad}$'),
    ]
    box_text = '\n'.join(box_lines)
    ax.text(
        4.2,
        3.2,
        box_text,
        fontsize=9.5,
        va='top',
        ha='left',
        bbox=dict(
            boxstyle='round,pad=0.6',
            facecolor='#ffffff',
            edgecolor='#333333',
            lw=1.2,
            alpha=0.95,
        ),
    )

    # Legend & Framing
    ax.set_xlim(-4.2, 10.5)
    ax.set_ylim(-4.2, 4.5)
    ax.set_aspect('equal')
    ax.axis('off')
    ax.set_title(
        'Dynamical Architecture of Secular Resonance Sweeping in the Early Solar'
        ' System',
        fontsize=13.5,
        pad=15,
        fontweight='bold',
    )

    plt.tight_layout()
    pdf_path = os.path.join(SCRIPT_DIR, 'fig_diagram.pdf')
    png_path = os.path.join(SCRIPT_DIR, 'fig_diagram.png')
    plt.savefig(pdf_path)
    plt.savefig(png_path)
    plt.close()
    print(f'✅ Created {pdf_path} and {png_path}')


def main():
    print(
        '========================================================================'
    )
    print('Generating Plots for Paper #239: Batygin & Morbidelli (2013)')
    print(
        '========================================================================'
    )
    d_sweep, d_grid, d_traj, d_sens = load_data()
    make_comparison_plot(d_sweep, d_grid, d_traj, d_sens)
    make_model_choices_plot(d_sweep, d_grid, d_traj, d_sens)
    make_diagram_plot()
    print('✅ All plots generated successfully!')


if __name__ == '__main__':
    main()
