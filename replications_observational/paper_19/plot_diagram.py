# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for TRAPPIST-1 Resonant Chain

import matplotlib.pyplot as plt

fig, ax = plt.subplots(figsize=(9, 5))

# Host Star TRAPPIST-1 (left)
star = plt.Circle((-4.5, 0),
                  1.0,
                  color='firebrick',
                  ec='darkred',
                  lw=2.5,
                  zorder=10,
                  label=r'Host Star TRAPPIST-1 ($0.0898 M_\odot$)')
ax.add_patch(star)
ax.text(-4.5,
        0,
        'TRAPPIST-1\n(M8V Dwarf)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10,
        color='yellow')

# 7 Resonant Planet Orbits & Circles
planets = [('b', -3.0, 0.40, '8:5', 'coral'), ('c', -1.8, 0.45, '5:3', 'gold'),
           ('d', -0.6, 0.35, '3:2', 'mediumseagreen'),
           ('e', 0.6, 0.42, '3:2', 'deepskyblue'),
           ('f', 1.8, 0.44, '4:3', 'royalblue'),
           ('g', 3.0, 0.48, '3:2', 'mediumpurple'),
           ('h', 4.2, 0.38, 'End', 'orchid')]

for name, x, r, ratio, color in planets:
    circle = plt.Circle((x, 0), r, color=color, ec='black', lw=1.5, zorder=12)
    ax.add_patch(circle)
    ax.text(x,
            -0.9,
            f'TRAPPIST-1{name}',
            ha='center',
            fontweight='bold',
            fontsize=9)
    if ratio != 'End':
        ax.annotate('',
                    xy=(x + r + 0.1, 0),
                    xytext=(x + 1.2 - 0.4, 0),
                    arrowprops=dict(arrowstyle="<->", color="gray", lw=1.5))
        ax.text(x + 0.6,
                0.3,
                ratio,
                ha='center',
                fontweight='bold',
                fontsize=9,
                color='navy')

# 3-Body Laplace Resonant Angle Feature
ax.text(
    0.0,
    2.2,
    r'3-Body Laplace Resonant Angle $\Phi = 3 \lambda_e - 5 \lambda_f + 2 \lambda_g \approx 0^\circ$ ($\Delta \Phi = \pm 1.2^\circ$)',
    ha='center',
    fontweight='bold',
    fontsize=10,
    color='darkgreen',
    bbox=dict(boxstyle="round,pad=0.4", fc="lightgreen", ec="darkgreen",
              lw=1.5))

ax.set_xlim(-6.0, 5.5)
ax.set_ylim(-2.0, 3.2)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: TRAPPIST-1 Seven-Planet Resonant Laplace Chain',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_19/fig_diagram.pdf')
fig.savefig('replications_observational/paper_19/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #19 physical configuration diagram!")
