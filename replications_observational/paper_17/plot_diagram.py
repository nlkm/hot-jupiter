# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for WASP-12b Tidal Orbital Decay

import matplotlib.pyplot as plt

fig, ax = plt.subplots(figsize=(8, 5))

# Host Star WASP-12 (center-left)
star = plt.Circle((-3.0, 0),
                  2.2,
                  color='gold',
                  ec='orange',
                  lw=2.5,
                  zorder=10,
                  label=r'Host Star WASP-12 ($1.59 R_\odot$)')
ax.add_patch(star)
ax.text(-3.0,
        0,
        'Host Star\nWASP-12',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=11,
        color='darkred')

# Stellar Tidal Bulge (tilted by lag angle \delta)
bulge = plt.Circle((-3.0, 0), 2.35, color='orange', alpha=0.35, zorder=9)
ax.add_patch(bulge)
ax.text(-3.0,
        -2.8,
        r'Stellar Tidal Bulge Lag $\delta = 1 / (2 Q_*^\prime)$',
        color='darkorange',
        fontweight='bold',
        fontsize=10,
        ha='center')

# Distorted Egg-Shaped Hot Jupiter WASP-12b (right side)
ellipse = plt.Circle((3.5, 0),
                     1.2,
                     color='firebrick',
                     ec='black',
                     lw=2,
                     zorder=12,
                     label='WASP-12b ($1.90 R_J$)')
ax.add_patch(ellipse)
ax.text(3.5,
        -1.8,
        'Tidally Distorted\nWASP-12b',
        ha='center',
        fontweight='bold',
        fontsize=11,
        color='firebrick')

# Inward Infall Trajectory Arrow
ax.annotate('',
            xy=(1.2, 0),
            xytext=(2.2, 0),
            arrowprops=dict(arrowstyle="->", color="crimson", lw=3.0))
ax.text(1.7,
        0.4,
        r'Inward Spiral ($\dot{P} = -29.27\text{ ms/yr}$)',
        color='crimson',
        fontweight='bold',
        fontsize=10,
        ha='center')

# Tidal Torque Arrow \tau_tidal
ax.annotate('',
            xy=(-3.0, 2.5),
            xytext=(-1.5, 2.5),
            arrowprops=dict(arrowstyle="->", color="navy", lw=2.2))
ax.text(-2.25,
        2.8,
        r'Stellar Tidal Torque $\tau_{\text{tidal}}$',
        color='navy',
        fontweight='bold',
        fontsize=10,
        ha='center')

ax.set_xlim(-6.0, 6.0)
ax.set_ylim(-3.5, 3.5)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: WASP-12b Tidal Dissipation & Inward Orbital Decay',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_17/fig_diagram.pdf')
fig.savefig('replications_observational/paper_17/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #17 physical configuration diagram!")
