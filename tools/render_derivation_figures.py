"""
Render publication-quality 2D vector geometric diagrams for Appendix derivations.
"""

import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def render_hydrostatic_diagram():
    fig, ax = plt.subplots(figsize=(5, 4.5))
    ax.set_aspect('equal')
    ax.axis('off')

    # Draw planet sector
    arc = patches.Arc((0, 0), 6.0, 6.0, angle=0, theta1=0, theta2=90, color='#1f77b4', lw=2)
    ax.add_patch(arc)

    # Shell element
    shell = patches.Wedge((0, 0), 2.2, 0, 90, width=0.5, facecolor='#6baed6', edgecolor='#08519c', alpha=0.6)
    ax.add_patch(shell)

    # Center label
    ax.scatter([0], [0], color='black', s=40)
    ax.text(0.1, -0.2, r"Center $r=0$, $m(r)=0$", fontsize=9, fontweight='bold')

    # Radius labels
    ax.annotate(r"$r$", xy=(0,0), xytext=(1.2, 0.2), arrowprops=dict(arrowstyle='<-', color='gray'))
    ax.annotate(r"$r+dr$", xy=(0,0), xytext=(1.7, 1.2), arrowprops=dict(arrowstyle='<-', color='gray'))

    # Force Vectors
    ax.annotate(r"$F_{\mathrm{in}} = P(r) A$", xy=(1.2, 1.2), xytext=(0.5, 0.5),
                arrowprops=dict(arrowstyle='->', color='blue', lw=2), fontsize=9, fontweight='bold')
    ax.annotate(r"$F_{\mathrm{out}} = P(r+dr) A$", xy=(1.55, 1.55), xytext=(2.2, 2.2),
                arrowprops=dict(arrowstyle='->', color='red', lw=2), fontsize=9, fontweight='bold')
    ax.annotate(r"$F_g = \frac{G m dm}{r^2}$", xy=(1.35, 1.35), xytext=(2.2, 1.0),
                arrowprops=dict(arrowstyle='->', color='green', lw=2), fontsize=9, fontweight='bold')

    ax.set_title("Derivation A1: 1D Hydrostatic Shell Force Balance", fontsize=10, fontweight='bold')
    plt.tight_layout()
    fig.savefig("outputs/derivation_hydrostatic_geom.pdf", bbox_inches='tight')
    fig.savefig("paper/figures/derivation_hydrostatic_geom.pdf", bbox_inches='tight')
    plt.close(fig)

def render_core_strain_diagram():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(6.5, 3))
    ax1.set_aspect('equal')
    ax2.set_aspect('equal')
    ax1.axis('off')
    ax2.axis('off')

    # Uncompressed Core
    c1 = patches.Circle((0, 0), 1.2, facecolor='#fdae6b', edgecolor='#e6550d', lw=2)
    ax1.add_patch(c1)
    ax1.text(0, 0, r"Uncompressed Core" + "\n" + r"$R_0, V_0, \rho_0$" + "\n" + r"$(f = 0)$",
             ha='center', va='center', fontsize=8, fontweight='bold')

    # Compressed Core
    c2 = patches.Circle((0, 0), 0.8, facecolor='#e6550d', edgecolor='#a63603', lw=2)
    ax2.add_patch(c2)
    ax2.text(0, 0, r"Compressed Core" + "\n" + r"$R, V, \rho$" + "\n" + r"$(f > 0)$",
             ha='center', va='center', fontsize=8, fontweight='bold', color='white')

    fig.suptitle(r"Derivation A2: Eulerian Finite Strain $f = \frac{1}{2}[(\rho/\rho_0)^{2/3} - 1]$", fontsize=10, fontweight='bold')
    plt.tight_layout()
    fig.savefig("outputs/derivation_core_strain_geom.pdf", bbox_inches='tight')
    fig.savefig("paper/figures/derivation_core_strain_geom.pdf", bbox_inches='tight')
    plt.close(fig)

def render_fermi_sphere_diagram():
    fig, ax = plt.subplots(figsize=(4.5, 4))
    ax.set_aspect('equal')
    ax.axis('off')

    # Axes
    ax.arrow(0, 0, 2.2, 0, head_width=0.1, head_length=0.1, fc='black', ec='black')
    ax.arrow(0, 0, 0, 2.2, head_width=0.1, head_length=0.1, fc='black', ec='black')
    ax.text(2.3, -0.1, r"$p_x$", fontsize=10)
    ax.text(-0.1, 2.3, r"$p_z$", fontsize=10)

    # Fermi Circle
    fermi = patches.Circle((0, 0), 1.6, facecolor='#9ecae1', edgecolor='#3182bd', alpha=0.6, lw=2)
    ax.add_patch(fermi)

    ax.annotate(r"Fermi Radius $p_F = \hbar(3\pi^2 n_e)^{1/3}$", xy=(1.1, 1.1), xytext=(0.2, 1.7),
                arrowprops=dict(arrowstyle='->', color='blue', lw=1.5), fontsize=8, fontweight='bold')

    ax.set_title("Derivation A3: 3D Quantum Phase Fermi Sphere", fontsize=10, fontweight='bold')
    plt.tight_layout()
    fig.savefig("outputs/derivation_fermi_sphere_geom.pdf", bbox_inches='tight')
    fig.savefig("paper/figures/derivation_fermi_sphere_geom.pdf", bbox_inches='tight')
    plt.close(fig)

def render_radiative_slab_diagram():
    fig, ax = plt.subplots(figsize=(5, 3.5))
    ax.axis('off')

    # Atmosphere Slab
    slab = patches.Rectangle((-2, 0.5), 4, 1.2, facecolor='#cccccc', edgecolor='black', lw=1.5)
    ax.add_patch(slab)
    ax.text(0, 1.1, r"Atmosphere Radiative Slab ($\tau, T(\tau)$)", ha='center', fontsize=9, fontweight='bold')

    # Fluxes
    ax.annotate(r"Incoming Stellar Flux $F_{\mathrm{irr}} \downarrow$", xy=(0, 1.7), xytext=(0, 2.5),
                arrowprops=dict(arrowstyle='->', color='#d95f02', lw=2.5), ha='center', fontsize=9, fontweight='bold')
    ax.annotate(r"Intrinsic Internal Flux $F_{\mathrm{int}} \uparrow$", xy=(0, 0.5), xytext=(0, -0.3),
                arrowprops=dict(arrowstyle='<-', color='#7570b3', lw=2.5), ha='center', fontsize=9, fontweight='bold')

    ax.text(2.2, 1.7, r"$\tau = 0$ (Top)", fontsize=8)
    ax.text(2.2, 0.4, r"$\tau \gg 1$ (Deep)", fontsize=8)

    ax.set_title("Derivation A4: Guillot (2010) Radiative Transfer Slab", fontsize=10, fontweight='bold')
    plt.tight_layout()
    fig.savefig("outputs/derivation_radiative_slab_geom.pdf", bbox_inches='tight')
    fig.savefig("paper/figures/derivation_radiative_slab_geom.pdf", bbox_inches='tight')
    plt.close(fig)

def render_tidal_bulge_diagram():
    fig, ax = plt.subplots(figsize=(5.5, 3.5))
    ax.set_aspect('equal')
    ax.axis('off')

    # Star
    star = patches.Circle((-2.0, 0), 0.7, facecolor='#ff7f0e', edgecolor='black')
    ax.add_patch(star)
    ax.text(-2.0, 0, r"$M_*$", ha='center', va='center', fontweight='bold')

    # Line of centers
    ax.plot([-2.0, 2.5], [0, 0], ls='--', color='gray')

    # Deformed Planet
    planet = patches.Ellipse((1.8, 0), 1.4, 0.9, angle=20, facecolor='#1f77b4', edgecolor='black', alpha=0.7)
    ax.add_patch(planet)
    ax.text(1.8, 0, r"$M_p$", ha='center', va='center', color='white', fontweight='bold')

    # Phase lag angle
    ax.text(2.6, 0.3, r"Phase Lag $\delta = \frac{1}{2Q}$", fontsize=8, color='red', fontweight='bold')

    ax.set_title("Derivation A6: Tidal Bulge Deformation & Phase Lag", fontsize=10, fontweight='bold')
    plt.tight_layout()
    fig.savefig("outputs/derivation_tidal_bulge_geom.pdf", bbox_inches='tight')
    fig.savefig("paper/figures/derivation_tidal_bulge_geom.pdf", bbox_inches='tight')
    plt.close(fig)

def render_roche_lobe_diagram():
    fig, ax = plt.subplots(figsize=(5.5, 3.5))
    ax.set_aspect('equal')
    ax.axis('off')

    # Star & Planet
    star = patches.Circle((-1.8, 0), 0.7, facecolor='#ff7f0e', edgecolor='black')
    ax.add_patch(star)
    ax.text(-1.8, 0, r"$M_*$", ha='center', va='center', fontweight='bold')

    planet = patches.Circle((1.5, 0), 0.4, facecolor='#1f77b4', edgecolor='black')
    ax.add_patch(planet)
    ax.text(1.5, 0, r"$M_p$", ha='center', va='center', color='white', fontweight='bold')

    # L1 saddle point
    ax.scatter([0.4], [0], color='red', s=40)
    ax.text(0.4, 0.2, r"$L_1$ Saddle Point", ha='center', color='red', fontsize=8, fontweight='bold')

    # Stream
    ax.annotate(r"Nozzle Gas Stream $\dot{M}_{\mathrm{RLOF}}$", xy=(-1.0, 0), xytext=(1.0, -0.6),
                arrowprops=dict(arrowstyle='->', color='red', lw=1.8), fontsize=8, fontweight='bold')

    ax.set_title("Derivation A9: Roche Lobe Equipotential & Mass Loss", fontsize=10, fontweight='bold')
    plt.tight_layout()
    fig.savefig("outputs/derivation_roche_lobe_geom.pdf", bbox_inches='tight')
    fig.savefig("paper/figures/derivation_roche_lobe_geom.pdf", bbox_inches='tight')
    plt.close(fig)

def main():
    os.makedirs("outputs", exist_ok=True)
    os.makedirs("paper/figures", exist_ok=True)
    print("Rendering 2D vector geometric derivation diagrams...")
    render_hydrostatic_diagram()
    render_core_strain_diagram()
    render_fermi_sphere_diagram()
    render_radiative_slab_diagram()
    render_tidal_bulge_diagram()
    render_roche_lobe_diagram()
    print("All geometric derivation PDF diagrams rendered successfully to paper/figures/.")

if __name__ == "__main__":
    main()
