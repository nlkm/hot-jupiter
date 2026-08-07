"""
Render publication-quality 2D vector geometric diagrams for Appendix derivations.
Ensures elegant text positioning, proper margins, and zero overlaps.
"""

import os

import matplotlib.pyplot as plt
from matplotlib import patches


def render_hydrostatic_diagram():
    fig, ax = plt.subplots(figsize=(6, 5))
    ax.set_aspect('equal')
    ax.axis('off')

    # Outer planet sphere sector
    arc = patches.Arc((0, 0),
                      7.0,
                      7.0,
                      angle=0,
                      theta1=0,
                      theta2=90,
                      color='#2b5c8f',
                      lw=2.5)
    ax.add_patch(arc)

    # Shell element
    shell = patches.Wedge((0, 0),
                          2.7,
                          0,
                          90,
                          width=0.6,
                          facecolor='#9ecae1',
                          edgecolor='#08519c',
                          alpha=0.7,
                          lw=1.5)
    ax.add_patch(shell)

    # Center marker & label
    ax.scatter([0], [0], color='black', s=50, zorder=5)
    ax.text(0.15,
            -0.3,
            r"Center $r=0$, $m(r)=0$",
            fontsize=9,
            fontweight='bold',
            bbox=dict(boxstyle='round,pad=0.3',
                      facecolor='white',
                      edgecolor='gray',
                      alpha=0.8))

    # Radius arrows
    ax.annotate(r"Radius $r$",
                xy=(1.4, 0),
                xytext=(1.0, -0.6),
                arrowprops=dict(arrowstyle='->', color='#3182bd', lw=1.5),
                fontsize=9,
                fontweight='bold')
    ax.annotate(r"Radius $r+dr$",
                xy=(1.9, 1.9),
                xytext=(2.8, 2.5),
                arrowprops=dict(arrowstyle='->', color='#08519c', lw=1.5),
                fontsize=9,
                fontweight='bold')

    # Force Vectors
    ax.annotate(r"Gas Pressure $F_{\mathrm{in}} = P(r) A$",
                xy=(1.48, 1.48),
                xytext=(0.1, 0.9),
                arrowprops=dict(arrowstyle='->', color='#1f77b4', lw=2.5),
                fontsize=9,
                fontweight='bold',
                bbox=dict(boxstyle='round,pad=0.3',
                          facecolor='#e6f2ff',
                          edgecolor='#1f77b4',
                          alpha=0.9))

    ax.annotate(r"Gas Pressure $F_{\mathrm{out}} = P(r+dr) A$",
                xy=(1.91, 1.91),
                xytext=(2.2, 1.6),
                arrowprops=dict(arrowstyle='->', color='#d62728', lw=2.5),
                fontsize=9,
                fontweight='bold',
                bbox=dict(boxstyle='round,pad=0.3',
                          facecolor='#ffe6e6',
                          edgecolor='#d62728',
                          alpha=0.9))

    ax.annotate(r"Gravity $F_g = \frac{G m dm}{r^2}$",
                xy=(1.68, 1.68),
                xytext=(2.6, 0.5),
                arrowprops=dict(arrowstyle='->', color='#2ca02c', lw=2.5),
                fontsize=9,
                fontweight='bold',
                bbox=dict(boxstyle='round,pad=0.3',
                          facecolor='#e6ffe6',
                          edgecolor='#2ca02c',
                          alpha=0.9))

    ax.set_title("Derivation A1: 1D Hydrostatic Shell Force Balance",
                 fontsize=11,
                 fontweight='bold',
                 pad=15)
    plt.tight_layout()
    fig.savefig("outputs/derivation_hydrostatic_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    fig.savefig("paper/figures/derivation_hydrostatic_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    plt.close(fig)


def render_core_strain_diagram():
    fig, ax = plt.subplots(figsize=(6.5, 3.2))
    ax.set_aspect('equal')
    ax.axis('off')
    ax.set_xlim(-3.6, 3.6)
    ax.set_ylim(-1.6, 1.6)

    # Uncompressed Core
    c1 = patches.Circle((-2.0, 0),
                        1.2,
                        facecolor='#fdd0a2',
                        edgecolor='#e6550d',
                        lw=2.5)
    ax.add_patch(c1)
    ax.text(-2.0,
            0,
            r"Uncompressed Core" + "\n" + r"$R_0, V_0, \rho_0$" + "\n" +
            r"$(f = 0)$",
            ha='center',
            va='center',
            fontsize=9,
            fontweight='bold',
            color='#8c2d04')

    # Compression Arrow
    ax.annotate(r"Isotropic Compression" + "\n" + r"Pressure $P > 0$",
                xy=(0.7, 0),
                xytext=(-0.6, 0),
                arrowprops=dict(arrowstyle='->', color='#7f7f7f', lw=3.0),
                ha='center',
                va='center',
                fontsize=9,
                fontweight='bold',
                bbox=dict(boxstyle='round,pad=0.3',
                          facecolor='#f0f0f0',
                          edgecolor='gray',
                          alpha=0.9))

    # Compressed Core
    c2 = patches.Circle((2.2, 0),
                        0.8,
                        facecolor='#e6550d',
                        edgecolor='#7f2704',
                        lw=2.5)
    ax.add_patch(c2)
    ax.text(2.2,
            0,
            r"Compressed Core" + "\n" + r"$R, V, \rho$" + "\n" + r"$(f > 0)$",
            ha='center',
            va='center',
            fontsize=8.5,
            fontweight='bold',
            color='white')

    ax.set_title(
        r"Derivation A2: Eulerian Finite Strain $f = \frac{1}{2}\left[\left(\frac{\rho}{\rho_0}\right)^{2/3} - 1\right]$",
        fontsize=11,
        fontweight='bold',
        pad=12)
    fig.savefig("outputs/derivation_core_strain_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    fig.savefig("paper/figures/derivation_core_strain_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    plt.close(fig)


def render_fermi_sphere_diagram():
    fig, ax = plt.subplots(figsize=(5.5, 4.2))
    ax.set_aspect('equal')
    ax.axis('off')
    ax.set_xlim(-2.2, 3.2)
    ax.set_ylim(-1.6, 2.8)

    # Axes
    ax.arrow(0,
             0,
             2.4,
             0,
             head_width=0.08,
             head_length=0.08,
             fc='black',
             ec='black',
             lw=1.5)
    ax.arrow(0,
             0,
             0,
             2.4,
             head_width=0.08,
             head_length=0.08,
             fc='black',
             ec='black',
             lw=1.5)
    ax.arrow(0,
             0,
             -1.2,
             -1.0,
             head_width=0.08,
             head_length=0.08,
             fc='black',
             ec='black',
             lw=1.5)

    ax.text(2.5, -0.1, r"$p_x$", fontsize=10, fontweight='bold')
    ax.text(-0.1, 2.5, r"$p_z$", fontsize=10, fontweight='bold')
    ax.text(-1.4, -1.1, r"$p_y$", fontsize=10, fontweight='bold')

    # Fermi Sphere
    fermi = patches.Circle((0, 0),
                           1.7,
                           facecolor='#9ecae1',
                           edgecolor='#3182bd',
                           alpha=0.5,
                           lw=2)
    ax.add_patch(fermi)
    ax.add_patch(
        patches.Ellipse((0, 0),
                        3.4,
                        1.0,
                        fill=False,
                        edgecolor='#3182bd',
                        ls='--',
                        lw=1.2))

    # Radius vector & annotation
    ax.plot([0, 1.20], [0, 1.20], color='#08519c', lw=2.0)
    ax.scatter([1.20], [1.20], color='#08519c', s=35, zorder=5)

    ax.text(1.3,
            1.3,
            r"Fermi Radius $p_F = \hbar(3\pi^2 n_e)^{1/3}$",
            fontsize=9,
            fontweight='bold',
            bbox=dict(boxstyle='round,pad=0.3',
                      facecolor='#e6f2ff',
                      edgecolor='#3182bd',
                      alpha=0.9))

    # Quantum cell annotation
    ax.text(-1.8,
            1.4,
            r"Phase Cell $h^3 = (2\pi\hbar)^3$" + "\n" +
            r"Holds 2 $e^-$ ($\uparrow, \downarrow$)",
            fontsize=8.5,
            fontweight='bold',
            bbox=dict(boxstyle='round,pad=0.3',
                      facecolor='#fff7bc',
                      edgecolor='#d95f0e',
                      alpha=0.9))

    ax.set_title("Derivation A3: 3D Quantum Phase Space Fermi Sphere",
                 fontsize=11,
                 fontweight='bold',
                 pad=12)
    fig.savefig("outputs/derivation_fermi_sphere_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    fig.savefig("paper/figures/derivation_fermi_sphere_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    plt.close(fig)


def render_radiative_slab_diagram():
    fig, ax = plt.subplots(figsize=(6.5, 3.8))
    ax.axis('off')
    ax.set_xlim(-2.8, 4.8)
    ax.set_ylim(-0.8, 2.8)

    # Atmosphere Slab
    slab = patches.Rectangle((-2.2, 0.4),
                             4.4,
                             1.2,
                             facecolor='#e0e0e0',
                             edgecolor='black',
                             lw=2.0)
    ax.add_patch(slab)
    ax.text(0,
            1.0,
            r"Atmosphere Radiative Slab ($\tau$, $T(\tau)$)" + "\n" +
            r"Eddington Double-Gray Model",
            ha='center',
            va='center',
            fontsize=9.5,
            fontweight='bold')

    # Incoming Stellar Irradiation
    ax.annotate(r"Incoming Stellar Flux $F_{\mathrm{irr}} \downarrow$",
                xy=(0, 1.6),
                xytext=(0, 2.4),
                arrowprops=dict(arrowstyle='->', color='#e6550d', lw=3.0),
                ha='center',
                fontsize=9.5,
                fontweight='bold',
                bbox=dict(boxstyle='round,pad=0.3',
                          facecolor='#fee6ce',
                          edgecolor='#e6550d',
                          alpha=0.9))

    # Intrinsic Interior Heat
    ax.annotate(r"Intrinsic Thermal Flux $F_{\mathrm{int}} \uparrow$",
                xy=(0, 0.4),
                xytext=(0, -0.4),
                arrowprops=dict(arrowstyle='<-', color='#7570b3', lw=3.0),
                ha='center',
                fontsize=9.5,
                fontweight='bold',
                bbox=dict(boxstyle='round,pad=0.3',
                          facecolor='#efedf5',
                          edgecolor='#7570b3',
                          alpha=0.9))

    # Optical depth boundaries
    ax.text(2.4,
            1.6,
            r"$\tau = 0$ (Top of Atmosphere)",
            fontsize=8.5,
            fontweight='bold',
            va='center')
    ax.text(2.4,
            0.4,
            r"$\tau \gg 1$ (Deep Convective Interior)",
            fontsize=8.5,
            fontweight='bold',
            va='center')

    ax.set_title("Derivation A4: Guillot (2010) Radiative Transfer Slab",
                 fontsize=11,
                 fontweight='bold',
                 pad=12)
    fig.savefig("outputs/derivation_radiative_slab_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    fig.savefig("paper/figures/derivation_radiative_slab_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    plt.close(fig)


def render_tidal_bulge_diagram():
    fig, ax = plt.subplots(figsize=(6.5, 4))
    ax.set_aspect('equal')
    ax.axis('off')

    # Star
    star = patches.Circle((-2.2, 0),
                          0.8,
                          facecolor='#ff7f0e',
                          edgecolor='#a63603',
                          lw=2.0)
    ax.add_patch(star)
    ax.text(-2.2,
            0,
            r"Host Star" + "\n" + r"$M_*$",
            ha='center',
            va='center',
            fontweight='bold',
            color='white',
            fontsize=9)

    # Line of centers
    ax.plot([-2.2, 2.8], [0, 0], ls='--', color='gray', lw=1.2)

    # Deformed Planet Ellipse
    planet = patches.Ellipse((2.0, 0),
                             1.5,
                             0.95,
                             angle=18,
                             facecolor='#6baed6',
                             edgecolor='#08519c',
                             alpha=0.8,
                             lw=2.0)
    ax.add_patch(planet)
    ax.text(2.0,
            0,
            r"Planet $M_p$",
            ha='center',
            va='center',
            color='black',
            fontweight='bold',
            fontsize=9)

    # Phase lag angle arc
    ax.text(2.8,
            0.45,
            r"Phase Lag $\delta = \frac{1}{2Q}$" + "\n" +
            r"(Internal Viscous Friction)",
            fontsize=8.5,
            color='#d95f02',
            fontweight='bold',
            bbox=dict(boxstyle='round,pad=0.3',
                      facecolor='#fee6ce',
                      edgecolor='#d95f02',
                      alpha=0.9))

    # Atmospheric wind / Ohmic current annotation
    ax.text(
        0.0,
        -1.1,
        r"Atmospheric Zonal Wind $\mathbf{v} \times \mathbf{B}$" + "\n" +
        r"Drives Ohmic Current $P_{\mathrm{Ohmic}} = \int \frac{J^2}{\sigma} dV$",
        ha='center',
        fontsize=8.5,
        fontweight='bold',
        bbox=dict(boxstyle='round,pad=0.3',
                  facecolor='#e6f2ff',
                  edgecolor='#1f77b4',
                  alpha=0.9))

    ax.set_title("Derivation A6: Tidal Bulge Deformation & Ohmic Dissipation",
                 fontsize=11,
                 fontweight='bold',
                 pad=12)
    plt.tight_layout()
    fig.savefig("outputs/derivation_tidal_bulge_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    fig.savefig("paper/figures/derivation_tidal_bulge_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    plt.close(fig)


def render_roche_lobe_diagram():
    fig, ax = plt.subplots(figsize=(6.5, 4))
    ax.set_aspect('equal')
    ax.axis('off')

    # Star
    star = patches.Circle((-2.0, 0),
                          0.75,
                          facecolor='#ff7f0e',
                          edgecolor='#a63603',
                          lw=2.0)
    ax.add_patch(star)
    ax.text(-2.0,
            0,
            r"Star $M_*$",
            ha='center',
            va='center',
            fontweight='bold',
            color='white',
            fontsize=9)

    # Overflowing Planet
    planet = patches.Circle((1.8, 0),
                            0.5,
                            facecolor='#6baed6',
                            edgecolor='#08519c',
                            lw=2.0)
    ax.add_patch(planet)
    ax.text(1.8,
            0,
            r"Planet $M_p$",
            ha='center',
            va='center',
            color='black',
            fontweight='bold',
            fontsize=8.5)

    # L1 saddle point marker
    ax.scatter([0.5], [0], color='#d95f02', s=50, zorder=5)
    ax.text(0.5,
            0.35,
            r"Inner Lagrange $L_1$" + "\n" +
            r"Saddle ($\nabla \Phi_{\mathrm{eff}} = 0$)",
            ha='center',
            color='#d95f02',
            fontsize=8.5,
            fontweight='bold',
            bbox=dict(boxstyle='round,pad=0.3',
                      facecolor='#fee6ce',
                      edgecolor='#d95f02',
                      alpha=0.9))

    # Gas stream
    ax.annotate(r"Hydrodynamic Nozzle Stream $\dot{M}_{\mathrm{RLOF}}$",
                xy=(-1.1, 0),
                xytext=(1.2, -0.8),
                arrowprops=dict(arrowstyle='->',
                                color='#d62728',
                                lw=2.5,
                                connectionstyle="arc3,rad=-0.2"),
                fontsize=8.5,
                fontweight='bold',
                bbox=dict(boxstyle='round,pad=0.3',
                          facecolor='#ffe6e6',
                          edgecolor='#d62728',
                          alpha=0.9))

    ax.set_title("Derivation A9: Roche Lobe Overflow (RLOF) & Mass Loss",
                 fontsize=11,
                 fontweight='bold',
                 pad=12)
    plt.tight_layout()
    fig.savefig("outputs/derivation_roche_lobe_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    fig.savefig("paper/figures/derivation_roche_lobe_geom.pdf",
                bbox_inches='tight',
                dpi=300)
    plt.close(fig)


def main():
    os.makedirs("outputs", exist_ok=True)
    os.makedirs("paper/figures", exist_ok=True)
    print(
        "Rendering 2D vector geometric derivation diagrams with elegant text placement..."
    )
    render_hydrostatic_diagram()
    render_core_strain_diagram()
    render_fermi_sphere_diagram()
    render_radiative_slab_diagram()
    render_tidal_bulge_diagram()
    render_roche_lobe_diagram()
    print(
        "All geometric derivation PDF diagrams rendered successfully to paper/figures/."
    )


if __name__ == "__main__":
    main()
