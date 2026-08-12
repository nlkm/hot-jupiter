# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Multi-Plot & Model Development Script for Observational Paper #11: Pluto-Charon Mutual Binary

import matplotlib.pyplot as plt
import numpy as np

# Set style
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')

# FIGURE 1: Model Choices & Parameter Sensitivity Analysis
fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# Panel A: Mutual Orbital Period P vs Semi-Major Axis a [km]
a_grid = np.linspace(15000, 25000, 500)
G_const = 6.67430e-11
M_total = 1.303e22 + 1.586e21
P_grid = (2.0 * np.pi * np.sqrt(
    (a_grid * 1000.0)**3 / (G_const * M_total))) / 86400.0

ax1.plot(a_grid,
         P_grid,
         'r-',
         lw=2.5,
         label=r'Keplerian Period $P \propto a^{3/2}$')
ax1.axvline(19596.0,
            color='navy',
            linestyle=':',
            label='Pluto-Charon ($a = 19,596$ km)')
ax1.scatter([19596.0], [6.387], color='crimson', s=80, zorder=5)
ax1.set_xlabel('Mutual Semi-Major Axis $a$ [km]', fontsize=12)
ax1.set_ylabel('Orbital Period $P$ [days]', fontsize=12)
ax1.set_title('Model Choice: Period vs Semi-Major Axis',
              fontsize=13,
              fontweight='bold')
ax1.legend(loc='lower right', fontsize=10)

# Panel B: Barycenter Distance vs Mass Ratio q
q_grid = np.linspace(0.05, 0.20, 500)
# r_bary = a * q / (1 + q)
r_bary_grid = 19596.0 * (q_grid / (1.0 + q_grid))

ax2.plot(q_grid,
         r_bary_grid,
         'b-',
         lw=2.5,
         label=r'Barycenter Offset $r_{\text{bary}}(q)$')
ax2.axhline(1188.3,
            color='darkorange',
            linestyle='--',
            label=r'Pluto Surface Radius ($R_P = 1188$ km)')
ax2.axvline(0.1217,
            color='crimson',
            linestyle=':',
            label=r'Observed Mass Ratio $q = 0.1217$')
ax2.set_xlabel(r'Mass Ratio $q = M_C / M_P$', fontsize=12)
ax2.set_ylabel('Barycenter Distance $r_{\text{bary}}$ [km]', fontsize=12)
ax2.set_title('Model Choice: Barycenter Offset vs Mass Ratio',
              fontsize=13,
              fontweight='bold')
ax2.legend(loc='upper left', fontsize=10)

plt.tight_layout()
fig1.savefig('replications_observational/paper_11/fig_model_choices.png',
             dpi=300)
fig1.savefig('replications_observational/paper_11/fig_model_choices.pdf')
plt.close(fig1)

# FIGURE 2: Observations vs Model Comparison
fig2, (ax3, ax4) = plt.subplots(1, 2, figsize=(12, 5))

# Panel C: Orbital Period Comparison
categories = [
    'New Horizons & HST\nObserved Period',
    'First-Principles\nKepler Binary Model'
]
obs_val = 6.38723
obs_err = 0.00005
model_val = 6.38705

ax3.bar(categories[0],
        obs_val,
        yerr=obs_err,
        capsize=6,
        color='navy',
        alpha=0.85,
        width=0.4,
        label='Observed')
ax3.bar(categories[1],
        model_val,
        color='crimson',
        alpha=0.85,
        width=0.4,
        label='Model Prediction')

ax3.set_ylabel('Binary Orbital Period [days]', fontsize=12)
ax3.set_title('Pluto-Charon Orbital Period Comparison',
              fontsize=13,
              fontweight='bold')
ax3.set_ylim(6.380, 6.395)
ax3.legend(loc='lower right', fontsize=10)

# Panel D: Multi-Instrument Dataset Consistency
datasets = [
    'Stellar Occultations (1988-2005)', 'HST FGS/WFPC2 Astrometry',
    'New Horizons LORRI (2015)'
]
periods = [6.38720, 6.38725, 6.38723]
errs = [0.00010, 0.00005, 0.00001]

ax4.errorbar(periods,
             datasets,
             xerr=errs,
             fmt='o',
             color='darkgreen',
             ecolor='gray',
             elinewidth=2,
             capsize=5,
             markersize=8)
ax4.axvline(6.38705,
            color='crimson',
            linestyle='--',
            label='C++ Model Fit ($6.3871$ d)')
ax4.set_xlabel('Orbital Period [days]', fontsize=12)
ax4.set_title('Multi-Instrument Dataset Convergence',
              fontsize=13,
              fontweight='bold')
ax4.legend(loc='lower left', fontsize=10)

plt.tight_layout()
fig2.savefig('replications_observational/paper_11/fig_comparison.png', dpi=300)
fig2.savefig('replications_observational/paper_11/fig_comparison.pdf')
plt.close(fig2)

print("✅ Saved Paper #11 multi-panel diagnostic figures!")
