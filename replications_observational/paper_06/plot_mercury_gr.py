# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Plotting & Comparison Script for Observational Paper #6: Mercury GR Precession & Solar J2

import matplotlib.pyplot as plt

# Set style
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# 1. Precession Rates breakdown
components = [
    'Planetary\nPerturbations', 'General\nRelativity (GR)',
    'Solar Oblateness\n($J_2 = 2.25 \\times 10^{-7}$)'
]
rates = [531.63, 42.982, 0.0286]
colors = ['gray', 'crimson', 'gold']

bars = ax1.bar(components, rates, color=colors, alpha=0.85, width=0.45)
ax1.set_ylabel('Precession Rate [arcsec/century]', fontsize=12)
ax1.set_title('Contributions to Mercury Pericenter Precession',
              fontsize=13,
              fontweight='bold')
ax1.set_yscale('log')
for bar in bars:
    yval = bar.get_height()
    ax1.text(bar.get_x() + bar.get_width() / 2.0,
             yval * 1.15,
             f'{yval:.3f}"/cy',
             ha='center',
             va='bottom',
             fontweight='bold')

ax1.set_ylim(0.005, 1000.0)

# 2. Non-Newtonian Precession Comparison: MESSENGER vs GR Model
categories = [
    'MESSENGER Observed\nNon-Newtonian Rate',
    'General Relativity\nModel Prediction'
]
obs_val = 42.9800
obs_err = 0.0400
model_val = 42.9820

ax2.bar(categories[0],
        obs_val,
        yerr=obs_err,
        capsize=6,
        color='navy',
        alpha=0.85,
        width=0.4,
        label='MESSENGER Observed')
ax2.bar(categories[1],
        model_val,
        color='crimson',
        alpha=0.85,
        width=0.4,
        label='GR Model Prediction')

ax2.set_ylabel('Pericenter Precession Rate [arcsec/century]', fontsize=12)
ax2.set_title('Non-Newtonian Pericenter Advance Comparison',
              fontsize=13,
              fontweight='bold')
ax2.set_ylim(42.5, 43.5)
ax2.legend(loc='lower right', fontsize=10)

plt.tight_layout()
plt.savefig('replications_observational/paper_06/fig_comparison.png', dpi=300)
plt.savefig('replications_observational/paper_06/fig_comparison.pdf')
print("✅ Saved Paper #6 comparison figures!")
