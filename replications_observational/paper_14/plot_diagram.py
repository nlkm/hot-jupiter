# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for HD 209458b Hydrodynamic Escape

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(8, 5))

# Draw Host Star (left side)
ax.scatter([-6.0], [0],
           color='gold',
           s=1200,
           zorder=10,
           label='Host Star HD 209458')
ax.text(-6.0,
        -1.8,
        'Host Star\nHD 209458',
        ha='center',
        fontweight='bold',
        fontsize=11)

# EUV Flux arrows
for y_pos in np.linspace(-1.2, 1.2, 5):
    ax.annotate('',
                xy=(-0.8, y_pos),
                xytext=(-4.5, y_pos),
                arrowprops=dict(arrowstyle="->",
                                color="orange",
                                lw=2,
                                linestyle="-"))
ax.text(-2.8,
        1.5,
        r'Incident Stellar XUV Flux $F_{\text{XUV}}$',
        color='darkorange',
        fontweight='bold',
        fontsize=11)

# Hot Jupiter HD 209458b (center)
planet = plt.Circle((0, 0),
                    0.8,
                    color='navy',
                    ec='black',
                    lw=2,
                    zorder=12,
                    label='HD 209458b ($1.38 R_J$)')
ax.add_patch(planet)
ax.text(0,
        -1.5,
        'Hot Jupiter\nHD 209458b',
        ha='center',
        fontweight='bold',
        fontsize=11,
        color='navy')

# Hydrodynamic Wind & Roche Lobe Boundary
roche = plt.Circle((0, 0),
                   1.8,
                   color='crimson',
                   fill=False,
                   linestyle='--',
                   lw=1.8,
                   label=r'Roche Lobe Boundary ($R_{\text{Roche}} \sim 3 R_p$)')
ax.add_patch(roche)

# Extended Hydrogen Cometary Tail (streaming rightward)
tail_x = np.linspace(0.8, 7.0, 200)
tail_upper = 0.8 + 0.3 * (tail_x - 0.8)**1.2
tail_lower = -tail_upper

ax.fill_between(
    tail_x,
    tail_lower,
    tail_upper,
    color='crimson',
    alpha=0.3,
    zorder=5,
    label=
    r'Extended Hydrogen Tail ($\dot{M} \approx 5 \times 10^{10}\text{ g/s}$)')
ax.text(4.0,
        0.0,
        r'Escaping Hydrogen Wind ($\Delta F/F = 15\%$ Ly-$\alpha$ Transit)',
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
        r'Stellar Ly-$\alpha$ Radiation Pressure',
        color='darkred',
        fontsize=9,
        fontweight='bold')

ax.set_xlim(-7.5, 8.0)
ax.set_ylim(-3.0, 3.0)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: HD 209458b Hydrodynamic Photoevaporation',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_14/fig_diagram.pdf')
fig.savefig('replications_observational/paper_14/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #14 physical configuration diagram!")
