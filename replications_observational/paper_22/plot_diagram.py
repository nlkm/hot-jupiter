# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for HAT-P-11b Metastable Helium Escape

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(8, 5))

# Active Host Star HAT-P-11 (left)
star = plt.Circle((-3.0, 0),
                  1.8,
                  color='orange',
                  ec='darkorange',
                  lw=2.5,
                  zorder=10,
                  label=r'Host Star HAT-P-11 (K4V)')
ax.add_patch(star)
ax.text(-3.0,
        0,
        'Active Host Star\nHAT-P-11 (K4V)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10,
        color='darkred')

# Escaping Neptune-Sized Planet HAT-P-11b Core
planet = plt.Circle((2.5, 0),
                    0.8,
                    color='navy',
                    ec='black',
                    lw=2,
                    zorder=12,
                    label=r'HAT-P-11b ($4.73 R_E$)')
ax.add_patch(planet)
ax.text(2.5,
        0,
        'HAT-P-11b\nCore',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=9,
        color='white')

# Trailing Metastable Helium Tail (He I 10830A)
tail_x = np.array([2.5, 3.5, 4.8, 5.8])
tail_y = np.array([0.0, 0.4, 1.2, 2.0])
ax.plot(tail_x, tail_y, color='coral', lw=4, linestyle='--', zorder=9)
ax.text(
    4.2,
    1.4,
    r'Trailing Helium Tail ($2.5 R_p$ Extent, $\dot{M} = 2.5 \times 10^{10}\text{ g/s}$)',
    color='firebrick',
    fontweight='bold',
    fontsize=9,
    ha='center')

# He I 10830A Infrared Transmission Ray
ax.annotate('',
            xy=(1.0, 0.8),
            xytext=(4.5, 0.8),
            arrowprops=dict(arrowstyle="<->", color="darkred", lw=2.2))
ax.text(
    2.75,
    1.1,
    r'HST WFC3 / Keck HIRES He I 10830\AA\ Absorption ($\Delta \delta = 1.08\%$)',
    color='darkred',
    fontweight='bold',
    fontsize=9,
    ha='center')

ax.set_xlim(-5.5, 6.5)
ax.set_ylim(-3.0, 3.0)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: HAT-P-11b Metastable Helium Escape & Trailing Cometary Tail',
    fontsize=11,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_22/fig_diagram.pdf')
fig.savefig('replications_observational/paper_22/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #22 physical configuration diagram!")
