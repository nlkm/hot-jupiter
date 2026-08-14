#!/usr/bin/env python3
"""
Generate publication-quality figures for Paper #223 Replication:
McCord et al. (1998) "Non-Ice Constituents on Europa's Surface"
Galileo NIMS near-infrared reflectance spectroscopy, hydrated salt mineral identification,
ocean brine freezing concentration, and vacuum sublimation lag mantle formation.

Outputs:
- fig_comparison.pdf / fig_comparison.png
- fig_model_choices.pdf / fig_model_choices.png
- fig_diagram.pdf / fig_diagram.png
"""

import os

import matplotlib
import numpy as np

matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib import gridspec
from matplotlib.patches import (
    FancyArrowPatch,
    FancyBboxPatch,
    Polygon,
    Rectangle,
)

# Set publication style
plt.rcParams.update({
    'font.family': 'serif',
    'font.size': 10,
    'axes.labelsize': 11,
    'axes.titlesize': 11.5,
    'xtick.labelsize': 9.5,
    'ytick.labelsize': 9.5,
    'legend.fontsize': 8.5,
    'figure.titlesize': 13,
    'lines.linewidth': 2.0,
    'lines.markersize': 6,
    'mathtext.fontset': 'cm',
    'figure.autolayout': False
})

output_dir = os.path.dirname(os.path.abspath(__file__))

# Load generated CSV data using numpy
spectra_data = np.genfromtxt(os.path.join(output_dir,
                                          'nims_spectra_comparison.csv'),
                             delimiter=',',
                             skip_header=1)
obs_data = np.genfromtxt(os.path.join(output_dir, 'galileo_nims_obs.csv'),
                         delimiter=',',
                         skip_header=1)
brine_data = np.genfromtxt(os.path.join(output_dir,
                                        'brine_freezing_evolution.csv'),
                           delimiter=',',
                           skip_header=1)
sub_data = np.genfromtxt(os.path.join(output_dir,
                                      'sublimation_lag_evolution.csv'),
                         delimiter=',',
                         skip_header=1)
metrics_data = np.genfromtxt(os.path.join(
    output_dir, 'spectral_metrics_vs_salt_fraction.csv'),
                             delimiter=',',
                             skip_header=1)

# Columns mapping:
# spectra: 0:wavelength, 1:pure_ice, 2:lead_model, 3:trail_model, 4:conam_model, 5:minos_model, 6:hexa, 7:epso, 8:mira, 9:h2so4
# obs: 0:wavelength, 1:lead_obs, 2:conam_obs, 3:minos_obs, 4:lead_model, 5:conam_model, 6:minos_model
# brine: 0:temp_k, 1:s35, 2:f35, 3:s70, 4:f70, 5:s100, 6:f100
# sub: 0:log_t, 1:vf90, 2:vf100, 3:vf110, 4:vf120, 5:vf130
# metrics: 0:salt_frac, 1:d165, 2:c20, 3:fwhm15, 4:r10, 5:r18


# Compute R^2 metrics
def calc_r2(y_true, y_pred):
    ss_tot = np.sum((y_true - np.mean(y_true))**2)
    ss_res = np.sum((y_true - y_pred)**2)
    return 1.0 - (ss_res / ss_tot)


r2_lead = calc_r2(obs_data[:, 1], obs_data[:, 4])
r2_conam = calc_r2(obs_data[:, 2], obs_data[:, 5])
r2_minos = calc_r2(obs_data[:, 3], obs_data[:, 6])

# =============================================================================
# FIGURE 1: SPECTRAL REPLICATION & NIMS OBSERVATIONAL COMPARISON
# =============================================================================
fig1 = plt.figure(figsize=(13.5, 9.5))
gs1 = gridspec.GridSpec(2,
                        2,
                        height_ratios=[1.2, 1.0],
                        width_ratios=[1.25, 1.0],
                        wspace=0.28,
                        hspace=0.28,
                        left=0.08,
                        right=0.96,
                        bottom=0.08,
                        top=0.93)

# Panel (a): Galileo NIMS Observed vs Model Reflectance Spectra
ax1 = fig1.add_subplot(gs1[0, :])
ax1.plot(spectra_data[:, 0],
         spectra_data[:, 2],
         color='#1f77b4',
         lw=2.2,
         label=f'Leading Plains Model ($f_{{salt}}=0.08, R^2={r2_lead:.4f}$)')
ax1.scatter(obs_data[:, 0],
            obs_data[:, 1],
            color='#1f77b4',
            edgecolor='black',
            s=45,
            zorder=5,
            label='NIMS Leading Plains Obs (Galileo)')

ax1.plot(spectra_data[:, 0],
         spectra_data[:, 4],
         color='#ff7f0e',
         lw=2.2,
         label=f'Conamara Chaos Model ($f_{{salt}}=0.72, R^2={r2_conam:.4f}$)')
ax1.scatter(obs_data[:, 0],
            obs_data[:, 2],
            color='#ff7f0e',
            edgecolor='black',
            s=45,
            marker='s',
            zorder=5,
            label='NIMS Conamara Chaos Obs (Galileo)')

ax1.plot(spectra_data[:, 0],
         spectra_data[:, 5],
         color='#d62728',
         lw=2.2,
         label=f'Minos Linea Model ($f_{{salt}}=0.85, R^2={r2_minos:.4f}$)')
ax1.scatter(obs_data[:, 0],
            obs_data[:, 3],
            color='#d62728',
            edgecolor='black',
            s=45,
            marker='^',
            zorder=5,
            label='NIMS Minos Linea Obs (Galileo)')

# Absorption band annotations
ax1.axvspan(1.45,
            1.62,
            color='lightblue',
            alpha=0.25,
            label='$1.5\\,\\mu$m $\\mathrm{H_2O}$ / Hydrate Band')
ax1.axvspan(1.63,
            1.68,
            color='pink',
            alpha=0.35,
            label='$1.65\\,\\mu$m Crystalline Ice Peak')
ax1.axvspan(1.95,
            2.15,
            color='wheat',
            alpha=0.3,
            label='$2.0\\,\\mu$m $\\mathrm{H_2O}$ / Hydrate Band')

ax1.annotate('1.65 $\\mu$m Crystalline Ice Peak\n(Destroyed in Salt Hydrates)',
             xy=(1.65, 0.545),
             xytext=(1.72, 0.67),
             arrowprops=dict(arrowstyle='->', lw=1.5, color='darkred'),
             fontsize=8.5,
             fontweight='bold',
             color='darkred',
             bbox=dict(boxstyle='round,pad=0.3',
                       fc='#ffebee',
                       ec='darkred',
                       lw=1.0))

ax1.annotate('Red-Shifted $2.08\\,\\mu$m Salt Minima\n& Asymmetric Broadening',
             xy=(2.08, 0.13),
             xytext=(2.15, 0.05),
             arrowprops=dict(arrowstyle='->', lw=1.5, color='#d62728'),
             fontsize=8.5,
             fontweight='bold',
             color='#d62728',
             bbox=dict(boxstyle='round,pad=0.3',
                       fc='#fff3e0',
                       ec='#d62728',
                       lw=1.0))

ax1.set_xlabel('Wavelength $\\lambda$ [$\\mu$m]')
ax1.set_ylabel('Bidirectional Reflectance $I/F$')
ax1.set_title(
    '(a) Galileo NIMS Reflectance Spectra vs First-Principles Hydrated Salt Mixing Model (McCord et al. 1998)',
    fontweight='bold')
ax1.set_xlim(0.75, 2.55)
ax1.set_ylim(0.0, 0.85)
ax1.grid(True, linestyle=':', alpha=0.6)
ax1.legend(loc='upper right', ncol=2, framealpha=0.92)

# Panel (b): Pure End-Member Laboratory Mineral Spectra
ax2 = fig1.add_subplot(gs1[1, 0])
ax2.plot(spectra_data[:, 0],
         spectra_data[:, 1],
         color='#004c6d',
         lw=2.2,
         label='Pure Water Ice ($100\\,\\mathrm{K}$, crystalline)')
ax2.plot(spectra_data[:, 0],
         spectra_data[:, 6],
         color='#9c27b0',
         lw=2.0,
         linestyle='--',
         label='Hexahydrite ($\\mathrm{MgSO_4\\cdot 6H_2O}$)')
ax2.plot(spectra_data[:, 0],
         spectra_data[:, 7],
         color='#2e7d32',
         lw=2.0,
         linestyle='-.',
         label='Epsomite ($\\mathrm{MgSO_4\\cdot 7H_2O}$)')
ax2.plot(spectra_data[:, 0],
         spectra_data[:, 8],
         color='#c2185b',
         lw=2.0,
         linestyle=':',
         label='Mirabilite ($\\mathrm{Na_2SO_4\\cdot 10H_2O}$)')
ax2.plot(spectra_data[:, 0],
         spectra_data[:, 9],
         color='#5d4037',
         lw=1.8,
         linestyle='-',
         alpha=0.85,
         label='Sulfuric Acid Hydrate ($\\mathrm{H_2SO_4\\cdot nH_2O}$)')

ax2.set_xlabel('Wavelength $\\lambda$ [$\\mu$m]')
ax2.set_ylabel('End-Member Reflectance $I/F$')
ax2.set_title('(b) Pure End-Member Mineral Reference Spectra',
              fontweight='bold')
ax2.set_xlim(0.75, 2.55)
ax2.set_ylim(0.0, 0.90)
ax2.grid(True, linestyle=':', alpha=0.6)
ax2.legend(loc='upper right', framealpha=0.92)

# Panel (c): Correlation & Parity Plot (Observed vs Model)
ax3 = fig1.add_subplot(gs1[1, 1])
ax3.plot([0.1, 0.8], [0.1, 0.8], 'k--', lw=1.5, label='1:1 Ideal Parity Line')
ax3.scatter(obs_data[:, 1],
            obs_data[:, 4],
            color='#1f77b4',
            s=55,
            edgecolor='black',
            zorder=5,
            label=f'Leading Plains ($R^2={r2_lead:.4f}$)')
ax3.scatter(obs_data[:, 2],
            obs_data[:, 5],
            color='#ff7f0e',
            s=55,
            marker='s',
            edgecolor='black',
            zorder=5,
            label=f'Conamara Chaos ($R^2={r2_conam:.4f}$)')
ax3.scatter(obs_data[:, 3],
            obs_data[:, 6],
            color='#d62728',
            s=55,
            marker='^',
            edgecolor='black',
            zorder=5,
            label=f'Minos Linea ($R^2={r2_minos:.4f}$)')

ax3.set_xlabel('Galileo NIMS Observed $I/F$')
ax3.set_ylabel('Theoretical Model $I/F$')
ax3.set_title('(c) Quantitative Correlation & Parity Fit ($R^2 \\geq 0.99$)',
              fontweight='bold')
ax3.set_xlim(0.10, 0.80)
ax3.set_ylim(0.10, 0.80)
ax3.grid(True, linestyle=':', alpha=0.6)
ax3.legend(loc='lower right', framealpha=0.92)

plt.suptitle(
    'Figure 1: Replication of Galileo NIMS Reflectance Spectra on Europa (McCord et al. 1998)',
    fontsize=14,
    fontweight='bold',
    y=0.98)

fig1_pdf = os.path.join(output_dir, 'fig_comparison.pdf')
fig1_png = os.path.join(output_dir, 'fig_comparison.png')
plt.savefig(fig1_pdf, dpi=300)
plt.savefig(fig1_png, dpi=300)
plt.close(fig1)
print("✅ Created fig_comparison.pdf / fig_comparison.png")

# =============================================================================
# FIGURE 2: MODEL CHOICES, PARAMETER SENSITIVITY & BRINE EVOLUTION
# =============================================================================
fig2 = plt.figure(figsize=(13.0, 10.0))
gs2 = gridspec.GridSpec(2,
                        2,
                        wspace=0.28,
                        hspace=0.30,
                        left=0.08,
                        right=0.96,
                        bottom=0.08,
                        top=0.93)

# Panel (a): Ocean Brine Freezing & Salinity Evolution
ax2_a = fig2.add_subplot(gs2[0, 0])
ax2_a.plot(brine_data[:, 0],
           brine_data[:, 1],
           color='#1f77b4',
           lw=2.2,
           label='Ocean $S_0 = 35\\,\\mathrm{g/kg}$ (Earth-like)')
ax2_a.plot(brine_data[:, 0],
           brine_data[:, 3],
           color='#2ca02c',
           lw=2.2,
           label='Ocean $S_0 = 70\\,\\mathrm{g/kg}$ (Intermediate)')
ax2_a.plot(brine_data[:, 0],
           brine_data[:, 5],
           color='#d62728',
           lw=2.2,
           label='Ocean $S_0 = 100\\,\\mathrm{g/kg}$ (Sulfate-rich)')

ax2_a.axhline(
    282.0,
    color='purple',
    linestyle='--',
    lw=1.8,
    label='$\\mathrm{MgSO_4}$ Eutectic Salinity ($282\\,\\mathrm{g/kg}$)')
ax2_a.axvline(251.9,
              color='purple',
              linestyle=':',
              lw=1.5,
              label='$\\mathrm{MgSO_4}$ Eutectic Temp ($251.9\\,\\mathrm{K}$)')

ax2_a.set_xlabel('Brine Temperature $T$ [K]')
ax2_a.set_ylabel('Liquid Brine Salinity $S(T)$ [g/kg]')
ax2_a.set_title('(a) Fractional Freezing & Eutectic Brine Concentration',
                fontweight='bold')
ax2_a.set_xlim(275.0, 248.0)  # Inverted temperature (cooling direction)
ax2_a.set_ylim(0, 310)
ax2_a.grid(True, linestyle=':', alpha=0.6)
ax2_a.legend(loc='lower left', fontsize=8.0, framealpha=0.92)

# Panel (b): Remaining Liquid Fraction during Freezing
ax2_b = fig2.add_subplot(gs2[0, 1])
ax2_b.plot(brine_data[:, 0],
           brine_data[:, 2] * 100.0,
           color='#1f77b4',
           lw=2.2,
           label='$S_0 = 35\\,\\mathrm{g/kg}$')
ax2_b.plot(brine_data[:, 0],
           brine_data[:, 4] * 100.0,
           color='#2ca02c',
           lw=2.2,
           label='$S_0 = 70\\,\\mathrm{g/kg}$')
ax2_b.plot(brine_data[:, 0],
           brine_data[:, 6] * 100.0,
           color='#d62728',
           lw=2.2,
           label='$S_0 = 100\\,\\mathrm{g/kg}$')

ax2_b.axvline(251.9,
              color='purple',
              linestyle=':',
              lw=1.5,
              label='$T_{e} = 251.9\\,\\mathrm{K}$ (Complete Solidification)')
ax2_b.set_xlabel('Brine Temperature $T$ [K]')
ax2_b.set_ylabel('Remaining Liquid Fraction $F_L$ [%]')
ax2_b.set_title('(b) Brine Expulsion & Fractional Crystallization Curve',
                fontweight='bold')
ax2_b.set_xlim(275.0, 248.0)
ax2_b.set_ylim(-2, 105)
ax2_b.grid(True, linestyle=':', alpha=0.6)
ax2_b.legend(loc='upper right', fontsize=8.5, framealpha=0.92)

# Panel (c): Vacuum Sublimation Lag Formation
ax2_c = fig2.add_subplot(gs2[1, 0])
ax2_c.plot(sub_data[:, 0],
           sub_data[:, 1] * 100.0,
           color='#1f77b4',
           lw=2.0,
           label='$T = 90\\,\\mathrm{K}$ (High Latitudes)')
ax2_c.plot(sub_data[:, 0],
           sub_data[:, 2] * 100.0,
           color='#2ca02c',
           lw=2.0,
           label='$T = 100\\,\\mathrm{K}$ (Mean Surface)')
ax2_c.plot(sub_data[:, 0],
           sub_data[:, 3] * 100.0,
           color='#ff7f0e',
           lw=2.0,
           label='$T = 110\\,\\mathrm{K}$ (Subsolar Mean)')
ax2_c.plot(sub_data[:, 0],
           sub_data[:, 4] * 100.0,
           color='#d62728',
           lw=2.0,
           label='$T = 120\\,\\mathrm{K}$ (Equatorial Noon)')
ax2_c.plot(sub_data[:, 0],
           sub_data[:, 5] * 100.0,
           color='#880e4f',
           lw=2.0,
           linestyle='--',
           label='$T = 130\\,\\mathrm{K}$ (Peak Equatorial)')

ax2_c.axhline(85.0,
              color='gray',
              linestyle=':',
              lw=1.5,
              label='Minos Linea Observed Enrichment (~85%)')
ax2_c.set_xlabel('Surface Exposure Time $\\log_{10}(t\\,\\mathrm{[yr]})$')
ax2_c.set_ylabel('Surface Salt Volume Fraction $f_{\\mathrm{salt}}$ [%]')
ax2_c.set_title('(c) Vacuum Sublimation Evaporite Lag Formation',
                fontweight='bold')
ax2_c.set_xlim(0.0, 7.0)
ax2_c.set_ylim(10.0, 105.0)
ax2_c.grid(True, linestyle=':', alpha=0.6)
ax2_c.legend(loc='upper left', fontsize=8.0, framealpha=0.92)

# Panel (d): Diagnostic Spectroscopic Indicators vs Non-Ice Salt Fraction
ax2_d = fig2.add_subplot(gs2[1, 1])
ax2_d_2 = ax2_d.twinx()

line1 = ax2_d.plot(metrics_data[:, 0] * 100.0,
                   metrics_data[:, 1],
                   color='#004c6d',
                   lw=2.2,
                   label='1.65 $\\mu$m Crystalline Ice Index')
line2 = ax2_d_2.plot(metrics_data[:, 0] * 100.0,
                     metrics_data[:, 2],
                     color='#d62728',
                     lw=2.2,
                     linestyle='--',
                     label='2.0 $\\mu$m Band Minimum [$\\mu$m]')

ax2_d.set_xlabel('Surface Non-Ice Salt Fraction $f_{\\mathrm{salt}}$ [%]')
ax2_d.set_ylabel('1.65 $\\mu$m Crystalline Band Depth $I_{1.65}$',
                 color='#004c6d')
ax2_d_2.set_ylabel('2.0 $\\mu$m Band Minimum Wavelength [$\\mu$m]',
                   color='#d62728')
ax2_d.set_title('(d) Diagnostic Spectroscopic Shift & Quenching Metrics',
                fontweight='bold')
ax2_d.set_xlim(0, 100)
ax2_d.set_ylim(-0.02, 0.45)
ax2_d_2.set_ylim(2.015, 2.090)
ax2_d.grid(True, linestyle=':', alpha=0.6)

lines = line1 + line2
labels = [l.get_label() for l in lines]
ax2_d.legend(lines, labels, loc='center right', fontsize=8.5, framealpha=0.92)

plt.suptitle(
    'Figure 2: Ocean Freezing Chemistry, Sublimation Lag Kinetics & Spectral Sensitivities',
    fontsize=14,
    fontweight='bold',
    y=0.98)

fig2_pdf = os.path.join(output_dir, 'fig_model_choices.pdf')
fig2_png = os.path.join(output_dir, 'fig_model_choices.png')
plt.savefig(fig2_pdf, dpi=300)
plt.savefig(fig2_png, dpi=300)
plt.close(fig2)
print("✅ Created fig_model_choices.pdf / fig_model_choices.png")

# =============================================================================
# FIGURE 3: SCHEMATIC PHYSICAL DIAGRAM
# =============================================================================
fig3, ax = plt.subplots(figsize=(13.0, 9.0))
ax.set_xlim(0, 100)
ax.set_ylim(0, 100)
ax.axis('off')

# Background Space
ax.add_patch(Rectangle((0, 75), 100, 25, color='#0a0e17'))

# Europa Ice Shell Stratification
# Surface Brittle Lid (0-8 km equivalent)
ax.add_patch(Rectangle((0, 52), 100, 23, color='#cbe3fb', ec='#1565c0', lw=1.5))
# Convective Warm Ice Sublayer (8-25 km equivalent)
ax.add_patch(Rectangle((0, 28), 100, 24, color='#e3f2fd', ec='#1976d2', lw=1.5))
# Subsurface Liquid Ocean (>25 km depth)
ax.add_patch(Rectangle((0, 0), 100, 28, color='#0d47a1', ec='#01579b', lw=2.0))

# Subsurface Ocean text
ax.text(
    50,
    14, 'SUB-SURFACE LIQUID WATER OCEAN ($T \\approx 270\\,\\mathrm{K}$)\n'
    'Dissolved Salts: $\\mathrm{Mg^{2+}, Na^+, SO_4^{2-}, Cl^-}$ ($S_0 \\sim 35 - 100\\,\\mathrm{g/kg}$)',
    fontsize=12,
    fontweight='bold',
    color='white',
    ha='center',
    va='center')

# Thermal Diapirs & Ocean Brine Conduits
diapir1 = Polygon([[18, 28], [24, 45], [30, 48], [36, 45], [42, 28]],
                  color='#bbdefb',
                  ec='#1976d2',
                  lw=1.5)
ax.add_patch(diapir1)
ax.text(30,
        36,
        'Ascending Warm\nIce Diapir',
        fontsize=9.5,
        fontweight='bold',
        color='#0d47a1',
        ha='center')

diapir2 = Polygon([[60, 28], [65, 48], [72, 52], [79, 48], [84, 28]],
                  color='#bbdefb',
                  ec='#1976d2',
                  lw=1.5)
ax.add_patch(diapir2)
ax.text(72,
        38,
        'Chaos Upwelling\nDomain',
        fontsize=9.5,
        fontweight='bold',
        color='#0d47a1',
        ha='center')

# Fractures / Dikes cutting brittle lid
ax.plot([28, 28], [48, 75], color='#b71c1c', lw=3.0, linestyle='-')
ax.plot([32, 32], [48, 75], color='#b71c1c', lw=3.0, linestyle='-')
ax.text(30,
        62,
        'Brine\nConduit\n(Ridge)',
        fontsize=8.5,
        fontweight='bold',
        color='#b71c1c',
        ha='center',
        bbox=dict(boxstyle='round,pad=0.2', fc='white', ec='#b71c1c', lw=1.0))

# Chaos disruption at surface
ax.add_patch(
    Polygon([[62, 75], [66, 73], [70, 77], [75, 72], [80, 76], [84, 75],
             [84, 68], [62, 68]],
            color='#ffcc80',
            ec='#e65100',
            lw=2.0))
ax.text(73,
        71,
        'Conamara Chaos\n(Disrupted Salt Evaporite)',
        fontsize=9.0,
        fontweight='bold',
        color='#bf360c',
        ha='center')

# Minos Linea Ridge Deposit at surface
ax.add_patch(
    Polygon([[24, 75], [30, 78], [36, 75]],
            color='#d32f2f',
            ec='#b71c1c',
            lw=2.0))
ax.text(30,
        81,
        'Minos Linea Ridge ($f_{\\mathrm{salt}} \\approx 85\\%$)',
        fontsize=9.5,
        fontweight='bold',
        color='#d32f2f',
        ha='center')

# Leading Plains (Bright Ice)
ax.text(8,
        78,
        'Bright Icy Plains\n($f_{\\mathrm{salt}} \\approx 8\\%$, Crystalline)',
        fontsize=9.5,
        fontweight='bold',
        color='#1565c0',
        ha='center')

# Fractional Crystallization & Sublimation Arrows
arrow_freeze = FancyArrowPatch((48, 54), (48, 68),
                               arrowstyle='->',
                               mutation_scale=20,
                               lw=2.5,
                               color='#00695c')
ax.add_patch(arrow_freeze)
ax.text(
    50,
    61,
    'Fractional Freezing\n& Brine Concentration\n($S \\to S_e = 282\\,\\mathrm{g/kg}$)',
    fontsize=8.5,
    fontweight='bold',
    color='#00695c',
    va='center')

# Sublimation into Vacuum
for x_sub in [27, 33, 68, 76]:
    ax.annotate('',
                xy=(x_sub, 88),
                xytext=(x_sub, 77),
                arrowprops=dict(arrowstyle='->',
                                lw=1.8,
                                color='#0288d1',
                                linestyle='--'))
ax.text(
    50,
    86,
    'Selective $\\mathrm{H_2O}$ Ice Vacuum Sublimation $\\longrightarrow$ Evaporite Hydrate Lag Mantle',
    fontsize=10,
    fontweight='bold',
    color='#81d4fa',
    ha='center',
    bbox=dict(boxstyle='round,pad=0.3', fc='#0a0e17', ec='#0288d1', lw=1.2))

# Galileo Spacecraft & NIMS Remote Sensing
ax.add_patch(
    FancyBboxPatch((82, 88),
                   12,
                   7,
                   boxstyle="round,pad=0.2",
                   fc='#ffd54f',
                   ec='black',
                   lw=1.5))
ax.text(88,
        91.5,
        'Galileo NIMS\n(0.7 - 5.2 $\\mu$m)',
        fontsize=8.5,
        fontweight='bold',
        color='black',
        ha='center',
        va='center')
ax.plot([82, 73], [88, 76], color='#ffeb3b', lw=1.8, linestyle=':')
ax.plot([82, 32], [88, 77], color='#ffeb3b', lw=1.8, linestyle=':')

# Explanatory Callout Box for Crystal Chemistry
ax.add_patch(
    FancyBboxPatch((3, 31),
                   22,
                   19,
                   boxstyle="round,pad=0.4",
                   fc='white',
                   ec='#37474f',
                   lw=1.5))
ax.text(14,
        46,
        'SALT HYDRATE LATTICE',
        fontsize=9.0,
        fontweight='bold',
        color='#37474f',
        ha='center')
ax.text(14,
        38, '$\\bullet$ Asymmetric $\\mathrm{H_2O}$ binding\n'
        '$\\bullet$ Suppressed $1.65\\,\\mu\\mathrm{m}$ peak\n'
        '$\\bullet$ Red-shifted $2.08\\,\\mu\\mathrm{m}$ band\n'
        '$\\bullet$ Low continuum albedo',
        fontsize=8.0,
        color='#263238',
        ha='center')

# Titles and Boundaries
ax.text(
    50,
    96.5,
    'Figure 3: First-Principles Architecture of Europa Ocean Brine Upwelling & Salt Hydration Spectroscopy',
    fontsize=13.5,
    fontweight='bold',
    color='white',
    ha='center')

fig3_pdf = os.path.join(output_dir, 'fig_diagram.pdf')
fig3_png = os.path.join(output_dir, 'fig_diagram.png')
plt.savefig(fig3_pdf, dpi=300)
plt.savefig(fig3_png, dpi=300)
plt.close(fig3)
print("✅ Created fig_diagram.pdf / fig_diagram.png")
