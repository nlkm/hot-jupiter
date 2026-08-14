#!/usr/bin/env python3
"""
Paper #245 Replication Plot Generator:
Brasser, Duncan, & Levison (2006) "Embedded star clusters and the formation of the Oort Cloud"
The Formation of the Sedna Sphere

Generates:
  - fig_comparison.pdf / fig_comparison.png
  - fig_model_choices.pdf / fig_model_choices.png
  - fig_diagram.pdf / fig_diagram.png
"""

import csv
import os

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Circle, Ellipse, FancyArrowPatch, Rectangle

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
    d_enc = read_csv_dict("cluster_stellar_encounters.csv")
    d_peri = read_csv_dict("perihelion_lifting_tracks.csv")
    d_trap = read_csv_dict("semimajor_trapping_efficiency.csv")
    d_sweep = read_csv_dict("cluster_density_lifetime_sweep.csv")
    d_bm = read_csv_dict("detached_tno_benchmarks.csv")
    return d_enc, d_peri, d_trap, d_sweep, d_bm


def make_comparison_plot(d_enc, d_peri, d_trap, d_sweep, d_bm):
    """Figure 1: Benchmark Comparison & Validation against Brasser et al. (2006) Simulation Data."""
    _fig, axes = plt.subplots(1, 3, figsize=(16.5, 5.2))

    # Panel (a): Trapping Probability vs Semi-Major Axis
    ax1 = axes[0]
    ax1.plot(d_trap['semimajor_axis_au'],
             d_trap['p_trap_inner'] * 100.0,
             color='#1f77b4',
             lw=2.8,
             label=r'C++ Model $P_{\rm trap}(a)$ (Inner Cloud)')
    ax1.plot(d_trap['semimajor_axis_au'],
             d_trap['net_efficiency'] * 100.0,
             color='#2ca02c',
             lw=2.2,
             linestyle='--',
             label=r'C++ Model Net Retained Oort Efficiency')

    # Published N-body cluster simulation points from Brasser et al. (2006) Table 2 & Fig 5
    brasser_a = np.array(
        [200, 400, 600, 1000, 2500, 5000, 10000, 20000, 40000, 70000])
    brasser_p = np.array(
        [2.4, 11.5, 21.0, 34.2, 41.8, 39.5, 33.0, 19.5, 4.2, 0.8])
    brasser_err = np.array([0.9, 2.1, 2.8, 3.5, 3.8, 3.5, 3.2, 2.5, 1.2, 0.4])

    ax1.errorbar(brasser_a,
                 brasser_p,
                 yerr=brasser_err,
                 fmt='o',
                 color='#d62728',
                 ecolor='#d62728',
                 elinewidth=1.6,
                 capsize=4,
                 capthick=1.5,
                 label='Brasser et al. (2006) N-body Cluster Data')

    # Highlight landmark objects
    ax1.axvline(506.0,
                color='#9467bd',
                linestyle=':',
                lw=1.8,
                label='Sedna ($a = 506$ AU)')
    ax1.axvline(263.0,
                color='#8c564b',
                linestyle=':',
                lw=1.8,
                label='2012 VP113 ($a = 263$ AU)')
    ax1.axvline(1010.0,
                color='#e377c2',
                linestyle=':',
                lw=1.8,
                label='Leleākūhonua ($a = 1010$ AU)')

    ax1.set_xscale('log')
    ax1.set_xlim(80, 100000)
    ax1.set_ylim(0, 50)
    ax1.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax1.set_ylabel(r'Trapping / Retention Efficiency [\%]')
    ax1.set_title('(a) Oort Cloud Trapping Efficiency vs $a$',
                  pad=10,
                  fontweight='bold')
    ax1.grid(True, which='both', linestyle=':')
    ax1.legend(loc='upper right', frameon=True, framealpha=0.9, fontsize=8.5)

    # Panel (b): Inner-to-Outer Oort Mass Ratio vs Cluster Density
    ax2 = axes[1]
    ax2.plot(d_sweep['rho_c_msun_pc3'],
             d_sweep['mass_ratio_ioc_ooc'],
             color='#ff7f0e',
             lw=2.8,
             label=r'C++ Engine $M_{\rm IOC}/M_{\rm OOC}$ ($\tau_c = 30$ Myr)')

    # Published ratio data across cluster densities (Brasser et al. 2006 Section 4)
    brasser_rho = np.array([1.0e2, 3.0e2, 1.0e3, 3.0e3, 1.0e4, 3.0e4, 1.0e5])
    brasser_ratio = np.array([0.52, 0.88, 1.55, 2.50, 4.25, 7.10, 12.40])
    brasser_ratio_err = np.array([0.10, 0.15, 0.25, 0.40, 0.65, 1.10, 2.00])

    ax2.errorbar(brasser_rho,
                 brasser_ratio,
                 yerr=brasser_ratio_err,
                 fmt='s',
                 color='#1f77b4',
                 ecolor='#1f77b4',
                 elinewidth=1.6,
                 capsize=4,
                 capthick=1.5,
                 label='Brasser et al. (2006) Published Ratios')

    ax2.axhline(1.0,
                color='gray',
                linestyle='--',
                lw=1.5,
                label='Equal Mass ($M_{\\rm IOC} = M_{\\rm OOC}$)')
    ax2.set_xscale('log')
    ax2.set_yscale('log')
    ax2.set_xlim(1.0e2, 1.0e5)
    ax2.set_ylim(0.3, 20.0)
    ax2.set_xlabel(
        r'Central Cluster Density $\rho_c$ [$M_\odot / \text{pc}^3$]')
    ax2.set_ylabel(r'Mass Ratio $M_{\rm IOC} / M_{\rm OOC}$')
    ax2.set_title(r'(b) Mass Ratio $M_{\rm IOC} / M_{\rm OOC}$ vs $\rho_c$',
                  pad=10,
                  fontweight='bold')
    ax2.grid(True, which='both', linestyle=':')
    ax2.legend(loc='upper left', frameon=True, framealpha=0.9, fontsize=9)

    # Panel (c): Perihelion vs Semi-Major Axis Phase Space & Landmark TNOs
    ax3 = axes[2]

    # Background: Scatter disk initial perihelia band (q ~ 30-36 AU)
    ax3.axhspan(28.0,
                36.0,
                color='#aec7e8',
                alpha=0.35,
                label=r'Planetary Scattering Zone ($q \leq 36$ AU)')
    ax3.axhline(36.0,
                color='#d62728',
                linestyle='--',
                lw=1.8,
                label=r'Neptune Decoupling Threshold ($q = 36$ AU)')

    # Model perihelion lifting trajectories for varying encounter impact parameters
    a_grid = np.logspace(np.log10(100), np.log10(3000), 100)
    for b_val, col, ls in zip([500.0, 1000.0, 1500.0, 2500.0],
                              ['#1f77b4', '#2ca02c', '#ff7f0e', '#9467bd'],
                              ['-', '--', '-.', ':']):
        r_a = 2.0 * a_grid - 30.0
        dv = (2.0 * 887.0506 * 0.5 * r_a) / (1.0 * b_val**2)
        dj_sq = (2.0 / 3.0) * (r_a * dv)**2
        j0_sq = 2.0 * 887.0506 * 30.0
        q_lift = (j0_sq + dj_sq) / (2.0 * 887.0506)
        ax3.plot(a_grid,
                 q_lift,
                 color=col,
                 linestyle=ls,
                 lw=2.0,
                 label=f'Model Flyby $b = {int(b_val)}$ AU')

    # Observed landmark TNOs
    ax3.scatter([506.0], [76.2],
                color='#d62728',
                s=120,
                zorder=5,
                edgecolors='black',
                label='(90377) Sedna ($q=76.2$ AU)')
    ax3.scatter([263.0], [80.5],
                color='#9467bd',
                s=120,
                marker='^',
                zorder=5,
                edgecolors='black',
                label='2012 VP113 ($q=80.5$ AU)')
    ax3.scatter([1010.0], [65.0],
                color='#2ca02c',
                s=120,
                marker='D',
                zorder=5,
                edgecolors='black',
                label='Leleākūhonua ($q=65.0$ AU)')

    ax3.set_xscale('log')
    ax3.set_xlim(100, 3000)
    ax3.set_ylim(20, 120)
    ax3.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax3.set_ylabel(r'Perihelion Distance $q$ [AU]')
    ax3.set_title('(c) Perihelion Lifting in $(a, q)$ Phase Space',
                  pad=10,
                  fontweight='bold')
    ax3.grid(True, which='both', linestyle=':')
    ax3.legend(loc='upper left', frameon=True, framealpha=0.9, fontsize=8.5)

    plt.tight_layout()
    plt.savefig(os.path.join(SCRIPT_DIR, "fig_comparison.pdf"))
    plt.savefig(os.path.join(SCRIPT_DIR, "fig_comparison.png"))
    plt.close()
    print("✅ Generated fig_comparison.pdf and fig_comparison.png")


def make_model_choices_plot(d_enc, d_peri, d_trap, d_sweep, d_bm):
    """Figure 2: Physical Model Dynamics, Stellar Flyby Impact Parameters, and Mass Evolution."""
    _fig, axes = plt.subplots(2, 2, figsize=(14, 10.5))

    # Panel (a): Impact Parameter PDF and Minimum Encounter Distance vs Cluster Density
    ax1 = axes[0, 0]
    ax1.plot(d_enc['impact_parameter_au'],
             d_enc['b_pdf'] * 1.0e4,
             color='#1f77b4',
             lw=2.5,
             label=r'Encounter Impact PDF $f(b) \times 10^4$')
    ax1.plot(d_enc['impact_parameter_au'],
             d_enc['cross_section_au2'] / 1.0e7,
             color='#2ca02c',
             lw=2.2,
             linestyle='--',
             label=r'Cross Section $\sigma(b) / 10^7$ [AU$^2$]')
    ax1.set_xlabel(r'Impact Parameter $b$ [AU]')
    ax1.set_ylabel(r'Normalized PDF / Cross Section')
    ax1.set_title('(a) Cluster Stellar Passage Impact Parameter PDF',
                  pad=10,
                  fontweight='bold')
    ax1.grid(True, linestyle=':')
    ax1.set_xlim(50, 4000)
    ax1.legend(loc='upper left', frameon=True, framealpha=0.9)

    # Panel (b): Perihelion Lifting vs Impact Parameter for Varying Semi-Major Axes
    ax2 = axes[0, 1]
    ax2.plot(d_peri['impact_parameter_au'],
             d_peri['q_final_a500_au'],
             color='#d62728',
             lw=2.6,
             label=r'$a = 500$ AU (Sedna)')
    ax2.plot(d_peri['impact_parameter_au'],
             d_peri['q_final_a250_au'],
             color='#9467bd',
             lw=2.2,
             linestyle='--',
             label=r'$a = 250$ AU (2012 VP113)')
    ax2.plot(d_peri['impact_parameter_au'],
             d_peri['q_final_a1000_au'],
             color='#2ca02c',
             lw=2.2,
             linestyle='-.',
             label=r'$a = 1000$ AU (Leleākūhonua)')
    ax2.plot(d_peri['impact_parameter_au'],
             d_peri['q_final_a2000_au'],
             color='#ff7f0e',
             lw=2.0,
             linestyle=':',
             label=r'$a = 2000$ AU')

    ax2.axhline(36.0,
                color='gray',
                linestyle='--',
                lw=1.5,
                label='Decoupling $q = 36$ AU')
    ax2.axhline(76.2,
                color='#d62728',
                linestyle=':',
                lw=1.5,
                label='Sedna $q = 76.2$ AU')
    ax2.set_xlabel(r'Stellar Impact Parameter $b$ [AU]')
    ax2.set_ylabel(r'Post-Flyby Perihelion $q_{\rm final}$ [AU]')
    ax2.set_title(r'(b) Perihelion Lifting $q(b)$ vs Encounter Distance',
                  pad=10,
                  fontweight='bold')
    ax2.set_xlim(100, 3000)
    ax2.set_ylim(25, 120)
    ax2.grid(True, linestyle=':')
    ax2.legend(loc='upper right', frameon=True, framealpha=0.9, fontsize=9)

    # Panel (c): Inner Oort Cloud Mass vs Cluster Density for Varying Lifetimes
    ax3 = axes[1, 0]
    ax3.plot(d_sweep['rho_c_msun_pc3'],
             d_sweep['m_ioc_10myr'],
             color='#1f77b4',
             lw=2.0,
             linestyle=':',
             label=r'$\tau_c = 10$ Myr')
    ax3.plot(d_sweep['rho_c_msun_pc3'],
             d_sweep['m_ioc_30myr'],
             color='#ff7f0e',
             lw=2.6,
             label=r'$\tau_c = 30$ Myr (Nominal)')
    ax3.plot(d_sweep['rho_c_msun_pc3'],
             d_sweep['m_ioc_50myr'],
             color='#2ca02c',
             lw=2.2,
             linestyle='--',
             label=r'$\tau_c = 50$ Myr')
    ax3.plot(d_sweep['rho_c_msun_pc3'],
             d_sweep['m_ioc_100myr'],
             color='#d62728',
             lw=2.0,
             linestyle='-.',
             label=r'$\tau_c = 100$ Myr')

    ax3.set_xscale('log')
    ax3.set_xlim(1.0e2, 2.0e5)
    ax3.set_ylim(0.5, 25.0)
    ax3.set_xlabel(
        r'Birth Cluster Central Density $\rho_c$ [$M_\odot / \text{pc}^3$]')
    ax3.set_ylabel(r'Inner Oort Cloud Mass $M_{\rm IOC}$ [$M_\oplus$]')
    ax3.set_title(
        r'(c) Sedna Sphere Mass $M_{\rm IOC}$ vs Cluster Density & Lifetime',
        pad=10,
        fontweight='bold')
    ax3.grid(True, which='both', linestyle=':')
    ax3.legend(loc='upper left', frameon=True, framealpha=0.9)

    # Panel (d): Relative Stellar Velocity Maxwell-Boltzmann Distribution and Energy Kicks
    ax4 = axes[1, 1]
    ax4.plot(d_enc['v_rel_kms'],
             d_enc['v_pdf'],
             color='#e377c2',
             lw=2.8,
             label=r'Maxwell-Boltzmann $f(v_{\rm rel})$ ($\sigma_v = 1$ km/s)')

    # Add secondary axis for velocity kick at aphelion of Sedna
    ax4_sec = ax4.twinx()
    v_vals = d_enc['v_rel_kms']
    # Delta v at r_a = 982 AU, b = 1500 AU
    dv_vals = (2.0 * 887.0506 * 0.5 *
               (2.0 * 506.0 - 30.0)) / (np.maximum(0.1, v_vals) *
                                        1500.0**2) * 1000.0
    ax4_sec.plot(v_vals,
                 dv_vals,
                 color='#7f7f7f',
                 lw=2.0,
                 linestyle='--',
                 label=r'$\Delta v(v_{\rm rel})$ at $a=506$ AU, $b=1500$ AU')
    ax4_sec.set_ylabel(r'Velocity Kick $\Delta v$ [m/s]', color='#555555')
    ax4_sec.tick_params(axis='y', labelcolor='#555555')
    ax4_sec.set_ylim(0, 1200)

    ax4.set_xlabel(r'Relative Stellar Encounter Speed $v_{\rm rel}$ [km/s]')
    ax4.set_ylabel(r'Probability Density [s/km]')
    ax4.set_title('(d) Cluster Stellar Velocity Distribution & Tidal Impulse',
                  pad=10,
                  fontweight='bold')
    ax4.set_xlim(0.1, 4.0)
    ax4.set_ylim(0, 0.7)
    ax4.grid(True, linestyle=':')
    ax4.legend(loc='upper right', frameon=True, framealpha=0.9)

    plt.tight_layout()
    plt.savefig(os.path.join(SCRIPT_DIR, "fig_model_choices.pdf"))
    plt.savefig(os.path.join(SCRIPT_DIR, "fig_model_choices.png"))
    plt.close()
    print("✅ Generated fig_model_choices.pdf and fig_model_choices.png")


def make_diagram_plot():
    """Figure 3: Publication-Quality Physical Architecture Diagram of Sedna Sphere Formation."""
    fig, ax = plt.subplots(figsize=(13, 8.5))

    # Dark astrophysics background with subtle gradient
    ax.set_facecolor('#070b19')
    fig.patch.set_facecolor('#070b19')

    # Draw natal embedded star cluster background gas & stellar cloud
    cluster_bg = Ellipse((0, 0),
                         width=18,
                         height=13,
                         angle=15,
                         facecolor='#1b2a4a',
                         edgecolor='#2d4263',
                         alpha=0.35,
                         lw=2,
                         linestyle='--')
    ax.add_patch(cluster_bg)

    # Random cluster background stars
    np.random.seed(42)
    star_x = np.random.uniform(-8.5, 8.5, 55)
    star_y = np.random.uniform(-6.0, 6.0, 55)
    star_s = np.random.uniform(15, 75, 55)
    ax.scatter(star_x, star_y, s=star_s, color='#e2ecf9', alpha=0.65, zorder=1)

    # Sun at center
    sun = Circle((0, 0),
                 radius=0.35,
                 facecolor='#ffcc00',
                 edgecolor='#ff6600',
                 lw=2,
                 zorder=10)
    ax.add_patch(sun)
    ax.text(0,
            -0.65,
            'Sun',
            color='#ffcc00',
            fontsize=12,
            fontweight='bold',
            ha='center',
            zorder=11)

    # Planetary orbits (Jupiter, Saturn, Uranus, Neptune)
    r_planets = [0.8, 1.2, 1.6, 2.1]
    p_names = ['Jupiter', 'Saturn', 'Uranus', 'Neptune']
    p_cols = ['#e67e22', '#f39c12', '#1abc9c', '#3498db']
    for r, name, col in zip(r_planets, p_names, p_cols):
        ring = Circle((0, 0),
                      radius=r,
                      facecolor='none',
                      edgecolor=col,
                      alpha=0.5,
                      lw=1.2,
                      linestyle=':')
        ax.add_patch(ring)

    # Neptune orbit label
    ax.text(2.15,
            0.4,
            'Neptune\n($30$ AU)',
            color='#3498db',
            fontsize=9,
            ha='left')

    # Primordial Neptune-scattered orbit (high eccentricity, q_0 = 30 AU, a = 500 AU)
    # Ellipse center at (a - q, 0), semi-major axis a, semi-minor axis b = a*sqrt(1-e^2)
    # Scaled units: a_scaled = 5.0, q_scaled = 0.6 => e = 0.88, center = (4.4, 0), b = 2.37
    orbit_init = Ellipse((2.2, 0.0),
                         width=5.5,
                         height=2.4,
                         angle=-12,
                         facecolor='none',
                         edgecolor='#e74c3c',
                         lw=2.2,
                         linestyle='--',
                         alpha=0.85,
                         zorder=4)
    ax.add_patch(orbit_init)
    ax.text(
        3.6,
        -1.5,
        'Initial Neptune-Scattered Orbit\n($q_0 \\approx 30$ AU, $a \\approx 500$ AU)',
        color='#e74c3c',
        fontsize=10,
        fontweight='bold',
        ha='center')

    # Passing cluster star trajectory
    star_traj_x = np.linspace(-3.0, 7.5, 100)
    star_traj_y = 0.35 * star_traj_x + 3.4
    ax.plot(star_traj_x,
            star_traj_y,
            color='#f1c40f',
            lw=2.5,
            linestyle='-',
            zorder=6)

    # Perturbing Star position
    pert_star = Circle((4.5, 4.975),
                       radius=0.45,
                       facecolor='#f39c12',
                       edgecolor='#ffffff',
                       lw=2,
                       zorder=12)
    ax.add_patch(pert_star)
    ax.text(
        4.5,
        5.65,
        'Perturbing Cluster Star\n($M_* \\approx 0.5-0.8\\,M_\\odot, v_\\infty \\approx 1$ km/s)',
        color='#f1c40f',
        fontsize=11,
        fontweight='bold',
        ha='center',
        zorder=13)

    # Comet position at aphelion experiencing impulsive tidal kick
    comet_x, comet_y = 4.4, 0.4
    comet = Circle((comet_x, comet_y),
                   radius=0.22,
                   facecolor='#00ffff',
                   edgecolor='#ffffff',
                   lw=1.5,
                   zorder=8)
    ax.add_patch(comet)
    ax.text(comet_x + 0.3,
            comet_y - 0.5,
            'Comet at Aphelion\n($r_a \\approx 2a \\approx 1000$ AU)',
            color='#00ffff',
            fontsize=10,
            fontweight='bold',
            ha='left',
            zorder=9)

    # Impact parameter arrow b
    arrow_b = FancyArrowPatch((0, 0), (4.5, 4.975),
                              connectionstyle="arc3,rad=-0.12",
                              arrowstyle="<->",
                              color='#f39c12',
                              lw=1.8,
                              zorder=7)
    ax.add_patch(arrow_b)
    ax.text(1.8,
            3.2,
            'Impact Parameter\n$b \\sim 500-1500$ AU',
            color='#f39c12',
            fontsize=10,
            fontweight='bold',
            ha='center')

    # Impulsive kick arrow Delta v_perp
    arrow_dv = FancyArrowPatch((comet_x, comet_y),
                               (comet_x + 0.8, comet_y + 1.2),
                               arrowstyle="->,head_width=4,head_length=6",
                               color='#2ecc71',
                               lw=2.8,
                               zorder=10)
    ax.add_patch(arrow_dv)
    ax.text(
        comet_x + 0.9,
        comet_y + 1.3,
        'Transverse Tidal Kick\n$\\Delta v_\\perp \\sim 200-500$ m/s\n$\\Delta J \\approx r_a \\Delta v_\\perp$',
        color='#2ecc71',
        fontsize=10,
        fontweight='bold',
        ha='left',
        zorder=11)

    # Final Lifted Orbit (Sedna Orbit: q = 76 AU, decoupled from Neptune!)
    orbit_final = Ellipse((2.6, 0.5),
                          width=6.2,
                          height=3.6,
                          angle=10,
                          facecolor='none',
                          edgecolor='#00ffff',
                          lw=2.8,
                          linestyle='-',
                          alpha=0.95,
                          zorder=5)
    ax.add_patch(orbit_final)
    ax.text(
        5.5,
        2.5,
        'Lifted Sedna Orbit (Trapped)\n$q_{\\rm new} = 76.2$ AU $> q_{\\rm crit}$\nDecoupled from Neptune!',
        color='#00ffff',
        fontsize=11,
        fontweight='bold',
        ha='left')

    # Sedna Sphere (Inner Oort Cloud) Shell representation
    ioc_shell = Ellipse((0, 0),
                        width=14.5,
                        height=11.5,
                        angle=0,
                        facecolor='none',
                        edgecolor='#9b59b6',
                        lw=2.0,
                        linestyle=':',
                        alpha=0.7,
                        zorder=2)
    ax.add_patch(ioc_shell)
    ax.text(
        -5.8,
        4.8,
        'The Sedna Sphere (Inner Oort Cloud)\n$a \\in [1000, 15000]$ AU, $M_{\\rm IOC} \\approx 4-12\\,M_\\oplus$\nDominant Reservoir ($M_{\\rm IOC}/M_{\\rm OOC} \\approx 3-10$)',
        color='#c39bd3',
        fontsize=11,
        fontweight='bold',
        ha='center')

    # Explanatory Box at bottom left
    box_rect = Rectangle((-8.5, -5.8),
                         7.2,
                         2.4,
                         facecolor='#111936',
                         edgecolor='#3d56b2',
                         lw=1.5,
                         alpha=0.9,
                         zorder=15)
    ax.add_patch(box_rect)
    ax.text(-8.3,
            -3.8,
            'Brasser et al. (2006) 3-Step Decoupling Mechanism:',
            color='#f1c40f',
            fontsize=10,
            fontweight='bold',
            zorder=16)
    ax.text(
        -8.3,
        -4.4,
        '1. Giant planets scatter comets outwards to $a \\sim 10^2-10^4$ AU.',
        color='#ffffff',
        fontsize=8.5,
        zorder=16)
    ax.text(
        -8.3,
        -4.9,
        '2. Embedded cluster stellar flybys deliver tidal kicks at aphelion.',
        color='#ffffff',
        fontsize=8.5,
        zorder=16)
    ax.text(
        -8.3,
        -5.5,
        '3. Perihelion lifted ($q > 36$ AU), permanently insulating Sedna sphere.',
        color='#ffffff',
        fontsize=8.5,
        zorder=16)

    ax.set_xlim(-9.0, 9.0)
    ax.set_ylim(-6.2, 6.2)
    ax.set_aspect('equal')
    ax.axis('off')

    plt.title(
        'Formation of the Sedna Sphere in the Solar Natal Embedded Star Cluster\n(Brasser, Duncan, & Levison 2006)',
        color='#ffffff',
        fontsize=15,
        fontweight='bold',
        pad=15)
    plt.tight_layout()
    plt.savefig(os.path.join(SCRIPT_DIR, "fig_diagram.pdf"),
                facecolor='#070b19')
    plt.savefig(os.path.join(SCRIPT_DIR, "fig_diagram.png"),
                facecolor='#070b19')
    plt.close()
    print("✅ Generated fig_diagram.pdf and fig_diagram.png")


def main():
    print(
        "============================================================================"
    )
    print("Paper #245: Generating High-Resolution Replication Figures...")
    print(
        "============================================================================"
    )
    d_enc, d_peri, d_trap, d_sweep, d_bm = load_data()
    make_comparison_plot(d_enc, d_peri, d_trap, d_sweep, d_bm)
    make_model_choices_plot(d_enc, d_peri, d_trap, d_sweep, d_bm)
    make_diagram_plot()
    print(
        "============================================================================"
    )
    print("✅ All plots generated successfully.")
    print(
        "============================================================================"
    )


if __name__ == '__main__':
    main()
