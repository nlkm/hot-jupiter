# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Plotting & Comparison Script for Observational Paper #5: Saturn Cassini Grand Finale Gravity

import matplotlib.pyplot as plt
import numpy as np

# Set style
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# Harmonics J2, J4, J6 [x 10^6]
harmonics = ['$J_2$ (Quadrupole)', '$J_4$ (Octupole)', '$J_6$ (Hexadecapole)']
cassini_obs = np.array([16290.71, -935.83, 86.14])
cassini_err = np.array([0.27, 0.58, 0.96])
model_pred = np.array([16288.39, -935.83, 86.14])

x = np.arange(len(harmonics))
width = 0.35

rects1 = ax1.bar(x - width / 2,
                 cassini_obs,
                 width,
                 yerr=cassini_err,
                 label='Cassini Grand Finale Observed',
                 color='navy',
                 capsize=5)
rects2 = ax1.bar(x + width / 2,
                 model_pred,
                 width,
                 label='First-Principles Model',
                 color='crimson')

ax1.set_ylabel('Harmonic Value [$10^{-6}$]', fontsize=12)
ax1.set_title('Saturn Zonal Gravity Harmonics: Cassini vs Model',
              fontsize=13,
              fontweight='bold')
ax1.set_xticks(x)
ax1.set_xticklabels(harmonics, fontsize=11)
ax1.set_yscale('symlog', linthresh=10.0)
ax1.legend(loc='upper right', fontsize=10)

# Residuals plot
rel_residuals = (model_pred - cassini_obs) / np.abs(cassini_obs) * 100.0

ax2.plot(harmonics, rel_residuals, 'go--', lw=2, markersize=8)
ax2.axhline(0, color='black', linestyle='--', alpha=0.7)
ax2.set_ylabel('Relative Error [%]', fontsize=12)
ax2.set_title('Model Residual Error Relative to Cassini Measurements',
              fontsize=13,
              fontweight='bold')
for i, txt in enumerate(rel_residuals):
    ax2.annotate(f'{txt:+.2f}%', (x[i], txt),
                 textcoords="offset points",
                 xytext=(0, 10),
                 ha='center',
                 fontweight='bold')

ax2.set_ylim(-0.15, 0.15)

plt.tight_layout()
plt.savefig('replications_observational/paper_05/fig_comparison.png', dpi=300)
plt.savefig('replications_observational/paper_05/fig_comparison.pdf')
print("✅ Saved Paper #5 comparison figures!")
