# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Multi-Plot & Model Development Script for Observational Paper #12: Eris-Dysnomia Mutual Binary

import matplotlib.pyplot as plt
import numpy as np

# Set style
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')

# FIGURE 1: Model Choices & Parameter Sensitivity Analysis
fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# Panel A: Mutual Orbital Period P vs Semi-Major Axis a [km]
a_grid = np.linspace(30000, 45000, 500)
G_const = 6.67430e-11
M_total = 1.66e22 + 1.0e20
P_grid = (2.0 * np.pi * np.sqrt(
    (a_grid * 1000.0)**3 / (G_const * M_total))) / 86400.0

ax1.plot(a_grid,
         P_grid,
         'r-',
         lw=2.5,
         label=r'Keplerian Period $P \propto a^{3/2}$')
ax1.axvline(37350.0,
            color='navy',
            linestyle=':',
            label='Eris-Dysnomia ($a = 37,350$ km)')
ax1.scatter([37350.0], [15.774], color='crimson', s=80, zorder=5)
ax1.set_xlabel('Mutual Semi-Major Axis $a$ [km]', fontsize=12)
ax1.set_ylabel('Orbital Period $P$ [days]', fontsize=12)
ax1.set_title('Model Choice: Period vs Orbit Size',
              fontsize=13,
              fontweight='bold')
ax1.legend(loc='lower right', fontsize=10)

# Panel B: Bulk Density vs Radius
R_grid = np.linspace(1000, 1300, 500)
# \rho = M / (4/3 \pi R^3)
rho_grid = 1.66e22 / ((4.0 / 3.0) * np.pi * (R_grid * 1000.0)**3)

ax2.plot(R_grid, rho_grid, 'b-', lw=2.5, label=r'Bulk Density $\rho(R)$')
ax2.axvline(1163.0,
            color='crimson',
            linestyle=':',
            label=r'Occultation Radius ($R_E = 1163$ km)')
ax2.axhline(2520.0,
            color='darkgreen',
            linestyle='--',
            label=r'Measured Density ($2520\text{ kg/m}^3$)')
ax2.set_xlabel('Eris Radius $R_E$ [km]', fontsize=12)
ax2.set_ylabel(r'Bulk Density $\rho$ [kg/m$^3$]', fontsize=12)
ax2.set_title('Model Choice: Bulk Density Sensitivity',
              fontsize=13,
              fontweight='bold')
ax2.legend(loc='upper right', fontsize=10)

plt.tight_layout()
fig1.savefig('replications_observational/paper_12/fig_model_choices.png',
             dpi=300)
fig1.savefig('replications_observational/paper_12/fig_model_choices.pdf')
plt.close(fig1)

# FIGURE 2: Observations vs Model Comparison
fig2, (ax3, ax4) = plt.subplots(1, 2, figsize=(12, 5))

# Panel C: Orbital Period Comparison
categories = [
    'ALMA & HST\nObserved Period', 'First-Principles\nKepler Binary Model'
]
obs_val = 15.7740
obs_err = 0.0002
model_val = 15.7232

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
ax3.set_title('Eris-Dysnomia Orbital Period Comparison',
              fontsize=13,
              fontweight='bold')
ax3.set_ylim(15.5, 16.0)
ax3.legend(loc='lower right', fontsize=10)

# Panel D: Dwarf Planet Density Comparison
dwarf_planets = ['Eris', 'Haumea', 'Pluto', 'Makemake', 'Ceres']
densities = [2520, 2018, 1850, 1700, 2160]
colors_dp = ['purple', 'teal', 'chocolate', 'darkgreen', 'gray']

ax4.bar(dwarf_planets, densities, color=colors_dp, alpha=0.85, width=0.5)
ax4.axhline(2520,
            color='purple',
            linestyle='--',
            label='Eris High Rock Fraction ($2520\text{ kg/m}^3$)')
ax4.set_ylabel(r'Bulk Density [kg/m$^3$]', fontsize=12)
ax4.set_title('Outer Solar System Dwarf Planet Densities',
              fontsize=13,
              fontweight='bold')
ax4.legend(loc='upper right', fontsize=9)

plt.tight_layout()
fig2.savefig('replications_observational/paper_12/fig_comparison.png', dpi=300)
fig2.savefig('replications_observational/paper_12/fig_comparison.pdf')
plt.close(fig2)

print("✅ Saved Paper #12 multi-panel diagnostic figures!")
