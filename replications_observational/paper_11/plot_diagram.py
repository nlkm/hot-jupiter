# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Textbook Physical Configuration & Schematic Diagram Generator for Pluto-Charon Binary System

import matplotlib.pyplot as plt

fig, ax = plt.subplots(figsize=(8, 5))

# Scale distances for schematic (Pluto at x=0, Charon at x=10)
x_pluto = 0.0
x_charon = 10.0
x_bary = 1.085  # Barycenter at 2127 km out of 19596 km

# Pluto sphere (radius scaled)
pluto = plt.Circle((x_pluto, 0),
                   1.2,
                   color='chocolate',
                   ec='black',
                   lw=2,
                   label='Pluto ($R_P = 1188$ km)')
ax.add_patch(pluto)
ax.text(x_pluto,
        -1.8,
        'Pluto\n($M_P = 1.303 \\times 10^{22}$ kg)',
        ha='center',
        fontweight='bold',
        fontsize=11)

# Charon sphere (radius scaled)
charon = plt.Circle((x_charon, 0),
                    0.61,
                    color='lightgray',
                    ec='black',
                    lw=2,
                    label='Charon ($R_C = 606$ km)')
ax.add_patch(charon)
ax.text(x_charon,
        -1.8,
        'Charon\n($M_C = 1.586 \\times 10^{21}$ kg)',
        ha='center',
        fontweight='bold',
        fontsize=11)

# System Barycenter Marker (+)
ax.scatter([x_bary], [0],
           color='crimson',
           marker='+',
           s=250,
           lw=3,
           zorder=10,
           label='System Barycenter')
ax.axvline(x_bary, color='crimson', linestyle=':', lw=1.5)
ax.text(x_bary,
        1.8,
        r'Barycenter ($r_{\text{bary}} = 2127$ km)',
        color='crimson',
        fontweight='bold',
        ha='center',
        fontsize=10)

# Orbit line & separation a
ax.annotate('',
            xy=(x_pluto, 0),
            xytext=(x_charon, 0),
            arrowprops=dict(arrowstyle="<->", color="navy", lw=1.8))
ax.text(5.0,
        0.3,
        r'Mutual Separation $a = 19,596$ km',
        color='navy',
        fontweight='bold',
        ha='center',
        fontsize=11)

# Tidal locking rotation arrows (Dual Synchronous Lock)
ax.annotate('',
            xy=(x_pluto + 0.8, 0.8),
            xytext=(x_pluto - 0.8, 0.8),
            arrowprops=dict(arrowstyle="->",
                            color="darkgreen",
                            lw=2,
                            connectionstyle="arc3,rad=0.5"))
ax.text(x_pluto,
        2.3,
        r'Tidal Lock ($P_{\text{rot}} = 6.387\text{ d}$)',
        color='darkgreen',
        fontweight='bold',
        ha='center',
        fontsize=9)

ax.annotate('',
            xy=(x_charon + 0.4, 0.4),
            xytext=(x_charon - 0.4, 0.4),
            arrowprops=dict(arrowstyle="->",
                            color="darkgreen",
                            lw=2,
                            connectionstyle="arc3,rad=0.5"))
ax.text(x_charon,
        2.3,
        r'Tidal Lock ($P_{\text{rot}} = 6.387\text{ d}$)',
        color='darkgreen',
        fontweight='bold',
        ha='center',
        fontsize=9)

ax.set_xlim(-3.0, 13.0)
ax.set_ylim(-3.0, 3.5)
ax.set_aspect('equal')
ax.axis('off')
ax.set_title(
    'Physical Configuration Diagram: Dual Synchronous Pluto-Charon Binary',
    fontsize=12,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig.savefig('replications_observational/paper_11/fig_diagram.pdf')
fig.savefig('replications_observational/paper_11/fig_diagram.png', dpi=300)
plt.close(fig)

print("✅ Saved Paper #11 physical configuration diagram!")
