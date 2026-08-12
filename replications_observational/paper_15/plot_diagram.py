# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for HD 189733b Flare Mass Loss

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(8, 5))

# Draw Host Star HD 189733 (left side) with flare eruption
ax.scatter([-6.0], [0],
           color='orangered',
           s=1400,
           zorder=10,
           label='Host Star HD 189733 (K-dwarf)')
ax.text(-6.0,
        -1.9,
        'Host Star\nHD 189733',
        ha='center',
        fontweight='bold',
        fontsize=11)

# Flare X-ray emission rays
for angle in np.linspace(-0.6, 0.6, 7):
    ax.annotate('',
                xy=(-3.5 + 2.5 * np.cos(angle), 2.5 * np.sin(angle)),
                xytext=(-5.2, 0),
                arrowprops=dict(arrowstyle="->",
                                color="crimson",
                                lw=2.2,
                                linestyle="-"))
ax.text(
    -3.8,
    1.6,
    r'Stellar X-Ray Flare Eruption $F_X \sim 10^5\text{ erg/cm}^2/\text{s}$',
    color='crimson',
    fontweight='bold',
    fontsize=11)

# Deep Blue Hot Jupiter HD 189733b (center)
planet = plt.Circle((0, 0),
                    0.75,
                    color='deepskyblue',
                    ec='midnightblue',
                    lw=2.5,
                    zorder=12,
                    label='HD 189733b ($1.14 R_J$)')
ax.add_patch(planet)
ax.text(0,
        -1.5,
        'Hot Jupiter\nHD 189733b',
        ha='center',
        fontweight='bold',
        fontsize=11,
        color='midnightblue')

# Flare-Enhanced Hydrodynamic Mass Loss Cloud
tail_x = np.linspace(0.75, 7.0, 200)
tail_upper = 0.75 + 0.4 * (tail_x - 0.75)**1.3
tail_lower = -tail_upper

ax.fill_between(
    tail_x,
    tail_lower,
    tail_upper,
    color='darkorange',
    alpha=0.35,
    zorder=5,
    label=
    r'Flare-Enhanced Wind ($\dot{M} \approx 4.5 \times 10^{11}\text{ g/s}$)')
ax.text(4.0,
        0.0,
        r'Transient Ly-$\alpha$ Absorption Spike ($\Delta F/F = 14.4\%$)',
        color='darkred',
        fontweight='bold',
        fontsize=10,
        ha='center')

# Radiation Pressure acceleration arrow
ax.annotate('',
            xy=(6.5, 0),
            xytext=(2.5, 0),
            arrowprops=dict(arrowstyle="->", color="darkred", lw=2.5))
ax.text(4.5,
        -0.6,
        r'X-Ray Heating & Momentum Drive',
        color='darkred',
        fontsize=9,
        fontweight='bold')

ax.set_xlim(-7.5, 8.0)
ax.set_ylim(-3.0, 3.0)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: HD 189733b Stellar Flare Driven Mass Loss',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_15/fig_diagram.pdf')
fig.savefig('replications_observational/paper_15/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #15 physical configuration diagram!")
