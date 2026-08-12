# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Plotting & Comparison Script for Observational Paper #2: Enceladus Tidal Ocean & Ice Shell

import matplotlib.pyplot as plt
import numpy as np

# Set style
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# 1. Conductive Heat Flux vs Ice Shell Thickness
d_km = np.linspace(2.0, 40.0, 500)
A_conduct = 567.0
T_base = 273.15
T_surf = 75.0
R_m = 252.1e3

flux_w_m2 = (A_conduct * np.log(T_base / T_surf)) / (d_km * 1000.0)
area_m2 = 4.0 * np.pi * R_m**2
global_heat_gw = (flux_w_m2 * area_m2) / 1.0e9

ax1.plot(d_km,
         global_heat_gw,
         'b-',
         lw=2,
         label='Global Conductive Heat Loss $Q(d)$')
ax1.axvline(20.0,
            color='gray',
            linestyle='--',
            label='Global Avg Thickness $d_{\\text{avg}} = 20$ km')
ax1.axvline(5.0,
            color='crimson',
            linestyle='--',
            label='South Polar Thickness $d_{\\text{south}} = 5$ km')
ax1.scatter([20.0, 5.0], [
    global_heat_gw[np.argmin(np.abs(d_km - 20))],
    0.1 * global_heat_gw[np.argmin(np.abs(d_km - 5))]
],
            color='red',
            s=80,
            zorder=5)

ax1.axhspan(12.7,
            18.9,
            color='amber' if 'amber' in plt.colormaps else 'gold',
            alpha=0.3,
            label='Cassini CIRS Observed $Q_{\\text{obs}} = 15.8 \\pm 3.1$ GW')
ax1.set_xlabel('Ice Shell Thickness $d$ [km]', fontsize=12)
ax1.set_ylabel('Total Heat Loss $Q$ [GW]', fontsize=12)
ax1.set_title('Conductive Heat Transport vs Shell Thickness',
              fontsize=13,
              fontweight='bold')
ax1.legend(loc='upper right', fontsize=9)

# 2. Bar Chart Comparison: Cassini CIRS vs First-Principles Tidal Model
categories = [
    'Cassini CIRS\nObserved Output',
    'Model Tidal\nDissipation $E_{\\text{tidal}}$',
    'Model South Polar\nConductive ($d=5$ km)'
]
values = [15.80, 15.88, 11.71]
errors = [3.10, 0.0, 0.0]
colors = ['crimson', 'navy', 'darkgreen']

bars = ax2.bar(categories,
               values,
               yerr=errors,
               capsize=6,
               color=colors,
               alpha=0.85,
               width=0.5)
ax2.set_ylabel('Heat Power [GW]', fontsize=12)
ax2.set_title('Cassini CIRS Observations vs Tidal Model',
              fontsize=13,
              fontweight='bold')
for bar in bars:
    yval = bar.get_height()
    ax2.text(bar.get_x() + bar.get_width() / 2.0,
             yval + 0.5,
             f'{yval:.2f} GW',
             ha='center',
             va='bottom',
             fontweight='bold')

ax2.set_ylim(0, 22.0)

plt.tight_layout()
plt.savefig('replications_observational/paper_02/fig_comparison.png', dpi=300)
plt.savefig('replications_observational/paper_02/fig_comparison.pdf')
print("✅ Saved Paper #2 comparison figures!")
