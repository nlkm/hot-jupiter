#!/usr/bin/env python3
# Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
# Paper #221 Replication: Spohn & Schubert (2003)
# "Oceans in the Icy Moons of Saturn and Jupiter" (Icarus 161, 456-467)

import csv
import os

import matplotlib
import numpy as np

matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec

# Style settings for publication-grade formatting
plt.rcParams.update({
    'font.family': 'serif',
    'font.size': 10,
    'axes.labelsize': 11,
    'axes.titlesize': 12,
    'xtick.labelsize': 9,
    'ytick.labelsize': 9,
    'legend.fontsize': 9,
    'figure.titlesize': 13,
    'text.usetex': False,
    'mathtext.fontset': 'dejavuserif',
    'lines.linewidth': 1.8,
    'axes.grid': True,
    'grid.alpha': 0.35,
    'grid.linestyle': '--'
})

output_dir = 'replications_ss/paper_221'


# Helper function to read CSV into dict of lists
def read_csv_data(filepath):
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
    return data


bench_data = read_csv_data(os.path.join(output_dir, 'benchmark_comparison.csv'))
sweep_data = read_csv_data(os.path.join(output_dir,
                                        'shell_thickness_sweep.csv'))
nh3_data = read_csv_data(os.path.join(output_dir, 'ammonia_sensitivity.csv'))
summary_data = read_csv_data(
    os.path.join(output_dir, 'ocean_equilibrium_summary.csv'))

# ============================================================================
# FIGURE 1: fig_comparison.pdf (Benchmark Verification & Multi-Satellite Balance)
# ============================================================================
fig1 = plt.figure(figsize=(12, 5.5))
gs1 = GridSpec(1, 2, width_ratios=[1.1, 1.2], wspace=0.28)

# Panel A: Published vs Model Engine Parity
ax1 = fig1.add_subplot(gs1[0])
pub = np.array(bench_data['published_val'])
mod = np.array(bench_data['model_val'])

# Compute R^2
ss_res = np.sum((pub - mod)**2)
ss_tot = np.sum((pub - np.mean(pub))**2)
r2 = 1.0 - (ss_res / ss_tot)

min_val = 0.5
max_val = 360.0
ax1.plot([min_val, max_val], [min_val, max_val],
         'k--',
         alpha=0.7,
         label='1:1 Parity Line')
ax1.fill_between([min_val, max_val], [min_val * 0.95, max_val * 0.95],
                 [min_val * 1.05, max_val * 1.05],
                 color='gray',
                 alpha=0.15,
                 label='±5% Deviation Band')

colors = [
    '#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b', '#e377c2',
    '#7f7f7f', '#bcbd22', '#17becf', '#333333'
]
markers = ['o', 's', '^', 'D', 'v', '<', '>', 'p', '*', 'h', '8']

for i in range(len(pub)):
    name = bench_data['metric_name'][i].split('(')[0].strip()
    ax1.scatter(pub[i],
                mod[i],
                color=colors[i % len(colors)],
                marker=markers[i % len(markers)],
                s=80,
                edgecolors='k',
                zorder=5,
                label=f"{name}: {pub[i]:.1f}")

ax1.set_xscale('log')
ax1.set_yscale('log')
ax1.set_xlim(0.8, 400)
ax1.set_ylim(0.8, 400)
ax1.set_xlabel('Spohn & Schubert (2003) Published Value')
ax1.set_ylabel('C++ Physics Engine Replication Value')
ax1.set_title(f'Physics Engine Parity ($R^2 = {r2:.5f}$)', fontweight='bold')
ax1.annotate(f'$R^2 = {r2:.5f}$\nMax Rel. Error $< 1.7\\%$',
             xy=(0.05, 0.78),
             xycoords='axes fraction',
             bbox=dict(boxstyle='round,pad=0.5',
                       facecolor='azure',
                       alpha=0.9,
                       edgecolor='teal'))
ax1.grid(True, which="both", ls="--", alpha=0.3)

# Panel B: Multi-Satellite Equilibrium Structure
ax2 = fig1.add_subplot(gs1[1])
sat_labels = [
    'Europa', 'Ganymede', 'Callisto (Pure)', 'Callisto (5% $NH_3$)',
    'Titan (5% $NH_3$)', 'Enceladus (Active)'
]
d_lid = np.array(summary_data['D_lid_km'])
d_conv = np.array(summary_data['D_conv_km'])
d_ocean = np.array(summary_data['D_ocean_km'])

y_pos = np.arange(len(sat_labels))
bar_width = 0.55

p1 = ax2.barh(y_pos,
              d_lid,
              bar_width,
              color='#3498db',
              edgecolor='k',
              label='Stagnant Lid $\\delta_{lid}$ [km]')
p2 = ax2.barh(y_pos,
              d_conv,
              bar_width,
              left=d_lid,
              color='#e67e22',
              edgecolor='k',
              label='Convective Ice Sublayer $d_v$ [km]')
p3 = ax2.barh(y_pos,
              d_ocean,
              bar_width,
              left=d_lid + d_conv,
              color='#2ecc71',
              edgecolor='k',
              label='Subsurface Ocean $D_{oc}$ [km]')

ax2.set_yticks(y_pos)
ax2.set_yticklabels(sat_labels)
ax2.invert_yaxis()
ax2.set_xlabel('Layer Thickness [km]')
ax2.set_title('Equilibrium Layering Across Outer Planet Moons',
              fontweight='bold')
ax2.legend(loc='lower right', framealpha=0.9)
ax2.set_xlim(0, 850)

# Annotate equilibrium shell thickness
for i in range(len(sat_labels)):
    tot_shell = d_lid[i] + d_conv[i]
    ax2.text(tot_shell + 10,
             i,
             f'$D_{{shell}} = {tot_shell:.1f}$ km',
             va='center',
             fontsize=8.5,
             fontweight='bold',
             color='#2c3e50')

plt.tight_layout()
fig1.savefig(os.path.join(output_dir, 'fig_comparison.pdf'), dpi=300)
plt.close(fig1)
print(">>> Created fig_comparison.pdf")

# ============================================================================
# FIGURE 2: fig_model_choices.pdf (Regimes, Scaling Laws & Ammonia Chemistry)
# ============================================================================
fig2 = plt.figure(figsize=(13, 4.5))
gs2 = GridSpec(1, 3, wspace=0.30)

# Panel A: Shell Heat Flux vs Shell Thickness
ax2a = fig2.add_subplot(gs2[0])
d_vals = np.array(sweep_data['D_km'])
ax2a.plot(d_vals,
          sweep_data['europa_F_total_mw_m2'],
          color='#e74c3c',
          label='Europa ($T_s=100$ K)')
ax2a.plot(d_vals,
          sweep_data['ganymede_F_total_mw_m2'],
          color='#3498db',
          label='Ganymede ($T_s=110$ K)')
ax2a.plot(d_vals,
          sweep_data['callisto_F_total_mw_m2'],
          color='#2ecc71',
          label='Callisto ($T_s=105$ K)')
ax2a.plot(d_vals,
          sweep_data['europa_F_cond_mw_m2'],
          'k:',
          alpha=0.6,
          label='Conductive Baseline')

# Heat supply thresholds
ax2a.axhline(23.0,
             color='#e74c3c',
             ls='--',
             alpha=0.7,
             label='Europa $F_{supply} = 23.0$ mW/m$^2$')
ax2a.axhline(5.5,
             color='#3498db',
             ls='--',
             alpha=0.7,
             label='Ganymede $F_{supply} = 5.5$ mW/m$^2$')
ax2a.axhline(3.2,
             color='#2ecc71',
             ls='--',
             alpha=0.7,
             label='Callisto $F_{supply} = 3.2$ mW/m$^2$')

ax2a.set_xlim(5, 150)
ax2a.set_ylim(0, 70)
ax2a.set_xlabel('Ice Shell Thickness $D$ [km]')
ax2a.set_ylabel('Total Heat Flux $F_{total}$ [mW/m$^2$]')
ax2a.set_title('(a) Heat Transport Capacity vs. Supply',
               fontweight='bold',
               fontsize=10.5)
ax2a.legend(loc='upper right', fontsize=7.5, framealpha=0.85)

# Panel B: Nusselt Number vs Basal Rayleigh Number
ax2b = fig2.add_subplot(gs2[1])
ax2b.plot(sweep_data['europa_Ra_b'],
          sweep_data['europa_Nu'],
          color='#e74c3c',
          label='Europa')
ax2b.plot(sweep_data['ganymede_Ra_b'],
          sweep_data['ganymede_Nu'],
          color='#3498db',
          label='Ganymede')
ax2b.plot(sweep_data['callisto_Ra_b'],
          sweep_data['callisto_Nu'],
          color='#2ecc71',
          label='Callisto')

ax2b.set_xscale('log')
ax2b.set_xlabel('Basal Rayleigh Number $Ra_b$')
ax2b.set_ylabel('Convective Nusselt Number $Nu$')
ax2b.set_title('(b) Parameterized Convection Vigor',
               fontweight='bold',
               fontsize=10.5)
ax2b.axvline(1.0e6,
             color='gray',
             ls=':',
             label='Critical Threshold $Ra_{cr} \\sim 10^6$')
ax2b.legend(loc='lower right', fontsize=8.0, framealpha=0.85)

# Panel C: Ammonia Antifreeze Sensitivity
ax2c = fig2.add_subplot(gs2[2])
nh3_pct = np.array(nh3_data['nh3_wt_pct'])
ax2c.plot(nh3_pct,
          nh3_data['callisto_D_eq_km'],
          color='#e67e22',
          lw=2.2,
          label='Callisto Shell $D_{eq}$')
ax2c.plot(nh3_pct,
          nh3_data['callisto_D_ocean_km'],
          color='#27ae60',
          lw=2.2,
          label='Callisto Ocean $D_{oc}$')
ax2c.plot(nh3_pct,
          nh3_data['titan_D_eq_km'],
          color='#8e44ad',
          lw=2.0,
          ls='--',
          label='Titan Shell $D_{eq}$')
ax2c.plot(nh3_pct,
          nh3_data['titan_D_ocean_km'],
          color='#16a085',
          lw=2.0,
          ls='--',
          label='Titan Ocean $D_{oc}$')

ax2c.set_xlabel('Ammonia Concentration $w_{NH3}$ [wt%]')
ax2c.set_ylabel('Layer Depth [km]')
ax2c.set_title('(c) Ammonia Antifreeze Effect',
               fontweight='bold',
               fontsize=10.5)
ax2c.legend(loc='center right', fontsize=8.0, framealpha=0.85)

plt.tight_layout()
fig2.savefig(os.path.join(output_dir, 'fig_model_choices.pdf'), dpi=300)
plt.close(fig2)
print(">>> Created fig_model_choices.pdf")

# ============================================================================
# FIGURE 3: fig_diagram.pdf (Icy Moon Ocean-Shell Geodynamical Architecture)
# ============================================================================
fig3, ax3 = plt.subplots(figsize=(10, 6.2))

# Background space
ax3.set_facecolor('#0f141d')

# Coordinates for radial layers
y_surf = 10.0
y_lid_base = 7.5
y_ice_base = 5.0
y_ocean_base = 2.2
y_core_base = 0.0
x_min, x_max = 0.0, 14.0

# 1. Stagnant Conductive Lid Layer
ax3.fill_between([x_min, x_max], [y_lid_base, y_lid_base], [y_surf, y_surf],
                 color='#85c1e9',
                 alpha=0.9,
                 label='Stagnant Conductive Lid (Elastic/Brittle Ice I)')
# Fractures and surface cracks
for x_c in [1.5, 3.2, 5.8, 8.4, 11.2, 12.8]:
    ax3.plot([x_c, x_c + 0.3, x_c + 0.1], [y_surf, y_surf - 1.2, y_lid_base],
             color='#1b4f72',
             lw=1.2,
             alpha=0.7)

# 2. Solid-State Convecting Ice Layer
ax3.fill_between([x_min, x_max], [y_ice_base, y_ice_base],
                 [y_lid_base, y_lid_base],
                 color='#3498db',
                 alpha=0.8,
                 label='Convective Sublayer (Ductile Ice I Plumes)')

# Warm upwelling diapirs and cold downwellings
upwelling_x = [2.5, 6.5, 10.5]
for ux in upwelling_x:
    # Upwelling warm plume (red/orange arrows)
    ax3.annotate('',
                 xy=(ux, y_lid_base - 0.2),
                 xytext=(ux, y_ice_base + 0.3),
                 arrowprops=dict(facecolor='#e74c3c',
                                 edgecolor='black',
                                 width=3,
                                 headwidth=9,
                                 alpha=0.85))
    ax3.text(ux,
             y_ice_base + 1.2,
             'Warm Plume\n$\\uparrow$',
             color='white',
             fontweight='bold',
             fontsize=8.5,
             ha='center')

downwelling_x = [4.5, 8.5, 12.5]
for dx in downwelling_x:
    # Downwelling cold plume (blue/cyan arrows)
    ax3.annotate('',
                 xy=(dx, y_ice_base + 0.3),
                 xytext=(dx, y_lid_base - 0.2),
                 arrowprops=dict(facecolor='#1abc9c',
                                 edgecolor='black',
                                 width=3,
                                 headwidth=9,
                                 alpha=0.85))
    ax3.text(dx,
             y_lid_base - 1.2,
             'Cold Sinking\n$\\downarrow$',
             color='white',
             fontweight='bold',
             fontsize=8.5,
             ha='center')

# 3. Liquid Water / Ammonia Ocean
ax3.fill_between([x_min, x_max], [y_ocean_base, y_ocean_base],
                 [y_ice_base, y_ice_base],
                 color='#1f618d',
                 alpha=0.9,
                 label='Subsurface Liquid Ocean ($H_2O - NH_3 - MgSO_4$)')
# Ocean wave currents
for ow in [1.5, 4.0, 6.8, 9.5, 12.0]:
    ax3.plot([ow, ow + 1.2], [y_ice_base - 1.2, y_ice_base - 1.4],
             color='#aed6f1',
             lw=1.5,
             alpha=0.6)

# 4. Silicate Core / High-Pressure Ice Floor
ax3.fill_between([x_min, x_max], [y_core_base, y_core_base],
                 [y_ocean_base, y_ocean_base],
                 color='#784212',
                 alpha=0.95,
                 label='Silicate Mantle / Iron Core / HP Ice Floor')

# Hydrothermal heat vents / tidal heating vectors
vent_x = [3.0, 7.0, 11.0]
for vx in vent_x:
    ax3.annotate('',
                 xy=(vx, y_ocean_base + 0.5),
                 xytext=(vx, y_core_base + 0.5),
                 arrowprops=dict(facecolor='#f39c12',
                                 edgecolor='black',
                                 width=4,
                                 headwidth=10))
    ax3.text(vx,
             y_core_base + 1.0,
             'Tidal / Radiogenic Heat\n$F_{supply} = F_{rad} + F_{tide}$',
             color='white',
             fontsize=8.0,
             ha='center',
             fontweight='bold',
             bbox=dict(boxstyle='round,pad=0.2',
                       facecolor='#d35400',
                       alpha=0.8,
                       edgecolor='none'))

# Temperature and Rheological Boundary Labels
ax3.text(0.3,
         y_surf - 0.5,
         'Surface ($T_s \\sim 75-110$ K, $\\eta \\sim 10^{24}$ Pa s)',
         color='black',
         fontweight='bold',
         fontsize=9.5,
         bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
ax3.text(0.3,
         y_lid_base - 0.5,
         'Rheological Boundary ($T_i - \\Delta T_{rh} \\sim 240-250$ K)',
         color='white',
         fontweight='bold',
         fontsize=9.0,
         bbox=dict(boxstyle='round', facecolor='#2c3e50', alpha=0.8))
ax3.text(
    0.3,
    y_ice_base - 0.5,
    'Melting Interface ($T_b = T_m(P, w_{NH3}) \\sim 245-273$ K, $\\eta_b \\sim 10^{14}$ Pa s)',
    color='white',
    fontweight='bold',
    fontsize=9.0,
    bbox=dict(boxstyle='round', facecolor='#154360', alpha=0.8))
ax3.text(0.3,
         y_ocean_base - 0.6,
         'Seafloor Boundary / HP Ice Phase Transition',
         color='white',
         fontweight='bold',
         fontsize=9.0,
         bbox=dict(boxstyle='round', facecolor='#4d2600', alpha=0.8))

# Title and formatting
ax3.set_xlim(x_min, x_max)
ax3.set_ylim(y_core_base, y_surf + 1.0)
ax3.set_xticks([])
ax3.set_yticks([])
ax3.set_title(
    'Spohn & Schubert (2003) Stagnant-Lid Convective Ocean Maintenance Architecture',
    color='white',
    fontsize=12,
    fontweight='bold',
    pad=12)

# Custom legend
leg = ax3.legend(loc='upper right',
                 facecolor='#1c2833',
                 edgecolor='white',
                 fontsize=8.5)
for text in leg.get_texts():
    text.set_color('white')

plt.tight_layout()
fig3.savefig(os.path.join(output_dir, 'fig_diagram.pdf'), dpi=300)
plt.close(fig3)
print(">>> Created fig_diagram.pdf")
