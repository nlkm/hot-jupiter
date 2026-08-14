#!/usr/bin/env python3
"""
Generate publication-quality figures for Paper #206:
Ross & Schubert (1987) "Tidal Dissipation in Europa's Ice Shell" (Nature 325, 133-134).

Generates:
  1. fig_comparison.pdf (and .png): Radial Volumetric Tidal Heating Profile q_tide(r)
  2. fig_model_choices.pdf (and .png): Total Tidal Power vs Ice Grain Size & Shell Thickness
  3. fig_diagram.pdf (and .png): Europa Viscoelastic Ice Shell Schematic & Rheological Dissipation Mechanism
"""

import csv
import os

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import FancyArrowPatch, Rectangle

# Set global matplotlib styling
plt.rcParams.update({
    'font.family': 'serif',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 14,
    'figure.dpi': 300,
    'text.usetex': False,
    'lines.linewidth': 2.0,
})

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


def read_csv_data(filepath):
    data = {}
    with open(filepath, 'r') as f:
        reader = csv.reader(f)
        headers = next(reader)
        for h in headers:
            data[h] = []
        for row in reader:
            for h, val in zip(headers, row):
                data[h].append(float(val))
    return {h: np.array(vals) for h, vals in data.items()}


def plot_radial_comparison():
    csv_path = os.path.join(SCRIPT_DIR, "europa_volumetric_heating.csv")
    if not os.path.exists(csv_path):
        print(f"Error: {csv_path} not found.")
        return

    df = read_csv_data(csv_path)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5.2))

    # Left plot: Volumetric heating rate vs depth
    depth = df['depth_km']
    q_cond_uW = df['q_tide_cond_w_m3'] * 1e6
    q_conv_uW = df['q_tide_conv_w_m3'] * 1e6

    ax1.plot(depth,
             q_conv_uW,
             color='#d95f02',
             lw=2.5,
             label=r'Convective Shell (Ross & Schubert 1987)')
    ax1.plot(depth,
             q_cond_uW,
             color='#7570b3',
             lw=2.0,
             ls='--',
             label=r'Pure Conduction (Static Lid)')

    # Basal ductile peak highlight
    ax1.axvspan(15.0,
                20.0,
                color='#fee0d2',
                alpha=0.5,
                label=r'Ductile Basal Layer ($T > 240\ \mathrm{K}$)')
    ax1.axvline(20.0,
                color='gray',
                ls=':',
                lw=1.5,
                label=r'Ice-Ocean Interface ($z = 20\ \mathrm{km}$)')

    ax1.set_xlabel(r'Depth below Surface $z$ [km]')
    ax1.set_ylabel(
        r'Volumetric Tidal Dissipation Rate $q_{\mathrm{tide}}(z)$ [$\mu\mathrm{W/m}^3$]'
    )
    ax1.set_title('Volumetric Tidal Heating Rate vs. Depth', fontweight='bold')
    ax1.set_xlim(0, 20.5)
    ax1.set_ylim(0, 7.5)
    ax1.grid(True, linestyle=':', alpha=0.6)
    ax1.legend(loc='upper left', frameon=True, fancybox=True, framealpha=0.9)

    # Right plot: Viscosity & Maxwell Relaxation Dissipation Kernel
    temp_k = df['temp_k_conv']
    phi_diss = df['phi_dissipation']

    color = '#1b9e77'
    ax2.plot(depth,
             temp_k,
             color=color,
             lw=2.5,
             label=r'Temperature $T(z)$ [K]')
    ax2.set_xlabel(r'Depth below Surface $z$ [km]')
    ax2.set_ylabel(r'Ice Shell Temperature $T$ [K]', color=color)
    ax2.tick_params(axis='y', labelcolor=color)
    ax2.set_xlim(0, 20.5)
    ax2.set_ylim(80, 290)
    ax2.grid(True, linestyle=':', alpha=0.6)

    ax2_twin = ax2.twinx()
    color_twin = '#e7298a'
    ax2_twin.plot(depth,
                  phi_diss,
                  color=color_twin,
                  lw=2.5,
                  ls='-.',
                  label=r'Dissipation Function $\Phi(\omega\tau_M)$')
    ax2_twin.set_ylabel(
        r'Maxwell Dissipation Function $\Phi = \frac{\omega\tau_M}{1 + (\omega\tau_M)^2}$',
        color=color_twin)
    ax2_twin.tick_params(axis='y', labelcolor=color_twin)
    ax2_twin.set_ylim(0, 0.55)

    ax2.set_title('Thermal Profile & Viscoelastic Resonance', fontweight='bold')

    # Combined legend for right subplot
    lines_1, labels_1 = ax2.get_legend_handles_labels()
    lines_2, labels_2 = ax2_twin.get_legend_handles_labels()
    ax2.legend(lines_1 + lines_2,
               labels_1 + labels_2,
               loc='upper left',
               frameon=True,
               fancybox=True,
               framealpha=0.9)

    plt.tight_layout()
    fig.savefig(os.path.join(SCRIPT_DIR, 'fig_comparison.pdf'),
                bbox_inches='tight')
    fig.savefig(os.path.join(SCRIPT_DIR, 'fig_comparison.png'),
                dpi=300,
                bbox_inches='tight')
    plt.close()
    print(" Created fig_comparison.pdf and fig_comparison.png")


def plot_model_choices():
    csv_grain = os.path.join(SCRIPT_DIR, "europa_power_vs_grain_size.csv")
    csv_thick = os.path.join(SCRIPT_DIR, "europa_power_vs_thickness.csv")
    if not os.path.exists(csv_grain) or not os.path.exists(csv_thick):
        print("Error: Required CSV files not found.")
        return

    df_grain = read_csv_data(csv_grain)
    df_thick = read_csv_data(csv_thick)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5.2))

    # Left plot: Total power vs Grain Size
    grain_size = df_grain['grain_size_mm']
    power_conv = df_grain['power_tw_conv']
    power_cond = df_grain['power_tw_cond']

    ax1.plot(grain_size,
             power_conv,
             color='#2b83ba',
             lw=2.5,
             label=r'Convective Shell ($D = 20\ \mathrm{km}$)')
    ax1.plot(grain_size,
             power_cond,
             color='#fdae61',
             lw=2.0,
             ls='--',
             label=r'Conductive Shell ($D = 20\ \mathrm{km}$)')

    # Typical grain size band (0.5 - 3 mm)
    ax1.axvspan(0.5,
                3.0,
                color='#abdda4',
                alpha=0.35,
                label=r'Plausible Grain Size ($0.5 - 3.0\ \mathrm{mm}$)')
    ax1.axhline(2.5,
                color='gray',
                ls=':',
                lw=1.5,
                label=r'Equilibrium Balance $\approx 2.5\ \mathrm{TW}$')

    ax1.set_xlabel(r'Ice Grain Size $d$ [mm]')
    ax1.set_ylabel(r'Total Viscoelastic Tidal Power $P_{\mathrm{tide}}$ [TW]')
    ax1.set_title('Tidal Power vs. Ice Grain Size', fontweight='bold')
    ax1.set_xlim(0.1, 10.0)
    ax1.set_ylim(0, 6.0)
    ax1.grid(True, linestyle=':', alpha=0.6)
    ax1.legend(loc='upper right', frameon=True, fancybox=True, framealpha=0.9)

    # Right plot: Power & Conductive Loss vs Shell Thickness (Thermal Equilibrium)
    h_km = df_thick['shell_thickness_km']
    p_tide = df_thick['power_tw_conv']
    q_cond_loss = df_thick['cond_loss_tw']

    ax2.plot(h_km,
             p_tide,
             color='#d7191c',
             lw=2.5,
             label=r'Tidal Heat Generation $P_{\mathrm{tide}}(D)$')
    ax2.plot(h_km,
             q_cond_loss,
             color='#2b83ba',
             lw=2.5,
             ls='--',
             label=r'Conductive Heat Loss $Q_{\mathrm{cond}}(D)$')

    # Equilibrium point intersection
    idx_eq = np.argmin(np.abs(p_tide - q_cond_loss))
    eq_thickness = h_km[idx_eq]
    eq_power = p_tide[idx_eq]
    ax2.plot(
        eq_thickness,
        eq_power,
        'o',
        markersize=9,
        color='#008837',
        zorder=5,
        label=rf'Equilibrium: $D_e \approx {eq_thickness:.1f}\ \mathrm{{km}}$')

    ax2.set_xlabel(r'Ice Shell Thickness $D_{\mathrm{shell}}$ [km]')
    ax2.set_ylabel(r'Power [TW]')
    ax2.set_title('Thermal Equilibrium & Stable Shell Thickness',
                  fontweight='bold')
    ax2.set_xlim(5, 45)
    ax2.set_ylim(0, 8.0)
    ax2.grid(True, linestyle=':', alpha=0.6)
    ax2.legend(loc='upper right', frameon=True, fancybox=True, framealpha=0.9)

    plt.tight_layout()
    fig.savefig(os.path.join(SCRIPT_DIR, 'fig_model_choices.pdf'),
                bbox_inches='tight')
    fig.savefig(os.path.join(SCRIPT_DIR, 'fig_model_choices.png'),
                dpi=300,
                bbox_inches='tight')
    plt.close()
    print(" Created fig_model_choices.pdf and fig_model_choices.png")


def plot_europa_diagram():
    fig, ax = plt.subplots(figsize=(10, 6.5))

    # Cross section layers
    rect_bg = Rectangle((0, 0), 10, 6.5, color='#f7f7f9')
    ax.add_patch(rect_bg)

    # Core
    core = Rectangle(
        (0.8, 0.8),
        8.4,
        1.2,
        facecolor='#a6611a',
        edgecolor='#543005',
        lw=2,
        label=r'Silicate Mantle & Metallic Core ($r < 1460\ \mathrm{km}$)')
    ax.add_patch(core)
    ax.text(
        5.0,
        1.4,
        'Silicate Mantle / Rocky Core ($R \\approx 1460\\ \\mathrm{km}$, $\\rho \\approx 3200\\ \\mathrm{kg/m^3}$)\nHydrothermal venting & seafloor serpentinization',
        ha='center',
        va='center',
        color='white',
        fontweight='bold',
        fontsize=10)

    # Ocean
    ocean = Rectangle(
        (0.8, 2.0),
        8.4,
        1.6,
        facecolor='#4393c3',
        edgecolor='#2166ac',
        lw=2,
        label=
        r'Global Liquid Water Ocean ($D_{\mathrm{ocean}} \approx 80-100\ \mathrm{km}$)'
    )
    ax.add_patch(ocean)
    ax.text(
        5.0,
        2.8,
        'Decoupled Global Liquid Ocean ($\\sim 100\\ \\mathrm{km}$ depth)\nPermits large tidal flexing without core dissipation suppression ($h_2 \\approx 1.2$)',
        ha='center',
        va='center',
        color='white',
        fontweight='bold',
        fontsize=10)

    # Ductile Ice Sublayer
    ductile = Rectangle(
        (0.8, 3.6),
        8.4,
        1.4,
        facecolor='#f4a582',
        edgecolor='#d6604d',
        lw=2,
        label=r'Ductile Viscoelastic Ice Shell ($T \sim 240-273\ \mathrm{K}$)')
    ax.add_patch(ductile)
    ax.text(
        5.0,
        4.3,
        'Ductile Warm Ice Sublayer ($T = 240-273.15\\ \\mathrm{K}$, $\\eta \\sim 10^{13}-10^{15}\\ \\mathrm{Pa\\cdot s}$)\n'
        +
        r'PEAK TIDAL DISSIPATION REGION ($\omega \tau_M \approx 1$, $q_{\mathrm{tide}} \sim 6-7\ \mu\mathrm{W/m}^3$)',
        ha='center',
        va='center',
        color='#67001f',
        fontweight='bold',
        fontsize=10)

    # Brittle Stagnant Lid
    brittle = Rectangle(
        (0.8, 5.0),
        8.4,
        0.8,
        facecolor='#92c5de',
        edgecolor='#0571b0',
        lw=2,
        label=r'Brittle Stagnant Ice Lid ($T = 100-240\ \mathrm{K}$)')
    ax.add_patch(brittle)
    ax.text(
        5.0,
        5.4,
        'Cold Brittle Conductive Lid ($T_{\\mathrm{surf}} \\approx 100\\ \\mathrm{K}$, $\\eta > 10^{18}\\ \\mathrm{Pa\\cdot s}$, Elastic Stiff Lid)',
        ha='center',
        va='center',
        color='#023858',
        fontweight='bold',
        fontsize=9.5)

    # Add tidal strain / stress arrows
    for x_pos in [1.8, 3.8, 6.2, 8.2]:
        arrow1 = FancyArrowPatch((x_pos - 0.4, 4.3), (x_pos + 0.4, 4.3),
                                 arrowstyle='<->',
                                 mutation_scale=14,
                                 color='#b2182b',
                                 lw=2.5)
        ax.add_patch(arrow1)
    ax.text(
        5.0,
        4.8,
        r'Cyclic Diurnal Tidal Flexing $\epsilon_{\mathrm{eff}} \sim 4.2 \times 10^{-5}$ ($P_{\mathrm{orb}} = 3.55\ \mathrm{days}$)',
        ha='center',
        va='center',
        color='#b2182b',
        fontsize=10,
        style='italic')

    # Jupiter gravity indicator
    jupiter_arrow = FancyArrowPatch((9.5, 3.2), (10.1, 3.2),
                                    arrowstyle='->',
                                    mutation_scale=20,
                                    color='#762a83',
                                    lw=3)
    ax.add_patch(jupiter_arrow)
    ax.text(9.7,
            3.6,
            'To Jupiter\n($e = 0.009$)',
            ha='center',
            va='bottom',
            color='#762a83',
            fontweight='bold',
            fontsize=10)

    ax.set_xlim(0, 10.6)
    ax.set_ylim(0.4, 6.2)
    ax.axis('off')
    ax.set_title(
        "Europa Viscoelastic Tidal Dissipation Architecture (Ross & Schubert 1987)",
        fontsize=13,
        fontweight='bold',
        pad=15)

    plt.tight_layout()
    fig.savefig(os.path.join(SCRIPT_DIR, 'fig_diagram.pdf'),
                bbox_inches='tight')
    fig.savefig(os.path.join(SCRIPT_DIR, 'fig_diagram.png'),
                dpi=300,
                bbox_inches='tight')
    plt.close()
    print(" Created fig_diagram.pdf and fig_diagram.png")


if __name__ == '__main__':
    plot_radial_comparison()
    plot_model_choices()
    plot_europa_diagram()
    print(" All figures generated successfully.")
