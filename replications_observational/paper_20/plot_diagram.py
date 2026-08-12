# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for Kepler-223 Resonant Chain

import matplotlib.pyplot as plt

fig, ax = plt.subplots(figsize=(9, 5))

# Host Star Kepler-223 (left)
star = plt.Circle((-4.0, 0),
                  1.2,
                  color='gold',
                  ec='orange',
                  lw=2.5,
                  zorder=10,
                  label=r'Host Star Kepler-223 ($1.13 M_\odot$)')
ax.add_patch(star)
ax.text(-4.0,
        0,
        'Kepler-223\n(G-Type Star)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10,
        color='darkred')

# 4 Sub-Neptune Planets & Resonant Ratios
planets = [('b', -2.2, 0.50, '4:3', 'coral', '7.4 M_e'),
           ('c', -0.6, 0.45, '3:2', 'deepskyblue', '5.1 M_e'),
           ('d', 1.0, 0.55, '4:3', 'mediumpurple', '8.0 M_e'),
           ('e', 2.6, 0.42, 'End', 'seagreen', '4.8 M_e')]

for name, x, r, ratio, color, mass in planets:
    circle = plt.Circle((x, 0), r, color=color, ec='black', lw=1.5, zorder=12)
    ax.add_patch(circle)
    ax.text(x,
            -0.9,
            f'Kepler-223{name}\n({mass})',
            ha='center',
            fontweight='bold',
            fontsize=9)
    if ratio != 'End':
        ax.annotate('',
                    xy=(x + r + 0.1, 0),
                    xytext=(x + 1.6 - 0.55, 0),
                    arrowprops=dict(arrowstyle="<->", color="gray", lw=1.5))
        ax.text(x + 0.8,
                0.35,
                ratio,
                ha='center',
                fontweight='bold',
                fontsize=10,
                color='navy')

# 4-Planet Resonant Lock Badge
ax.text(
    -0.2,
    2.2,
    r'Pristine 4-Planet Resonant Chain Ratio 8 : 6 : 4 : 3 ($\Delta \Phi = \pm 2.4^\circ$)',
    ha='center',
    fontweight='bold',
    fontsize=10,
    color='darkblue',
    bbox=dict(boxstyle="round,pad=0.4", fc="lavender", ec="navy", lw=1.5))

ax.set_xlim(-5.5, 4.0)
ax.set_ylim(-2.0, 3.2)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: Kepler-223 Four-Planet Resonant Laplace Chain',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_20/fig_diagram.pdf')
fig.savefig('replications_observational/paper_20/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #20 physical configuration diagram!")
