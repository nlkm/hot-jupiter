# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for Planet Nine Secular Clustering

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(7, 7))

# Draw Sun
ax.scatter([0], [0], color='gold', s=400, zorder=10, label='Sun')

# Neptune's Orbit (30 AU) - reference scale
neptune_orbit = plt.Circle((0, 0),
                           0.6,
                           color='blue',
                           fill=False,
                           linestyle='--',
                           lw=1.5,
                           label='Neptune (30 AU)')
ax.add_patch(neptune_orbit)

# Planet Nine Orbit (Eccentric, anti-aligned, a_9 = 460 AU, e_9 = 0.25)
t_grid = np.linspace(0, 2 * np.pi, 300)
a_p9 = 3.5
e_p9 = 0.25
varpi_p9 = np.radians(60)

r_p9 = a_p9 * (1 - e_p9**2) / (1 + e_p9 * np.cos(t_grid - varpi_p9))
x_p9 = r_p9 * np.cos(t_grid)
y_p9 = r_p9 * np.sin(t_grid)

ax.plot(
    x_p9,
    y_p9,
    'r-',
    lw=2.5,
    label='Planet Nine Orbit ($a_9 \\sim 460$ AU, $M_9 \\sim 6 M_{\\oplus}$)')

# Planet Nine Position
p9_idx = 0
ax.scatter([x_p9[p9_idx]], [y_p9[p9_idx]], color='darkred', s=120, zorder=8)
ax.text(x_p9[p9_idx] + 0.2,
        y_p9[p9_idx] + 0.2,
        r'Planet Nine ($6 M_{\oplus}$)',
        color='darkred',
        fontweight='bold',
        fontsize=10)

# Clustered eTNO Orbits (Anti-aligned, \varpi_eTNO \sim 240 deg, \Delta \varpi \sim 180 deg)
varpi_etno_base = np.radians(240)
colors_etno = ['purple', 'teal', 'darkgreen', 'navy', 'indigo']
etno_names = ['Sedna', '2012 VP113', 'Leleakuhua', '2013 FT28', '2015 BP519']

for i, (col, name) in enumerate(zip(colors_etno, etno_names)):
    varpi_i = varpi_etno_base + np.radians((i - 2) * 6)
    a_i = 2.5 + i * 0.2
    e_i = 0.65
    r_i = a_i * (1 - e_i**2) / (1 + e_i * np.cos(t_grid - varpi_i))
    x_i = r_i * np.cos(t_grid)
    y_i = r_i * np.sin(t_grid)
    ax.plot(x_i, y_i, color=col, linestyle='-', alpha=0.7, lw=1.5)

    # Label perihelion point
    peri_idx = np.argmin(r_i)
    ax.scatter([x_i[peri_idx]], [y_i[peri_idx]], color=col, s=30, zorder=6)

ax.text(
    -2.5,
    -2.5,
    r'Clustered eTNOs ($\Delta \varpi = \varpi_{\text{eTNO}} - \varpi_9 \approx 180^{\circ}$)',
    fontsize=11,
    color='purple',
    fontweight='bold')

# Anti-alignment double arrow
ax.annotate('',
            xy=(1.8 * np.cos(varpi_p9), 1.8 * np.sin(varpi_p9)),
            xytext=(-1.8 * np.cos(varpi_p9), -1.8 * np.sin(varpi_p9)),
            arrowprops=dict(arrowstyle="<->",
                            color="black",
                            linestyle=":",
                            lw=1.5))
ax.text(0.3,
        -1.2,
        r'$\Delta \varpi \approx 180^{\circ}$ (Anti-aligned)',
        fontsize=10,
        fontweight='bold')

ax.set_xlim(-4.5, 4.5)
ax.set_ylim(-4.5, 4.5)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: Planet Nine Secular eTNO Orbit Alignment',
    fontsize=11,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_10/fig_diagram.pdf')
fig.savefig('replications_observational/paper_10/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #10 physical configuration diagram!")
