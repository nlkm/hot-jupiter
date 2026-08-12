# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for TOI-560b Young Sub-Neptune Escape

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(8, 5))

# Active Host Star TOI-560 (left)
star = plt.Circle((-3.0, 0),
                  1.6,
                  color='darkorange',
                  ec='orangered',
                  lw=2.5,
                  zorder=10,
                  label=r'Host Star TOI-560 (500 Myr K4V)')
ax.add_patch(star)
ax.text(-3.0,
        0,
        'Active Host Star\nTOI-560 (500 Myr)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10,
        color='darkred')

# Escaping Young Sub-Neptune TOI-560b Core
planet = plt.Circle((2.5, 0),
                    0.6,
                    color='mediumpurple',
                    ec='black',
                    lw=2,
                    zorder=12,
                    label=r'TOI-560b ($2.80 R_E$)')
ax.add_patch(planet)
ax.text(2.5,
        0,
        'TOI-560b\nCore',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=8.5,
        color='white')

# Hydrodynamic Helium Outflow Wind (v = 10.2 km/s)
wind_x = np.array([2.5, 3.4, 4.6, 5.5])
wind_y = np.array([0.0, 0.3, 0.9, 1.5])
ax.plot(wind_x, wind_y, color='deeppink', lw=3.5, linestyle='--', zorder=9)
ax.text(
    4.0,
    1.2,
    r'Outflowing Wind ($v_{\text{wind}} = 10.2\text{ km/s}$, $\dot{M} = 4.2 \times 10^{10}\text{ g/s}$)',
    color='purple',
    fontweight='bold',
    fontsize=8.5,
    ha='center')

# He I 10830A Transmission Absorption Ray
ax.annotate('',
            xy=(1.0, 0.6),
            xytext=(4.2, 0.6),
            arrowprops=dict(arrowstyle="<->", color="darkred", lw=2.2))
ax.text(
    2.6,
    0.85,
    r'JWST NIRSpec / Keck HIRES He I 10830\AA\ Absorption ($\Delta \delta = 0.68\%$)',
    color='darkred',
    fontweight='bold',
    fontsize=8.5,
    ha='center')

ax.set_xlim(-5.5, 6.5)
ax.set_ylim(-3.0, 3.0)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: TOI-560b Young Sub-Neptune Hydrodynamic Atmospheric Escape',
    fontsize=10.5,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_23/fig_diagram.pdf')
fig.savefig('replications_observational/paper_23/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #23 physical configuration diagram!")
