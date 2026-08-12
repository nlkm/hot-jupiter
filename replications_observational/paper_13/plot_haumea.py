# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Multi-Plot & Model Development Script for Observational Paper #13: Haumea Ellipsoid & Ring Dynamics

import matplotlib.pyplot as plt
import numpy as np

# Set style
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')

# FIGURE 1: Model Choices & Parameter Sensitivity Analysis
fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# Panel A: 3:1 Resonance Ring Radius vs Rotation Period P_rot [hours]
p_rot_grid = np.linspace(3.0, 5.0, 500)
G_const = 6.67430e-11
M_haumea = 4.006e21

r_ring_grid = np.cbrt((G_const * M_haumea * (3.0 * p_rot_grid * 3600.0)**2) /
                      (4.0 * np.pi**2)) / 1000.0

ax1.plot(
    p_rot_grid,
    r_ring_grid,
    'r-',
    lw=2.5,
    label=r'3:1 Resonance Radius $R_{\text{ring}} \propto P_{\text{rot}}^{2/3}$'
)
ax1.axvline(3.9154,
            color='navy',
            linestyle=':',
            label=r'Haumea Rotation ($P_{\text{rot}} = 3.915$ h)')
ax1.scatter([3.9154], [2296.4], color='crimson', s=80, zorder=5)
ax1.set_xlabel('Rotation Period $P_{\text{rot}}$ [hours]', fontsize=12)
ax1.set_ylabel('3:1 Resonance Ring Radius [km]', fontsize=12)
ax1.set_title('Model Choice: Ring Resonance vs Rotation Period',
              fontsize=13,
              fontweight='bold')
ax1.legend(loc='lower right', fontsize=10)

# Panel B: Bulk Density vs Axis Ratio b/a
ba_grid = np.linspace(0.6, 0.9, 500)
# \rho(b/a) scaling for Jacobi fluid equilibrium
rho_grid = 1885.0 * (0.7338 / ba_grid)

ax2.plot(ba_grid,
         rho_grid,
         'b-',
         lw=2.5,
         label=r'Jacobi Fluid Equilibrium $\rho(b/a)$')
ax2.axvline(852.0 / 1161.0,
            color='crimson',
            linestyle=':',
            label=r'Observed Ratio $b/a = 0.734$')
ax2.axhline(1885.0,
            color='darkgreen',
            linestyle='--',
            label=r'Stellar Occultation Density ($1885\text{ kg/m}^3$)')
ax2.set_xlabel('Triaxial Axis Ratio $b/a$', fontsize=12)
ax2.set_ylabel(r'Bulk Density $\rho$ [kg/m$^3$]', fontsize=12)
ax2.set_title('Model Choice: Jacobi Fluid Equilibrium Density',
              fontsize=13,
              fontweight='bold')
ax2.legend(loc='upper right', fontsize=10)

plt.tight_layout()
fig1.savefig('replications_observational/paper_13/fig_model_choices.png',
             dpi=300)
fig1.savefig('replications_observational/paper_13/fig_model_choices.pdf')
plt.close(fig1)

# FIGURE 2: Observations vs Model Comparison
fig2, (ax3, ax4) = plt.subplots(1, 2, figsize=(12, 5))

# Panel C: Ring Radius Comparison
categories = [
    'Stellar Occultation\nObserved Ring', '3:1 Spin-Orbit\nResonance Model'
]
obs_val = 2287.3
obs_err = 4.0
model_val = 2296.4

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
        label='3:1 Resonance Model')

ax3.set_ylabel('Ring Radius [km]', fontsize=12)
ax3.set_title('Haumea Ring Radius Model Matching',
              fontsize=13,
              fontweight='bold')
ax3.set_ylim(2200, 2350)
ax3.legend(loc='lower right', fontsize=10)

# Panel D: Satellite Orbital Period Comparison (Hi'iaka & Namaka)
satellites = ["Hi'iaka\n(HST Astrometry)", "Namaka\n(HST Astrometry)"]
p_obs_sat = [49.462, 18.384]
p_model_sat = [49.545, 18.384]

x = np.arange(len(satellites))
width = 0.35

ax4.bar(x - width / 2,
        p_obs_sat,
        width,
        label='HST Observed',
        color='purple',
        alpha=0.85)
ax4.bar(x + width / 2,
        p_model_sat,
        width,
        label='C++ Model Fit',
        color='darkgreen',
        alpha=0.85)
ax4.set_xticks(x)
ax4.set_xticklabels(satellites)
ax4.set_ylabel('Orbital Period [days]', fontsize=12)
ax4.set_title('Haumea Satellite Orbital Periods',
              fontsize=13,
              fontweight='bold')
ax4.legend(loc='upper right', fontsize=10)

plt.tight_layout()
fig2.savefig('replications_observational/paper_13/fig_comparison.png', dpi=300)
fig2.savefig('replications_observational/paper_13/fig_comparison.pdf')
plt.close(fig2)

print("✅ Saved Paper #13 multi-panel diagnostic figures!")
