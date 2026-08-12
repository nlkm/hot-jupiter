# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for Comet Outgassing

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

# Draw Bi-lobed Comet Nucleus (67P rubber-duck shape)
circle1 = plt.Circle((-0.3, 0.4), 0.55, color='gray', ec='black', lw=2)
circle2 = plt.Circle((0.4, -0.2), 0.85, color='darkgray', ec='black', lw=2)
ax.add_patch(circle1)
ax.add_patch(circle2)

# Sublimation Outgassing Jets erupting from neck and sunlit surface
for angle, scale in zip([120, 140, 160, 90, 70], [1.2, 1.5, 1.4, 1.1, 1.3]):
    rad = np.radians(angle)
    x0, y0 = -0.3 + 0.55 * np.cos(rad), 0.4 + 0.55 * np.sin(rad)
    x1, y1 = x0 + scale * np.cos(rad), y0 + scale * np.sin(rad)
    ax.annotate('',
                xy=(x1, y1),
                xytext=(x0, y0),
                arrowprops=dict(arrowstyle="->", color="cyan", lw=2.0))

ax.text(-1.8,
        1.8,
        r'Sublimation Jets ($\vec{v}_{\text{gas}} \approx 800\text{ m/s}$)',
        fontsize=10,
        color='cyan',
        fontweight='bold')

# Net Non-Gravitational Rocket Thrust F_ng
ax.annotate('',
            xy=(1.5, -0.6),
            xytext=(0.4, -0.2),
            arrowprops=dict(arrowstyle="->", color="crimson", lw=3.0))
ax.text(
    1.2,
    -1.0,
    r'$\vec{F}_{\text{ng}} = -\dot{M} \vec{v}_{\text{gas}}$ (Rocket Thrust)',
    fontsize=11,
    color='crimson',
    fontweight='bold')

# Radial A1 and Transverse A2 vector decomposition
ax.annotate('',
            xy=(1.5, -0.2),
            xytext=(0.4, -0.2),
            arrowprops=dict(arrowstyle="->", color="navy", lw=2.0))
ax.text(1.2, -0.05, r'Radial $A_1 \hat{\mathbf{r}}$', fontsize=10, color='navy')

ax.annotate('',
            xy=(0.4, -1.2),
            xytext=(0.4, -0.2),
            arrowprops=dict(arrowstyle="->", color="darkgreen", lw=2.0))
ax.text(0.5,
        -1.3,
        r'Transverse $A_2 \hat{\mathbf{t}}$',
        fontsize=10,
        color='darkgreen')

ax.set_xlim(-4.0, 3.0)
ax.set_ylim(-2.5, 2.5)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: Comet Non-Gravitational Rocket Effect',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_09/fig_diagram.pdf')
fig.savefig('replications_observational/paper_09/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #9 physical configuration diagram!")
