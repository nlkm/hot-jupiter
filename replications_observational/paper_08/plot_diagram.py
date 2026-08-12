# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for Yarkovsky Effect

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(7, 7))

# Draw Sun & Solar Rays
ax.scatter([-3.5], [0], color='gold', s=1200, zorder=5, label='Sun')
for y in np.linspace(-1.5, 1.5, 7):
    ax.annotate('',
                xy=(-0.9, y),
                xytext=(-2.8, y),
                arrowprops=dict(arrowstyle="->", color="orange", lw=1.8))

# Draw Asteroid
circle = plt.Circle((0, 0), 1.0, color='slategray', ec='black', lw=2)
ax.add_patch(circle)

# Morning / Afternoon temperature shading gradient
theta = np.linspace(0, 2 * np.pi, 200)
# Afternoon sector (upper right / quadrant) warm region
ax.fill_between(1.0 * np.cos(theta[25:75]),
                1.0 * np.sin(theta[25:75]),
                color='orangered',
                alpha=0.4,
                label='Afternoon Hot Spot')

# Spin vector \omega (Retrograde spin)
ax.annotate('',
            xy=(0, 1.6),
            xytext=(0, -1.6),
            arrowprops=dict(arrowstyle="->", color="darkred", lw=2.5))
ax.text(0.1,
        1.4,
        r'$\vec{\omega}_{\text{spin}}$ (Spin Axis)',
        fontsize=11,
        color='darkred',
        fontweight='bold')

# Thermal Re-radiation Photons & Recoil Force F_Yark
ax.annotate('',
            xy=(0.8, 0.8),
            xytext=(1.8, 1.8),
            arrowprops=dict(arrowstyle="->", color="crimson", lw=2.5))
ax.text(1.2, 1.9, r'Thermal Photons ($p = E/c$)', fontsize=10, color='crimson')

# Net Yarkovsky Recoil Force F_Yark (points opposite to photon emission)
ax.annotate('',
            xy=(-1.2, -1.2),
            xytext=(0.4, 0.4),
            arrowprops=dict(arrowstyle="->", color="navy", lw=3.0))
ax.text(-1.8,
        -1.5,
        r'$\vec{F}_{\text{Yark}}$ (Recoil Thrust)',
        fontsize=11,
        color='navy',
        fontweight='bold')

# Orbital Velocity Vector v_orb
ax.annotate('',
            xy=(1.5, -0.2),
            xytext=(1.5, 1.2),
            arrowprops=dict(arrowstyle="->", color="darkgreen", lw=2.5))
ax.text(1.6,
        0.5,
        r'$\vec{v}_{\text{orb}}$ (Orbital Velocity)',
        fontsize=11,
        color='darkgreen',
        fontweight='bold')

# Orbit trajectory curve
orbit_arc = np.linspace(-1.2, 1.2, 100)
ax.plot(np.full_like(orbit_arc, 1.5), orbit_arc, 'g--', lw=1.5)

ax.set_xlim(-4.0, 3.0)
ax.set_ylim(-2.5, 2.5)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title('Physical Configuration Diagram: Diurnal Yarkovsky Effect',
             fontsize=12,
             fontweight='bold',
             pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_08/fig_diagram.pdf')
fig.savefig('replications_observational/paper_08/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #8 physical configuration diagram!")
