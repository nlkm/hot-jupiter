#!/usr/bin/env python3
"""
Paper #237 Replication Plot Generator:
Thommes, Duncan, & Levison (2002) "The Formation of Uranus and Neptune in the Jupiter-Saturn Region"

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

# Configure publication typography and styling
plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.sans-serif': ['DejaVu Sans', 'Arial', 'Helvetica'],
    'mathtext.fontset': 'cm',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 15,
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
                    data[h].append(float(val) if val != '' else np.nan)
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
    d_time = read_csv_dict("formation_timescales.csv")
    d_traj = read_csv_dict("orbital_evolution_tracks.csv")
    d_out = read_csv_dict("disk_mass_outcomes.csv")
    d_damp = read_csv_dict("eccentricity_damping_sweep.csv")
    return d_time, d_traj, d_out, d_damp


def make_comparison_plot(d_time, d_traj, d_out, d_damp):
    """Figure 1: Core Accretion Crisis, Outcome Branching, and Perihelion Decoupling."""
    _fig, axes = plt.subplots(1, 3, figsize=(17, 5.2))

    # Panel (a): In Situ Core Accretion Timescale Crisis vs Distance
    ax1 = axes[0]
    ax1.plot(d_time['semi_major_axis_au'],
             d_time['t_insitu_myr'],
             color='#d62728',
             lw=2.5,
             label=r'In Situ Accretion $t_{\rm acc} \propto a^{3.0}$')
    ax1.plot(d_time['semi_major_axis_au'],
             d_time['t_model_myr'],
             color='#2ca02c',
             lw=2.5,
             linestyle='--',
             label='Thommes et al. (Interstitial+Scatter)')
    ax1.axhline(
        5.0,
        color='black',
        linestyle=':',
        lw=2.0,
        label=r'Gas Nebula Lifetime $\tau_{\rm disk} \approx 5\,\mathrm{Myr}$')

    # Highlight Uranus and Neptune distances
    ax1.scatter([19.2, 30.1], [180.0, 850.0], color='#d62728', s=70, zorder=5)
    ax1.annotate('Uranus in situ\n~180 Myr',
                 xy=(19.2, 180),
                 xytext=(12.0, 350),
                 arrowprops=dict(arrowstyle="->", color='#d62728', lw=1.2),
                 fontweight='bold',
                 fontsize=9)
    ax1.annotate('Neptune in situ\n~850 Myr',
                 xy=(30.1, 850),
                 xytext=(22.0, 1100),
                 arrowprops=dict(arrowstyle="->", color='#d62728', lw=1.2),
                 fontweight='bold',
                 fontsize=9)

    # Highlight Jupiter-Saturn condensation zone
    ax1.axvspan(5.2,
                8.6,
                color='#2ca02c',
                alpha=0.15,
                label='J-S Accretion Zone (5.2-8.6 AU)')

    ax1.set_yscale('log')
    ax1.set_xlim(4, 38)
    ax1.set_ylim(0.5, 3000)
    ax1.set_xlabel('Semi-Major Axis $a$ [AU]')
    ax1.set_ylabel(r'Core Formation Timescale [Myr]')
    ax1.set_title('(a) In Situ Core Accretion Crisis vs Distance',
                  pad=10,
                  fontweight='bold')
    ax1.grid(True, which='both', linestyle=':')
    ax1.legend(loc='upper left', framealpha=0.92, fontsize=8.5)

    # Panel (b): Outcome Fractions vs Planetesimal Disk Mass
    ax2 = axes[1]
    ax2.plot(d_out['disk_mass_mearth'],
             d_out['p_success_4planets'] * 100.0,
             color='#1f77b4',
             lw=2.5,
             label='4 Giant Planets (Success)')
    ax2.plot(d_out['disk_mass_mearth'],
             d_out['p_ejection_3planets'] * 100.0,
             color='#ff7f0e',
             lw=2.2,
             linestyle='--',
             label='1 Core Ejected (3 Giants)')
    ax2.plot(d_out['disk_mass_mearth'],
             d_out['p_collision'] * 100.0,
             color='#d62728',
             lw=2.0,
             linestyle='-.',
             label='Collision / Merger')
    ax2.plot(d_out['disk_mass_mearth'],
             d_out['p_undamped'] * 100.0,
             color='#9467bd',
             lw=2.0,
             linestyle=':',
             label='Under-Damped / High $e$')

    # Literature benchmark data from Thommes et al. (2002) Tables 1 & 2
    mask = ~np.isnan(d_out['lit_p_success'])
    ax2.scatter(d_out['disk_mass_mearth'][mask],
                d_out['lit_p_success'][mask] * 100.0,
                color='#1f77b4',
                marker='s',
                s=55,
                edgecolors='k',
                zorder=5,
                label='Thommes (2002) 4-Planet')
    ax2.scatter(d_out['disk_mass_mearth'][mask],
                d_out['lit_p_ejection'][mask] * 100.0,
                color='#ff7f0e',
                marker='^',
                s=55,
                edgecolors='k',
                zorder=5,
                label='Thommes (2002) Ejection')
    ax2.scatter(d_out['disk_mass_mearth'][mask],
                d_out['lit_p_collision'][mask] * 100.0,
                color='#d62728',
                marker='o',
                s=45,
                edgecolors='k',
                zorder=5)

    # Optimum disk mass band
    ax2.axvspan(30,
                45,
                color='#1f77b4',
                alpha=0.12,
                label=r'Optimal Disk Mass ($30-45\,M_\oplus$)')

    ax2.set_xlim(5, 75)
    ax2.set_ylim(0, 80)
    ax2.set_xlabel(r'Planetesimal Disk Mass $M_{\rm disk}\ [M_\oplus]$')
    ax2.set_ylabel('Outcome Probability [%]')
    ax2.set_title('(b) System Architecture Fates vs $M_{\\rm disk}$',
                  pad=10,
                  fontweight='bold')
    ax2.grid(True, linestyle=':')
    ax2.legend(loc='upper right', framealpha=0.92, fontsize=8.0)

    # Panel (c): Time Evolution of Uranus & Neptune Perihelia Decoupling
    ax3 = axes[2]
    ax3.plot(d_traj['time_myr'],
             d_traj['a1_au'],
             color='#1f77b4',
             lw=2.5,
             label=r'Proto-Uranus $a_1(t)$')
    ax3.plot(d_traj['time_myr'],
             d_traj['q1_au'],
             color='#1f77b4',
             lw=1.8,
             linestyle='--',
             label=r'Proto-Uranus $q_1(t)$')
    ax3.plot(d_traj['time_myr'],
             d_traj['a2_au'],
             color='#2ca02c',
             lw=2.5,
             label=r'Proto-Neptune $a_2(t)$')
    ax3.plot(d_traj['time_myr'],
             d_traj['q2_au'],
             color='#2ca02c',
             lw=1.8,
             linestyle='--',
             label=r'Proto-Neptune $q_2(t)$')

    # Giant planet influence zones
    ax3.axhspan(4.8,
                5.6,
                color='#ff7f0e',
                alpha=0.20,
                label='Jupiter Orbit Zone')
    ax3.axhspan(8.0,
                9.2,
                color='#e377c2',
                alpha=0.20,
                label='Saturn Orbit Zone')

    # Modern target distances
    ax3.axhline(19.2, color='#1f77b4', linestyle=':', lw=1.5, alpha=0.8)
    ax3.axhline(30.1, color='#2ca02c', linestyle=':', lw=1.5, alpha=0.8)

    ax3.annotate('Perihelion Lifting\n$q > 12$ AU (Decoupled)',
                 xy=(3.5, 12.5),
                 xytext=(4.5, 7.0),
                 arrowprops=dict(arrowstyle="->", color='black', lw=1.2),
                 fontweight='bold',
                 fontsize=8.5)

    ax3.set_xlim(0, 15)
    ax3.set_ylim(4, 35)
    ax3.set_xlabel('Evolution Time [Myr]')
    ax3.set_ylabel('Heliocentric Distance [AU]')
    ax3.set_title('(c) Dynamical Friction & Perihelion Decoupling',
                  pad=10,
                  fontweight='bold')
    ax3.grid(True, linestyle=':')
    ax3.legend(loc='lower right', framealpha=0.92, fontsize=8.0)

    plt.tight_layout()
    pdf_path = os.path.join(SCRIPT_DIR, "fig_comparison.pdf")
    png_path = os.path.join(SCRIPT_DIR, "fig_comparison.png")
    plt.savefig(pdf_path)
    plt.savefig(png_path)
    plt.close()
    print(f"✅ Generated {pdf_path} and {png_path}")


def make_model_choices_plot(d_time, d_traj, d_out, d_damp):
    """Figure 2: Physical Model Choices, Sensitivity, and Dynamics Diagnostics."""
    _fig, axes = plt.subplots(1, 3, figsize=(17, 5.2))

    # Panel (a): Eccentricity Damping Evolution across Disk Masses
    ax1 = axes[0]
    colors = ['#d62728', '#ff7f0e', '#1f77b4', '#2ca02c', '#9467bd']
    labels = [
        r'$M_{\rm disk} = 15\,M_\oplus$', r'$M_{\rm disk} = 25\,M_\oplus$',
        r'$M_{\rm disk} = 35\,M_\oplus$ (Nominal)',
        r'$M_{\rm disk} = 50\,M_\oplus$', r'$M_{\rm disk} = 70\,M_\oplus$'
    ]
    keys = ['e_m15', 'e_m25', 'e_m35', 'e_m50', 'e_m70']

    for c, l, k in zip(colors, labels, keys):
        lw = 2.8 if '35' in k else 1.8
        ax1.plot(d_damp['time_myr'], d_damp[k], color=c, lw=lw, label=l)

    ax1.axhline(0.046,
                color='black',
                linestyle=':',
                lw=1.5,
                label='Modern Uranus $e = 0.046$')
    ax1.axhline(0.009,
                color='gray',
                linestyle='--',
                lw=1.5,
                label='Modern Neptune $e = 0.009$')

    ax1.set_xlim(0, 12)
    ax1.set_ylim(0, 0.65)
    ax1.set_xlabel('Time [Myr]')
    ax1.set_ylabel('Orbital Eccentricity $e(t)$')
    ax1.set_title('(a) Eccentricity Damping vs Disk Mass $M_{\\rm disk}$',
                  pad=10,
                  fontweight='bold')
    ax1.grid(True, linestyle=':')
    ax1.legend(loc='upper right', framealpha=0.92, fontsize=8.5)

    # Panel (b): Safronov Scattering Parameter & Maximum Velocity Kick
    ax2 = axes[1]
    planets = [
        'Proto-Core\n(7 AU)', 'Saturn\n(8.6 AU)', 'Uranus\n(19.2 AU)',
        'Neptune\n(30.1 AU)', 'Jupiter\n(5.2 AU)'
    ]
    thetas = [1.89, 6.10, 5.75, 10.07, 10.39]
    bar_colors = ['#9467bd', '#e377c2', '#1f77b4', '#2ca02c', '#d62728']

    bars = ax2.bar(planets,
                   thetas,
                   color=bar_colors,
                   width=0.55,
                   edgecolor='black',
                   lw=1.2)
    ax2.axhline(1.0,
                color='black',
                linestyle='--',
                lw=1.5,
                label=r'Accretion / Weak Scatter Threshold ($\Theta = 1$)')
    ax2.axhline(8.0,
                color='red',
                linestyle=':',
                lw=1.5,
                label=r'Hyperbolic Ejection Regime ($\Theta > 8$)')

    for bar, val in zip(bars, thetas):
        yval = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width() / 2.0,
                 yval + 0.25,
                 f'{val:.2f}',
                 ha='center',
                 va='bottom',
                 fontweight='bold',
                 fontsize=9)

    ax2.set_ylim(0, 13.0)
    ax2.set_ylabel(r'Safronov Number $\Theta = (M_p / M_\odot)(a_p / R_p)$')
    ax2.set_title('(b) Gravitational Scattering Regimes',
                  pad=10,
                  fontweight='bold')
    ax2.grid(True, axis='y', linestyle=':')
    ax2.legend(loc='upper left', framealpha=0.92, fontsize=8.5)

    # Panel (c): Core-Swapping Probability and Final Semi-Major Axis Distribution
    ax3 = axes[2]
    ax3.plot(d_out['disk_mass_mearth'],
             d_out['p_swapped'] * 100.0,
             color='#8c564b',
             lw=2.5,
             label='Uranus-Neptune Swapped Orbits')
    ax3.plot(d_out['disk_mass_mearth'],
             (d_out['p_success_4planets'] - d_out['p_swapped']) * 100.0,
             color='#17becf',
             lw=2.5,
             linestyle='--',
             label='Unswapped Native Ordering')

    ax3.axvspan(30,
                45,
                color='#8c564b',
                alpha=0.15,
                label=r'Nominal Regime ($\sim 45\%$ Swapping)')

    ax3.set_xlim(5, 75)
    ax3.set_ylim(0, 35)
    ax3.set_xlabel(r'Planetesimal Disk Mass $M_{\rm disk}\ [M_\oplus]$')
    ax3.set_ylabel('Probability [%]')
    ax3.set_title('(c) Core Order Inversion (Swapping)',
                  pad=10,
                  fontweight='bold')
    ax3.grid(True, linestyle=':')
    ax3.legend(loc='upper right', framealpha=0.92, fontsize=8.5)

    plt.tight_layout()
    pdf_path = os.path.join(SCRIPT_DIR, "fig_model_choices.pdf")
    png_path = os.path.join(SCRIPT_DIR, "fig_model_choices.png")
    plt.savefig(pdf_path)
    plt.savefig(png_path)
    plt.close()
    print(f"✅ Generated {pdf_path} and {png_path}")


def make_diagram_plot():
    """Figure 3: Publication Architecture Schematic of the Thommes et al. 2002 Mechanism."""
    _fig, ax = plt.subplots(figsize=(15, 7.5))
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)
    ax.axis('off')

    # Title Banner
    ax.text(50,
            96,
            "Thommes et al. (1999, 2002) Ice Giant Formation Paradigm",
            ha='center',
            va='center',
            fontsize=15,
            fontweight='bold',
            color='#002060')
    ax.text(
        50,
        92,
        "Formation of Uranus and Neptune in the Jupiter-Saturn Region via Gravitational Scattering & Dynamical Friction",
        ha='center',
        va='center',
        fontsize=11,
        fontstyle='italic',
        color='#333333')

    # Step Boxes
    # 1. In Situ Crisis
    box1 = patches.FancyBboxPatch((3, 50),
                                  28,
                                  36,
                                  boxstyle="round,pad=1.2",
                                  facecolor='#fbebe8',
                                  edgecolor='#d62728',
                                  lw=2.0)
    ax.add_patch(box1)
    ax.text(17,
            82,
            "1. In Situ Accretion Crisis",
            ha='center',
            va='center',
            fontsize=12,
            fontweight='bold',
            color='#d62728')
    text1 = (
        r"• Standard growth time:"
        "\n"
        r"  $t_{\rm acc} \propto \frac{a^{3.5}}{\Sigma_0}$"
        "\n"
        r"• At $a = 19.2\,\mathrm{AU}$ (Uranus): $t \sim 180\,\mathrm{Myr}$"
        "\n"
        r"• At $a = 30.1\,\mathrm{AU}$ (Neptune): $t \sim 850\,\mathrm{Myr}$"
        "\n"
        r"• Nebula gas lifetime: $\tau_{\rm gas} \approx 3-5\,\mathrm{Myr}$"
        "\n"
        r"$\Rightarrow$ Cannot capture observed"
        "\n"
        r"   $1-2\,M_\oplus$ H/He envelope in situ!")
    ax.text(5, 65, text1, ha='left', va='center', fontsize=9.5, linespacing=1.4)

    # 2. Interstitial Oligarchic Growth
    box2 = patches.FancyBboxPatch((36, 50),
                                  28,
                                  36,
                                  boxstyle="round,pad=1.2",
                                  facecolor='#eaf2f8',
                                  edgecolor='#1f77b4',
                                  lw=2.0)
    ax.add_patch(box2)
    ax.text(50,
            82,
            "2. Oligarchic Core Growth",
            ha='center',
            va='center',
            fontsize=12,
            fontweight='bold',
            color='#1f77b4')
    text2 = (r"• Cores grow between J & S:"
             "\n"
             r"  $a_{\rm init} \in [5.5, 8.5]\,\mathrm{AU}$"
             "\n"
             r"• High solid surface density:"
             "\n"
             r"  $\Sigma_{\rm solid} \approx 30-50\,\mathrm{g/cm}^2$"
             "\n"
             r"• Fast accretion timescale:"
             "\n"
             r"  $t_{\rm acc} \sim 1.5-3.5\,\mathrm{Myr} < \tau_{\rm gas}$"
             "\n"
             r"• Multiple $10-15\,M_\oplus$ cores"
             "\n"
             r"  readily form before gas dispersal.")
    ax.text(38,
            65,
            text2,
            ha='left',
            va='center',
            fontsize=9.5,
            linespacing=1.4)

    # 3. Gas Giant Destabilization & Scattering
    box3 = patches.FancyBboxPatch((69, 50),
                                  28,
                                  36,
                                  boxstyle="round,pad=1.2",
                                  facecolor='#fef5e7',
                                  edgecolor='#ff7f0e',
                                  lw=2.0)
    ax.add_patch(box3)
    ax.text(83,
            82,
            "3. Gas Giant Scattering",
            ha='center',
            va='center',
            fontsize=12,
            fontweight='bold',
            color='#ff7f0e')
    text3 = (r"• Jupiter ($318\,M_\oplus$) & Saturn ($95\,M_\oplus$)"
             "\n"
             r"  undergo runaway gas accretion."
             "\n"
             r"• Strong resonant perturbations"
             "\n"
             r"  destabilize proto-ice giants."
             "\n"
             r"• Safronov number $\Theta_J \approx 10.39 \gg 1$"
             "\n"
             r"• Cores scattered onto eccentric"
             "\n"
             r"  outer orbits: $Q \sim 20-40\,\mathrm{AU}$, $e \sim 0.6$.")
    ax.text(71,
            65,
            text3,
            ha='left',
            va='center',
            fontsize=9.5,
            linespacing=1.4)

    # Horizontal Connecting Arrows
    ax.annotate('',
                xy=(35.5, 68),
                xytext=(31.5, 68),
                arrowprops=dict(arrowstyle="->", lw=2.5, color='#002060'))
    ax.annotate('',
                xy=(68.5, 68),
                xytext=(64.5, 68),
                arrowprops=dict(arrowstyle="->", lw=2.5, color='#002060'))

    # 4. Dynamical Friction & Orbital Decoupling Banner
    box4 = patches.FancyBboxPatch((10, 6),
                                  80,
                                  38,
                                  boxstyle="round,pad=1.2",
                                  facecolor='#eafaf1',
                                  edgecolor='#2ca02c',
                                  lw=2.2)
    ax.add_patch(box4)
    ax.text(
        50,
        40,
        "4. Dynamical Friction Damping & Planetary Decoupling in Outer Disk",
        ha='center',
        va='center',
        fontsize=13,
        fontweight='bold',
        color='#2ca02c')

    text4_left = (
        r"• Massive Primordial Planetesimal Disk:"
        "\n"
        r"  $M_{\rm disk} \approx 30-50\,M_\oplus$, $r \in [10, 32]\,\mathrm{AU}$"
        "\n"
        r"• Chandrasekhar Dynamical Friction Drag:"
        "\n"
        r"  $\mathbf{F}_{\rm df} = -\frac{4\pi G^2 M_{\rm core}^2 \rho \ln\Lambda}{v^3} \mathbf{v}$"
        "\n"
        r"• Damping timescale: $\tau_{\rm damp} \approx 1.0-3.0\,\mathrm{Myr}$")
    ax.text(14,
            23,
            text4_left,
            ha='left',
            va='center',
            fontsize=10,
            linespacing=1.4)

    text4_right = (
        r"• Eccentricity Circularization: $\frac{de}{dt} = -\frac{e}{\tau_{\rm damp}} \Rightarrow e \to 0.01-0.05$"
        "\n"
        r"• Perihelion Lifting Mechanism:"
        "\n"
        r"  $\frac{dq}{dt} \approx \frac{a e}{\tau_{\rm damp}}[1 - 2e(1-e)] > 0$"
        "\n"
        r"• Perihelia lift out of Jupiter/Saturn reach ($q > 15\,\mathrm{AU}$),"
        "\n"
        r"  freezing stable modern orbits at $a_U \approx 19.2\,\mathrm{AU}$, $a_N \approx 30.1\,\mathrm{AU}$."
    )
    ax.text(52,
            23,
            text4_right,
            ha='left',
            va='center',
            fontsize=10,
            linespacing=1.4)

    # Arrow from box 3 to box 4
    ax.annotate('',
                xy=(50, 44.5),
                xytext=(83, 49.5),
                arrowprops=dict(
                    arrowstyle="->",
                    lw=2.5,
                    color='#2ca02c',
                    connectionstyle="angle,angleA=-90,angleB=180,rad=10"))

    pdf_path = os.path.join(SCRIPT_DIR, "fig_diagram.pdf")
    png_path = os.path.join(SCRIPT_DIR, "fig_diagram.png")
    plt.savefig(pdf_path)
    plt.savefig(png_path)
    plt.close()
    print(f"✅ Generated {pdf_path} and {png_path}")


def main():
    print(
        "============================================================================"
    )
    print(
        "Generating Publication Figures for Paper #237 (Thommes et al. 2002)..."
    )
    print(
        "============================================================================"
    )
    d_time, d_traj, d_out, d_damp = load_data()
    make_comparison_plot(d_time, d_traj, d_out, d_damp)
    make_model_choices_plot(d_time, d_traj, d_out, d_damp)
    make_diagram_plot()
    print("✅ All figures generated successfully.")


if __name__ == "__main__":
    main()
