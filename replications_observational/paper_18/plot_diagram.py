# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for WASP-43b Tidal Circularization

import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(figsize=(8, 5))

# Host Star WASP-43 (center-left)
star = plt.Circle((-3.5, 0),
                  1.5,
                  color='orange',
                  ec='darkorange',
                  lw=2.5,
                  zorder=10,
                  label=r'Host Star WASP-43 ($0.667 R_\odot$)')
ax.add_patch(star)
ax.text(-3.5,
        0,
        'Host Star\nWASP-43 (K7V)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=11,
        color='darkred')

# Circular Orbit Path (e = 0)
orbit_circ = plt.Circle((-3.5, 0),
                        5.5,
                        color='blue',
                        fill=False,
                        linestyle='-',
                        lw=2,
                        label=r'Circularized Orbit ($e \approx 0$)')
ax.add_patch(orbit_circ)

# Primordial Eccentric Orbit Path (e_0 = 0.2)
theta = np.linspace(0, 2 * np.pi, 200)
r_ecc = 5.5 * (1 - 0.2**2) / (1 + 0.2 * np.cos(theta))
x_ecc = -3.5 + r_ecc * np.cos(theta)
y_ecc = r_ecc * np.sin(theta)
ax.plot(x_ecc,
        y_ecc,
        'r--',
        lw=1.8,
        label=r'Primordial Eccentric Orbit ($e_0 = 0.2$)')

# Massive Hot Jupiter WASP-43b (on circular orbit)
planet = plt.Circle((2.0, 0),
                    0.9,
                    color='saddlebrown',
                    ec='black',
                    lw=2,
                    zorder=12,
                    label='WASP-43b ($2.05 M_J$)')
ax.add_patch(planet)
ax.text(2.0,
        -1.4,
        'Massive Hot Jupiter\nWASP-43b',
        ha='center',
        fontweight='bold',
        fontsize=11,
        color='saddlebrown')

# Planetary Tidal Dissipation & Circularization Arrow
ax.annotate('',
            xy=(2.0, 1.2),
            xytext=(2.0, 2.8),
            arrowprops=dict(arrowstyle="->", color="crimson", lw=2.5))
ax.text(
    2.0,
    3.2,
    r'Planetary Tidal Damping $\tau_e = \frac{2}{21} \frac{Q_*^\prime}{n} \left(\frac{M_p}{M_*}\right) \left(\frac{a}{R_p}\right)^5 \approx 7.5\text{ Myr}$',
    color='crimson',
    fontweight='bold',
    fontsize=10,
    ha='center')

ax.set_xlim(-7.0, 6.0)
ax.set_ylim(-4.0, 4.0)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: WASP-43b Tidal Eccentricity Circularization',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_18/fig_diagram.pdf')
fig.savefig('replications_observational/paper_18/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #18 physical configuration diagram!")
