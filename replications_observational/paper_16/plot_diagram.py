# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for GJ 436b Giant Hydrogen Cloud

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(8, 5))

# Draw M-Dwarf Host Star GJ 436 (left side)
ax.scatter([-6.0], [0],
           color='darkred',
           s=1000,
           zorder=10,
           label='M-Dwarf Host Star GJ 436')
ax.text(-6.0,
        -1.8,
        'Host Star\nGJ 436 (M3.5V)',
        ha='center',
        fontweight='bold',
        fontsize=11,
        color='darkred')

# XUV rays
for y_pos in np.linspace(-1.2, 1.2, 5):
    ax.annotate('',
                xy=(-1.2, y_pos),
                xytext=(-4.8, y_pos),
                arrowprops=dict(arrowstyle="->",
                                color="crimson",
                                lw=2,
                                linestyle="-"))
ax.text(-3.0,
        1.5,
        r'Stellar XUV Irradiance $F_{\text{XUV}}$',
        color='crimson',
        fontweight='bold',
        fontsize=11)

# Warm Neptune GJ 436b (center)
planet = plt.Circle((0, 0),
                    0.5,
                    color='darkslateblue',
                    ec='black',
                    lw=2,
                    zorder=12,
                    label='GJ 436b ($4.3 R_E$)')
ax.add_patch(planet)
ax.text(0,
        -1.2,
        'Warm Neptune\nGJ 436b',
        ha='center',
        fontweight='bold',
        fontsize=11,
        color='darkslateblue')

# Gigantic Hydrogen Coma Envelope surrounding planet
coma = plt.Circle((0, 0),
                  2.2,
                  color='mediumorchid',
                  alpha=0.35,
                  zorder=6,
                  label=r'Giant Coma Envelope ($R_{\text{coma}} \sim 30 R_p$)')
ax.add_patch(coma)

# Extended Trailing Cometary Tail (spanning 22 hours)
tail_x = np.linspace(0.5, 7.5, 200)
tail_upper = 2.2 + 0.3 * (tail_x - 0.5)**1.1
tail_lower = -tail_upper

ax.fill_between(
    tail_x,
    tail_lower,
    tail_upper,
    color='darkorchid',
    alpha=0.25,
    zorder=5,
    label=r'Asymmetric Trailing Tail ($\Delta F/F = 56.3\%$ Ly-$\alpha$)')
ax.text(
    4.0,
    0.0,
    r'Evaporating Hydrogen Wind ($\Delta t_{\text{transit}} = 22\text{ hours}$)',
    color='indigo',
    fontweight='bold',
    fontsize=10,
    ha='center')

# Radiation Pressure arrow
ax.annotate('',
            xy=(7.0, 0),
            xytext=(2.5, 0),
            arrowprops=dict(arrowstyle="->", color="indigo", lw=2.5))
ax.text(4.8,
        -0.6,
        r'Stellar Radiation Pressure Acceleration',
        color='indigo',
        fontsize=9,
        fontweight='bold')

ax.set_xlim(-7.5, 8.5)
ax.set_ylim(-3.2, 3.2)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: GJ 436b Giant Extended Hydrogen Cloud',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_16/fig_diagram.pdf')
fig.savefig('replications_observational/paper_16/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #16 physical configuration diagram!")
