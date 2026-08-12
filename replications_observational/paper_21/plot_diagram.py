# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for KELT-9b Ultra-Hot Thermosphere

import matplotlib.pyplot as plt

fig, ax = plt.subplots(figsize=(8, 5))

# Host Star KELT-9 (center-left)
star = plt.Circle((-3.0, 0),
                  2.2,
                  color='deepskyblue',
                  ec='blue',
                  lw=2.5,
                  zorder=10,
                  label=r'Host Star KELT-9 ($10,170\text{ K}$ A0V)')
ax.add_patch(star)
ax.text(-3.0,
        0,
        'Scorching Host Star\nKELT-9 (A0V, 10,170 K)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10,
        color='white')

# Ultra-Hot Jupiter KELT-9b Core
planet = plt.Circle((3.5, 0),
                    1.2,
                    color='crimson',
                    ec='black',
                    lw=2,
                    zorder=12,
                    label=r'KELT-9b Core ($1.89 R_J$)')
ax.add_patch(planet)
ax.text(3.5,
        0,
        'Ultra-Hot\nKELT-9b Core',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10,
        color='yellow')

# Extended Hydrogen Thermosphere Shell (R_therm = 1.32 R_p)
thermosphere = plt.Circle((3.5, 0),
                          1.6,
                          color='magenta',
                          alpha=0.35,
                          zorder=9,
                          label=r'Extended Thermosphere ($1.32 R_p$)')
ax.add_patch(thermosphere)
ax.text(
    3.5,
    -2.1,
    r'Extended $H\alpha$ Thermosphere ($R_{\text{therm}} = 1.32 R_p$, $H = 8315\text{ km}$)',
    ha='center',
    fontweight='bold',
    fontsize=10,
    color='darkmagenta')

# High-Resolution Transmission Spectroscopy Ray
ax.annotate('',
            xy=(1.5, 1.2),
            xytext=(5.2, 1.2),
            arrowprops=dict(arrowstyle="<->", color="purple", lw=2.2))
ax.text(
    3.35,
    1.5,
    r'CARMENES / HARPS-N Balmer $H\alpha$ Transit Absorption ($\Delta \delta = 1.15\%$)',
    color='purple',
    fontweight='bold',
    fontsize=10,
    ha='center')

ax.set_xlim(-6.0, 6.0)
ax.set_ylim(-3.5, 3.5)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: KELT-9b Hydrodynamic Thermospheric Expansion & Balmer Absorption',
    fontsize=11,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_21/fig_diagram.pdf')
fig.savefig('replications_observational/paper_21/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #21 physical configuration diagram!")
