# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for WASP-121b Tidal Deformability & RLOF

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Ellipse

fig, ax = plt.subplots(figsize=(8, 5))

# Host Star WASP-121 (left)
star = plt.Circle((-3.2, 0),
                  1.8,
                  color='gold',
                  ec='goldenrod',
                  lw=2.5,
                  zorder=10,
                  label=r'Host Star WASP-121 (F6V)')
ax.add_patch(star)
ax.text(-3.2,
        0,
        'Host Star\nWASP-121 (F6V)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10,
        color='darkred')

# Prolate Tidal Ellipsoid WASP-121b (Lemon Shape: major axis along sub-stellar line)
planet = Ellipse((2.2, 0),
                 width=1.8,
                 height=1.3,
                 angle=0,
                 color='crimson',
                 ec='black',
                 lw=2,
                 zorder=12,
                 label=r'WASP-121b Prolate Ellipsoid ($1.08 R_p$)')
ax.add_patch(planet)
ax.text(2.2,
        0,
        'WASP-121b\nEllipsoid',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=8.5,
        color='white')

# Roche Lobe Equipotential & RLOF Gas Stream
roche_x = np.array([2.2, 1.2, 0.2, -0.8])
roche_y = np.array([0.0, 0.2, 0.5, 0.8])
ax.plot(roche_x, roche_y, color='darkorange', lw=3.5, linestyle='--', zorder=9)
ax.text(
    0.6,
    0.7,
    r'Roche Lobe Overflow Stream ($\dot{M}_{\text{metals}} = 1.0 \times 10^{11}\text{ g/s}$)',
    color='darkorange',
    fontweight='bold',
    fontsize=8.5,
    ha='center')

# Fe II / Mg II NUV Absorption Ray
ax.annotate('',
            xy=(0.8, -0.6),
            xytext=(3.6, -0.6),
            arrowprops=dict(arrowstyle="<->", color="purple", lw=2.2))
ax.text(
    2.2,
    -0.9,
    r'HST STIS / VLT UVES NUV Fe II / Mg II Absorption ($\Delta \delta = 0.85\%$)',
    color='purple',
    fontweight='bold',
    fontsize=8.5,
    ha='center')

ax.set_xlim(-5.5, 6.5)
ax.set_ylim(-3.0, 3.0)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: WASP-121b Prolate Tidal Deformability & Heavy Metal RLOF',
    fontsize=10.5,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_24/fig_diagram.pdf')
fig.savefig('replications_observational/paper_24/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #24 physical configuration diagram!")
