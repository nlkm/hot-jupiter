# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for Eris-Dysnomia Binary System

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(7, 7))

# Draw Eris at origin
eris = plt.Circle((0, 0),
                  0.8,
                  color='whitesmoke',
                  ec='black',
                  lw=2,
                  zorder=8,
                  label='Eris ($R_E = 1163$ km)')
ax.add_patch(eris)
ax.text(0,
        0,
        'Eris\n($1.66 \\times 10^{22}$ kg)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10)

# Dysnomia orbit (a = 37350 km)
t_grid = np.linspace(0, 2 * np.pi, 300)
a_orbit = 3.2
x_orbit = a_orbit * np.cos(t_grid)
y_orbit = a_orbit * np.sin(t_grid)
ax.plot(x_orbit,
        y_orbit,
        'b--',
        lw=1.8,
        label='Dysnomia Orbit ($a = 37,350$ km)')

# Dysnomia position
d_angle = np.radians(45)
x_d = a_orbit * np.cos(d_angle)
y_d = a_orbit * np.sin(d_angle)

dysnomia = plt.Circle((x_d, y_d),
                      0.35,
                      color='gray',
                      ec='black',
                      lw=1.5,
                      zorder=9)
ax.add_patch(dysnomia)
ax.text(x_d + 0.3,
        y_d + 0.3,
        'Dysnomia\n($P = 15.774$ d)',
        fontweight='bold',
        fontsize=10,
        color='navy')

# Gravitational force arrow F_grav
ax.annotate('',
            xy=(0, 0),
            xytext=(x_d, y_d),
            arrowprops=dict(arrowstyle="->", color="crimson", lw=2.5))
ax.text(x_d * 0.5 - 0.3,
        y_d * 0.5 + 0.2,
        r'$\vec{F}_{\text{grav}}$',
        color='crimson',
        fontweight='bold',
        fontsize=12)

# Orbital velocity arrow v_orb
v_dx = -np.sin(d_angle) * 1.2
v_dy = np.cos(d_angle) * 1.2
ax.annotate('',
            xy=(x_d + v_dx, y_d + v_dy),
            xytext=(x_d, y_d),
            arrowprops=dict(arrowstyle="->", color="darkgreen", lw=2.5))
ax.text(x_d + v_dx * 0.5 + 0.2,
        y_d + v_dy * 0.5,
        r'$\vec{v}_{\text{orb}}$',
        color='darkgreen',
        fontweight='bold',
        fontsize=12)

ax.set_xlim(-4.2, 4.2)
ax.set_ylim(-4.2, 4.2)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title('Physical Configuration Diagram: Eris-Dysnomia Binary Orbit',
             fontsize=12,
             fontweight='bold',
             pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_12/fig_diagram.pdf')
fig.savefig('replications_observational/paper_12/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #12 physical configuration diagram!")
