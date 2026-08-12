# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Multi-Plot & Model Development Script for Observational Paper #6: Mercury GR Precession & Solar J2

import matplotlib.pyplot as plt
import numpy as np

# Set style
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')

# FIGURE 1: Model Choices & Parameter Sensitivity Analysis
fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# Panel A: GR vs Solar J2 Precession as a Function of Semi-Major Axis a [AU]
a_grid = np.linspace(0.1, 1.0, 500)
e_merc = 0.205630
P_grid = (a_grid)**1.5 * 0.240849 / (0.387098)**1.5  # Kepler 3rd Law in years

# GR rate \propto 1 / (a * (1-e^2) * P) \propto a^(-2.5)
gr_a = 42.982 * (0.387098 / a_grid)**2.5
# Solar J2 rate \propto 1 / (a^2 * (1-e^2)^2 * P) \propto a^(-3.5)
j2_a = 0.0286 * (0.387098 / a_grid)**3.5

ax1.plot(
    a_grid,
    gr_a,
    'r-',
    lw=2,
    label='General Relativity $\\Delta \\varpi_{\\text{GR}} \\propto a^{-2.5}$')
ax1.plot(a_grid,
         j2_a,
         'g--',
         lw=2,
         label='Solar $J_2 = 2.25 \\times 10^{-7} \\propto a^{-3.5}$')
ax1.axvline(0.387098,
            color='navy',
            linestyle=':',
            label='Mercury ($a = 0.387$ AU)')
ax1.set_yscale('log')
ax1.set_xlabel('Semi-Major Axis $a$ [AU]', fontsize=12)
ax1.set_ylabel('Precession Rate [arcsec/century]', fontsize=12)
ax1.set_title('Model Sensitivity: Precession vs Distance',
              fontsize=13,
              fontweight='bold')
ax1.legend(loc='upper right', fontsize=10)

# Panel B: GR Precession vs Orbital Eccentricity e
e_grid = np.linspace(0.0, 0.6, 500)
gr_e = 42.982 * (1.0 - e_merc**2) / (1.0 - e_grid**2)

ax2.plot(
    e_grid,
    gr_e,
    'b-',
    lw=2,
    label='GR Precession $\\Delta \\varpi_{\\text{GR}}(e) \\propto (1-e^2)^{-1}$'
)
ax2.axvline(e_merc,
            color='crimson',
            linestyle=':',
            label=f'Mercury $e = {e_merc}$')
ax2.set_xlabel('Orbital Eccentricity $e$', fontsize=12)
ax2.set_ylabel('Precession Rate [arcsec/century]', fontsize=12)
ax2.set_title('Model Sensitivity: Precession vs Eccentricity',
              fontsize=13,
              fontweight='bold')
ax2.legend(loc='upper left', fontsize=10)

plt.tight_layout()
fig1.savefig('replications_observational/paper_06/fig_model_choices.png',
             dpi=300)
fig1.savefig('replications_observational/paper_06/fig_model_choices.pdf')
plt.close(fig1)

# FIGURE 2: Observations vs Model Comparison
fig2, (ax3, ax4) = plt.subplots(1, 2, figsize=(12, 5))

# Panel C: Precession Rates breakdown
components = [
    'Planetary\nPerturbations', 'General\nRelativity (GR)',
    'Solar Oblateness\n($J_2 = 2.25 \\times 10^{-7}$)'
]
rates = [531.63, 42.982, 0.0286]
colors = ['gray', 'crimson', 'gold']

bars = ax3.bar(components, rates, color=colors, alpha=0.85, width=0.45)
ax3.set_ylabel('Precession Rate [arcsec/century]', fontsize=12)
ax3.set_title('Contributions to Mercury Pericenter Precession',
              fontsize=13,
              fontweight='bold')
ax3.set_yscale('log')
for bar in bars:
    yval = bar.get_height()
    ax3.text(bar.get_x() + bar.get_width() / 2.0,
             yval * 1.15,
             f'{yval:.3f}"/cy',
             ha='center',
             va='bottom',
             fontweight='bold')

ax3.set_ylim(0.005, 1000.0)

# Panel D: Non-Newtonian Precession Comparison: MESSENGER vs GR Model
categories = [
    'MESSENGER Observed\nNon-Newtonian Rate',
    'General Relativity\nModel Prediction'
]
obs_val = 42.9800
obs_err = 0.0400
model_val = 42.9820

ax4.bar(categories[0],
        obs_val,
        yerr=obs_err,
        capsize=6,
        color='navy',
        alpha=0.85,
        width=0.4,
        label='MESSENGER Observed')
ax4.bar(categories[1],
        model_val,
        color='crimson',
        alpha=0.85,
        width=0.4,
        label='GR Model Prediction')

ax4.set_ylabel('Pericenter Precession Rate [arcsec/century]', fontsize=12)
ax4.set_title('Non-Newtonian Pericenter Advance Comparison',
              fontsize=13,
              fontweight='bold')
ax4.set_ylim(42.5, 43.5)
ax4.legend(loc='lower right', fontsize=10)

plt.tight_layout()
fig2.savefig('replications_observational/paper_06/fig_comparison.png', dpi=300)
fig2.savefig('replications_observational/paper_06/fig_comparison.pdf')
plt.close(fig2)

print("✅ Saved Paper #6 multi-panel diagnostic figures!")
