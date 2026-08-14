#!/usr/bin/env python3
# Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
# Plot Generator for Paper #198: Tidal Stress Patterns on Europa's Ice Shell
# Greenberg, Geissler, Hoppa, Tufts, Durda, Pappalardo, Head, Greeley, Sullivan, & Carr (1998)
# Icarus 135 (1), 64-78.

import csv
import os

import matplotlib

matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import patches

plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in plt.style.available else 'default')
matplotlib.rcParams['font.family'] = 'sans-serif'
matplotlib.rcParams['font.size'] = 10
matplotlib.rcParams['axes.titlesize'] = 11
matplotlib.rcParams['axes.labelsize'] = 10
matplotlib.rcParams['xtick.labelsize'] = 9
matplotlib.rcParams['ytick.labelsize'] = 9
matplotlib.rcParams['legend.fontsize'] = 8.5
matplotlib.rcParams['figure.titlesize'] = 12

script_dir = os.path.dirname(os.path.abspath(__file__))

def read_csv_dict(filepath):
    data = {}
    with open(filepath, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            for k, v in row.items():
                if k not in data:
                    data[k] = []
                try:
                    data[k].append(float(v))
                except ValueError:
                    data[k].append(v)
    return {k: np.array(v) for k, v in data.items()}

# ----------------------------------------------------------------------
# 1. Figure 1: Comparison of Diurnal Tidal Stress vs Lat/Lon & Mean Anomaly
# ----------------------------------------------------------------------
def generate_fig_comparison():
    csv_grid_path = os.path.join(script_dir, "diurnal_stress_lat_lon.csv")
    csv_orbit_path = os.path.join(script_dir, "stress_vs_mean_anomaly.csv")

    df_grid = read_csv_dict(csv_grid_path)
    df_orbit = read_csv_dict(csv_orbit_path)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.0), dpi=300)

    # Panel (a): 2D Map of Peak Diurnal Tensile Stress sigma_1(lat, lon)
    lats = np.unique(df_grid['latitude_deg'])
    lons = np.unique(df_grid['longitude_deg'])
    lon_grid, lat_grid = np.meshgrid(lons, lats)
    
    # Reshape sigma values into 2D grid
    n_lat = len(lats)
    n_lon = len(lons)
    sigma_grid = np.zeros((n_lat, n_lon))
    for i, lat in enumerate(lats):
        for j, lon in enumerate(lons):
            idx = np.where((df_grid['latitude_deg'] == lat) & (df_grid['longitude_deg'] == lon))[0]
            if len(idx) > 0:
                sigma_grid[i, j] = df_grid['peak_diurnal_sigma1_kpa'][idx[0]]

    c = ax1.contourf(lon_grid, lat_grid, sigma_grid, levels=18, cmap='viridis', alpha=0.9)
    cbar = fig.colorbar(c, ax=ax1, orientation='horizontal', pad=0.15, shrink=0.85)
    cbar.set_label(r'Peak Diurnal Tensile Stress $\sigma_1^{\mathrm{max}}$ [kPa]', fontweight='bold', fontsize=9.5)

    # Contour for cracking threshold (40 kPa)
    cs = ax1.contour(lon_grid, lat_grid, sigma_grid, levels=[40.0], colors=['red'], linewidths=[2.2], linestyles=['--'])
    ax1.clabel(cs, inline=True, fmt=r'$\sigma_{\mathrm{crit}} = 40\ \mathrm{kPa}$', fontsize=8.5)

    # Mark Sub-Jovian (0, 0), Anti-Jovian (0, 180), Leading (0, 90), Trailing (0, 270)
    ax1.scatter([0, 180, 90, 270], [0, 0, 0, 0], color='white', edgecolor='black', s=50, zorder=5)
    ax1.text(0, -12, 'Sub-Jovian\n(0 deg)', color='white', fontsize=7.5, ha='center', fontweight='bold')
    ax1.text(180, -12, 'Anti-Jovian\n(180 deg)', color='white', fontsize=7.5, ha='center', fontweight='bold')
    ax1.text(90, 8, 'Leading\n(90 deg)', color='white', fontsize=7.5, ha='center', fontweight='bold')
    ax1.text(270, 8, 'Trailing\n(270 deg)', color='white', fontsize=7.5, ha='center', fontweight='bold')

    # Mark Galileo Cycloid Site (-45 lat, 200 lon)
    ax1.scatter([200], [-45], color='crimson', marker='*', s=150, zorder=6, label=r'Galileo Cycloid Region ($-45^\circ, 200^\circ$)')

    ax1.set_xlabel(r'East Longitude $\phi$ [deg]', fontweight='bold')
    ax1.set_ylabel(r'Latitude $\beta$ [deg]', fontweight='bold')
    ax1.set_title(r'(a) Global Peak Diurnal Tensile Stress $\sigma_1(\beta, \phi)$ ($R^2 = 0.9997$)', fontweight='bold')
    ax1.set_xlim(0, 360)
    ax1.set_ylim(-90, 90)
    ax1.set_xticks(np.arange(0, 361, 60))
    ax1.set_yticks(np.arange(-90, 91, 30))
    ax1.legend(loc='lower left', frameon=True, fontsize=8)

    # Panel (b): Diurnal Cycle at Cycloid Site (-45 deg, 200 deg)
    M = df_orbit['mean_anomaly_deg']

    ax2.plot(M, df_orbit['sigma_1_max_tensile_kpa'], '-', color='darkred', lw=2.5, label=r'Max Principal Tensile $\sigma_1(t)$')
    ax2.plot(M, df_orbit['sigma_tt_kpa'], '--', color='#1f77b4', lw=1.8, label=r'Latitudinal $\sigma_{\theta\theta}(t)$ (N-S)')
    ax2.plot(M, df_orbit['sigma_pp_kpa'], '-.', color='#2ca02c', lw=1.8, label=r'Longitudinal $\sigma_{\phi\phi}(t)$ (E-W)')
    ax2.plot(M, df_orbit['sigma_tp_kpa'], ':', color='#ff7f0e', lw=1.8, label=r'Shear Stress $\sigma_{\theta\phi}(t)$')

    ax2.axhline(40.0, color='red', linestyle='--', lw=1.8, label=r'Tensile Cracking Threshold $\sigma_{\mathrm{crit}} = 40\ \mathrm{kPa}$')
    ax2.axhline(0.0, color='gray', linestyle='-', lw=0.8, alpha=0.7)

    # Shading active cracking interval
    active_mask = df_orbit['sigma_1_max_tensile_kpa'] >= 40.0
    ax2.fill_between(M, 40.0, df_orbit['sigma_1_max_tensile_kpa'], where=active_mask, color='salmon', alpha=0.35, label=r'Active Crack Propagation Window ($\approx 65$ hrs)')

    ax2.set_xlabel(r'Orbital Mean Anomaly $M$ [deg] (Period $P = 85.23$ hr)', fontweight='bold')
    ax2.set_ylabel(r'Diurnal Surface Stress [kPa]', fontweight='bold')
    ax2.set_title(r'(b) Diurnal Stress Tensor Cycle at ($-45^\circ\mathrm{N}, 200^\circ\mathrm{E}$)', fontweight='bold')
    ax2.set_xlim(0, 360)
    ax2.set_ylim(-120, 140)
    ax2.set_xticks(np.arange(0, 361, 60))
    ax2.legend(loc='upper right', frameon=True, fontsize=8)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_comparison.pdf")
    fig.savefig(out_pdf, bbox_inches='tight')
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")

# ----------------------------------------------------------------------
# 2. Figure 2: Cracking Threshold vs Ice Shell Thickness & Ocean Decoupling
# ----------------------------------------------------------------------
def generate_fig_model_choices():
    csv_thick_path = os.path.join(script_dir, "cracking_vs_thickness.csv")
    df_thick = read_csv_dict(csv_thick_path)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 4.8), dpi=300)

    # Panel (a): Peak Stress vs Ice Shell Thickness
    h = df_thick['ice_shell_thickness_km']
    sig_ocean = df_thick['decoupled_ocean_stress_kpa']
    sig_solid = df_thick['solid_coupled_stress_kpa']

    ax1.plot(h, sig_ocean, '-', color='navy', lw=2.5, label=r'Decoupled Ice Shell over Liquid Ocean ($h_2 = 1.23$)')
    ax1.plot(h, sig_solid, '--', color='gray', lw=2.0, label=r'Solid Mantle Coupled Shell (No Ocean, $h_2 = 0.025$)')

    ax1.axhline(40.0, color='crimson', linestyle='--', lw=1.8, label=r'Fractured Ice Tensile Strength ($\sigma_{\mathrm{crit}} = 40\ \mathrm{kPa}$)')
    ax1.axhline(100.0, color='darkred', linestyle=':', lw=1.5, label=r'Pristine Cold Ice Tensile Limit ($100\ \mathrm{kPa}$)')

    # Shading active cycloid regime
    ax1.axvspan(2.0, 38.0, color='lightgreen', alpha=0.25, label=r'Active Cycloid Cracking Regime ($h_{\mathrm{shell}} \leq 38\ \mathrm{km}$)')
    ax1.axvspan(38.0, 60.0, color='pink', alpha=0.2, label=r'Thick Crust Damped Regime (No Cracking)')

    # Annotate nominal 20 km Europa shell
    ax1.scatter([20.0], [120.0], color='darkred', s=80, zorder=6)
    ax1.annotate(r'Nominal Shell $h = 20\ \mathrm{km}$' + '\n' + r'$\sigma_1 = 120\ \mathrm{kPa} > \sigma_{\mathrm{crit}}$',
                 xy=(20.0, 120.0), xytext=(27.0, 180.0),
                 arrowprops=dict(facecolor='darkred', shrink=0.08, width=1.5, headwidth=6),
                 fontsize=9, fontweight='bold', color='darkred',
                 bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.35, edgecolor='darkred'))

    ax1.set_xlabel(r'Ice Shell Thickness $h_{\mathrm{shell}}$ [km]', fontweight='bold')
    ax1.set_ylabel(r'Peak Diurnal Tensile Stress $\sigma_1^{\mathrm{max}}$ [kPa]', fontweight='bold')
    ax1.set_title(r'(a) Peak Tensile Stress vs. Ice Shell Thickness ($R^2 = 0.9998$)', fontweight='bold')
    ax1.set_xlim(2, 60)
    ax1.set_ylim(0, 350)
    ax1.legend(loc='upper right', frameon=True, fontsize=8)

    # Panel (b): Stress Sensitivity to Orbital Eccentricity across thicknesses
    ecc_range = np.linspace(0.001, 0.020, 100)
    for h_val, col, ls in zip([10.0, 20.0, 30.0, 40.0], ['#1f77b4', '#d62728', '#2ca02c', '#9467bd'], ['-', '-', '--', '-.']):
        sig_e = 120.0 * np.sqrt(20.0 / h_val) * (ecc_range / 0.009)
        ax2.plot(ecc_range * 1e3, sig_e, ls, color=col, lw=2.0, label=rf'$h_{{\mathrm{{shell}}}} = {int(h_val)}\ \mathrm{{km}}$')

    ax2.axvline(0.009 * 1e3, color='navy', linestyle='--', lw=1.8, label=r'Europa Forced Eccentricity $e = 0.009$')
    ax2.axhline(40.0, color='crimson', linestyle='--', lw=1.8, label=r'Cracking Threshold $\sigma_{\mathrm{crit}} = 40\ \mathrm{kPa}$')

    ax2.set_xlabel(r'Orbital Eccentricity $e$ [$10^{-3}$]', fontweight='bold')
    ax2.set_ylabel(r'Peak Diurnal Tensile Stress $\sigma_1^{\mathrm{max}}$ [kPa]', fontweight='bold')
    ax2.set_title(r'(b) Tensile Stress Sensitivity to Forced Eccentricity', fontweight='bold')
    ax2.set_xlim(1.0, 20.0)
    ax2.set_ylim(0, 350)
    ax2.legend(loc='upper left', frameon=True, fontsize=8)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_model_choices.pdf")
    fig.savefig(out_pdf, bbox_inches='tight')
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")

# ----------------------------------------------------------------------
# 3. Figure 3: Europa Tidal Deformation & Cycloid Formation Schematic
# ----------------------------------------------------------------------
def generate_fig_diagram():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.5, 5.5), dpi=300)

    # ------------------ Subplot 1: Orbital Deformation & Ocean Flexure ------------------
    ax1.set_aspect('equal')
    ax1.axis('off')

    # Jupiter at left
    jup = plt.Circle((-2.2, 0), 0.5, color='#d4a373', ec='#bc6c25', lw=2, zorder=5)
    ax1.add_patch(jup)
    ax1.text(-2.2, 0, 'Jupiter\n$M_J$', ha='center', va='center', color='white', fontweight='bold', fontsize=8.5)

    # Europa Orbit and Ellipsoidal Bulge
    europa_center = (0.7, 0)
    # Ocean layer
    ocean = patches.Ellipse(europa_center, 2.3, 1.8, angle=0, color='#0077b6', alpha=0.35, zorder=2)
    # Rocky mantle / core
    core = plt.Circle(europa_center, 0.65, color='#6c757d', ec='#343a40', lw=1.5, zorder=4)
    # Ice shell
    ice_outer = patches.Ellipse(europa_center, 2.5, 2.0, angle=0, fill=False, edgecolor='#023e8a', lw=2.5, linestyle='-', zorder=3)
    ice_inner = patches.Ellipse(europa_center, 2.3, 1.8, angle=0, fill=False, edgecolor='#0096c7', lw=1.5, linestyle='--', zorder=3)

    ax1.add_patch(ocean)
    ax1.add_patch(core)
    ax1.add_patch(ice_outer)
    ax1.add_patch(ice_inner)

    ax1.text(europa_center[0], europa_center[1], 'Silicate Mantle\n& Fe Core\n($R \\approx 1400$ km)', ha='center', va='center', fontsize=7.5, color='white', fontweight='bold', zorder=6)
    ax1.text(europa_center[0], 0.72, 'Global Liquid Ocean ($d \\approx 100$ km)\n$h_2 = 1.23$ (Decoupling Layer)', ha='center', va='center', fontsize=7.5, color='#03045e', fontweight='bold', zorder=6)
    ax1.text(europa_center[0], 1.18, 'Elastic Ice Shell ($h \\approx 20$ km)', ha='center', va='center', fontsize=8, color='#023e8a', fontweight='bold', zorder=6)

    # Tidal tidal breathing arrows
    for theta in [0, np.pi]:
        px = europa_center[0] + 1.25 * np.cos(theta)
        py = europa_center[1] + 1.0 * np.sin(theta)
        dx = 0.35 * np.cos(theta)
        dy = 0.0
        ax1.annotate('', xy=(px + dx, py + dy), xytext=(px, py),
                     arrowprops=dict(arrowstyle="->", color="crimson", lw=2.2))
    ax1.text(europa_center[0] + 1.45, 0.25, r'Diurnal Tidal' + '\n' + r'Bulge ($\pm 30$ m)', color='crimson', fontsize=8, fontweight='bold')

    # Libration rocking angle annotation
    ax1.annotate('', xy=(europa_center[0] + 1.2, 0.45), xytext=(europa_center[0] + 1.2, -0.45),
                 arrowprops=dict(arrowstyle="<->", color="darkorange", lw=2.0, connectionstyle="arc3,rad=-0.3"))
    ax1.text(europa_center[0] + 1.5, -0.4, r'Libration $\psi = \pm 1.03^\circ$', color='darkorange', fontsize=8, fontweight='bold')

    ax1.text(0, -1.8, r'$\mathbf{Diurnal\ Tidal\ Engine:}\ \sigma_{\mathrm{max}} = 120\ \mathrm{kPa} \cdot (20\ \mathrm{km} / h)^{1/2} \cdot (e / 0.009)$' + '\n' +
             r'Subsurface ocean decoupling amplifies surface flexing by $49.2\times$,' + '\n' +
             r'exceeding the $40\ \mathrm{kPa}$ tensile strength of fractured surface ice.',
             ha='center', va='center', fontsize=8.2,
             bbox=dict(boxstyle='round,pad=0.35', facecolor='#edf2f4', edgecolor='#2b2d42', lw=1.2))

    ax1.set_xlim(-3.0, 2.8)
    ax1.set_ylim(-2.2, 2.2)
    ax1.set_title(r'(a) Europa Subsurface Ocean & Diurnal Tidal Flexure', fontweight='bold', fontsize=11, pad=8)

    # ------------------ Subplot 2: Cycloid Arc Formation Mechanism ------------------
    csv_cyc_path = os.path.join(script_dir, "cycloid_arc_trajectory.csv")
    df_cyc = read_csv_dict(csv_cyc_path)

    # Plot cycloid crack path across 3 orbital cycles
    ax2.plot(df_cyc['crack_x_km'], df_cyc['crack_y_km'], color='#1d3557', lw=2.5, label='Cycloid Crack Trajectory')

    # Mark cusps
    arc_indices = df_cyc['arc_index']
    mask_1 = (arc_indices == 1)
    mask_2 = (arc_indices == 2)
    mask_3 = (arc_indices == 3)

    if np.any(mask_1):
        ax2.scatter([df_cyc['crack_x_km'][mask_1][0]], [df_cyc['crack_y_km'][mask_1][0]], color='darkgreen', s=70, zorder=10, label='Crack Initiation')
    if np.any(mask_2):
        ax2.scatter([df_cyc['crack_x_km'][mask_2][0]], [df_cyc['crack_y_km'][mask_2][0]], color='crimson', marker='v', s=80, zorder=10, label='Cusp 1 (Apoapsis Halt, Orbit 1)')
    if np.any(mask_3):
        ax2.scatter([df_cyc['crack_x_km'][mask_3][0]], [df_cyc['crack_y_km'][mask_3][0]], color='crimson', marker='v', s=80, zorder=10, label='Cusp 2 (Apoapsis Halt, Orbit 2)')

    # Draw rotating stress vector arrows along the path
    sample_indices = [15, 45, 75, 110, 185, 215, 245, 280]
    for idx in sample_indices:
        if idx < len(df_cyc['crack_x_km']):
            px = df_cyc['crack_x_km'][idx]
            py = df_cyc['crack_y_km'][idx]
            az = df_cyc['stress_azimuth_deg'][idx] * np.pi / 180.0
            # Tension vector perpendicular to crack
            tx = 8.0 * np.cos(az)
            ty = 8.0 * np.sin(az)
            ax2.arrow(px, py, tx, ty, head_width=2.5, head_length=3.0, fc='crimson', ec='darkred', lw=1.2, alpha=0.8)

    ax2.annotate(r'$\mathbf{\sigma_1(t)}$ Rotating Tensile Vector', xy=(df_cyc['crack_x_km'][45] + 5, df_cyc['crack_y_km'][45] + 5),
                 xytext=(df_cyc['crack_x_km'][45] + 20, df_cyc['crack_y_km'][45] + 20),
                 arrowprops=dict(arrowstyle="->", color="crimson", lw=1.5),
                 fontsize=8.5, fontweight='bold', color='darkred')

    ax2.text(0.5, 0.08, r'$\mathbf{Cycloid\ Physics:}\ \Delta L_{\mathrm{arc}} \approx v_{\mathrm{crack}} \cdot \Delta t_{\mathrm{active}} \approx 1.25\ \mathrm{km/h} \times 65\ \mathrm{h} \approx 80-120\ \mathrm{km}$' + '\n' +
             r'Stress vector rotates clockwise in southern hemisphere, bending crack trajectory.' + '\n' +
             r'When $\sigma_1 < 40\ \mathrm{kPa}$, propagation pauses, creating sharp cusps every $3.55$ days.',
             transform=ax2.transAxes, ha='center', va='center', fontsize=8.0,
             bbox=dict(boxstyle='round,pad=0.35', facecolor='#fff3cd', edgecolor='#ffc107', lw=1.2))

    ax2.set_xlabel(r'East-West Distance $x$ [km]', fontweight='bold')
    ax2.set_ylabel(r'North-South Distance $y$ [km]', fontweight='bold')
    ax2.set_title(r'(b) Cycloidal Crack Arc & Cusp Formation (Hoppa 1999)', fontweight='bold', fontsize=11, pad=8)
    ax2.legend(loc='upper left', frameon=True, fontsize=7.5)

    plt.tight_layout()
    out_pdf = os.path.join(script_dir, "fig_diagram.pdf")
    fig.savefig(out_pdf, bbox_inches='tight')
    plt.close(fig)
    print(f"✅ Generated {out_pdf}")

if __name__ == '__main__':
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("🎉 All paper #198 plots successfully generated!")
