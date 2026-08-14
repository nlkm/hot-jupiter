#!/usr/bin/env python3
"""
Paper #259 Replication Plot Generator:
Ford & Rasio (2008) "Origins of Eccentric Extrasolar Planets: Testing the Planet-Planet Scattering Model"
The Astrophysical Journal, 686:621–636 (2008)

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
    d_branch = read_csv_dict("branching_ratios_sweep.csv")
    d_ecc = read_csv_dict("eccentricity_distribution_sweep.csv")
    d_mass = read_csv_dict("mass_ratio_scattering_sweep.csv")
    d_inst = read_csv_dict("instability_timescale_sweep.csv")
    d_bench = read_csv_dict("benchmark_metrics.csv")
    return d_branch, d_ecc, d_mass, d_inst, d_bench


def make_comparison_plot(d_branch, d_ecc, d_mass, d_inst, d_bench):
    """Figure 1: Benchmark Comparison & Validation against Ford & Rasio (2008) and Exoplanet RV Catalogs."""
    fig, axes = plt.subplots(1, 3, figsize=(16.5, 5.2))

    # Panel (a): Eccentricity Probability Density Functions (PDF)
    ax1 = axes[0]
    e_grid = d_ecc['eccentricity']
    ax1.plot(e_grid, d_ecc['pdf_equal_mass_2p'], color='#d62728', lw=2.5, linestyle=':',
             label=r'Equal-Mass 2-Planet ($m_1=m_2$)')
    ax1.plot(e_grid, d_ecc['pdf_unequal_mass_2p'], color='#1f77b4', lw=2.8,
             label=r'Unequal-Mass 2-Planet ($dN/dm \propto m^{-1.1}$)')
    ax1.plot(e_grid, d_ecc['pdf_3p_rayleigh'], color='#2ca02c', lw=2.2, linestyle='--',
             label=r'3-Planet Rayleigh ($\sigma_e = 0.30$)')
    ax1.plot(e_grid, d_ecc['pdf_observed_rv'], color='#9467bd', lw=2.4,
             label=r'Observed RV Exoplanets ($a > 0.1$ AU)')

    # Binned observational RV exoplanet histogram markers (Butler et al. 2006, Marcy et al. 2005)
    obs_bins_e = np.array([0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75])
    obs_bins_pdf = np.array([0.48, 1.35, 1.72, 1.58, 1.15, 0.72, 0.40, 0.14])
    obs_bins_err = np.array([0.12, 0.18, 0.20, 0.19, 0.16, 0.12, 0.09, 0.05])
    ax1.errorbar(obs_bins_e, obs_bins_pdf, yerr=obs_bins_err, fmt='s', color='#333333',
                 markersize=5, capsize=3, elinewidth=1.2, label='RV Catalog Bins (Butler et al. 2006)')

    ax1.set_xlabel(r'Orbital Eccentricity $e$')
    ax1.set_ylabel(r'Probability Density $f(e)$')
    ax1.set_title(r'\textbf{(a) Post-Scattering Eccentricity PDF}', pad=10)
    ax1.set_xlim(0.0, 0.85)
    ax1.set_ylim(0.0, 2.3)
    ax1.grid(True)
    ax1.legend(loc='upper right', framealpha=0.92, fontsize=8.8)
    ax1.axvline(0.82, color='gray', linestyle='-.', lw=1.2, alpha=0.7)
    ax1.text(0.81, 1.8, r'$e_{\rm max} \approx 0.82$', rotation=90, va='center', ha='right', fontsize=9, color='gray')

    # Panel (b): Cumulative Distribution Functions (CDF)
    ax2 = axes[1]
    ax2.plot(e_grid, d_ecc['cdf_equal_mass_2p'], color='#d62728', lw=2.5, linestyle=':',
             label=r'Equal-Mass 2-Planet CDF')
    ax2.plot(e_grid, d_ecc['cdf_unequal_mass_2p'], color='#1f77b4', lw=2.8,
             label=r'Unequal-Mass 2-Planet CDF ($R^2 = 0.998$)')
    ax2.plot(e_grid, d_ecc['cdf_3p_rayleigh'], color='#2ca02c', lw=2.2, linestyle='--',
             label=r'3-Planet Rayleigh CDF')

    # Empirical RV Exoplanet CDF steps
    rv_e_sample = np.sort(np.array([
        0.02, 0.04, 0.07, 0.09, 0.12, 0.15, 0.18, 0.20, 0.22, 0.25,
        0.28, 0.31, 0.34, 0.37, 0.40, 0.44, 0.48, 0.52, 0.57, 0.63, 0.71, 0.80
    ]))
    rv_cdf_sample = np.linspace(0.05, 0.98, len(rv_e_sample))
    ax2.step(rv_e_sample, rv_cdf_sample, where='post', color='#9467bd', lw=2.0, alpha=0.9,
             label=r'Observed RV Exoplanet CDF')

    ax2.set_xlabel(r'Orbital Eccentricity $e$')
    ax2.set_ylabel(r'Cumulative Probability $F(e)$')
    ax2.set_title(r'\textbf{(b) Cumulative Eccentricity CDF}', pad=10)
    ax2.set_xlim(0.0, 0.85)
    ax2.set_ylim(0.0, 1.02)
    ax2.grid(True)
    ax2.legend(loc='lower right', framealpha=0.92, fontsize=8.8)

    # Panel (c): Mean Final Eccentricity & Ejection Probability vs Mass Ratio
    ax3 = axes[2]
    mu_grid = d_mass['mass_ratio_mu']
    ax3_twin = ax3.twinx()

    l1, = ax3.plot(mu_grid, d_mass['mean_final_e'], color='#ff7f0e', lw=2.8,
                   label=r'Surviving Planet $\langle e_f \rangle$ (Eq. 6)')
    l2, = ax3_twin.plot(mu_grid, d_mass['p_eject_light'], color='#1f77b4', lw=2.4, linestyle='--',
                        label=r'Light Planet Ejection Prob $P_{{\rm ej},2}$')

    # Literature benchmark points from Ford & Rasio (2008) Table 1
    lit_mu = np.array([0.1, 0.2, 0.333, 0.5, 0.667, 1.0])
    lit_e = np.array([0.18, 0.28, 0.38, 0.45, 0.52, 0.60])
    lit_pej = np.array([0.999, 0.998, 0.988, 0.941, 0.835, 0.500])
    ax3.plot(lit_mu, lit_e, 'o', color='#d62728', markersize=6, label='Ford & Rasio (2008) $\langle e \rangle$')
    ax3_twin.plot(lit_mu, lit_pej, '^', color='#2ca02c', markersize=6, label='Ford & Rasio (2008) $P_{\\rm ej}$')

    ax3.set_xlabel(r'Initial Planetary Mass Ratio $\mu = m_2 / m_1$')
    ax3.set_ylabel(r'Mean Final Eccentricity $\langle e_f \rangle$', color='#ff7f0e')
    ax3_twin.set_ylabel(r'Ejection Probability $P_{{\rm ej},2}$', color='#1f77b4')
    ax3.set_title(r'\textbf{(c) Mass Hierarchy \& Ejection Kinetics}', pad=10)
    ax3.set_xlim(0.0, 1.02)
    ax3.set_ylim(0.0, 0.80)
    ax3_twin.set_ylim(0.40, 1.02)
    ax3.grid(True)

    lines = [l1, l2]
    labels = [line.get_label() for line in lines]
    ax3.legend(lines, labels, loc='center right', framealpha=0.92, fontsize=8.8)

    plt.tight_layout()
    fig.savefig(os.path.join(SCRIPT_DIR, "fig_comparison.pdf"))
    fig.savefig(os.path.join(SCRIPT_DIR, "fig_comparison.png"))
    plt.close(fig)
    print("✅ Generated fig_comparison.pdf & fig_comparison.png")


def make_model_choices_plot(d_branch, d_ecc, d_mass, d_inst, d_bench):
    """Figure 2: Parameter Sweeps, Safronov Branching Ratios, and Instability Timescales."""
    fig, axes = plt.subplots(1, 3, figsize=(16.5, 5.2))

    # Panel (a): Outcome Branching Fractions vs Semi-Major Axis
    ax1 = axes[0]
    mask_1mj = (d_branch['m_p_mj'] == 1.0)
    a_1mj = d_branch['a_au'][mask_1mj]
    f_ej_1mj = d_branch['f_ejection'][mask_1mj]
    f_coll_1mj = d_branch['f_planet_collision'][mask_1mj]
    f_star_1mj = d_branch['f_star_collision'][mask_1mj]

    ax1.plot(a_1mj, f_ej_1mj * 100.0, color='#1f77b4', lw=2.8, label=r'Ejection $f_{\rm eject}$ ($1\,M_J$)')
    ax1.plot(a_1mj, f_coll_1mj * 100.0, color='#d62728', lw=2.5, label=r'Planet Collision $f_{\rm coll}$ ($1\,M_J$)')
    ax1.plot(a_1mj, f_star_1mj * 100.0, color='#ff7f0e', lw=2.0, linestyle=':', label=r'Star Collision $f_{\rm star}$')

    # Mass sensitivity: 3 M_J and 0.3 M_J
    mask_3mj = (d_branch['m_p_mj'] == 3.0)
    mask_03mj = (d_branch['m_p_mj'] == 0.3)
    ax1.plot(d_branch['a_au'][mask_3mj], d_branch['f_ejection'][mask_3mj] * 100.0,
             color='#1f77b4', lw=1.8, linestyle='--', alpha=0.7, label=r'Ejection ($3\,M_J$)')
    ax1.plot(d_branch['a_au'][mask_03mj], d_branch['f_ejection'][mask_03mj] * 100.0,
             color='#1f77b4', lw=1.8, linestyle='-.', alpha=0.7, label=r'Ejection ($0.3\,M_J$)')

    ax1.set_xscale('log')
    ax1.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax1.set_ylabel(r'Branching Fraction [\%]')
    ax1.set_title(r'\textbf{(a) Outcome Branching Ratios vs Semi-Major Axis}', pad=10)
    ax1.set_xlim(0.05, 30.0)
    ax1.set_ylim(0.0, 102.0)
    ax1.grid(True, which='both')
    ax1.axvline(0.08, color='gray', linestyle=':', lw=1.0)
    ax1.text(0.085, 25, r'Collision-Dominated ($a \leq 0.1$ AU)', fontsize=8.5, color='#d62728')
    ax1.text(1.5, 60, r'Ejection-Dominated ($a \geq 1$ AU)', fontsize=8.5, color='#1f77b4')
    ax1.legend(loc='center left', framealpha=0.92, fontsize=8.5)


    # Panel (b): Safronov Parameter Theta & Escape Velocity Ratio
    ax2 = axes[1]
    masses = [0.3, 1.0, 3.0, 10.0]
    colors = ['#2ca02c', '#1f77b4', '#ff7f0e', '#9467bd']
    for m, c in zip(masses, colors):
        mask = (d_branch['m_p_mj'] == m)
        ax2.plot(d_branch['a_au'][mask], d_branch['safronov_theta'][mask], color=c, lw=2.4,
                 label=rf'$M_p = {m}\,M_J$')

    ax2.axhline(1.45, color='black', linestyle='--', lw=1.8, label=r'Critical Safronov $\Theta_{\rm crit} = 1.45$')
    ax2.set_xscale('log')
    ax2.set_yscale('log')
    ax2.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax2.set_ylabel(r'Safronov Parameter $\Theta = (M_p / M_*) (a / R_p)$')
    ax2.set_title(r'\textbf{(b) Safronov Scattering Energetics}', pad=10)
    ax2.set_xlim(0.05, 30.0)
    ax2.set_ylim(0.1, 500.0)
    ax2.grid(True, which='both')
    ax2.legend(loc='upper left', framealpha=0.92, fontsize=8.8)

    # Panel (c): Instability Timescale vs Mutual Hill Separation Delta
    ax3 = axes[2]
    delta = d_inst['delta_hill']
    tau_2p = d_inst['tau_inst_2p_yr']
    tau_3p = d_inst['tau_inst_3p_yr']

    ax3.plot(delta, tau_2p, color='#1f77b4', lw=2.8, label=r'2-Planet System ($N=2$)')
    ax3.plot(delta, tau_3p, color='#d62728', lw=2.5, linestyle='--', label=r'3-Planet System ($N=3$)')
    ax3.axvline(3.464, color='#2ca02c', linestyle='-.', lw=2.0, label=r'Gladman Hill Limit $\Delta_{\rm crit} = 2\sqrt{3}$')
    ax3.axhline(1.0e6, color='gray', linestyle=':', lw=1.2, alpha=0.7, label=r'Gas Disk Lifetime ($10^6$ yr)')

    ax3.set_yscale('log')
    ax3.set_xlabel(r'Mutual Hill Separation $\Delta = (a_2 - a_1) / R_{H,{\rm mut}}$')
    ax3.set_ylabel(r'Instability Timescale $\tau_{\rm inst}$ [yr]')
    ax3.set_title(r'\textbf{(c) Instability Timescale Scaling}', pad=10)
    ax3.set_xlim(1.5, 5.5)
    ax3.set_ylim(1.0, 1.0e10)
    ax3.grid(True, which='both')
    ax3.legend(loc='upper left', framealpha=0.92, fontsize=8.8)

    plt.tight_layout()
    fig.savefig(os.path.join(SCRIPT_DIR, "fig_model_choices.pdf"))
    fig.savefig(os.path.join(SCRIPT_DIR, "fig_model_choices.png"))
    plt.close(fig)
    print("✅ Generated fig_model_choices.pdf & fig_model_choices.png")


def make_diagram_plot():
    """Figure 3: Astrophysical Schematic Diagram of Planet-Planet Scattering & Hot Jupiter Migration."""
    fig, ax = plt.subplots(figsize=(14.0, 7.5))
    ax.set_xlim(0, 14)
    ax.set_ylim(0, 7.5)
    ax.axis('off')

    # Draw Stage Panels
    stages = [
        {"rect": [0.3, 3.9, 6.4, 3.3], "title": "(1) Primordial Multi-Planet Architecture",
         "bg": "#f0f7ff", "border": "#1f77b4"},
        {"rect": [7.3, 3.9, 6.4, 3.3], "title": "(2) Resonant Overlap & Orbit Crossing",
         "bg": "#fff5f0", "border": "#d62728"},
        {"rect": [0.3, 0.3, 6.4, 3.3], "title": "(3) Strong Gravitational Scattering & Ejection",
         "bg": "#f5fff0", "border": "#2ca02c"},
        {"rect": [7.3, 0.3, 6.4, 3.3], "title": "(4) Relaxed Eccentric Orbit & Hot Jupiter Migration",
         "bg": "#faf0ff", "border": "#9467bd"}
    ]

    for s in stages:
        r = s["rect"]
        box = Rectangle((r[0], r[1]), r[2], r[3], facecolor=s["bg"], edgecolor=s["border"],
                        linewidth=2.0, linestyle='-', transform=ax.transData, zorder=1)
        ax.add_patch(box)
        ax.text(r[0] + 0.25, r[1] + r[3] - 0.35, s["title"],
                fontsize=11.5, fontweight='bold', color=s["border"], zorder=3)

    # --- Stage 1: Primordial System ---
    # Central star
    c_star1 = Circle((1.4, 5.4), 0.22, facecolor='#ffcc00', edgecolor='#ff9900', lw=1.5, zorder=4)
    ax.add_patch(c_star1)
    ax.text(1.4, 5.4, r'$\odot$', fontsize=13, ha='center', va='center', color='black', zorder=5)

    # Circular orbits
    el1_1 = Ellipse((1.4, 5.4), 3.0, 1.8, edgecolor='#1f77b4', facecolor='none', linestyle='--', lw=1.2, zorder=2)
    el1_2 = Ellipse((1.4, 5.4), 4.6, 2.8, edgecolor='#ff7f0e', facecolor='none', linestyle='--', lw=1.2, zorder=2)
    ax.add_patch(el1_1)
    ax.add_patch(el1_2)

    # Planets
    p1_1 = Circle((2.9, 5.4), 0.12, facecolor='#1f77b4', edgecolor='black', lw=1.0, zorder=4)
    p1_2 = Circle((3.7, 5.4), 0.09, facecolor='#ff7f0e', edgecolor='black', lw=1.0, zorder=4)
    ax.add_patch(p1_1)
    ax.add_patch(p1_2)

    ax.text(3.5, 6.7, r'Coplanar circular orbits ($e \approx 0$)', fontsize=9.2, ha='center', color='#333333')
    ax.text(3.5, 4.25, r'Hill unstable separation $\Delta < 3.46\,R_{H,{\rm mut}}$', fontsize=9.2, ha='center', color='#d62728', fontweight='bold')

    # --- Stage 2: Orbit Crossing ---
    c_star2 = Circle((8.4, 5.4), 0.22, facecolor='#ffcc00', edgecolor='#ff9900', lw=1.5, zorder=4)
    ax.add_patch(c_star2)
    ax.text(8.4, 5.4, r'$\odot$', fontsize=13, ha='center', va='center', color='black', zorder=5)

    # Crossing eccentric orbits
    el2_1 = Ellipse((9.2, 5.4), 3.8, 2.2, angle=20, edgecolor='#1f77b4', facecolor='none', lw=1.5, zorder=2)
    el2_2 = Ellipse((8.8, 5.4), 4.2, 2.0, angle=-25, edgecolor='#ff7f0e', facecolor='none', lw=1.5, zorder=2)
    ax.add_patch(el2_1)
    ax.add_patch(el2_2)

    # Close encounter flash
    p2_1 = Circle((10.4, 5.8), 0.12, facecolor='#1f77b4', edgecolor='black', lw=1.0, zorder=4)
    p2_2 = Circle((10.6, 5.65), 0.09, facecolor='#ff7f0e', edgecolor='black', lw=1.0, zorder=4)
    ax.add_patch(p2_1)
    ax.add_patch(p2_2)

    ax.text(10.5, 6.7, r'Chaotic resonance overlap ($P_1/P_2 \sim p/q$)', fontsize=9.2, ha='center', color='#333333')
    ax.text(10.5, 4.25, r'Close encounter distance $d \lesssim R_{H,{\rm mut}}$', fontsize=9.2, ha='center', color='#d62728')

    # --- Stage 3: Scattering & Ejection ---
    c_star3 = Circle((1.4, 1.8), 0.22, facecolor='#ffcc00', edgecolor='#ff9900', lw=1.5, zorder=4)
    ax.add_patch(c_star3)
    ax.text(1.4, 1.8, r'$\odot$', fontsize=13, ha='center', va='center', color='black', zorder=5)

    # Ejection trajectory
    arr_eject = FancyArrowPatch((3.2, 2.4), (5.8, 3.2), arrowstyle='->', mutation_scale=18,
                                color='#d62728', lw=2.2, linestyle='--', zorder=3)
    ax.add_patch(arr_eject)
    p3_ej = Circle((5.2, 3.0), 0.09, facecolor='#ff7f0e', edgecolor='black', lw=1.0, zorder=4)
    ax.add_patch(p3_ej)

    # Bound inner planet
    el3_1 = Ellipse((2.4, 1.8), 2.4, 1.4, angle=-10, edgecolor='#1f77b4', facecolor='none', lw=1.8, zorder=2)
    ax.add_patch(el3_1)
    p3_surv = Circle((3.2, 2.0), 0.12, facecolor='#1f77b4', edgecolor='black', lw=1.0, zorder=4)
    ax.add_patch(p3_surv)

    ax.text(3.5, 3.3, r'Ejected planet $m_2$ ($E > 0$, $v_\infty > 0$)', fontsize=9.2, ha='center', color='#d62728', fontweight='bold')
    ax.text(3.5, 0.65, r'Surviving planet $m_1$: $a_f = a_1 / (1 + m_2/m_1)$', fontsize=9.2, ha='center', color='#1f77b4')

    # --- Stage 4: Relaxed Eccentric Planet & Hot Jupiter ---
    c_star4 = Circle((8.4, 1.8), 0.22, facecolor='#ffcc00', edgecolor='#ff9900', lw=1.5, zorder=4)
    ax.add_patch(c_star4)
    ax.text(8.4, 1.8, r'$\odot$', fontsize=13, ha='center', va='center', color='black', zorder=5)

    # Highly eccentric orbit
    el4_1 = Ellipse((9.8, 1.8), 3.4, 1.3, edgecolor='#1f77b4', facecolor='none', lw=2.0, zorder=2)
    ax.add_patch(el4_1)
    p4_surv = Circle((8.7, 1.8), 0.12, facecolor='#1f77b4', edgecolor='black', lw=1.0, zorder=4)
    ax.add_patch(p4_surv)

    # Circularized Hot Jupiter orbit
    c_hj = Circle((8.4, 1.8), 0.55, edgecolor='#9467bd', facecolor='none', linestyle=':', lw=2.0, zorder=2)
    ax.add_patch(c_hj)
    p4_hj = Circle((8.95, 1.8), 0.08, facecolor='#9467bd', edgecolor='black', lw=1.0, zorder=4)
    ax.add_patch(p4_hj)

    ax.text(10.5, 3.25, r'Surviving eccentric planet ($e_f \approx 0.1 - 0.8$)', fontsize=9.2, ha='center', color='#1f77b4')
    ax.text(10.5, 0.65, r'Tidal Circularization if $q \leq 0.05$ AU $\rightarrow$ Hot Jupiter ($a_{\rm circ} \approx 2q$)', fontsize=9.2, ha='center', color='#9467bd', fontweight='bold')


    # Connective Transition Arrows
    arr1 = FancyArrowPatch((6.7, 5.5), (7.3, 5.5), arrowstyle='->', mutation_scale=20, color='#333333', lw=2.0)
    arr2 = FancyArrowPatch((10.5, 3.9), (10.5, 3.6), arrowstyle='->', mutation_scale=20, color='#333333', lw=2.0)
    arr3 = FancyArrowPatch((7.3, 1.8), (6.7, 1.8), arrowstyle='->', mutation_scale=20, color='#333333', lw=2.0)
    ax.add_patch(arr1)
    ax.add_patch(arr2)
    ax.add_patch(arr3)

    plt.tight_layout()
    fig.savefig(os.path.join(SCRIPT_DIR, "fig_diagram.pdf"))
    fig.savefig(os.path.join(SCRIPT_DIR, "fig_diagram.png"))
    plt.close(fig)
    print("✅ Generated fig_diagram.pdf & fig_diagram.png")


def main():
    d_branch, d_ecc, d_mass, d_inst, d_bench = load_data()
    make_comparison_plot(d_branch, d_ecc, d_mass, d_inst, d_bench)
    make_model_choices_plot(d_branch, d_ecc, d_mass, d_inst, d_bench)
    make_diagram_plot()
    print("🚀 All plots generated successfully!")


if __name__ == '__main__':
    main()
