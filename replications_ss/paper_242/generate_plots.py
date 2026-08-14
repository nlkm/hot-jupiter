#!/usr/bin/env python3
"""
Paper #242 Replication Plot Generator:
Kathryn Volk & Renu Malhotra (2017) "The Curvature of the Distant Kuiper Belt"
The Astronomical Journal, 154:62

Generates publication-quality figures:
  - fig_comparison.pdf / fig_comparison.png
  - fig_model_choices.pdf / fig_model_choices.png
  - fig_diagram.pdf / fig_diagram.png
"""

import csv
import os

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import LogNorm

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
    d_prof = read_csv_dict("laplace_plane_profiles.csv")
    d_bin = read_csv_dict("kbo_observational_binned.csv")
    d_ma = read_csv_dict("parameter_sweep_mass_distance.csv")
    d_in = read_csv_dict("parameter_sweep_inclination_node.csv")
    d_sec = read_csv_dict("secular_precession_timescales.csv")
    return d_prof, d_bin, d_ma, d_in, d_sec


def make_comparison_plot(d_prof, d_bin):
    """Figure 1: Benchmark Comparison & Validation against Volk & Malhotra (2017)."""
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))

    # -------------------------------------------------------------------------
    # Panel (a): Mean Orbital Inclination i(a) vs Semi-Major Axis a
    # -------------------------------------------------------------------------
    ax1 = axes[0, 0]
    ax1.plot(d_prof['a_au'],
             d_prof['inc_4p_deg'],
             color='#2b5c8f',
             lw=2.2,
             linestyle='--',
             label=r'4-Planet Standard Laplace Plane')
    ax1.plot(
        d_prof['a_au'],
        d_prof['inc_5p_deg'],
        color='#d62728',
        lw=2.6,
        label=r'5-Planet Warped Model ($M_p=1.5\,M_{\mathrm{Mars}}, a_p=60\,$AU)'
    )
    ax1.axhline(1.579,
                color='#7f7f7f',
                lw=1.2,
                linestyle=':',
                label=r'Invariable Plane ($i=1.58^\circ$)')

    # Observational data with error bars
    ax1.errorbar(d_bin['a_mean_au'],
                 d_bin['inc_obs_deg'],
                 yerr=d_bin['inc_err_deg'],
                 fmt='o',
                 color='#111111',
                 ecolor='#111111',
                 elinewidth=1.8,
                 capsize=4,
                 capthick=1.5,
                 markersize=6.5,
                 label=r'Observed KBO Mean Plane (Volk & Malhotra 2017)')

    ax1.set_xlim(30, 150)
    ax1.set_ylim(1.0, 9.0)
    ax1.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax1.set_ylabel(r'Mean Plane Inclination $i$ [deg]')
    ax1.set_title(r'(a) Mean Orbital Inclination vs Distance',
                  pad=10,
                  fontweight='bold')
    ax1.grid(True, linestyle=':')
    ax1.legend(loc='upper right', framealpha=0.92)

    # Highlight Classical vs Distant Warp
    ax1.axvspan(42.0,
                48.0,
                color='#2ca02c',
                alpha=0.10,
                label='Classical Belt (42-48 AU)')
    ax1.axvspan(50.0,
                80.0,
                color='#ff7f0e',
                alpha=0.12,
                label='Warped Distant Zone (50-80 AU)')
    ax1.text(45.0,
             7.8,
             'Classical Core\n(Unwarped)',
             fontsize=8.5,
             ha='center',
             color='#1b691b',
             fontweight='bold')
    ax1.text(70.0,
             7.8,
             'Curvature Peak\n(Warped)',
             fontsize=8.5,
             ha='center',
             color='#b35400',
             fontweight='bold')

    # -------------------------------------------------------------------------
    # Panel (b): Longitude of Ascending Node Omega(a) vs Semi-Major Axis a
    # -------------------------------------------------------------------------
    ax2 = axes[0, 1]
    ax2.plot(d_prof['a_au'],
             d_prof['node_4p_deg'],
             color='#2b5c8f',
             lw=2.2,
             linestyle='--',
             label=r'4-Planet Laplace Plane')
    ax2.plot(d_prof['a_au'],
             d_prof['node_5p_deg'],
             color='#d62728',
             lw=2.6,
             label=r'5-Planet Perturber ($\Omega_p=85.0^\circ$)')
    ax2.axhline(107.58,
                color='#7f7f7f',
                lw=1.2,
                linestyle=':',
                label=r'Invariable Plane ($\Omega=107.6^\circ$)')

    ax2.errorbar(d_bin['a_mean_au'],
                 d_bin['node_obs_deg'],
                 yerr=d_bin['node_err_deg'],
                 fmt='s',
                 color='#111111',
                 ecolor='#111111',
                 elinewidth=1.8,
                 capsize=4,
                 capthick=1.5,
                 markersize=6.5,
                 label=r'Observed Ascending Node $\Omega_{\mathrm{obs}}$')

    ax2.set_xlim(30, 150)
    ax2.set_ylim(70, 140)
    ax2.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax2.set_ylabel(r'Longitude of Ascending Node $\Omega$ [deg]')
    ax2.set_title(r'(b) Longitude of Ascending Node vs Distance',
                  pad=10,
                  fontweight='bold')
    ax2.grid(True, linestyle=':')
    ax2.legend(loc='upper right', framealpha=0.92)

    # -------------------------------------------------------------------------
    # Panel (c): Angular Warp Offset Delta theta(a) vs Distance
    # -------------------------------------------------------------------------
    ax3 = axes[1, 0]
    ax3.plot(
        d_prof['a_au'],
        d_prof['warp_offset_deg'],
        color='#9467bd',
        lw=2.8,
        label=
        r'Warp Offset $\Delta\theta(a) = \arccos(\mathbf{n}_4 \cdot \mathbf{n}_5)$'
    )
    ax3.scatter(d_bin['a_mean_au'],
                d_bin['warp_offset_deg'],
                color='#8c564b',
                s=55,
                zorder=5,
                edgecolor='black',
                label=r'Binned Observational Warp $\Delta\theta_k$')

    ax3.axvline(60.0,
                color='#d62728',
                linestyle=':',
                lw=1.5,
                label=r'Perturber Orbit $a_p = 60.0\,$AU')
    ax3.set_xlim(30, 150)
    ax3.set_ylim(0.0, 8.0)
    ax3.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax3.set_ylabel(r'Angular Warp Angle $\Delta\theta$ [deg]')
    ax3.set_title(r'(c) Laplace Plane Curvature Warp Profile',
                  pad=10,
                  fontweight='bold')
    ax3.grid(True, linestyle=':')
    ax3.legend(loc='upper right', framealpha=0.92)
    ax3.annotate(r'Local Peak $\Delta\theta_{\max} \approx 7.2^\circ$',
                 xy=(60.0, 7.2),
                 xytext=(85.0, 5.0),
                 arrowprops=dict(facecolor='black',
                                 shrink=0.08,
                                 width=1.2,
                                 headwidth=6),
                 fontweight='bold',
                 fontsize=9.5)

    # -------------------------------------------------------------------------
    # Panel (d): Residuals and Chi^2 Improvement across Observation Bins
    # -------------------------------------------------------------------------
    ax4 = axes[1, 1]
    bins_x = np.arange(len(d_bin['bin_label']))
    width = 0.35

    ax4.bar(bins_x - width / 2,
            d_bin['chi2_4p'],
            width,
            label=r'4-Planet Baseline ($\chi^2_{\mathrm{tot}}=66.9$)',
            color='#1f77b4',
            edgecolor='black',
            alpha=0.85)
    ax4.bar(bins_x + width / 2,
            d_bin['chi2_5p'],
            width,
            label=r'5-Planet Perturber ($\chi^2_{\mathrm{tot}}=31.2$)',
            color='#2ca02c',
            edgecolor='black',
            alpha=0.85)

    labels = [
        '35-40 AU', '40-42 AU', '42-45 AU', '45-48 AU', '50-60 AU', '60-80 AU',
        '80-150 AU'
    ]
    ax4.set_xticks(bins_x)
    ax4.set_xticklabels(labels, rotation=25, ha='right', fontsize=9)
    ax4.set_ylabel(r'Bin $\chi_k^2$ Contribution')
    ax4.set_title(
        r'(d) Goodness-of-Fit $\chi^2$ Comparison ($\Delta\chi^2=35.7, p < 10^{-4}$)',
        pad=10,
        fontweight='bold')
    ax4.grid(True, axis='y', linestyle=':')
    ax4.legend(loc='upper right', framealpha=0.92)

    # Annotation of statistical significance
    ax4.text(0.04,
             0.85,
             r'$\Delta\chi^2 = 35.70$' + '\n' +
             r'$p$-value $< 10^{-4}$ (Highly Significant)',
             transform=ax4.transAxes,
             fontsize=10,
             fontweight='bold',
             bbox=dict(boxstyle='round,pad=0.5',
                       facecolor='#ffffcc',
                       edgecolor='#cccc00',
                       alpha=0.9))

    plt.tight_layout()
    pdf_path = os.path.join(SCRIPT_DIR, "fig_comparison.pdf")
    png_path = os.path.join(SCRIPT_DIR, "fig_comparison.png")
    fig.savefig(pdf_path)
    fig.savefig(png_path)
    plt.close(fig)
    print(f"✅ Saved {pdf_path} and {png_path}")


def make_model_choices_plot(d_ma, d_in, d_sec):
    """Figure 2: Parameter Sensitivities, Secular Breakdown, and Timescales."""
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))

    # -------------------------------------------------------------------------
    # Panel (a): 2D Parameter Space Heatmap: a_p vs M_p
    # -------------------------------------------------------------------------
    ax1 = axes[0, 0]
    a_vals = np.unique(d_ma['a_perturber_au'])
    m_vals = np.unique(d_ma['m_perturber_earth'])
    chi2_grid = np.zeros((len(m_vals), len(a_vals)))

    for i, m in enumerate(m_vals):
        for j, a in enumerate(a_vals):
            mask = (d_ma['a_perturber_au'] == a) & (d_ma['m_perturber_earth']
                                                    == m)
            if np.any(mask):
                chi2_grid[i, j] = d_ma['chi2_total'][mask][0]

    A_grid, M_grid = np.meshgrid(a_vals, m_vals)
    c1 = ax1.pcolormesh(A_grid,
                        M_grid,
                        chi2_grid,
                        cmap='viridis_r',
                        norm=LogNorm(vmin=28.0, vmax=2000.0),
                        shading='auto')
    cb1 = fig.colorbar(c1, ax=ax1)
    cb1.set_label(r'Total $\chi^2$ (dof = 10)')

    # Contours
    cs1 = ax1.contour(A_grid,
                      M_grid,
                      chi2_grid,
                      levels=[32.0, 40.0, 60.0, 100.0],
                      colors=['white', 'cyan', 'yellow', 'orange'],
                      linewidths=1.5)
    ax1.clabel(cs1, inline=True, fontsize=8, fmt='%.0f')

    # Best-fit fiducial point
    ax1.plot(60.0,
             0.16,
             marker='*',
             color='#ff0000',
             markersize=14,
             markeredgecolor='white',
             markeredgewidth=1.2,
             label=r'Fiducial ($a_p=60\,$AU, $M_p=0.16\,M_\oplus$)')

    ax1.set_xlabel(r'Perturber Semi-Major Axis $a_p$ [AU]')
    ax1.set_ylabel(r'Perturber Mass $M_p$ [$M_\oplus$]')
    ax1.set_title(r'(a) Parameter Space: Distance $a_p$ vs Mass $M_p$',
                  pad=10,
                  fontweight='bold')
    ax1.legend(loc='upper right', framealpha=0.92)

    # -------------------------------------------------------------------------
    # Panel (b): 2D Parameter Space Heatmap: i_p vs Omega_p
    # -------------------------------------------------------------------------
    ax2 = axes[0, 1]
    inc_vals = np.unique(d_in['inc_perturber_deg'])
    node_vals = np.unique(d_in['node_perturber_deg'])
    chi2_in_grid = np.zeros((len(inc_vals), len(node_vals)))

    for i, inc in enumerate(inc_vals):
        for j, node in enumerate(node_vals):
            mask = (d_in['inc_perturber_deg']
                    == inc) & (d_in['node_perturber_deg'] == node)
            if np.any(mask):
                chi2_in_grid[i, j] = d_in['chi2_total'][mask][0]

    Node_grid, Inc_grid = np.meshgrid(node_vals, inc_vals)
    c2 = ax2.pcolormesh(Node_grid,
                        Inc_grid,
                        chi2_in_grid,
                        cmap='plasma_r',
                        norm=LogNorm(vmin=31.0, vmax=500.0),
                        shading='auto')
    cb2 = fig.colorbar(c2, ax=ax2)
    cb2.set_label(r'Total $\chi^2$ (dof = 10)')

    cs2 = ax2.contour(Node_grid,
                      Inc_grid,
                      chi2_in_grid,
                      levels=[33.0, 36.0, 45.0, 65.0],
                      colors=['white', 'cyan', 'yellow', 'green'],
                      linewidths=1.5)
    ax2.clabel(cs2, inline=True, fontsize=8, fmt='%.0f')

    ax2.plot(85.0,
             8.5,
             marker='*',
             color='#ff0000',
             markersize=14,
             markeredgecolor='white',
             markeredgewidth=1.2,
             label=r'Fiducial ($\Omega_p=85^\circ, i_p=8.5^\circ$)')

    ax2.set_xlabel(r'Perturber Ascending Node $\Omega_p$ [deg]')
    ax2.set_ylabel(r'Perturber Inclination $i_p$ [deg]')
    ax2.set_title(r'(b) Parameter Space: Orientation ($\Omega_p, i_p$)',
                  pad=10,
                  fontweight='bold')
    ax2.legend(loc='upper right', framealpha=0.92)

    # -------------------------------------------------------------------------
    # Panel (c): Secular Nodal Coupling Rate Breakdown B_j(a)
    # -------------------------------------------------------------------------
    ax3 = axes[1, 0]
    ax3.plot(d_sec['a_au'],
             d_sec['B_nep_arcsec_yr'],
             color='#1f77b4',
             lw=2.2,
             label=r'Neptune ($B_{\mathrm{Nep}}$)')
    ax3.plot(d_sec['a_au'],
             d_sec['B_ura_arcsec_yr'],
             color='#2ca02c',
             lw=1.8,
             label=r'Uranus ($B_{\mathrm{Ura}}$)')
    ax3.plot(d_sec['a_au'],
             d_sec['B_sat_arcsec_yr'],
             color='#ff7f0e',
             lw=1.8,
             label=r'Saturn ($B_{\mathrm{Sat}}$)')
    ax3.plot(d_sec['a_au'],
             d_sec['B_jup_arcsec_yr'],
             color='#8c564b',
             lw=1.8,
             label=r'Jupiter ($B_{\mathrm{Jup}}$)')
    ax3.plot(d_sec['a_au'],
             d_sec['B_pert_arcsec_yr'],
             color='#d62728',
             lw=2.4,
             label=r'Perturber $P_X$ ($B_{\mathrm{pert}}$)')
    ax3.plot(d_sec['a_au'],
             d_sec['B_tot_arcsec_yr'],
             color='#111111',
             lw=2.6,
             linestyle='--',
             label=r'Total $B_{\mathrm{tot}}(a)$')

    ax3.set_yscale('log')
    ax3.set_xlim(30, 150)
    ax3.set_ylim(1e-4, 5.0)
    ax3.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax3.set_ylabel(r'Secular Coupling Rate $B_j$ [arcsec/yr]')
    ax3.set_title(r'(c) Secular Precession Frequency Breakdown',
                  pad=10,
                  fontweight='bold')
    ax3.grid(True, which='both', linestyle=':')
    ax3.legend(loc='upper right', framealpha=0.92)

    # -------------------------------------------------------------------------
    # Panel (d): Secular Precession Timescale T_prec(a) vs System Age
    # -------------------------------------------------------------------------
    ax4 = axes[1, 1]
    ax4.plot(
        d_sec['a_au'],
        d_sec['T_prec_myr'],
        color='#9467bd',
        lw=2.6,
        label=r'Precession Period $T_{\mathrm{prec}} = 2\pi / B_{\mathrm{tot}}$'
    )
    ax4.axhline(4500.0,
                color='#d62728',
                linestyle='--',
                lw=1.8,
                label=r'Age of Solar System (4.5 Gyr)')
    ax4.axhline(
        1500.0,
        color='#ff7f0e',
        linestyle=':',
        lw=1.5,
        label=r'Phase-Mixing Limit ($t = 3\,T_{\mathrm{prec}} \leq 4.5\,$Gyr)')

    ax4.axvspan(30.0,
                78.0,
                color='#2ca02c',
                alpha=0.12,
                label=r'Fully Phase-Mixed ($N_{\mathrm{cyc}} \geq 3$)')
    ax4.axvspan(78.0,
                150.0,
                color='#7f7f7f',
                alpha=0.10,
                label=r'Partial Phase-Mixing')

    ax4.set_yscale('log')
    ax4.set_xlim(30, 150)
    ax4.set_ylim(1.0, 20000.0)
    ax4.set_xlabel(r'Semi-Major Axis $a$ [AU]')
    ax4.set_ylabel(r'Secular Precession Timescale $T_{\mathrm{prec}}$ [Myr]')
    ax4.set_title(r'(d) Nodal Precession Timescales & Phase Mixing',
                  pad=10,
                  fontweight='bold')
    ax4.grid(True, which='both', linestyle=':')
    ax4.legend(loc='lower right', framealpha=0.92)

    plt.tight_layout()
    pdf_path = os.path.join(SCRIPT_DIR, "fig_model_choices.pdf")
    png_path = os.path.join(SCRIPT_DIR, "fig_model_choices.png")
    fig.savefig(pdf_path)
    fig.savefig(png_path)
    plt.close(fig)
    print(f"✅ Saved {pdf_path} and {png_path}")


def make_diagram_plot():
    """Figure 3: Schematic Architecture Diagram of the Kuiper Belt Laplace Plane Warp."""
    fig = plt.figure(figsize=(13, 8.5))
    ax = fig.add_subplot(111)

    r = np.linspace(0, 140, 600)

    # 1. Unperturbed 4-Planet Laplace Plane (slope tan(i_4) ~ tan(1.65 deg))
    slope_4p = np.tan(1.65 * np.pi / 180.0)
    z_4p = r * slope_4p

    # 2. Invariable Plane (slope tan(1.58 deg))
    slope_inv = np.tan(1.579 * np.pi / 180.0)
    z_inv = r * slope_inv

    # 3. Warped 5-Planet Laplace Plane profile Z_L(R)
    z_5p = np.zeros_like(r)
    for i, rad in enumerate(r):
        if rad < 30.0 or rad < 50.0:
            z_5p[i] = rad * slope_4p
        elif rad <= 60.0:
            frac = (rad - 50.0) / 10.0
            inc_eff = 1.7 + frac * (8.5 - 1.7)
            z_5p[i] = rad * np.tan(inc_eff * np.pi / 180.0)
        elif rad <= 80.0:
            frac = (rad - 60.0) / 20.0
            inc_eff = 8.5 - frac * (8.5 - 2.0)
            z_5p[i] = rad * np.tan(inc_eff * np.pi / 180.0)
        else:
            frac = min(1.0, (rad - 80.0) / 60.0)
            inc_eff = 2.0 - frac * (2.0 - 1.7)
            z_5p[i] = rad * np.tan(inc_eff * np.pi / 180.0)

    # Plot Reference Planes
    ax.plot(r,
            np.zeros_like(r),
            color='#7f7f7f',
            linestyle='--',
            lw=1.5,
            label=r'Ecliptic Reference Plane ($Z = 0$)')
    ax.plot(
        r,
        z_inv,
        color='#2ca02c',
        linestyle=':',
        lw=2.0,
        label=r'Solar System Invariable Plane ($i_{\mathrm{inv}} = 1.58^\circ$)'
    )
    ax.plot(
        r,
        z_4p,
        color='#1f77b4',
        linestyle='-.',
        lw=2.2,
        label=r'4-Planet Classical Laplace Plane ($i_{L,4} \approx 1.65^\circ$)'
    )
    ax.plot(r,
            z_5p,
            color='#d62728',
            lw=3.2,
            label=r'Warped Laplace Plane Surface ($P_X$ Perturber Model)')

    # Shaded Kuiper Belt Populations
    r_class = np.linspace(42, 48, 100)
    z_class_mid = r_class * slope_4p
    ax.fill_between(r_class,
                    z_class_mid - 1.5,
                    z_class_mid + 1.5,
                    color='#1f77b4',
                    alpha=0.25,
                    label=r'Classical Kuiper Belt ($42-48\,$AU, Phase-Mixed)')

    r_dist = np.linspace(50, 80, 200)
    z_dist_mid = np.interp(r_dist, r, z_5p)
    ax.fill_between(
        r_dist,
        z_dist_mid - 2.5,
        z_dist_mid + 2.5,
        color='#ff7f0e',
        alpha=0.30,
        label=r'Distant Kuiper Belt ($50-80\,$AU, Warped Mean Plane)')

    r_scat = np.linspace(80, 135, 200)
    z_scat_mid = np.interp(r_scat, r, z_5p)
    ax.fill_between(r_scat,
                    z_scat_mid - 4.0,
                    z_scat_mid + 4.0,
                    color='#9467bd',
                    alpha=0.20,
                    label=r'Detached / Scattered Disc ($80-140\,$AU)')

    # Giant Planet Markers
    planets = [('Sun', 0.0, 0.0, '#ffcc00', 16, -1.2),
               ('Jupiter (5.2 AU)', 5.2, 5.2 * np.tan(1.3 * np.pi / 180),
                '#b35900', 10, 0.8),
               ('Saturn (9.6 AU)', 9.6, 9.6 * np.tan(2.5 * np.pi / 180),
                '#e6b800', 9, -1.1),
               ('Uranus (19.2 AU)', 19.2, 19.2 * np.tan(0.8 * np.pi / 180),
                '#00b3b3', 8, 0.8),
               ('Neptune (30.1 AU)', 30.1, 30.1 * np.tan(1.8 * np.pi / 180),
                '#0040ff', 9, -1.1),
               (r'Perturber $P_X$ ($60\,$AU, $i=8.5^\circ$)', 60.0,
                60.0 * np.tan(8.5 * np.pi / 180), '#ff0000', 13, 0.8)]

    for name, pr, pz, color, size, offset_y in planets:
        ax.plot(pr,
                pz,
                marker='o',
                color=color,
                markersize=size,
                markeredgecolor='black',
                markeredgewidth=1.2)
        ax.text(pr,
                pz + offset_y,
                name,
                fontsize=9.0,
                fontweight='bold',
                ha='center',
                bbox=dict(boxstyle='round,pad=0.2',
                          facecolor='white',
                          edgecolor=color,
                          alpha=0.88))

    # Curvature warp angle indicator arc
    ax.annotate('',
                xy=(60.0, z_5p[np.abs(r - 60.0).argmin()]),
                xytext=(60.0, z_4p[np.abs(r - 60.0).argmin()]),
                arrowprops=dict(arrowstyle='<->', color='#d62728', lw=2.0))
    ax.text(62.0,
            4.5,
            r'Warp Deflection $\Delta\theta \approx 7.2^\circ$' + '\n' +
            r'($\Delta i \approx 1.5^\circ - 3.0^\circ$)',
            color='#d62728',
            fontsize=10,
            fontweight='bold')

    # Precession cone illustration
    ax.annotate(r'Nodal Precession $\dot{\Omega} = B_{\mathrm{tot}}(a)$' +
                '\n' + r'($T_{\mathrm{prec}} \ll 4.5\,$Gyr, Phase-Mixed)',
                xy=(45.0, 1.3),
                xytext=(35.0, 7.0),
                arrowprops=dict(facecolor='black',
                                shrink=0.05,
                                width=1.0,
                                headwidth=5),
                fontsize=9.5,
                fontweight='bold',
                bbox=dict(boxstyle='round,pad=0.3',
                          facecolor='#e6f2ff',
                          edgecolor='#1f77b4'))

    ax.set_xlim(-5, 145)
    ax.set_ylim(-3.5, 12.0)
    ax.set_xlabel(r'Heliocentric Distance $R$ [AU]', fontsize=13)
    ax.set_ylabel(r'Vertical Height $Z$ [AU]', fontsize=13)
    ax.set_title(
        r'Physical Architecture of the Warped Distant Kuiper Belt & Laplace Plane',
        fontsize=14,
        fontweight='bold',
        pad=12)
    ax.grid(True, linestyle=':', alpha=0.6)
    ax.legend(loc='upper left', framealpha=0.92, fontsize=9.5)

    plt.tight_layout()
    pdf_path = os.path.join(SCRIPT_DIR, "fig_diagram.pdf")
    png_path = os.path.join(SCRIPT_DIR, "fig_diagram.png")
    fig.savefig(pdf_path)
    fig.savefig(png_path)
    plt.close(fig)
    print(f"✅ Saved {pdf_path} and {png_path}")


def main():
    print("Loading simulation datasets...")
    d_prof, d_bin, d_ma, d_in, d_sec = load_data()
    print("Generating Figure 1: Benchmark Comparison...")
    make_comparison_plot(d_prof, d_bin)
    print("Generating Figure 2: Model Choices & Parameter Sweeps...")
    make_model_choices_plot(d_ma, d_in, d_sec)
    print("Generating Figure 3: Physical Architecture Diagram...")
    make_diagram_plot()
    print("✨ All publication figures generated successfully!")


if __name__ == '__main__':
    main()
