# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for Haumea System

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(8, 5))

# Draw Haumea Triaxial Ellipsoid (scaled schematic)
t_grid = np.linspace(0, 2 * np.pi, 300)
a_semi = 1.6
b_semi = 1.1

x_haumea = a_semi * np.cos(t_grid)
y_haumea = b_semi * np.sin(t_grid)
ax.plot(x_haumea, y_haumea, color='saddlebrown', lw=2.5)
ax.fill(x_haumea,
        y_haumea,
        color='burlywood',
        alpha=0.8,
        label='Haumea Ellipsoid ($1161 \\times 852 \\times 513$ km)')
ax.text(0,
        0,
        'Haumea\n($P_{\\text{rot}} = 3.915$ h)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10)

# Spin Vector Arrow
ax.annotate('',
            xy=(0, 2.2),
            xytext=(0, 0),
            arrowprops=dict(arrowstyle="->", color="darkred", lw=2.5))
ax.text(0.2,
        2.0,
        r'$\vec{\omega}_{\text{rot}}$ (3.915 h)',
        color='darkred',
        fontweight='bold',
        fontsize=10)

# Ring (R_ring = 2287 km)
r_ring_scale = 2.8
x_ring = r_ring_scale * np.cos(t_grid)
y_ring = (r_ring_scale * 0.3) * np.sin(t_grid)
ax.plot(x_ring,
        y_ring,
        color='gray',
        linestyle='-',
        lw=3,
        label='Dense Ring ($R_{\\text{ring}} = 2287$ km, 3:1 Resonance)')

# Outer Satellite Orbits (Hi'iaka & Namaka)
ax.plot(4.5 * np.cos(t_grid),
        4.5 * 0.4 * np.sin(t_grid),
        'b:',
        lw=1.5,
        label='Namaka Orbit ($a_N = 25,657$ km)')
ax.plot(6.2 * np.cos(t_grid),
        6.2 * 0.4 * np.sin(t_grid),
        'm--',
        lw=1.5,
        label="Hi'iaka Orbit ($a_H = 49,880$ km)")

# Satellite positions
ax.scatter([4.5 * np.cos(0.8)], [4.5 * 0.4 * np.sin(0.8)],
           color='blue',
           s=60,
           zorder=8)
ax.text(4.5 * np.cos(0.8) + 0.2,
        4.5 * 0.4 * np.sin(0.8),
        'Namaka',
        color='blue',
        fontweight='bold',
        fontsize=9)

ax.scatter([-6.2 * np.cos(0.5)], [-6.2 * 0.4 * np.sin(0.5)],
           color='purple',
           s=80,
           zorder=8)
ax.text(-6.2 * np.cos(0.5) - 1.2,
        -6.2 * 0.4 * np.sin(0.5),
        "Hi'iaka",
        color='purple',
        fontweight='bold',
        fontsize=9)

ax.set_xlim(-8.0, 8.0)
ax.set_ylim(-3.5, 3.5)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: Haumea Triaxial Ellipsoid, Ring, & Satellite System',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_13/fig_diagram.pdf')
fig.savefig('replications_observational/paper_13/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #13 physical configuration diagram!")
