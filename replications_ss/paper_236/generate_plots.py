#!/usr/bin/env python3
"""
Generate publication-quality figures for Paper #236 Replication:
Kokubo & Ida (2000) "Formation of Protoplanets from Planetesimals in the Solar Nebula"
Icarus 143, 15-27.

Outputs:
- fig_comparison.pdf / fig_comparison.png
- fig_model_choices.pdf / fig_model_choices.png
- fig_diagram.pdf / fig_diagram.png
"""

import os

import matplotlib

matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import gridspec
from matplotlib.patches import Circle, FancyArrowPatch, FancyBboxPatch, Wedge

# Set publication typography & aesthetics
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
    'lines.markersize': 5,
    'mathtext.fontset': 'cm',
    'figure.autolayout': False
})

output_dir = os.path.dirname(os.path.abspath(__file__))

# Load generated simulation CSV data
mass_data = np.genfromtxt(os.path.join(output_dir,
                                       'mass_distribution_snapshots.csv'),
                          delimiter=',',
                          skip_header=1)
vel_data = np.genfromtxt(os.path.join(output_dir,
                                      'velocity_dispersion_evolution.csv'),
                         delimiter=',',
                         skip_header=1)
growth_data = np.genfromtxt(os.path.join(output_dir,
                                         'oligarch_growth_timeseries.csv'),
                            delimiter=',',
                            skip_header=1)
rad_data = np.genfromtxt(os.path.join(output_dir,
                                      'isolation_mass_radial_scaling.csv'),
                         delimiter=',',
                         skip_header=1)
sens_data = np.genfromtxt(os.path.join(output_dir,
                                       'model_choices_gas_drag.csv'),
                          delimiter=',',
                          skip_header=1)

# =============================================================================
# FIGURE 1: OLIGARCHIC MASS SPECTRUM & VELOCITY DISPERSION REPLICATION
# =============================================================================
fig1 = plt.figure(figsize=(13.5, 10.5))
gs1 = gridspec.GridSpec(2,
                        2,
                        wspace=0.26,
                        hspace=0.28,
                        left=0.08,
                        right=0.96,
                        bottom=0.08,
                        top=0.93)

# Panel 1(a): Cumulative Mass Spectrum N_c(>M)
ax1 = fig1.add_subplot(gs1[0, 0])
m_grams = mass_data[:, 0]
m_earth = mass_data[:, 1]

# Plot cumulative distributions at 4 key epochs matching Kokubo & Ida (2000) Fig. 1
ax1.loglog(m_grams,
           mass_data[:, 6],
           label=r'$t = 0\text{ yr}$ (Initial Mono-mass Swarm)',
           color='#4B5563',
           lw=2.2,
           ls=':')
ax1.loglog(m_grams,
           mass_data[:, 7],
           label=r'$t = 5\times 10^4\text{ yr}$ (Runaway Growth Stage)',
           color='#2563EB',
           lw=2.2,
           ls='--')
ax1.loglog(m_grams,
           mass_data[:, 8],
           label=r'$t = 2\times 10^5\text{ yr}$ (Emerging Oligarchs)',
           color='#D97706',
           lw=2.2,
           ls='-.')
ax1.loglog(m_grams,
           mass_data[:, 9],
           label=r'$t = 5\times 10^5\text{ yr}$ (Oligarchic Isolation)',
           color='#DC2626',
           lw=2.5,
           ls='-')

# Reference analytical power-law slope dN/dM ~ M^(-2.67) ==> Nc ~ M^(-1.67)
m_ref = np.logspace(23.2, 25.0, 50)
nc_ref = 8000.0 * (m_ref / 1.0e23)**(-1.67)
ax1.loglog(m_ref, nc_ref, color='#111827', lw=1.5, ls='--', alpha=0.7)
ax1.text(2.0e24,
         250.0,
         r'$\propto M^{-1.67}$',
         fontsize=9.5,
         fontweight='bold',
         color='#111827')

# Annotations for mass gap & oligarch peak
ax1.axvspan(3.0e25,
            6.0e25,
            color='#F3F4F6',
            alpha=0.6,
            label='Oligarch Accretion Gap')
ax1.set_xlabel(r'Body Mass $M\ [\text{g}]$', fontweight='bold')
ax1.set_ylabel(r'Cumulative Number $N_c(>M)$', fontweight='bold')
ax1.set_title(r'\textbf{(a) Cumulative Mass Spectrum Evolution $N_c(>M)$}',
              pad=8)
ax1.set_xlim(1.0e23, 1.0e27)
ax1.set_ylim(0.5, 2.0e4)
ax1.grid(True, which='both', ls=':', alpha=0.5)
ax1.legend(loc='lower left', frameon=True, framealpha=0.92)

# Secondary top x-axis in Earth masses
ax1_top = ax1.twiny()
ax1_top.set_xscale('log')
ax1_top.set_xlim(1.0e23 / 5.972e27, 1.0e27 / 5.972e27)
ax1_top.set_xlabel(r'Body Mass $[M_\oplus]$', fontsize=9.5, color='#374151')

# Panel 1(b): Differential Mass Spectrum dN/dM at t = 500 kyr (Bimodal Distribution)
ax2 = fig1.add_subplot(gs1[0, 1])
ax2.loglog(m_grams,
           mass_data[:, 5],
           color='#DC2626',
           lw=2.4,
           label=r'Simulated $N(M)$ at $t = 500\text{ kyr}$')
ax2.loglog(m_grams,
           mass_data[:, 4],
           color='#D97706',
           lw=2.0,
           ls='-.',
           label=r'Simulated $N(M)$ at $t = 200\text{ kyr}$')
ax2.loglog(m_grams,
           mass_data[:, 3],
           color='#2563EB',
           lw=1.8,
           ls='--',
           label=r'Simulated $N(M)$ at $t = 50\text{ kyr}$')

ax2.axvspan(2.0e25, 7.0e25, color='#FEE2E2', alpha=0.5)
ax2.text(2.8e25,
         0.05,
         'Mass Gap\n(Desert)',
         fontsize=8.5,
         color='#991B1B',
         ha='center')
ax2.annotate('Isolated\nOligarchs\n($M \\approx 0.1 M_\\oplus$)',
             xy=(1.0e26, 15.0),
             xytext=(2.0e26, 80.0),
             arrowprops=dict(arrowstyle='->', lw=1.5, color='#DC2626'),
             fontsize=8.5,
             fontweight='bold',
             color='#DC2626',
             ha='center')

ax2.set_xlabel(r'Body Mass $M\ [\text{g}]$', fontweight='bold')
ax2.set_ylabel(r'Number per Log Bin $dN / d\log M$', fontweight='bold')
ax2.set_title(r'\textbf{(b) Bimodal Mass Distribution \& Oligarch Peak}', pad=8)
ax2.set_xlim(1.0e23, 5.0e26)
ax2.set_ylim(0.01, 2.0e3)
ax2.grid(True, which='both', ls=':', alpha=0.5)
ax2.legend(loc='upper right', frameon=True, framealpha=0.92)

# Panel 1(c): Velocity Dispersion Equilibrium e_tilde and i_tilde vs Time
ax3 = fig1.add_subplot(gs1[1, 0])
time_kyr = vel_data[:, 0] / 1.0e3
e_tilde = vel_data[:, 3]
i_tilde = vel_data[:, 4]

ax3.plot(time_kyr,
         e_tilde,
         color='#059669',
         lw=2.4,
         label=r'Reduced Eccentricity $\tilde{e} = e / h$')
ax3.plot(time_kyr,
         i_tilde,
         color='#0284C7',
         lw=2.2,
         ls='--',
         label=r'Reduced Inclination $\tilde{i} = i / h$')
ax3.axhline(
    5.0,
    color='#047857',
    ls=':',
    lw=1.6,
    label=r'Equilibrium $\tilde{e}_{\text{eq}} \approx 5.0$ (Kokubo \& Ida 2000)'
)
ax3.axhline(
    2.5,
    color='#0369A1',
    ls=':',
    lw=1.6,
    label=
    r'Equilibrium $\tilde{i}_{\text{eq}} \approx 2.5$ ($\tilde{e} \approx 2\tilde{i}$)'
)

ax3.axvspan(0,
            30,
            color='#FEF3C7',
            alpha=0.5,
            label='Stirring Relaxation Phase')
ax3.set_xlabel(r'Time $t\ [\text{kyr}]$', fontweight='bold')
ax3.set_ylabel(r'Reduced Velocity Dispersion $[h]$', fontweight='bold')
ax3.set_title(r'\textbf{(c) Planetesimal Velocity Dispersion Equilibrium}',
              pad=8)
ax3.set_xlim(0, 500)
ax3.set_ylim(0, 7.5)
ax3.grid(True, ls=':', alpha=0.6)
ax3.legend(loc='lower right', frameon=True, framealpha=0.92)

# Panel 1(d): Oligarch Mass Growth & Specific Growth Rate Comparison
ax4 = fig1.add_subplot(gs1[1, 1])
t_grow_kyr = growth_data[:, 0] / 1.0e3
m_olig = growth_data[:, 1]
m_runaway = growth_data[:, 2]

ax4.plot(t_grow_kyr,
         m_olig,
         color='#DC2626',
         lw=2.4,
         label=r'Oligarchic Growth ($\dot{M} \propto M^{2/3}$)')
ax4.plot(t_grow_kyr,
         m_runaway,
         color='#4F46E5',
         lw=2.0,
         ls='--',
         label=r'Runaway Accretion ($\dot{M} \propto M^{4/3}$)')
ax4.axhline(0.1143,
            color='#B91C1C',
            ls=':',
            lw=1.6,
            label=r'Isolation Mass $M_{\text{iso}} \approx 0.114\ M_\oplus$')

ax4.set_xlabel(r'Time $t\ [\text{kyr}]$', fontweight='bold')
ax4.set_ylabel(r'Protoplanet Mass $M\ [M_\oplus]$', fontweight='bold')
ax4.set_title(r'\textbf{(d) Protoplanet Accretion Trajectory \& Saturation}',
              pad=8)
ax4.set_xlim(0, 500)
ax4.set_ylim(0, 0.14)
ax4.grid(True, ls=':', alpha=0.6)
ax4.legend(loc='lower right', frameon=True, framealpha=0.92)

fig1.suptitle(
    r'\textbf{Replication of Kokubo \& Ida (2000): Oligarchic Growth \& Velocity Equilibrium}',
    fontsize=14,
    y=0.98)
fig1_path_pdf = os.path.join(output_dir, 'fig_comparison.pdf')
fig1_path_png = os.path.join(output_dir, 'fig_comparison.png')
fig1.savefig(fig1_path_pdf, dpi=300, bbox_inches='tight')
fig1.savefig(fig1_path_png, dpi=300, bbox_inches='tight')
plt.close(fig1)
print(f" Saved {fig1_path_pdf} and {fig1_path_png}")

# =============================================================================
# FIGURE 2: MODEL SENSITIVITIES, GAS DRAG DAMPING & RADIAL SCALING
# =============================================================================
fig2 = plt.figure(figsize=(13.5, 10.5))
gs2 = gridspec.GridSpec(2,
                        2,
                        wspace=0.26,
                        hspace=0.28,
                        left=0.08,
                        right=0.96,
                        bottom=0.08,
                        top=0.93)

# Panel 2(a): Radial Isolation Mass Scaling M_iso(a) across Disk Models
ax21 = fig2.add_subplot(gs2[0, 0])
a_au = rad_data[:, 0]
m_iso_mmsn = rad_data[:, 1]
m_iso_flat = rad_data[:, 2]
m_iso_p1 = rad_data[:, 3]

ax21.plot(
    a_au,
    m_iso_mmsn,
    color='#DC2626',
    lw=2.4,
    label=r'MMSN $\Sigma \propto a^{-1.5}\ (M_{\text{iso}} \propto a^{3/4})$')
ax21.plot(
    a_au,
    m_iso_p1,
    color='#D97706',
    lw=2.2,
    ls='--',
    label=
    r'Shallow Disk $\Sigma \propto a^{-1.0}\ (M_{\text{iso}} \propto a^{1.5})$')
ax21.plot(
    a_au,
    m_iso_flat,
    color='#2563EB',
    lw=2.0,
    ls='-.',
    label=r'Flat Disk $\Sigma = \text{const}\ (M_{\text{iso}} \propto a^3)$')

ax21.set_xlabel(r'Semi-Major Axis $a\ [\text{AU}]$', fontweight='bold')
ax21.set_ylabel(r'Isolation Mass $M_{\text{iso}}\ [M_\oplus]$',
                fontweight='bold')
ax21.set_title(
    r'\textbf{(a) Radial Scaling of Oligarch Isolation Mass $M_{\text{iso}}(a)$}',
    pad=8)
ax21.set_xlim(0.4, 5.0)
ax21.set_ylim(0, 0.40)
ax21.grid(True, ls=':', alpha=0.6)
ax21.legend(loc='upper left', frameon=True, framealpha=0.92)

# Panel 2(b): Accretion Timescale tau_growth(a) across Solar System
ax22 = fig2.add_subplot(gs2[0, 1])
tau_myr = rad_data[:, 4]

ax22.semilogy(
    a_au,
    tau_myr,
    color='#7C3AED',
    lw=2.4,
    label=
    r'$\tau_{\text{growth}} \approx 0.12 \left(\frac{a}{1\,\text{AU}}\right)^{2.5}\ \text{Myr}$'
)
ax22.axvspan(0.7,
             1.5,
             color='#DC2626',
             alpha=0.12,
             label=r'Terrestrial Planet Zone ($t \sim 0.1 - 0.3\text{ Myr}$)')
ax22.axvspan(2.0,
             3.5,
             color='#D97706',
             alpha=0.12,
             label=r'Asteroid Belt Zone ($t \sim 0.7 - 2.5\text{ Myr}$)')
ax22.axvspan(4.5,
             5.5,
             color='#2563EB',
             alpha=0.12,
             label=r'Giant Planet Core Zone ($t \sim 5 - 10\text{ Myr}$)')

ax22.set_xlabel(r'Semi-Major Axis $a\ [\text{AU}]$', fontweight='bold')
ax22.set_ylabel(r'Growth Timescale $\tau_{\text{growth}}\ [\text{Myr}]$',
                fontweight='bold')
ax22.set_title(
    r'\textbf{(b) Oligarch Formation Timescale vs Heliocentric Distance}',
    pad=8)
ax22.set_xlim(0.4, 5.0)
ax22.set_ylim(0.01, 20.0)
ax22.grid(True, which='both', ls=':', alpha=0.5)
ax22.legend(loc='lower right', frameon=True, framealpha=0.92)

# Panel 2(c): Sensitivity of e_tilde_eq to Planetesimal Mass m_0
ax23 = fig2.add_subplot(gs2[1, 0])
log_m = sens_data[:, 0]
et_m21 = sens_data[:, 3]
et_m23 = sens_data[:, 4]
et_m24 = sens_data[:, 5]

ax23.plot(log_m,
          et_m21,
          color='#0284C7',
          lw=2.0,
          ls='--',
          label=r'$m_0 = 10^{21}\text{ g}$ (Small Planetesimals)')
ax23.plot(log_m,
          et_m23,
          color='#059669',
          lw=2.4,
          label=r'$m_0 = 10^{23}\text{ g}$ (Fiducial Planetesimals)')
ax23.plot(log_m,
          et_m24,
          color='#D97706',
          lw=2.0,
          ls='-.',
          label=r'$m_0 = 10^{24}\text{ g}$ (Large Planetesimals)')

ax23.set_xlabel(r'$\log_{10}(M_{\text{oligarch}} / \text{g})$',
                fontweight='bold')
ax23.set_ylabel(r'Equilibrium $\tilde{e}_{\text{eq}} = e / h$',
                fontweight='bold')
ax23.set_title(
    r'\textbf{(c) Invariance of $\tilde{e}_{\text{eq}}$ to Oligarch \& Planetesimal Mass}',
    pad=8)
ax23.set_xlim(23.0, 27.0)
ax23.set_ylim(2.5, 7.5)
ax23.grid(True, ls=':', alpha=0.6)
ax23.legend(loc='lower right', frameon=True, framealpha=0.92)

# Panel 2(d): Gas Drag Damping Impact on Growth Rate dM/dt
ax24 = fig2.add_subplot(gs2[1, 1])
dM_dt_gas = sens_data[:, 6] * (365.25 * 86400.0) / 5.972e24  # M_earth / yr
dM_dt_nogas = sens_data[:, 7] * (365.25 * 86400.0) / 5.972e24

ax24.semilogy(
    log_m,
    dM_dt_gas,
    color='#059669',
    lw=2.4,
    label=r'With Gas Drag Damping ($\tilde{e} \approx 5$, Efficient Accretion)')
ax24.semilogy(
    log_m,
    dM_dt_nogas,
    color='#DC2626',
    lw=2.0,
    ls='--',
    label=r'Without Gas Drag (Heated to $\tilde{e} \sim 15$, Suppressed Growth)'
)

ax24.set_xlabel(r'$\log_{10}(M_{\text{oligarch}} / \text{g})$',
                fontweight='bold')
ax24.set_ylabel(r'Accretion Rate $dM/dt\ [M_\oplus / \text{yr}]$',
                fontweight='bold')
ax24.set_title(
    r'\textbf{(d) Gas Drag Accretion Boost via Gravitational Focusing}', pad=8)
ax24.set_xlim(23.0, 27.0)
ax24.set_ylim(1.0e-10, 1.0e-5)
ax24.grid(True, which='both', ls=':', alpha=0.5)
ax24.legend(loc='lower right', frameon=True, framealpha=0.92)

fig2.suptitle(
    r'\textbf{Model Parameter Choices, Radial Scalings \& Aerodynamic Drag}',
    fontsize=14,
    y=0.98)
fig2_path_pdf = os.path.join(output_dir, 'fig_model_choices.pdf')
fig2_path_png = os.path.join(output_dir, 'fig_model_choices.png')
fig2.savefig(fig2_path_pdf, dpi=300, bbox_inches='tight')
fig2.savefig(fig2_path_png, dpi=300, bbox_inches='tight')
plt.close(fig2)
print(f" Saved {fig2_path_pdf} and {fig2_path_png}")

# =============================================================================
# FIGURE 3: SCHEMATIC ARCHITECTURE DIAGRAM
# =============================================================================
fig3 = plt.figure(figsize=(14.0, 8.5))
ax3 = fig3.add_subplot(1, 1, 1)
ax3.set_xlim(0, 140)
ax3.set_ylim(0, 85)
ax3.axis('off')

# Central Sun
sun = Circle((12, 42.5),
             6.5,
             facecolor='#F59E0B',
             edgecolor='#D97706',
             lw=2.5,
             zorder=5)
ax3.add_patch(sun)
ax3.text(12,
         42.5,
         r'\textbf{Sun}' + '\n' + r'$1\,M_\odot$',
         ha='center',
         va='center',
         color='white',
         fontsize=11,
         fontweight='bold',
         zorder=6)

# Orbit arcs
for r in [45, 75, 105]:
    wedge = Wedge((12, 42.5),
                  r,
                  -40,
                  40,
                  width=0.4,
                  facecolor='#E5E7EB',
                  edgecolor='#9CA3AF',
                  lw=1.0,
                  ls=':',
                  zorder=1)
    ax3.add_patch(wedge)

# Oligarch Protoplanets
olig1 = Circle((52, 42.5),
               3.8,
               facecolor='#DC2626',
               edgecolor='#991B1B',
               lw=2.0,
               zorder=10)
olig2 = Circle((82, 42.5),
               4.2,
               facecolor='#DC2626',
               edgecolor='#991B1B',
               lw=2.0,
               zorder=10)
olig3 = Circle((112, 42.5),
               4.0,
               facecolor='#DC2626',
               edgecolor='#991B1B',
               lw=2.0,
               zorder=10)
ax3.add_patch(olig1)
ax3.add_patch(olig2)
ax3.add_patch(olig3)

ax3.text(52,
         42.5,
         r'$M_1$',
         ha='center',
         va='center',
         color='white',
         fontweight='bold',
         fontsize=10,
         zorder=11)
ax3.text(82,
         42.5,
         r'$M_2$',
         ha='center',
         va='center',
         color='white',
         fontweight='bold',
         fontsize=10,
         zorder=11)
ax3.text(112,
         42.5,
         r'$M_3$',
         ha='center',
         va='center',
         color='white',
         fontweight='bold',
         fontsize=10,
         zorder=11)

# Feeding zone annular envelopes
feed1 = FancyBboxPatch((42, 28),
                       20,
                       29,
                       boxstyle="round,pad=1.5",
                       facecolor='#FEE2E2',
                       edgecolor='#EF4444',
                       lw=1.8,
                       ls='--',
                       alpha=0.4,
                       zorder=2)
feed2 = FancyBboxPatch((72, 26),
                       20,
                       33,
                       boxstyle="round,pad=1.5",
                       facecolor='#FEE2E2',
                       edgecolor='#EF4444',
                       lw=1.8,
                       ls='--',
                       alpha=0.4,
                       zorder=2)
feed3 = FancyBboxPatch((102, 27),
                       20,
                       31,
                       boxstyle="round,pad=1.5",
                       facecolor='#FEE2E2',
                       edgecolor='#EF4444',
                       lw=1.8,
                       ls='--',
                       alpha=0.4,
                       zorder=2)
ax3.add_patch(feed1)
ax3.add_patch(feed2)
ax3.add_patch(feed3)

# Mutual orbital spacing arrows
arrow_sep = FancyArrowPatch((52, 53), (82, 53),
                            arrowstyle='<->',
                            mutation_scale=15,
                            lw=2.0,
                            color='#1F2937')
ax3.add_patch(arrow_sep)
ax3.text(67,
         56.5,
         r'$\Delta a \approx 10\,r_{\text{H}}\ \ (\text{Orbital Spacing})$',
         ha='center',
         va='bottom',
         fontsize=10,
         fontweight='bold',
         color='#1F2937')

# Planetesimals swarm dots
np.random.seed(42)
for _ in range(85):
    px = np.random.uniform(32, 128)
    py = np.random.uniform(22, 63)
    # Exclude inside oligarchs
    if min((px - 52)**2 + (py - 42.5)**2, (px - 82)**2 + (py - 42.5)**2,
           (px - 112)**2 + (py - 42.5)**2) > 25:
        dot = Circle((px, py),
                     0.7,
                     facecolor='#3B82F6',
                     edgecolor='#1D4ED8',
                     lw=0.6,
                     zorder=8)
        ax3.add_patch(dot)

# Physical process callout boxes
# 1. Viscous Stirring Callout
box_vs = FancyBboxPatch((30, 68),
                        34,
                        13,
                        boxstyle="round,pad=0.8",
                        facecolor='#ECFDF5',
                        edgecolor='#10B981',
                        lw=1.8,
                        zorder=12)
ax3.add_patch(box_vs)
ax3.text(
    47,
    74.5,
    r'\textbf{1. Viscous Stirring by Oligarchs}' + '\n' +
    r'$\left(\frac{de^2}{dt}\right)_{\text{VS}} \approx 25 \left(\frac{M}{M_*}\right)^{4/3} \frac{\Omega}{b \tilde{e}^2} \ln \Lambda$',
    ha='center',
    va='center',
    fontsize=8.5,
    color='#065F46',
    zorder=13)

# 2. Gas Drag Damping Callout
box_drag = FancyBboxPatch((76, 68),
                          34,
                          13,
                          boxstyle="round,pad=0.8",
                          facecolor='#EFF6FF',
                          edgecolor='#3B82F6',
                          lw=1.8,
                          zorder=12)
ax3.add_patch(box_drag)
ax3.text(
    93,
    74.5,
    r'\textbf{2. Aerodynamic Gas Drag Damping}' + '\n' +
    r'$\left(\frac{de^2}{dt}\right)_{\text{drag}} \approx - \frac{C_{\text{D}} \rho_{\text{g}} v_{\text{K}}}{\rho_{\text{p}} r_{\text{p}}} e^3$',
    ha='center',
    va='center',
    fontsize=8.5,
    color='#1E40AF',
    zorder=13)

# 3. Equilibrium State Callout
box_eq = FancyBboxPatch((28, 4),
                        38,
                        14,
                        boxstyle="round,pad=0.8",
                        facecolor='#FEF3C7',
                        edgecolor='#F59E0B',
                        lw=1.8,
                        zorder=12)
ax3.add_patch(box_eq)
ax3.text(
    47,
    11.0,
    r'\textbf{3. Velocity Dispersion Equilibrium}' + '\n' +
    r'$\tilde{e}_{\text{eq}} \approx 5.0,\ \ \tilde{i}_{\text{eq}} \approx 2.5\ \ (e \approx 2i)$'
    + '\n' +
    r'$\tilde{e}_{\text{eq}} \propto M^{1/15} m_0^{1/15} \Sigma_{\text{gas}}^{-1/5} a^{1/5}$',
    ha='center',
    va='center',
    fontsize=8.5,
    color='#92400E',
    zorder=13)

# 4. Accretion & Equalization Callout
box_acc = FancyBboxPatch((78, 4),
                         44,
                         14,
                         boxstyle="round,pad=0.8",
                         facecolor='#FEE2E2',
                         edgecolor='#EF4444',
                         lw=1.8,
                         zorder=12)
ax3.add_patch(box_acc)
ax3.text(
    100,
    11.0,
    r'\textbf{4. Oligarchic Accretion \& Equalization}' + '\n' +
    r'$\frac{dM}{dt} \approx 2\sqrt{2\pi} R^2 \left(\frac{v_{\text{esc}}}{e v_{\text{K}}}\right)^2 \Sigma_{\text{m}} \Omega \propto M^{2/3} \Sigma_{\text{m}}$'
    + '\n' +
    r'$\frac{1}{M}\frac{dM}{dt} \propto M^{-1/3}\ \ \rightarrow\ \ \text{Self-Regulated Mass Convergence}$',
    ha='center',
    va='center',
    fontsize=8.2,
    color='#991B1B',
    zorder=13)

# Connectors from boxes to visual elements
ax3.annotate('',
             xy=(52, 47),
             xytext=(47, 68),
             arrowprops=dict(arrowstyle='->', lw=1.5, color='#10B981', ls='-'))
ax3.annotate('',
             xy=(85, 47),
             xytext=(93, 68),
             arrowprops=dict(arrowstyle='->', lw=1.5, color='#3B82F6', ls='-'))

ax3.set_title(
    r'\textbf{Dynamical Architecture of Oligarchic Planetary Growth (Kokubo \& Ida 2000)}',
    fontsize=13.5,
    pad=12)
fig3_path_pdf = os.path.join(output_dir, 'fig_diagram.pdf')
fig3_path_png = os.path.join(output_dir, 'fig_diagram.png')
fig3.savefig(fig3_path_pdf, dpi=300, bbox_inches='tight')
fig3.savefig(fig3_path_png, dpi=300, bbox_inches='tight')
plt.close(fig3)
print(f" Saved {fig3_path_pdf} and {fig3_path_png}")
