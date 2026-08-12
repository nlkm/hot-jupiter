# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for LTT 9779b Ultra-Hot Neptune Albedo

import matplotlib.pyplot as plt

fig, ax = plt.subplots(figsize=(8, 5))

# Host Star LTT 9779 (left)
star = plt.Circle((-3.0, 0),
                  1.7,
                  color='gold',
                  ec='orange',
                  lw=2.5,
                  zorder=10,
                  label=r'Host Star LTT 9779 (G7V)')
ax.add_patch(star)
ax.text(-3.0,
        0,
        'Host Star\nLTT 9779 (G7V)',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=10,
        color='darkred')

# Ultra-Hot Neptune LTT 9779b with Reflective Cloud Deck
planet = plt.Circle((2.5, 0),
                    0.8,
                    color='lightgray',
                    ec='black',
                    lw=2,
                    zorder=12,
                    label=r'LTT 9779b ($4.72 R_E$)')
ax.add_patch(planet)
# Metallic cloud deck overlay
cloud = plt.Circle((2.5, 0),
                   0.82,
                   fill=False,
                   color='cyan',
                   lw=3,
                   linestyle=':',
                   zorder=13)
ax.add_patch(cloud)
ax.text(2.5,
        0,
        'LTT 9779b\nCore & Cloud',
        ha='center',
        va='center',
        fontweight='bold',
        fontsize=8.5,
        color='black')

# Incident & Reflected Light Rays (A_g = 0.80)
ax.annotate('',
            xy=(1.7, 0.4),
            xytext=(-1.3, 0.4),
            arrowprops=dict(arrowstyle="->", color="gold", lw=2.5))
ax.text(0.2,
        0.65,
        r'Incident Stellar Light',
        color='darkorange',
        fontweight='bold',
        fontsize=8.5,
        ha='center')

ax.annotate('',
            xy=(-1.3, -0.4),
            xytext=(1.7, -0.4),
            arrowprops=dict(arrowstyle="->", color="cyan", lw=2.5))
ax.text(
    0.2,
    -0.65,
    r'Reflected Light ($A_g = 0.80$, $\delta_{\text{eclipse}} = 225\text{ ppm}$)',
    color='darkcyan',
    fontweight='bold',
    fontsize=8.5,
    ha='center')

ax.set_xlim(-5.5, 6.5)
ax.set_ylim(-3.0, 3.0)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: LTT 9779b Extreme Geometric Albedo & Reflective Silicate Clouds',
    fontsize=10.5,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_25/fig_diagram.pdf')
fig.savefig('replications_observational/paper_25/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #25 physical configuration diagram!")
